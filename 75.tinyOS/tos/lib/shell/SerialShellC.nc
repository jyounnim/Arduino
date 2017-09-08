/*
 * Copyright (c) 2012 Johny Mattsson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the copyright holders nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

generic module SerialShellC(uint8_t num_cmds)
{
  provides interface Init;
  provides interface ShellOutput[uint8_t id];
  provides interface ShellCommand as CommandList[uint8_t id];

  uses interface ShellCommand[uint8_t id];
  uses interface ShellExecute[uint8_t id];
  uses interface CommandLineParser;
  uses interface StdControl;
  uses interface UartStream;
}
implementation
{
  enum { NO_CMD = 0xff };
  enum { ABORT_KEY = 0x03 }; // ctrl-c

  typedef enum {
    PROMPT_BARE, PROMPT_OK, PROMPT_BUSY, PROMPT_FAIL, PROMPT_ABORT,
    PROMPT_UNKNOWN, PROMPT_SYNTAX
  } prompt_t;

  prompt_t new_prompt;

  uint8_t cur_cmd = NO_CMD;
  bool printing_prompt = FALSE;
  volatile bool buffer_locked = FALSE;

  #ifndef SERIAL_SHELL_BUFFER_SIZE
  #define SERIAL_SHELL_BUFFER_SIZE 80
  #endif

  char recv_buf[SERIAL_SHELL_BUFFER_SIZE] = { 0 };
  char *rx = recv_buf;

  #ifndef SERIAL_SHELL_MAX_ARGS
  #define SERIAL_SHELL_MAX_ARGS 9
  #endif

  char *argv[SERIAL_SHELL_MAX_ARGS];

  void print_prompt (prompt_t type)
  {
    static const char *prompts[] = {
      "# ",
      "OK\r\n# ",
      "BUSY\r\n# ",
      "FAILED\r\n# ",
      "^C\r\n# ",
      "UNKNOWN\r\n# ",
      "SYNTAX ERROR\r\n# "
    };
    const char *prompt = prompts[type];

    printing_prompt =
      (call UartStream.send ((uint8_t *)prompt, strlen (prompt)) == SUCCESS) ?
        TRUE : FALSE;
  }

  inline prompt_t error_to_prompt (error_t result)
  {
    switch (result)
    {
      case SUCCESS: return PROMPT_OK;
      case EBUSY: return PROMPT_BUSY;
      case ECANCEL: return PROMPT_ABORT;
      default: return PROMPT_FAIL;
    }
  }

  task void cmd_exit ()
  {
    print_prompt (new_prompt);
    if (!printing_prompt)
    {
      post cmd_exit ();
      return;
    }
  }

  command error_t Init.init ()
  {
    error_t res = call StdControl.start ();
    return ecombine (res, call UartStream.enableReceiveInterrupt ());
  }

 
  command error_t ShellOutput.output[uint8_t id] (const char *str, size_t len)
  {
    if (id != cur_cmd)
      return SUCCESS; // Note: use SUCCESS to work with ecombine nicely
    else
      return call UartStream.send ((uint8_t *)str, len);
  }

  command size_t ShellOutput.limit[uint8_t id] ()
  {
    // We don't need to buffer/packetize the output, so no real limit
    return (size_t)-1;
  }

  event void ShellExecute.executeDone[uint8_t id] (error_t result)
  {
    if (id != cur_cmd)
      return; // not our execute

    new_prompt = error_to_prompt (result);
    post cmd_exit ();
  }


  task void abort_requested ()
  {
    if (cur_cmd != NO_CMD)
      call ShellExecute.abort[cur_cmd] ();
    else
      print_prompt (PROMPT_ABORT);
  }

  task void output_done ()
  {
    if (printing_prompt)
    {
      printing_prompt = FALSE;
      cur_cmd = NO_CMD;
      atomic buffer_locked = FALSE;
    }
    else
      if (cur_cmd != NO_CMD)
        signal ShellOutput.outputDone[cur_cmd] ();
  }

  task void parse_command ()
  {
    uint8_t argc = SERIAL_SHELL_MAX_ARGS;
    error_t res =
      call CommandLineParser.parse (recv_buf, &argc, argv);

    if (res == SUCCESS)
    {
      uint8_t i;

      if (!argc || !*argv[0]) // no command, just reprint prompt
      {
        new_prompt = PROMPT_BARE;
        goto no_run;
      }

      for (i = 0; i < num_cmds; ++i)
      {
        if (strcmp (argv[0], call ShellCommand.getCommandString[i] ()) == 0)
          break;
      }

      if (i == num_cmds)
        new_prompt = PROMPT_UNKNOWN; // command not found
      else
      {
        error_t result;
        cur_cmd = i;
        result = call ShellExecute.execute[i] (argc, (const char **)argv);
        if (result == SUCCESS)
          return; // now running a command
        new_prompt = error_to_prompt (result);
      }
    }
    else
      new_prompt = PROMPT_SYNTAX;

  no_run:
    post cmd_exit ();
  }


  async event void UartStream.sendDone (uint8_t *buf, uint16_t len, error_t res)
  {
    post output_done ();
  }

  async event void UartStream.receiveDone (uint8_t *buf, uint16_t len, error_t res) {}

  async event void UartStream.receivedByte (uint8_t byte)
  {
    if (byte == ABORT_KEY)
    {
      rx = recv_buf;
      post abort_requested ();
      return;
    }

    if (buffer_locked)
      return;

    if (byte < ' ' || byte == 0x7f)
    {
      switch (byte)
      {
        case 0x08: // Backspace
        case 0x7f: // Delete
          if (rx > recv_buf)
            --rx;
          break;
        case '\r':
        case '\n':
          *rx = 0;
          rx = recv_buf;
          buffer_locked = TRUE;
          post parse_command ();
          break;
        default: break;
      }
      return;
    }

    if (rx < (recv_buf + sizeof (recv_buf) -1))
      *rx++ = byte;
  }

  command const char *CommandList.getCommandString[uint8_t id] ()
  {
    return call ShellCommand.getCommandString[id] ();
  }

  default command const char *ShellCommand.getCommandString[uint8_t id] ()
  {
    return "";
  }

  default command error_t ShellExecute.execute[uint8_t id] (uint8_t argc, const char *argv_[])
  {
    return FAIL;
  }

  default command void ShellExecute.abort[uint8_t id] () {}
  default event void ShellOutput.outputDone[uint8_t id] () {}
}

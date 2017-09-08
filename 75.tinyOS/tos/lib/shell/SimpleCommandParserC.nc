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

/**
 * A simple command line parser, supporting double-quotes and backslash
 * escapes. Only the <space> character is considered a delimiter.
 */
generic module SimpleCommandParserC()
{
  provides interface CommandLineParser;
}
implementation
{

  command error_t CommandLineParser.parse (char *str, uint8_t *argc, char *argv[])
  {
    uint8_t max_args = *argc;
    char *p;
    bool escaped = FALSE, inquote = FALSE, inspace = TRUE;

    *argc = 0;

    for (p = str; *p && *argc < max_args; ++p)
    {
      if (escaped)
      {
        escaped = FALSE;
        continue;
      }
      if (*p == ' ' && !inquote && !escaped)
      {
        inspace = TRUE;
        *p = 0;
        continue;
      }
      if (inspace && !escaped && *p != ' ')
      {
        inspace = FALSE;
        argv[(*argc)++] = p;
        continue;
      }
      if (*p == '\\')
      {
        escaped = TRUE;
        continue;
      }
      if (*p == '"')
      {
        inquote = TRUE;
        continue;
      }
    }

    if (escaped || inquote)
      return EINVAL;
    else if (*p)
      return ENOMEM;
    else
      return SUCCESS;
  }

}

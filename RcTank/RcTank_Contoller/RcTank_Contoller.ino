/*
   March 2014 - TMRh20 - Updated along with High Speed RF24 Library fork
   Parts derived from examples by J. Coliz <maniacbug@ymail.com>
*/
/**
   Example for efficient call-response using ack-payloads

   This example continues to make use of all the normal functionality of the radios including
   the auto-ack and auto-retry features, but allows ack-payloads to be written optionlly as well.
   This allows very fast call-response communication, with the responding radio never having to
   switch out of Primary Receiver mode to send back a payload, but having the option to switch to
   primary transmitter if wanting to initiate communication instead of respond to a commmunication.
*/
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define RIGHT_FORWARD 2
#define RIGHT_REAR    3
#define LEFT_FORWARD  4
#define LEFT_REAR     5

#define testPin_HIGH     10
#define testPin_LOW      11

#define RIGHT_FORWARD_MASK  0b00000001
#define RIGHT_REAR_MASK     0b00000010
#define LEFT_FORWARD_MASK   0b00000100
#define LEFT_REAR_MASK      0b00001000

byte sleepCount=0;

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(7, 8);
// Topology
byte addresses[][6] = {"1Node", "2Node"};             // Radio pipe addresses for the 2 nodes to communicate.

// Role management: Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
typedef enum { role_ping_out = 1, role_pong_back } role_e;                 // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};  // The debug-friendly names of those roles
role_e role = role_ping_out;                                              // The role of the current running sketch

byte counter = 1;                                                          // A single byte to keep track of the data being sent back and forth

void setup() {
  Serial.begin(115200);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r", role_friendly_name[role]);
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  // Setup and configure radio

  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openWritingPipe(addresses[0]);        // Both radios listen on the same pipes by default, and switch when writing
  radio.openReadingPipe(1, addresses[1]);     // Open a reading pipe on address 0, pipe 1
  radio.startListening();                 // Start listening
  radio.powerUp();
  radio.setPALevel(RF24_PA_MIN);
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging

  // initialize remote pins as an input.
  pinMode(RIGHT_FORWARD, INPUT);
  pinMode(RIGHT_REAR, INPUT);
  pinMode(LEFT_FORWARD, INPUT);
  pinMode(LEFT_REAR, INPUT);

  pinMode(testPin_HIGH, OUTPUT);
  pinMode(testPin_LOW, OUTPUT);

  digitalWrite(testPin_HIGH, HIGH);
  digitalWrite(testPin_LOW, LOW);
}

void loop(void) {


  /****************** Ping Out Role ***************************/

  if (role == role_ping_out) {                              // Radio is in ping mode

    byte sendByte=0;
    byte gotByte;                                           // Initialize a variable for the incoming response

    radio.stopListening();                                  // First, stop listening so we can talk.
    printf("Now sending %d as payload. ", counter);         // Use a simple byte counter as payload
    unsigned long time = micros();                          // Record the current microsecond count

    if( digitalRead(RIGHT_FORWARD) )
      sendByte = RIGHT_FORWARD_MASK;
    if( digitalRead(RIGHT_REAR) )
      sendByte |= RIGHT_REAR_MASK;
      
    if( digitalRead(LEFT_FORWARD) )
      sendByte |= LEFT_FORWARD_MASK;
    if( digitalRead(LEFT_REAR) )
      sendByte |= LEFT_REAR_MASK;            

    if( sendByte == 0 )
      sleepCount++;
    else
      sleepCount = 0;

    if( sleepCount >= 10 )
    {
      printf("*** Sleep now ***\n\r");
      sleepCount = 120;
      delay(1000);
      return;
    }
                 
    if ( radio.write(&sendByte, 1) ) {                       // Send the counter variable to the other radio
      if (!radio.available()) {                           // If nothing in the buffer, we got an ack but it is blank
        printf("Got blank response. round-trip delay: %lu microseconds\n\r", micros() - time);
      } else {
        while (radio.available() ) {                    // If an ack with payload was received
          radio.read( &gotByte, 1 );                  // Read it, and display the response time
          printf("Got response %d, round-trip delay: %lu microseconds\n\r", gotByte, micros() - time);
          counter++;                                  // Increment the counter variable
        }
      }

    } else {
      printf("Sending failed.\n\r");  // If no ack response, sending failed
    }

    delay(500);  // Try again later
  }


  /****************** Pong Back Role ***************************/

  if ( role == role_pong_back ) {
    byte pipeNo, gotByte;                          // Declare variables for the pipe and the byte received
    while ( radio.available(&pipeNo)) {            // Read all available payloads
      radio.read( &gotByte, 1 );
      // Since this is a call-response. Respond directly with an ack payload.
      // Ack payloads are much more efficient than switching to transmit mode to respond to a call
      radio.writeAckPayload(pipeNo, &gotByte, 1 ); // This can be commented out to send empty payloads.
      printf("Sent response %d \n\r", gotByte);
    }
  }

  /****************** Change Roles via Serial Commands ***************************/

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back )
    {
      printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

      role = role_ping_out;                      // Change roles (ping out)
      radio.openWritingPipe(addresses[0]);       // Open different pipes when writing. Write on pipe 0, address 0
      radio.openReadingPipe(1, addresses[1]);    // Read on pipe 1, as address 1
      radio.printDetails();
    }
    else if ( c == 'R' && role == role_ping_out )
    {
      printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");

      role = role_pong_back;                    // Become the primary receiver (pong back)
      radio.openWritingPipe(addresses[1]);      // Since only two radios involved, both listen on the same addresses and pipe numbers in RX mode
      radio.openReadingPipe(1, addresses[0]);   // then switch pipes & addresses to transmit.
      radio.startListening();                   // Need to start listening after opening new reading pipes
      radio.printDetails();
    }
  }
}

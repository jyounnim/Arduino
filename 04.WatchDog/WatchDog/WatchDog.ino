#include <avr/wdt.h>

#define LED 13

void setup() {
  // immediately disable watchdog timer so set will not get interrupted
  wdt_disable();

  // initialize serial port
  Serial.begin(115200); 

  // initialize LED pins as an output.
  pinMode(LED, OUTPUT);
  
  Serial.println("------- WatchDog Test -------"); 
  delay(1000);

  // The timeouts I'm most likely to use are:
  // WDTO_1S
  // WDTO_2S
  // WDTO_4S
  // WDTO_8S
  wdt_enable(WDTO_4S);
}

void loop() {
  int i;

  for( i=0 ; i<8 ; i++)
  {
    Serial.println("LEDs ON"); 
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);                  // wait for 500ms
  
    Serial.println("LEDs OFF"); 
    digitalWrite(LED, LOW);
    delay(500);                  // wait for 500ms
    wdt_reset();
    Serial.print(i);
    Serial.print(":");
    Serial.println("Reset WatchDog"); 
  }

    for( i=0 ; i<8 ; i++)
  {
    Serial.println("LEDs ON"); 
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);                  // wait for 500ms
  
    Serial.println("LEDs OFF"); 
    digitalWrite(LED, LOW);
    delay(500);                  // wait for 500ms
  }
}  

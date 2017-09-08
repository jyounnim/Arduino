// 아래 헤더들을 추가해준다
#include <SPI.h>
#include "RF24.h"
 
// 0 번과 1번 아두이노(nRF모듈)를 나누어서 주고받기위한 용도
// 하나는 0으로, 하나는 1로 설정하고 컴파일 해야 한다
bool radioNumber = 0;
 
// 7 : CE , 8: CSN
RF24 radio(7,8);
 
// Pipe Address 를 설정하기 위한 값
byte addresses[][6] = {"1Node","2Node"};
 
// 누가 Ping 하고 Pong 할 것인지 결정 위함
bool role = 0;
 
void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
   
  //  RF24 모듈 초기화
  radio.begin();
  // PA 세기를 LOW 로 한다. 입력 전류 불안정을 감안함
  radio.setPALevel(RF24_PA_LOW);
   
  // radioNumber 에 따라 Write Pipe - Read Pipe 쌍의 주소를 지정해서 Open 한다
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
   
  // Reading Pipe 주소의 Listening 을 시작한다
  radio.startListening();
}
 
void loop() {
     
// Ping 보내는 역할
if (role == 1)  {
     
    // Write 를 하기 전에 반드시  Listening 을 Stop 해야한다
    radio.stopListening();
     
     
    Serial.println(F("Now sending"));
    unsigned long time = micros();                            
     
    // 데이터를 Write 한다. 이때 채널 내의 해당 주소로 Listen 하는 녀석이 없으면 Fail 떨어진다
     if (!radio.write( &time, sizeof(unsigned long) )){
       Serial.println(F("failed"));
     }
         
    // 보내고 나서 다시 LIsten 한다 
    radio.startListening();                                    
     
    unsigned long started_waiting_at = micros();               
    boolean timeout = false;
     
    // 수신 로직이다. Ping Timeout 을 200ms 로 잡고, Timeout 내에서 수신한다
    while ( ! radio.available() ){ 
      if (micros() - started_waiting_at > 200000 ){
          timeout = true;
          break;
      }      
    }
         
    if ( timeout ){ 
        Serial.println(F("Failed, response timed out."));
    }else{
        unsigned long got_time; 
        // Pong 이 왔을 때 데이터를 읽어온다
        radio.read( &got_time, sizeof(unsigned long) );
        unsigned long time = micros();
         
        // Ping 의 Round-Trip 시간을 계산하여 알려준다
        Serial.print(F("Sent "));
        Serial.print(time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        Serial.print(time-got_time);
        Serial.println(F(" microseconds"));
    }
     
    delay(1000);
  }
 
// Pong 쳐주는 역할
  if ( role == 0 )
  {
    unsigned long got_time;
     
    // 데이터가 있으면 데이터를 모두 읽어서 FIFO 버퍼 소진한다
    if( radio.available()){
      while (radio.available()) {                          
        radio.read( &got_time, sizeof(unsigned long) );          
      }
      
     // Listening 멈추고 Pong 을 Write 한다
      radio.stopListening(); 
      radio.write( &got_time, sizeof(unsigned long) );
      radio.startListening();       
      Serial.print(F("Sent response "));
      Serial.println(got_time);  
   }
 }
 
// Serial 을 통해서 T 와 R 에 따라 역할을 바꾼다
  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == 0 ){      
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
     
   }else
    if ( c == 'R' && role == 1 ){
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
       role = 0;                // Become the primary receiver (pong back)
       radio.startListening();
        
    }
  }
} 


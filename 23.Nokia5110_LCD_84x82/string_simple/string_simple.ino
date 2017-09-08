#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#define PIN_BL 2   //백라이트 제어를 위한 핀연결
 
//SCLK, DIN, DC, CE, RST 핀연결
Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 6);
 
void setup()
{
  //백라이트를 ON 함, HIGH = Turn Backlight OFF, LOW = Turn Backlight ON
  pinMode(PIN_BL,OUTPUT); 
  digitalWrite(PIN_BL,LOW); 
 
  //디스플레이 초기화
  display.begin();
 
  //디스플레이 Contrast 조절 (값이 올라가면 진해짐)
  display.setContrast(60);
 
  //텍스트 디스플레이
  display.clearDisplay();             //디스플레이 지우기
  display.setTextSize(1);             //텍스트 사이즈 조절
  display.setTextColor(BLACK);        //텍스트 색
  display.setCursor(0,0);             //커서 좌표
  display.println("Hello, world!");   //표시 텍스트
  display.setTextColor(WHITE, BLACK); // 'inverted' text (배경 까맣게 글자 하얗게)
  display.println(3.141592);          //숫자 표시
  display.setTextSize(2);             //텍스트 사이즈 조절
  display.setTextColor(BLACK);        //텍스트 색
  display.print("0x");                //표시 텍스트
  display.println(0xDEADBEEF, HEX);   //HEX 쓰기
  display.display();                  //위의 설정내용 표시
}
 
void loop()
{
 
}


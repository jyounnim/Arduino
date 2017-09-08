#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BH1750.h"
#include "DHT.h"
#include "SFE_BMP180.h"

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_RESET 12
#define OLED_CS    13

// RFG LEDs
#define B_LED  30
#define G_LED  31
#define R_LED  32

// Temperature and Humidity sensor
#define DHTTYPE DHT11   // DHT 11 
#define DHTPIN 34

// AirQuality Sensor
#define AIR_QUALITY_SENSOR  A0
#define ADC_REFRES 1023 //reference resolution used for conversions
#define MQ135_PULLDOWNRES 22000
#define MQ135_DEFAULTPPM 399 //default ppm of CO2 for calibration
#define MQ135_DEFAULTRO 68550 //default Ro for MQ135_DEFAULTPPM ppm of CO2
#define MQ135_SCALINGFACTOR 116.6020682 //CO2 gas value
#define MQ135_EXPONENT -2.769034857 //CO2 gas value
#define MQ135_MAXRSRO 2.428 //for CO2
#define MQ135_MINRSRO 0.358 //for CO2
float mq135_ro = 10000.0;    // this has to be tuned 10K Ohm
float valAIQ =0.0;

// Alcohol Sensor
#define ALCOHOL_SENSOR      A1

// Vibration Sensor
#define VIBRATION_SENSOR    A2

// Light Intensity Sensor 
BH1750 lightMeter;

// Presssure Sensor
SFE_BMP180 pressure;
#define IRVINE 28 // Altitude of Irvine is 28 meters (92 feet)

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
DHT dht(DHTPIN, DHTTYPE);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {                
  Serial.begin(115200);
  Serial2.begin(9600);   //Setup Bluetooth serial connecti/on to android

  // Init OLED display
  Serial.println("Init OLED display");
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Home Monitor Ver1.0");
  display.display();
  delay(1000);

  // Set LEDs
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);
  msg_off();

  // Set temperature and humidity sensor
  dht.begin();

  // Set Digital Intensity Sensor
  lightMeter.begin();

  // Set Pressure Sensor
  pressure.begin();
}

void msg_off()
{
  digitalWrite(R_LED, LOW); 
  digitalWrite(G_LED, LOW);
  digitalWrite(B_LED, LOW);
}

void msg_normal()
{
  digitalWrite(R_LED, LOW); 
  digitalWrite(G_LED, HIGH);
  digitalWrite(B_LED, LOW);
}

void msg_warning()
{
  digitalWrite(R_LED, LOW); 
  digitalWrite(G_LED, LOW);
  digitalWrite(B_LED, HIGH);
}

void msg_hazard()
{
  digitalWrite(R_LED, HIGH);
  digitalWrite(G_LED, LOW);
  digitalWrite(B_LED, LOW);
}

/*
 * get the ppm concentration
 */
double mq135_getppm(long resvalue, long ro) 
{
  double ret = 0;
  double validinterval = 0;
  
  validinterval = resvalue/(double)ro;
  if( (validinterval<MQ135_MAXRSRO) && (validinterval>MQ135_MINRSRO) ) 
  {
    ret = (double)MQ135_SCALINGFACTOR * pow( ((double)resvalue/ro), MQ135_EXPONENT);
  }
  return ret;
}

/*
 * get the calibrated ro based upon read resistance, and a know ppm
 */
long mq135_getro(long resvalue, double ppm) 
{
  return (long)(resvalue * exp( log(MQ135_SCALINGFACTOR/ppm) / MQ135_EXPONENT ));
}


void loop() {
  float h;
  float t;
  uint16_t AQvol;
  float Aval; 
  int AsensorValue;
  uint16_t lux;
  char status;
  double T,P,p0,a;

  Serial.println("");
  Serial2.println("");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Home Monitor Ver1.0");
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if ( isnan(t) || isnan(h) ) 
  {
    Serial.println("Failed to read from DHT");
  } 
  else {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println(" %");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" C");
    Serial2.print("Humidity: ");
    Serial2.print(h);
    Serial2.println(" %");
    Serial2.print("Temperature: ");
    Serial2.print(t);
    Serial2.println(" C");    
    display.print("Humidity: ");
    display.print(h);
    display.println(" %\t");
    display.print("Temperature: ");
    display.print(t);
    display.println(" C");
  }

/*
  Air Quality Chart - Small Count Reading (0.5 micron)+

  3000 +     = VERY POOR
  1050-3000  = POOR
  300-1050   = FAIR
  150-300    = GOOD
  75-150     = VERY GOOD
  0-75       = EXCELLENT
*/
  // Air Quality
  AsensorValue = analogRead(AIR_QUALITY_SENSOR);
  AQvol =  ((float)MQ135_PULLDOWNRES * (ADC_REFRES-AsensorValue) / AsensorValue); 
  mq135_ro = mq135_getro(AQvol, MQ135_DEFAULTPPM);
  //convert to ppm (using default ro)
  valAIQ = mq135_getppm(AQvol, MQ135_DEFAULTRO);
  Serial.print("Air Quality: ");
  Serial.print ( valAIQ,1 );
  Serial.println (" ppm");  
  Serial2.print("Air Quality: ");
  Serial2.print ( valAIQ,1 );
  Serial2.println (" ppm");  
  display.print("Air Qlty: ");
  display.print ( valAIQ,1 );
  display.println (" ppm");  
  
  // Alcohol Sensor
  // under 1.0 : Normal
  // 1.0~2.5 : Warning
  // over 2.5 : Danger
  AsensorValue = analogRead(ALCOHOL_SENSOR);
  Aval = (float)AsensorValue/1024*5.0;
  Serial.print("Alcohol Sensor : ");
  Serial.println(Aval);
  Serial2.print("Alcohol Sensor : ");
  Serial2.println(Aval);  
  display.print("Alcohol: ");
  display.println(Aval);

  // Vibration Sensor
  AsensorValue = analogRead(VIBRATION_SENSOR);
  Serial.print("Vibration Sensor : ");
  Serial.println(AsensorValue);
  Serial2.print("Vibration Sensor : ");
  Serial2.println(AsensorValue);
  display.print("Vibration: ");
  display.println(AsensorValue);

  // Digital Intensity Sensor
  lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  Serial2.print("Light: ");
  Serial2.print(lux);
  Serial2.println(" lx");
  display.print("Light: ");
  display.print(lux);
  display.println(" lx");

  // Temperature and pressure Sensor
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("Temperature2: ");
      Serial.print(T,2);
      Serial.print(" C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");
      Serial2.print("Temperature2: ");
      Serial2.print(T,2);
      Serial2.print(" C");      
      Serial2.print((9.0/5.0)*T+32.0,2);
      Serial2.println(" deg F");

      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("Absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");
          Serial2.print("Absolute pressure: ");
          Serial2.print(P,2);
          Serial2.print(" mb, ");
          Serial2.print(P*0.0295333727,2);
          Serial2.println(" inHg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,IRVINE); // we're at 28 meters (Irvine)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");
          Serial2.print("relative (sea-level) pressure: ");
          Serial2.print(p0,2);
          Serial2.print(" mb, ");
          Serial2.print(p0*0.0295333727,2);
          Serial2.println(" inHg");
          display.print("Pressure: ");
          display.print(p0,2);
          display.println(" mb");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P,p0);
          Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");
          Serial2.print("computed altitude: ");
          Serial2.print(a,0);
          Serial2.print(" meters, ");
          Serial2.print(a*3.28084,0);
          Serial2.println(" feet");

        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  display.display();

  // Control LEDs by serial
  if(Serial.available()) 
  {    
    char cmd = (char)Serial.read();
    char val;
    
    if(cmd == 'r' || cmd == 'R') 
    {
      val = digitalRead(R_LED);
      if( val == LOW )
      {      
        Serial.println("Red LED On!");
        Serial2.write("Red LED On\r\n");
        digitalWrite(R_LED, HIGH); 
      }else{
        Serial.println("Red LED Off!");
        Serial2.write("Red LED Off\r\n");
        digitalWrite(R_LED, LOW);       
      }
    }
    
    if(cmd == 'g' || cmd == 'G') 
    {
      val = digitalRead(G_LED);
      if( val == LOW )
      {      
        Serial.println("Green LED On!");
        Serial2.write("Green LED On\r\n");
        digitalWrite(G_LED, HIGH); 
      }else{
        Serial.println("Green LED Off!");
        Serial2.write("Green LED Off\r\n");
        digitalWrite(G_LED, LOW);       
      }
    }
    
    if(cmd == 'b' || cmd == 'B') 
    {
      val = digitalRead(B_LED);
      if( val == LOW )
      {      
        Serial.println("Blue LED On!");
        Serial2.write("Blue LED On\r\n");
        digitalWrite(B_LED, HIGH); 
      }else{
        Serial.println("Blue LED Off!");
        Serial2.write("Blue LED Off\r\n");
        digitalWrite(B_LED, LOW);       
      }
    }
  }

  // Control LEDs by BT  
  if(Serial2.available()) 
  {    
    char cmd = (char)Serial2.read();
    char val;
    
    if(cmd == 'r' || cmd == 'R') 
    {
      val = digitalRead(R_LED);
      if( val == LOW )
      {      
        Serial.println("Red LED On!");
        Serial2.write("Red LED On\r\n");
        digitalWrite(R_LED, HIGH); 
      }else{
        Serial.println("Red LED Off!");
        Serial2.write("Red LED Off\r\n");
        digitalWrite(R_LED, LOW);       
      }
    }
    
    if(cmd == 'g' || cmd == 'G') 
    {
      val = digitalRead(G_LED);
      if( val == LOW )
      {      
        Serial.println("Green LED On!");
        Serial2.write("Green LED On\r\n");
        digitalWrite(G_LED, HIGH); 
      }else{
        Serial.println("Green LED Off!");
        Serial2.write("Green LED Off\r\n");
        digitalWrite(G_LED, LOW);       
      }
    }
    
    if(cmd == 'b' || cmd == 'B') 
    {
      val = digitalRead(B_LED);
      if( val == LOW )
      {      
        Serial.println("Blue LED On!");
        Serial2.write("Blue LED On\r\n");
        digitalWrite(B_LED, HIGH); 
      }else{
        Serial.println("Blue LED Off!");
        Serial2.write("Blue LED Off\r\n");
        digitalWrite(B_LED, LOW);       
      }
    }
  }
  
  delay(1000);  
}



/*
  Arduino MQ135

  connect the sensor as follows :

  A H A   >>> 5V
  B       >>> A0
  H       >>> GND
  B       >>> 10K ohm >>> GND
 
  Contribution: epierre
  Based on David Gironi http://davidegironi.blogspot.fr/2014/01/cheap-co2-meter-using-mq135-sensor-with.html
  http://skylink.dl.sourceforge.net/project/davidegironi/avr-lib/avr_lib_mq135_01.zip
 
*/

#include <SPI.h>
#include <Wire.h> 

#define AIQ_SENSOR_ANALOG_PIN A0

//define mq135 pulldown resistor
#define ADC_REFRES 1023 //reference resolution used for conversions
#define MQ135_PULLDOWNRES 22000
#define MQ135_DEFAULTPPM 399 //default ppm of CO2 for calibration
#define MQ135_DEFAULTRO 68550 //default Ro for MQ135_DEFAULTPPM ppm of CO2
#define MQ135_SCALINGFACTOR 116.6020682 //CO2 gas value
#define MQ135_EXPONENT -2.769034857 //CO2 gas value
#define MQ135_MAXRSRO 2.428 //for CO2
#define MQ135_MINRSRO 0.358 //for CO2

unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in seconds)
//VARIABLES
//float Ro = 10000.0;    // this has to be tuned 10K Ohm
float mq135_ro = 10000.0;    // this has to be tuned 10K Ohm
int val = 0;           // variable to store the value coming from the sensor
float valAIQ =0.0;
float lastAIQ =0.0;


void setup()  
{
  Serial.begin(115200);
  Serial.println("AIQ Sensor MQ135 ver1.0");  
}

/*
 * get the calibrated ro based upon read resistance, and a know ppm
 */
long mq135_getro(long resvalue, double ppm) {
return (long)(resvalue * exp( log(MQ135_SCALINGFACTOR/ppm) / MQ135_EXPONENT ));
}

/*
 * get the ppm concentration
 */
double mq135_getppm(long resvalue, long ro) 
{
  double ret = 0;
  double validinterval = 0;
  
  validinterval = resvalue/(double)ro;
  if(validinterval<MQ135_MAXRSRO && validinterval>MQ135_MINRSRO) 
  {
    ret = (double)MQ135_SCALINGFACTOR * pow( ((double)resvalue/ro), MQ135_EXPONENT);
  }
  return ret;
}

void loop()      
{    
  int valr;
  uint16_t val;
  
  valr = analogRead(AIQ_SENSOR_ANALOG_PIN);// Get AIQ value
  val =  ((float)MQ135_PULLDOWNRES * (ADC_REFRES-valr) / valr); 
  mq135_ro = mq135_getro(val, MQ135_DEFAULTPPM);
  //convert to ppm (using default ro)
  valAIQ = mq135_getppm(val, MQ135_DEFAULTRO);

  Serial.println ( );
  Serial.print ( "Vrl: ");
  Serial.println ( val );
  Serial.print ( "Rs: ");
  Serial.println ( mq135_ro );
  Serial.print ( "Ratio:");
  Serial.print ( valAIQ );
  Serial.println ("ppm");
  
  delay(1000); //sleep for: sleepTime
}

 
/*****************************  MQGetPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         pcurve      - pointer to the curve of the target gas
Output:  ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm) 
         of the line could be derived if y(rs_ro_ratio) is provided. As it is a 
         logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic 
         value.
************************************************************************************/ 
int  MQGetPercentage(float rs_ro_ratio, float ro, float *pcurve)
{
  return (double)(pcurve[0] * pow(((double)rs_ro_ratio/ro), pcurve[1]));
}

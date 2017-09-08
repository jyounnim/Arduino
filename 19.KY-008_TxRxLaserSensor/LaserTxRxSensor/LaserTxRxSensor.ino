#define __ENABLE_TX_LASER__   
#define __ENABLE_RX_LASER__

#ifdef __ENABLE_TX_LASER__
#define LASER_TX     13
#define SWITCH       2
#endif

#ifdef __ENABLE_RX_LASER__
#define LASER_RX_LED 12
#define LASER_RX     3
#endif

#ifdef __ENABLE_TX_LASER__
char cTxLaserStatus = false;
char oldTx = false;
#endif

#ifdef __ENABLE_RX_LASER__
char cRxLaserStatus = false;
char oldRx = false;
#endif

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial port
  Serial.begin(115200); 
  
  // initialize LED pins as an output.
#ifdef __ENABLE_TX_LASER__
  pinMode(LASER_TX, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);
#endif

#ifdef __ENABLE_RX_LASER__
  pinMode(LASER_RX_LED, OUTPUT);
  pinMode(LASER_RX, INPUT_PULLUP);
#endif

  // setup interrupt
#ifdef __ENABLE_TX_LASER__  
  attachInterrupt(digitalPinToInterrupt(SWITCH), sw_isr, CHANGE);
#endif

#ifdef __ENABLE_RX_LASER__  
  attachInterrupt(digitalPinToInterrupt(LASER_RX), laser_rx_isr, CHANGE);
#endif

  Serial.println("------- Laser TxRx Test -------"); 
}

#ifdef __ENABLE_TX_LASER__  
void sw_isr()
{
  if( digitalRead( SWITCH ) == LOW )
  {
    digitalWrite(LASER_TX, HIGH);
    cTxLaserStatus = true;
  } else {
    digitalWrite(LASER_TX, LOW);
    cTxLaserStatus = false;
  }    
}
#endif

#ifdef __ENABLE_RX_LASER__  
void laser_rx_isr()
{
  if( digitalRead( LASER_RX ) == HIGH )
  {
    digitalWrite(LASER_RX_LED, HIGH);
    cRxLaserStatus = true;
  } else {
    digitalWrite(LASER_RX_LED, LOW);
    cRxLaserStatus = false;
  }    
}
#endif

// the loop function runs over and over again forever
void loop() 
{
#ifdef __ENABLE_TX_LASER__  
  if( oldTx != cTxLaserStatus )
  {
    Serial.print("Tx Status is changed: ");
    
    if( cTxLaserStatus == false)
      Serial.println(" OFF ");
    else
      Serial.println(" ON ");

    oldTx = cTxLaserStatus;
  }
#endif

#ifdef __ENABLE_RX_LASER__  
  if( oldRx != cRxLaserStatus)
  {
    Serial.print("Rx Status is changed: ");
    
    if( cRxLaserStatus == false)
      Serial.println(" Undetected ");
    else
      Serial.println(" Detected ");

    oldRx = cRxLaserStatus;    
  }
#endif  
}

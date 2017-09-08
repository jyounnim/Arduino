import processing.serial.*; 
 
Serial myPort;                       // Create object from Serial class

int val;                                 // Data received from the serial port 
 
void setup() { 
  size(200, 200); 
  frameRate(10); 
  
  println(Serial.list());

  // Open the port that the board is connected to and use the same speed (9600 bps) 
  myPort = new Serial(this, Serial.list()[1], 9600);
  
  fill(204); 
} 

void draw() { 
  if (0 < myPort.available()) {            // If data is available,
    val = myPort.read();                   // read it and store it in val
  } 
  
  background(255);                       // Set background to white

  if (val == 0)  {                       // If the serial value is 0,
    fill(0);                             // set fill to black
  } else {                               // If the serial value is not 0,
    fill(204);                           // set fill to light gray
  } 
  rect(50, 50, 100, 100);
} 
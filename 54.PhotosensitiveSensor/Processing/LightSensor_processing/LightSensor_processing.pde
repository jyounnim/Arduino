// Read data from the serial port and assign it to a variable. Set the fill a 
// rectangle on the screen using the value read from a light sensor connected 
// to the Wiring or Arduino board 
 
import processing.serial.*; 
 
Serial myPort;             // Create object from Serial class
int val;                   // Data received from the serial port 
 
void setup() { 
  size(200, 200); 
  noStroke(); 
  frameRate(10);                // Run 10 frames per second
  
  println(Serial.list()); 
  // Open the port that the board is connected to and use the same speed (9600 bps) 
  myPort = new Serial(this, Serial.list()[1], 9600);

  background(204);               // Clear background
  fill(204); 
} 

void draw() { 
  if (0 < myPort.available()) {    // If data is available to read,
    val = myPort.read();           // read it and store it in val
    val = (255-val)*255/200; //<>// //<>//
    if( val >= 255)
      val = 255;
    println("val = " + val);
    fill(val);                     // Set fill color with the value read
  } 

  rect(50, 50, 100, 100);        // Draw square
} 
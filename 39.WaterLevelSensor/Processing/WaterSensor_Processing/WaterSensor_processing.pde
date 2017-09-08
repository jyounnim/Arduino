import processing.serial.*; 
 
Serial myPort;             // Create object from Serial class

int val=0, level=100, tmp=0; // Data received from the serial port 
  
void setup() { 
  size(200, 450); 
  smooth();

  frameRate(10);                // Run 10 frames per second
  
  println(Serial.list()); 
  // Open the port that the board is connected to and use the same speed (9600 bps) 
  myPort = new Serial(this, Serial.list()[1], 9600);
} 

void draw() { 
  background(204,204,204);
  delay(10);
 
  if(myPort.available() > 0)
  { 
    val = myPort.read();
   
    if( val != tmp )
    {
      if( val >= 100 )
        level = 0;
      else
        level = (100-val);
      
      println("val = " + val);  
      println("level = " + level);
      tmp = val;
    }   
  }
  
  fill(0, 0, 128);
  rect(50, 50+(level*3), 100, 300-(level*3));        // Draw square
  noFill();
  rect(50, 50, 100, 300);
} 
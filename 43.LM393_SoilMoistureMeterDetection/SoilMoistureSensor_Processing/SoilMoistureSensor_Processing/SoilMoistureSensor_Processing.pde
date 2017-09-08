import processing.serial.*; 
 
Serial myPort;             // Create object from Serial class
PrintWriter output;

int val=0, level=100, tmp=0; // Data received from the serial port 
  
void setup() { 
  size(200, 450); 
  smooth();

  frameRate(30);                // Run 10 frames per second
  
  println(Serial.list()); 
  // Open the port that the board is connected to and use the same speed (9600 bps) 
  myPort = new Serial(this, Serial.list()[1], 9600);
  output = createWriter("output.txt");
} 

void draw() { 
  background(204,204,204);
  delay(10);
 
  int dd = day();    // Values from 1 - 31
  int mm = month();  // Values from 1 - 12
  int yyyy = year();   // 2003, 2004, 2005, etc.
  int ss = second();  // Values from 0 - 59
  int mi = minute();  // Values from 0 - 59
  int hh = hour();    // Values from 0 - 23
 
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

      output.println(yyyy + "/" + mm + "/" + dd + " " + hh + ":" + mi + ":" + ss + " -- " + val);       
      output.flush();       
    }   
  }

  // Draw box
  fill(255);
  rect(50, 50, 100, 300);
  line(40, 50, 50, 50);
  line(45, 125, 50, 125);
  line(40, 200, 50, 200);
  line(45, 275, 50, 275);
  line(40, 350, 50, 350); 

  // Write levels
  fill(0);
  textSize(10);
  text(100, 20, 55);  
  text(75, 25, 130);
  text(50, 25, 205);
  text(25, 25, 280);
  text(0, 30, 355);
  
  // Draw water level
  if( val >= 80)
    fill(128, 0, 0);
  else
    fill(0, 0, 128);
  rect(50, 50+(level*3), 100, 300-(level*3));        

  // Write data and time
  fill(0);
  textSize(15);
  text(yyyy + "/" + mm + "/" + dd + " ", 60, 20);
  textSize(20);
  text(hh + ":" + mi + ":" + ss, 60, 40);
  
  // Write water level
  fill(50);
  textSize(32);
  text(val, 85, 390);  
} 
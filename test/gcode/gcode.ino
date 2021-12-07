#include <SoftwareSerial.h>
#include <Wire.h>


void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  Wire.begin(); // join i2c bus (address optional for master)
  pinMode(LED_BUILTIN, OUTPUT);
  
}

byte x = 0;
bool fla = true;

void loop() {
  // put your main code here, to run repeatedly:
  
  Wire.beginTransmission(8); // transmit to device #4
  //Wire.write("M117 cona ");
  if(fla){
    Wire.write("G0 Y50\n"); // sends five bytes
  }else{
    Wire.write("G0 Y-50\n"); // sends five bytes
  }
  fla = !fla;
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on 
  delay(500);                       // wait for half a second
  
  //Wire.write("G0 Y300"); // sends five bytes
  //Wire.write(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off 
  delay(500);


  
  //x++;
  //delay(500);



}

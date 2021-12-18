#include <SoftwareSerial.h>
#include <Wire.h>

#define I2CADDRESS 8
#define WIDTH   100 
#define HEIGHT  100   
#define X_SIDE 5    // distance from the middle to the side (f)
#define Y_SIDE 30     // distance from the bottom (j)
#define X_OBSTACLE 20 // position to avoid the tree storage on the right
#define MOV_SPEED 100  // movemente speed mm/min


#define SETUP_1 "G17"
#define SETUP_2 "G21"
#define SETUP_3 "G91"

String gcode;
int order_gcode = 0;


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
  /*
  Wire.beginTransmission(8); // transmit to device #4
  //Wire.write("M117 Ola ");
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
  delay(500);*/

  
  Serial.println("Write order_gcode number (0-6)"); 
  delay(1000);
  

  delay(1000);
    
    //Serial.println(order_gcode);
    switch  (order_gcode){
      case 0:
        gcode = String(SETUP_1);
        break;
      case 1:
        gcode = String(SETUP_2);
        break;
      case 2:
        gcode = String(SETUP_3);
        break;
      case 3:
        gcode = "G0 X" + String(WIDTH/2) + " F" + String(MOV_SPEED);  
        break;
      case 4:
        gcode = "G1 Y" + String(HEIGHT) + " F" + String(MOV_SPEED);    
        break;
      case 5: //Depois de fazer o buraco
        gcode = "G1 Y-" + String(Y_SIDE) + " F" + String(MOV_SPEED); 
        break;
      case 6:
        gcode = "G1 X-" + String(X_SIDE) + " F" + String(MOV_SPEED); 
        break;
      case 7: //Depois de tirar a terra
        gcode = "G1 X" + String(2*X_SIDE) + " F" + String(MOV_SPEED); 
        break;
      case 8:
        gcode = "G1 Y" + String(HEIGHT-Y_SIDE) + " F" + String(MOV_SPEED); 
        break;
    }
    

    //Serial.println(gcode);
    char char_arr[gcode.length()+1];
    gcode.toCharArray(char_arr,gcode.length()+1);
    char_arr[gcode.length()+1] = '\0';
    
    //Wire.write(char_arr);    
    //Serial.print("Gcode:");
    //Serial.println(char_arr);

    if(order_gcode != 111){
      sendGcode(char_arr);
    }
      
    
    Serial.println(char_arr);
  

}

void updateState(){
    Serial.println("Update");
    Wire.requestFrom(I2CADDRESS,1);
    
    while(Wire.available()){

      order_gcode = Wire.read();
      Serial.print("Resposta: ");
      Serial.println(order_gcode);
    }

}

void sendGcode(char gcode[]){

  Serial.print("[Arduino]: A enviar gcode: ");
  Wire.beginTransmission(I2CADDRESS);
  Wire.write(gcode);

  Wire.endTransmission();

  Serial.print(gcode); 
  Serial.print("[Arduino]: Acabei de enviar ");
}


// -- receber do mestre ramps
void receiveEvent(int bytes){
  char message[30];

  Serial.println("[Arduino]: Recebi mensagem do RAMPS ! ");

  int i = 0;

  while(Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    message[i++] = c;
  }
  message[i] = '\0';

  Serial.print("[Arduino]:");
  Serial.println(message);

  order_gcode++;
  if(order_gcode > 8)
    order_gcode = 3;
}

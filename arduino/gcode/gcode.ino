#include <SoftwareSerial.h>
#include <Wire.h>

#define I2CADDRESS 8
#define WIDTH   100 
#define HEIGHT  100   
#define X_SIDE 5    // distance from the middle to the side (f)
#define Y_SIDE 30     // distance from the bottom (j)
#define X_OBSTACLE 20 // position to avoid the tree storage on the right
#define MOV_SPEED 100  // movemente speed mm/min

#define FREE "fre" //the slave (RAMPS) is free to receive another message
#define OCCUPIED "ocp" //the slave(RAMPS) is occupied and cannot receive another message

//Manipulator defines
#define MOVING 1 //Manipulator is moving
#define PERFURATING 2 //Manipulator is ready to, or perfurating
#define LIFTING 3 //Manipulator is ready to, or lifting after the perfuration
#define SOIL 4 //Manipulator is ready to, or dropping the soil
#define DONE 5 //Manipulator is done


#define SETUP_1 "G17"
#define SETUP_2 "G21"
#define SETUP_3 "G91"
#define GO_HOME "G28"


//GLOBAL VARIABLES
char slave_response[4];

String gcode;
int order_gcode = 3;
bool flag_ready_to_send = true;

//RUN THIS ON SETUP
//Will configure the i2c communication responsable to send the gcode commands
void gcode_configuration(){

  Wire.begin(I2CADDRESS);
}

//Send the gcode command to the slave (the RAMPS)
void sendGcode(char gcode[]){

  Serial.print("[Arduino]: A enviar gcode: ");

  //Start the transmission, send the message to the slave and ends the transmission
  Wire.beginTransmission(I2CADDRESS);
  Wire.write(gcode);
  Wire.endTransmission();

  Serial.println(gcode); 

}

//Checks if the slave (RAMPS) is ready to receive another message or not
void updateState(){
  
  Serial.print("Update: ");

  Wire.requestFrom(I2CADDRESS, 3);
  int i = 0;

  while(Wire.available()){

    slave_response[i] = Wire.read();
    i++;    
  }

  slave_response[i] = '\0';

  Serial.print("Resposta: ");
  Serial.println(slave_response);
}

//Controls the manipulator
//This function will send the gcode commands by i2c to the Ramps
int manipulator_control(int &manipulator_state){

  //Check if RAMPS is free to receive a new command
  updateState();

  //RAMPS is free and send the message
  if ( strcmp( slave_response, FREE) == 0){

    switch  (manipulator_state){
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

    //Send the command to RAMPS
    char char_arr[gcode.length()+1];
    gcode.toCharArray(char_arr,gcode.length()+1);
    char_arr[gcode.length()+1] = '\0';
    sendGcode(char_arr);
    
    //Updates the state of the state_machine
    //WARNING: ALSO NEEDS TO IMPLEMENT THE RESET
    manipulator_state++;
  }

  //Returns the current state of the manipulator
  if (manipulator_state == 1) return MOVING;
  
}



/////////////////////// EXAMPLE TO TEST THE FUNCTIONS CREATED ///////////////////////

void setup() {
  Serial.begin(115200);
  
  gcode_configuration();

  //Wire.begin(I2CADDRESS); // join i2c bus (address optional for master)
  //Wire.onReceive(receiveEvent);// receive codes from RAMPS

  pinMode(LED_BUILTIN, OUTPUT);
}

byte x = 0;
bool flag_first = true;

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





	//Teste para apenas um comando    
	gcode = "G0 X" + String(WIDTH/2)+ " F" + String(MOV_SPEED);

    //Serial.println(gcode);
    char char_arr[gcode.length()+1];
    gcode.toCharArray(char_arr,gcode.length()+1);
    char_arr[gcode.length()+1] = '\0';

    if(flag_ready_to_send and flag_first){
      sendGcode(char_arr);
      flag_ready_to_send = false;
      flag_first = false;
    }
      
    
    //Serial.println(char_arr);
}

/*
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

  flag_ready_to_send = true;
}
*/

#include "Gcode.h"

// RUN THIS ON SETUP
// Will configure the i2c communication responsable to send the gcode commands
void gcode_configuration()
{

  Wire.begin(I2CADDRESS);
}

// Send the gcode command to the slave (the RAMPS)
void sendGcode(char gcode[])
{

  // Start the transmission, send the message to the slave and ends the transmission
  Wire.beginTransmission(I2CADDRESS);
  Wire.write(gcode);
  Wire.endTransmission();
}

// Checks if the slave (RAMPS) is ready to receive another message or not
void updateState(char *message_received)
{

  Wire.requestFrom(I2CADDRESS, MESSAGE_SIZE);
  int i = 0;

  while (Wire.available())
  {

    message_received[i] = Wire.read();
    i++;
  }

  message_received[i] = '\0';
}

////ADICIONAR MAIS UM PARAMETRO QUE É A ALTURA DA BROCA AO CHÃO (LIDO PELO SENSOR + OFFSET QUE POSSA HAVER DA BROCA QUANDO MANIPULADOR ESTÁ EM CIMA)
////FAZER MAIS UM ESTADO QUE DESCE POR EXEMPLO "ALTURA - 10 CM" (PARA SE ATIVAR O MOTOR DA BROCA) E OUTRO ESTADO QUE DESCE 20 CM
////ADICIONAR RETURN PARA ESTA SITUAÇÃO
// Controls the manipulator
// This function will send the gcode commands by i2c to the Ramps
int manipulator_control(int &manipulator_state, int height)
{

  // Create aux variables
  char slave_response[MESSAGE_SIZE + 1]; //+1 due to '\0'

  // Check if RAMPS is free to receive a new command
  updateState(slave_response);

  // RAMPS is free and send the message
  if (strcmp(slave_response, FREE) == 0)
  {

    // Create string variable
    String gcode;

    // Checks the message to send
    switch (manipulator_state)
    {
    case 0:
      gcode = String(SETUP_1);
      break;
    case 1:
      gcode = String(SETUP_2);
      break;
    case 2:
      gcode = String(GO_HOME);
      break;
    case 3:
      gcode = "G0 X" + String(WIDTH / 2);
      break;
    case 4:
      gcode = "G1 Y" + String(HEIGHT + height - 50);
      break;
    case 5: // Start drill
      gcode = "G1 Y" + String(HOLESIZE);
      break;
    case 6: // Stop drill
      gcode = "G1 Y-" + String(HOLESIZE + 20);
      break;
    case 7: // Depois de fazer o buraco
      gcode = "G1 Y-" + String(Y_SIDE);
      break;
    case 8:
      gcode = "G1 X-" + String(X_SIDE);
      break;
    case 9: // Depois de tirar a terra
      gcode = "G1 X" + String(2 * X_SIDE);
      break;
    case 10:
      gcode = "G1 Y" + String(HEIGHT - Y_SIDE);
      break;
    default:
      gcode = "M117 ACABOU";
      break;
    }

    // Convert String to array of chars
    char char_arr[gcode.length() + 1];
    gcode.toCharArray(char_arr, gcode.length() + 1);
    char_arr[gcode.length() + 1] = '\0';

    // Send the command to RAMPS
    sendGcode(char_arr);

    // Updates the state of the state_machine
    // WARNING: ALSO NEEDS TO IMPLEMENT THE RESET (Will depends on the number of states in total)
    if (manipulator_state < 11)
      manipulator_state++;
  }

  // Returns the current state of the manipulator
  // WARINING: NEED TO ADD THE OTHER STATES
  if (manipulator_state == 1)
    return MOVING;
  if (manipulator_state == 11)
    return DONE;
}

// Controls the sweeper the drag the dirt back to the hole
int sweeper_control(int &sweeper_state)
{

  // Create aux variables
  char slave_response[MESSAGE_SIZE + 1]; //+1 due to '\0'

  // Check if RAMPS is free to receive a new command
  updateState(slave_response);

  // RAMPS is free and send the message
  if (strcmp(slave_response, FREE) == 0)
  {

    if (sweeper_state == 0)
    {

      // Create string variable
      String gcode;
      // ADD GCODE COMMAND
      // gcode = String(SETUP_1);

      // Convert String to array of chars
      char char_arr[gcode.length() + 1];
      gcode.toCharArray(char_arr, gcode.length() + 1);
      char_arr[gcode.length() + 1] = '\0';

      // Send the command to RAMPS
      sendGcode(char_arr);

      sweeper_state++;
    }
    else
    {
      sweeper_state = 0;

      return DONE;
    }
  }
  return MOVING;
}

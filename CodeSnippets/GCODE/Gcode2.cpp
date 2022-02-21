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
int manipulator_control(int &manipulator_state, int distance_to_ground)
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

    int distance_to_travel = distance_to_ground - 100;

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
      gcode = "G1 Y" + String(distance_to_travel);
      break;
    case 5:
      // Do nothing, just to wait untill it's done and ready to turn on the drill
      break;
    case 6:
      gcode = "G1 Y" + String(HOLE_HEIGHT);
      break;
    case 7:
      // Do nothing, just to wait the hole to be drilled
      break;
    // VERIFICAR se chega pro buraco
    case 8:                                  // Depois de fazer o buraco
      gcode = "G1 Y-" + String(HOLE_HEIGHT); // Y_SIDE);
      break;
    case 9:
      gcode = "G1 X-" + String(X_SIDE);
      break;
    case 10:
      // Do nothing, just to wait for the drill go to the place for dirt be dropped
      break;
    case 11: // Depois de tirar a terra
      gcode = "G1 X" + String(2 * X_SIDE);
      break;
    case 12:
      // Do nothing, just to wait for the drill go to the place for dirt be dropped on the other side
      break;
    case 13:
      gcode = "G1 Y" + String(distance_to_travel); // HEIGHT-Y_SIDE);
      break;
    case 14:
      // Do nothing, just to wait for the manipulator to reach the highest position
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
    if (manipulator_state == 15)
    {
      manipulator_state = 0;
      return DONE;
    }
    else
    {
      manipulator_state++;
    }
  }

  // Returns the current state of the manipulator
  // WARINING: NEED TO ADD THE OTHER STATES
  if ((manipulator_state == 1) || (manipulator_state == 2) || (manipulator_state == 3) || (manipulator_state == 4) || (manipulator_state == 5))
    return MOVING;
  if ((manipulator_state == 6) || (manipulator_state == 7))
    return PERFURATING;
  if ((manipulator_state == 8) || (manipulator_state == 9) || (manipulator_state == 10))
    return LIFTING;
  if ((manipulator_state == 11))
    return SOIL;
  if ((manipulator_state == 12))
    return MOVING;
  if ((manipulator_state == 13))
    return SOIL;
  if ((manipulator_state == 14))
    return MOVING;
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

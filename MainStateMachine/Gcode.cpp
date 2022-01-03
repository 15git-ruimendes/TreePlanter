#include "Gcode.h"
#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDRESS 8
#define FREE "fre"     // the slave (RAMPS) is free to receive another message
#define OCCUPIED "ocp" // the slave(RAMPS) is occupied and cannot receive another message

void setup_Wire(void)
{
    Wire.begin(I2C_ADDRESS);
}

int send_GCODE(char *buff)
{
    // Serial.print("[Arduino]: A enviar gcode: ");

    // Start the transmission, send the message to the slave and ends the transmission
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(buff);
    int res = Wire.endTransmission();

    return res;
    // Serial.println(buff);
}

bool updateState()
{

    // Serial.print("Update: ");
    char slave_response[4];

    Wire.requestFrom(I2C_ADDRESS, 3);
    int i = 0;

    while (Wire.available())
    {

        slave_response[i] = Wire.read();
        i++;
    }

    slave_response[i] = '\0';

    return strcmp(slave_response, FREE) == 0
    // Serial.print("Resposta: ");
    // Serial.println(slave_response);
}

// Controls the manipulator
// This function will send the gcode commands by i2c to the Ramps
int manipulator_control(int &manipulator_state)
{
    String gcode;
    // Check if RAMPS is free to receive a new command

    // RAMPS is free and send the message
    if (updateState())
    {

        switch (manipulator_state)
        {
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
            gcode = "G0 X" + String(WIDTH / 2) + " F" + String(MOV_SPEED);
            break;
        case 4:
            gcode = "G1 Y" + String(HEIGHT) + " F" + String(MOV_SPEED);
            break;
        case 5: // Depois de fazer o buraco
            gcode = "G1 Y-" + String(Y_SIDE) + " F" + String(MOV_SPEED);
            break;
        case 6:
            gcode = "G1 X-" + String(X_SIDE) + " F" + String(MOV_SPEED);
            break;
        case 7: // Depois de tirar a terra
            gcode = "G1 X" + String(2 * X_SIDE) + " F" + String(MOV_SPEED);
            break;
        case 8:
            gcode = "G1 Y" + String(HEIGHT - Y_SIDE) + " F" + String(MOV_SPEED);
            break;
        }

        // Send the command to RAMPS
        char char_arr[gcode.length() + 1];
        gcode.toCharArray(char_arr, gcode.length() + 1);
        char_arr[gcode.length() + 1] = '\0';
        send_Gcode(char_arr);

        // Updates the state of the state_machine
        // WARNING: ALSO NEEDS TO IMPLEMENT THE RESET
        manipulator_state++;
    }

    // Returns the current state of the manipulator
    if (manipulator_state == 1)
        return MOVING;
}
//
//
//
//
//
//
//
//
//
//

void create_Manipulator_GCODE(int x_axis, int y_axis, char *buff)
{
    ////free(GCODE);
    String aux;
    aux = "G0 X" + String(x_axis) + "Y" + String(y_axis) + " F" + String(MOV_SPEED);
    GCODE = (char *)malloc(sizeof(char) * (aux.length() + 1));
    aux.toCharArray(GCODE, aux.length() + 1);
    GCODE[aux.length() + 1] = '\0';
}

void create_Magazine_GCODE(char *buff)
{
    // free(GCODE);
    String aux;
    aux = "G0 Z" + String(MAG_ANGLE) + " F" + String(MOV_SPEED);
    GCODE = (char *)malloc(sizeof(char) * (aux.length() + 1));
    aux.toCharArray(GCODE, aux.length() + 1);
    GCODE[aux.length() + 1] = '\0';
}

void create_Sweeper_GCODE(int close_Open, char *buff)
{
    if (close_Open == 0) // Close
    {
        // free(GCODE);
        String aux;
        aux = "G0 E" + String(SWEEPER_DISTANCE) + " F" + String(MOV_SPEED);
        GCODE = (char *)malloc(sizeof(char) * (aux.length() + 1));
        aux.toCharArray(GCODE, aux.length() + 1);
        GCODE[aux.length() + 1] = '\0';
    }
    else if (close_Open == 1) // Open
    {
        // free(GCODE);
        String aux;
        aux = "G0 E-" + String(SWEEPER_DISTANCE) + " F" + String(MOV_SPEED);
        GCODE = (char *)malloc(sizeof(char) * (aux.length() + 1));
        aux.toCharArray(GCODE, aux.length() + 1);
        GCODE[aux.length() + 1] = '\0';
    }
}

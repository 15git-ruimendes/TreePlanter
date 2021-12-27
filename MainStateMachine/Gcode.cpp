#include "Gcode.h"
#include "Wire.h"

void setup_Wire(void)
{
    Wire.begin();
}

int send_GCODE(char *buff)
{
    Wire.beginTransmission(0x08);
    Wire.write(buff);

    int success = Wire.endTransmission(true);
    if (!success)
        return 1;
    else
        return 0;
}

int receive_Data(char *buff)
{
    int c = 0;
    Wire.requestFrom(0x08, 64);

    c = Wire.read();

    return (c == 5);
}

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

void calibrate_Manipulator()
{
    Serial.println("Running Setup Operations!!!");
    page = 99;
    state = 100;
    char setup[14] = "G17 G21 G91 \0";
    // send_GCODE(SETUP);
    if (send_GCODE(setup))
        return;
    else
        Serial.println("Error on SetUp Check All Connections");
    state = 400; // Bad request Error Setup State
}

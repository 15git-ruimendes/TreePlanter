#include "Arduino.h"
//#include <Wire.h>

#define BUTTON 1
#define DIST_SENS 2
#define WIDTH 100
#define HEIGHT 100
#define X_SIDE 5            // distance from the middle to the side (f)
#define Y_SIDE 30           // distance from the bottom (j)
#define X_OBSTACLE 20       // position to avoid the tree storage on the right
#define MAG_ANGLE 60        // Rotate magazine
#define SWEEPER_DISTANCE 20 // Amount to close sweeper
#define MOV_SPEED 100       // movemente speed mm/min
#define SETUP "G17 G21 G91"

int drill_X = 0;
int drill_Y = 0;
int state = 0;
int trees = 0;
long double distance = 0;
char *GCODE;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    calibrate_Manipulator();
}

int send_GCODE(char *buff)
{
    Wire.beginTransmission(8);
    Wire.write(buff);

    int success = Wire.endTransmission();
    if (!success)
        return 1;
    else
        return 0;
}

int receive_Data(char *buff)
{
    free(buff);
    buff = (char *)malloc(sizeof(char));
    Wire.requestFrom(8, 1);

    while (Wire.available())
    {
        buff = Wire.read();
    }
    if (buff == "-1")
        return 0;
    else
        return 1;
}

void create_Manipulator_GCODE(int x_axis, int y_axis, char *buff)
{
    free(buff);
    String aux;
    aux = "G0 X" + String(x_axis) + "Y" + String(y_axis) + " F" + String(MOV_SPEED);
    buff = (char *)malloc(sizeof(char) * (aux.length() + 1));
    aux.toCharArray(buff, aux.length() + 1);
    buff[aux.length() + 1] = '\0';
}

void create_Magazine_GCODE(char *buff)
{
    free(buff);
    String aux;
    aux = "G0 Z" + String(MAG_ANGLE) + " F" + String(MOV_SPEED);
    buff = (char *)malloc(sizeof(char) * (aux.length() + 1));
    aux.toCharArray(buff, aux.length() + 1);
    buff[aux.length() + 1] = '\0';
}

void create_Sweeper_GCODE(int close_Open, char *buff)
{
    if (close_Open == 0) // Close
    {
        free(buff);
        String aux;
        aux = "G0 E" + String(SWEEPER_DISTANCE) + " F" + String(MOV_SPEED);
        buff = (char *)malloc(sizeof(char) * (aux.length() + 1));
        aux.toCharArray(buff, aux.length() + 1);
        buff[aux.length() + 1] = '\0';
    }
    else if (close_Open == 1) // Open
    {
        free(buff);
        String aux;
        aux = "G0 E-" + String(SWEEPER_DISTANCE) + " F" + String(MOV_SPEED);
        buff = (char *)malloc(sizeof(char) * (aux.length() + 1));
        aux.toCharArray(buff, aux.length() + 1);
        buff[aux.length() + 1] = '\0';
    }
}

void display_LCD(int page_Number)
{
}

void calibrate_Manipulator()
{
    Serial.println("Running Setup Operations!!!");
    display_LCD(99);
    if (send_GCODE(SETUP))
        return;
    else
        Serial.println("Error on SetUp Check All Connections");
    state = 100; // Error Setup State
}

void update_Position(int x, int y)
{
}

int get_NumberOfTrees()
{
}

long double read_Distance()
{
    long double preVal = analogRead(DIST_SENS);
    return 29.998 * pow(preVal, -1.173);
}

void turn_On_Drill()
{
}

void loop()
{
    if (state == 100)
        return;

    if (state == 0) // Read number of trees in machine and maybe ask for clod(torrão) size
    {
        display_LCD(0);
        Serial.println("Please Reload and/or Enter Number of Trees in Magazine");
        get_NumberOfTrees();
    }
    else if (state == 1) // Wait for start command
    {
        display_LCD(1);
        Serial.println("Waiting for Start Command");
    }
    else if (state == 2) // Move. Don't Drill !!
    {
        display_LCD(2);
        Serial.println("Moving!!");
        create_Sweeper_GCODE(0, GCODE);
        send_GCODE(GCODE);
        create_Manipulator_GCODE(WIDTH / 2, Y_SIDE + distance - 5, GCODE);
        send_GCODE(GCODE);
    }
    else if (state == 3) // Move. Don't Drill !!
    {
        display_LCD(2);
        Serial.println("Moving!!");
        create_Sweeper_GCODE(0, GCODE);
        send_GCODE(GCODE);
        create_Manipulator_GCODE(WIDTH / 2, Y_SIDE + distance - 5, GCODE);
        send_GCODE(GCODE);
    }
    else if (state == 4) // Drill and move
    {
        display_LCD(3);
        Serial.println("Drilling!!");
        create_Manipulator_GCODE(0, distance + 15, GCODE);
        send_GCODE(GCODE);
        turn_On_Drill();
    }
    else if (state == 5) // Place Tree
    {
        display_LCD(4);
        Serial.println("Planting!!");
        create_Magazine_GCODE(GCODE);
        send_GCODE(GCODE);
    }
    else if (state == 6) // Activate sweeper
    {
        display_LCD(5);
        Serial.println("Sweeping!!");
        create_Sweeper_GCODE(0, GCODE);
        send_GCODE(GCODE);
    }
}

#include "Arduino.h"

#define BUTTON 1
#define DIST_SENS 2

int drill_X = 0;
int drill_Y = 0;
int state = 0;
int trees = 0;
long double distance = 0;
char GCODE[256];

void setup()
{
    Serial.begin(9600);
}

int send_GCODE(char *buff)
{
    bool success;
    if (success)
        return 1;
    else
        return 0;
}

int receive_Data(char *buff)
{
    bool success;
    if (success)
        return 1;
    else
        return 0;
}

void create_Manipulator_GCODE(int x_axis, int y_axis, char *buff)
{
}

void create_Magazine_GCODE(char *buff)
{
}

void create_Sweeper_GCODE(char *buff)
{
}

void display_LCD(int page_Number)
{
}

void calibrate_Manipulator()
{
}

void update_Position()
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

void loop()
{
    if (state == 0)
    {
        display_LCD(0);
    }
    else if (state == 0 && (digitalRead(BUTTON)))
    {
        display_LCD(1);
        trees = get_NumberOfTrees();
        state = 1;
    }
    else if (state == 1 && trees != 0)
    {
        distance = read_Distance();
        create_Manipulator_GCODE(distance, 0, GCODE);
        if (send_GCODE(GCODE))
            state = 2;
        else
        {
            display_LCD(3);
            Serial.print("Communication error!!!!\n");
        }
    }
    else if (trees == 0 && state == 1)
    {
        display_LCD(2);
        Serial.print("No Trees Warning!!!!\n");
        state = 0;
    }
    else if (state == 2)
    {
        create_Magazine_GCODE(GCODE);
        if (send_GCODE(GCODE))
            state = 3;
        else
        {
            display_LCD(3);
            Serial.print("Communication error!!!!\n");
        }
    }
    else if (state == 3)
    {
        create_Sweeper_GCODE(GCODE);
        if (send_GCODE(GCODE))
            state = 3;
        else
        {
            display_LCD(3);
            Serial.print("Communication error!!!!\n");
        }
    }
}

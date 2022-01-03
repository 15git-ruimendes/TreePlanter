#include "Arduino.h"

#include "LCD_display.h"
#include "Gcode.h"

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
int ControllerState = 0;
int prev_ControllerState = 0;
int trees = 2;
long double distance = 0;
char *GCODE;
char *RECV;
int page = 1;

void setup()
{
    setup_LCD();
    setup_Wire();

    Serial.begin(9600);

    pinMode(41, INPUT);
    digitalWrite(41, HIGH);
    pinMode(39, INPUT);
    digitalWrite(39, HIGH);
    pinMode(35, INPUT);
    digitalWrite(35, HIGH);

    calibrate_Manipulator();
}

void calibrate_Manipulator()
{
    Serial.println("Running Setup Operations!!!");
    page = 99;
    ControllerState = 100;
    char setup[14] = "G17 G21 G91 \0";
    // send_GCODE(SETUP);
    if (send_GCODE(setup))
        return;
    else
        Serial.println("Error on SetUp Check All Connections");
    ControllerState = 400; // Bad request Error Setup ControllerState
}

bool get_NumberOfTrees()
{
    trees = 10;
    return true * (ControllerState == 0);
}

long double read_Distance()
{
    long double preVal = analogRead(DIST_SENS);
    return 29.998 * pow(preVal, -1.173);
}

void turn_On_Drill()
{
}

bool read_Barrier_Sens()
{
    return true;
}

void loop()
{
    display_LCD(page);

    // Error ControllerStates

    // if (ControllerState == 400)

    // Setup ControllerStates

    if (ControllerState = 100)
    {
        if (receive_Data(RECV))
            ControllerState = 0;
    }

    // Action ControllerStates

    if (ControllerState == 0) // Read number of trees in machine and maybe ask for clod(torrão) size
    {
        page = 0;
        Serial.println("Please Reload and/or Enter Number of Trees in Magazine");
        ControllerState = 70;
        prev_ControllerState = 0;
    }
    else if (ControllerState == 1) // Wait for start command
    {
        page = 1;
        Serial.println("Waiting for Start Command");
        ControllerState = 70;
        prev_ControllerState = 1;
    }
    else if (ControllerState == 2) // Move. Don't Drill !!
    {
        page = 2;
        Serial.println("Moving!!");
        create_Sweeper_GCODE(0, GCODE);
        send_GCODE(GCODE);
        ControllerState = 50;
        prev_ControllerState = 2;
    }
    else if (ControllerState == 3) // Move. Don't Drill !!
    {
        create_Manipulator_GCODE(WIDTH / 2, Y_SIDE + distance - 5, GCODE);
        send_GCODE(GCODE);
        ControllerState = 50;
        prev_ControllerState = 3;
    }
    else if (ControllerState == 4) // Drill and move
    {
        page = 3;
        Serial.println("Drilling!!");
        create_Manipulator_GCODE(0, distance + 15, GCODE);
        send_GCODE(GCODE);
        turn_On_Drill();
        ControllerState = 50;
        prev_ControllerState = 4;
    }
    else if (ControllerState == 5) // Move. Don't Drill !!
    {
        page = 2 
        Serial.println("Moving!!");
        create_Manipulator_GCODE(0, Y_SIDE + distance + 15, GCODE);
        send_GCODE(GCODE);
        ControllerState = 50;
        prev_ControllerState = 5;
    }
    else if (ControllerState == 6) // Place Tree
    {
        page = 4;
        Serial.println("Planting!!");
        create_Magazine_GCODE(GCODE);
        send_GCODE(GCODE);
        ControllerState = 60;
        prev_ControllerState = 6;
    }
    else if (ControllerState == 7) // Activate sweeper
    {
        page = 5;
        Serial.println("Sweeping!!");
        create_Sweeper_GCODE(0, GCODE);
        send_GCODE(GCODE);
        ControllerState = 50;
        prev_ControllerState = 7;
    }

    // Transitions
    /*if (ControllerState == 0 && get_NumberOfTrees())
    {
        ControllerState = 1;
    }*/

    // Waiting ControllerStates
    // ControllerState 50 - Waiting for response from RAMPS
    // ControllerState 60 - Waiting for tree drop (10sec) then repeat step
    // ControllerState 70 - Waiting for LCD or Serial Comms
    if (ControllerState == 50)
    {
        delay(1000);
        if (receive_Data(RECV))
        {
            ControllerState = (prev_ControllerState) * (prev_ControllerState < 7) + 1 * (trees > 0);
            delay(1000);
        }
        else
        {
            send_GCODE(GCODE);
            delay(1000);
        }
    }
    if (ControllerState == 60)
    {
        if (read_Barrier_Sens())
        {
            trees--;
            ControllerState = ((prev_ControllerState) * (prev_ControllerState < 7) + 1) * (trees > 0);
        }
    }
}

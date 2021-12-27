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
int state = 0;
int prev_State = 0;
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

void update_Position(int x, int y)
{
}

bool get_NumberOfTrees()
{
    trees = 10;
    return true * (state == 0);
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

    // Error States

    // if (state == 400)

    // Setup States

    if (state = 100)
    {
        if (receive_Data(RECV))
            state = 0;
    }

    // Action States

    if (state == 0) // Read number of trees in machine and maybe ask for clod(torrão) size
    {
        page = 0;
        Serial.println("Please Reload and/or Enter Number of Trees in Magazine");
        state = 70;
        prev_State = 0;
    }
    else if (state == 1) // Wait for start command
    {
        page = 1;
        Serial.println("Waiting for Start Command");
        state = 70;
        prev_State = 1;
    }
    else if (state == 2) // Move. Don't Drill !!
    {
        page = 2;
        Serial.println("Moving!!");
        create_Sweeper_GCODE(0, GCODE);
        send_GCODE(GCODE);
        state = 50;
        prev_State = 2;
    }
    else if (state == 3) // Move. Don't Drill !!
    {
        create_Manipulator_GCODE(WIDTH / 2, Y_SIDE + distance - 5, GCODE);
        send_GCODE(GCODE);
        state = 50;
        prev_State = 3;
    }
    else if (state == 4) // Drill and move
    {
        page = 3;
        Serial.println("Drilling!!");
        create_Manipulator_GCODE(0, distance + 15, GCODE);
        send_GCODE(GCODE);
        turn_On_Drill();
        state = 50;
        prev_State = 4;
    }
    else if (state == 5) // Move. Don't Drill !!
    {
        page = 2 Serial.println("Moving!!");
        create_Manipulator_GCODE(0, Y_SIDE + distance + 15, GCODE);
        send_GCODE(GCODE);
        state = 50;
        prev_State = 5;
    }
    else if (state == 6) // Place Tree
    {
        page = 4;
        Serial.println("Planting!!");
        create_Magazine_GCODE(GCODE);
        send_GCODE(GCODE);
        state = 60;
        prev_State = 6;
    }
    else if (state == 7) // Activate sweeper
    {
        page = 5;
        Serial.println("Sweeping!!");
        create_Sweeper_GCODE(0, GCODE);
        send_GCODE(GCODE);
        state = 50;
        prev_State = 7;
    }

    // Transitions
    /*if (state == 0 && get_NumberOfTrees())
    {
        state = 1;
    }*/

    // Waiting States
    // State 50 - Waiting for response from RAMPS
    // State 60 - Waiting for tree drop (10sec) then repeat step
    // State 70 - Waiting for LCD or Serial Comms
    if (state == 50)
    {
        delay(1000);
        if (receive_Data(RECV))
        {
            state = (prev_State) * (prev_State < 7) + 1 * (trees > 0);
            delay(1000);
        }
        else
        {
            send_GCODE(GCODE);
            delay(1000);
        }
    }
    if (state == 60)
    {
        if (read_Barrier_Sens())
        {
            trees--;
            state = ((prev_State) * (prev_State < 7) + 1) * (trees > 0);
        }
    }
}

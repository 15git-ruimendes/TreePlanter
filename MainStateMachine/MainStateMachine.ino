#include "Arduino.h"
#include "Wire.h"
#include "U8glib.h"

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

U8GLIB_ST7920_128X64_1X u8g(23, 16, 17);

int drill_X = 0;
int drill_Y = 0;
int state = 0;
int prev_State = 0;
int trees = 2;
long double distance = 0;
char *GCODE;
char *RECV;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    calibrate_Manipulator();
}

int send_GCODE(char *buff)
{
    Serial.println(buff);
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

void display_LCD(int page_Number)
{
}

void calibrate_Manipulator()
{
    Serial.println("Running Setup Operations!!!");
    display_LCD(99);
    char setup[14] = "G17 G21 G91 \0";
    // char teste[7] = "Hello";
    // send_GCODE(SETUP);
    if (send_GCODE(setup))
        return;
    else
        Serial.println("Error on SetUp Check All Connections");
    state = 100; // Error Setup State
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

    Serial.println(state);
    // Error States

    // if (state == 100)

    // Action States

    if (state == 0) // Read number of trees in machine and maybe ask for clod(torrão) size
    {
        display_LCD(0);
        Serial.println("Please Reload and/or Enter Number of Trees in Magazine");
        state = 50;
        prev_State = 0;
    }
    else if (state == 1) // Wait for start command
    {
        display_LCD(1);
        Serial.println("Waiting for Start Command");
        create_Sweeper_GCODE(0, GCODE);
        send_GCODE(GCODE);
        state = 50;
        prev_State = 1;
    }
    else if (state == 2) // Move. Don't Drill !!
    {
        display_LCD(2);
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
        display_LCD(3);
        Serial.println("Drilling!!");
        create_Manipulator_GCODE(0, distance + 15, GCODE);
        send_GCODE(GCODE);
        turn_On_Drill();
        state = 50;
        prev_State = 4;
    }
    else if (state == 5) // Move. Don't Drill !!
    {
        display_LCD(2);
        Serial.println("Moving!!");
        create_Manipulator_GCODE(0, Y_SIDE + distance + 15, GCODE);
        send_GCODE(GCODE);
        state = 50;
        prev_State = 5;
    }
    else if (state == 6) // Place Tree
    {
        display_LCD(4);
        Serial.println("Planting!!");
        create_Magazine_GCODE(GCODE);
        send_GCODE(GCODE);
        state = 60;
        prev_State = 6;
    }
    else if (state == 7) // Activate sweeper
    {
        display_LCD(5);
        Serial.println("Sweeping!!");
        create_Sweeper_GCODE(0, GCODE);
        send_GCODE(GCODE);
        state = 50;
        prev_State = 7;
    }

    // Transitions
    if (state == 0 && get_NumberOfTrees())
    {
        state = 1;
    }

    // Waiting States
    // State 50 - Waiting for response from RAMPS
    // State 60 - Waiting for tree drop (10sec) then repeat step
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

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

U8GLIB_ST7920_128X64_1X u8g(23, 17, 16);

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
    Serial.begin(9600);
    Wire.begin();
    pinMode(41,INPUT);
    digitalWrite(41,HIGH);
    pinMode(39,INPUT);
    digitalWrite(39,HIGH);
    pinMode(35,INPUT);
    digitalWrite(35,HIGH);
    u8g.setColorIndex(1);
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

    if (page_Number == 1)
        page_1();
    else
        page_2();
}

void page_1()
{
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(0, 10, "Tree Planter!");
    u8g.drawLine(0, 15, 80, 15);
    u8g.drawLine(80, 0, 80, 64);
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(85, 10, "# Trees");
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(100, 25, "10");
    u8g.drawFrame(80, 28, 48, 18);
    u8g.drawStr(81, 40, "Reaload?");
    u8g.drawBox(80, 46, 48, 18);
    u8g.setColorIndex(0);
    u8g.drawStr(87, 55, "Start?");
    u8g.setColorIndex(1);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(5, 30, "Waiting");
    u8g.drawStr(5, 45, "For");
    u8g.drawStr(5, 60, "Start...");
}

void page_2()
{
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(0, 10, "Tree Planter!");
    u8g.drawLine(0, 15, 80, 15);
    u8g.drawLine(80, 0, 80, 64);
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(85, 10, "# Trees");
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(100, 25, "10");
    u8g.drawBox(80, 28, 48, 18);
    u8g.setColorIndex(0);
    u8g.drawStr(81, 40, "Reaload?");
    u8g.setColorIndex(1);
    u8g.drawFrame(80, 46, 48, 18);
    u8g.drawStr(87, 55, "Start?");
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(5, 30, "Waiting");
    u8g.drawStr(5, 45, "For");
    u8g.drawStr(5, 60, "Start...");
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
    u8g.firstPage();
    do
    {
        display_LCD(page);
    } while (u8g.nextPage());

    Serial.println(state);
    // Error States

    // if (state == 400)

    //Setup States

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
        page = 2
        Serial.println("Moving!!");
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

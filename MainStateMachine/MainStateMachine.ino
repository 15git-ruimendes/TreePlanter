#include "Arduino.h"
#include "LCD_display.h"
#include "Gcode.h"
#include "U8glib.h"

#define DIST_SENS 2
#define TREE_LIMIT 6
#define DELAY 250
#define ENC_BUTTON 18
#define ENC_RIGHT 2
#define ENC_LEFT 3

int trees = 99;
long double distance = 0;
int page = 1;
int movement, state = 3, res, top_page = 0, reload = 0;

static unsigned long last_interrupt = 0;
U8GLIB_ST7920_128X64_1X u8g(23, 17, 16);
display lcd;
void setup()
{

    setup_Wire();
    lcd.setup_LCD(u8g);
    Serial.begin(115200);

    pinMode(ENC_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENC_BUTTON), click, FALLING);
    pinMode(ENC_RIGHT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENC_RIGHT), right, FALLING);
    pinMode(ENC_LEFT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENC_LEFT), left, FALLING);

    // while (!calibrate_Manipulator());
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

void click()
{
    if (millis() - last_interrupt < DELAY)
    {
      last_interrupt = millis();
      return;
    }
    last_interrupt = millis();
    Serial.println("button");
    if (state != 3 && top_page == 0)
        state = 401;
    else if (state == 3 && top_page)
    {
        page = 4; // Reload page
        reload = 1;
    }
    else if (state == 3 && reload && (trees > 0))
    {
        page = 1;
        top_page = 0;
    }
    else if (state == 3 && top_page == 0)
        state = 4;
}
void right()
{
    if (millis() - last_interrupt < DELAY)
    {
        last_interrupt = millis();
        return;
    }
    last_interrupt = millis();
    Serial.println("right");
    if (!reload)
        top_page = 1;
    else if (reload)
        trees += (trees < TREE_LIMIT);
}
void left()
{
    if (millis() - last_interrupt < DELAY)
    {
        last_interrupt = millis();
        return;
    }
    last_interrupt = millis();
    Serial.println("left");
    if (!reload)
        top_page = 0;
    else if (reload)
        trees -= (trees > 0);
    
}

void loop()
{
    char trees_a[3];
    trees_a[0] = trees/10 + '0';
    trees_a[1] = trees%10 + '0';
    Serial.println(trees_a);
    Serial.println(top_page);
     u8g.firstPage();
    do
    {
      lcd.display_LCD(1,u8g,top_page,trees);
    } while (u8g.nextPage());
    /*if (state < 3)
        movement = manipulator_control(state);
    if (state > 4)
        movement = manipulator_control(state);*/

    switch (state)
    {
    case 0:
        page = 100;
        break;
    case 1:
        page = 100;
        break;
    case 2:
        page = 100;
        break;
    case 3:
        page = 1;
        break;
    case 4:
        page = 2;
        break;
    case 5:
        page = 3;
        break;
    case 6:
        page = 2;
        break;
    case 7:
        page = 4;
        break;
    /*case 8://Sweepers
        page = 5;*/
    default:
        break;
    }

    /*// Error ControllerStates

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
    }

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
    }*/
}

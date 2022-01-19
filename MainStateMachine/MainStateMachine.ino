#include <TB9051FTGMotorCarrier.h>

#include "Servo.h"
#include "Arduino.h"
#include "LCD_display.h"
#include "Gcode.h"
#include "U8glib.h"
#include <SharpIR.h>

#define TREE_LIMIT 6
#define TREE_DELAY 5000
#define DELAY 250

#define BUZZERPIN 7

#define DIST_SENS 2
#define BARRIER A0

#define ENC_BUTTON 18
#define ENC_RIGHT 2
#define ENC_LEFT 3

#define DRILL_EN 10
#define DRILL_PWM1 5
#define DRILL_PWM2 6

#define ServoPin 9

int trees = 0, tree_dropped = 0;
long double distance = 0;
int page = 1;
int movement, state = 0, res, top_page = 0, reload = 0, prev_state = 1;
static unsigned long last_interrupt = 0, prev_time = 0,treeWait = 0;

float speed = 0;
static TB9051FTGMotorCarrier driver{DRILL_PWM1, DRILL_PWM2};
U8GLIB_ST7920_128X64_1X u8g(23, 17, 16);
display lcd;
Servo treeDrop;
void setup()
{  
    treeDrop.attach(ServoPin);
    driver.enable();
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

void turn_Drill(int on)
{
    if (on)
    {
        for (int i = 0; i < 10; i++)
        {
            driver.setOutput(speed);
            speed += 0.1;
            delay(100);
        }
    }
    if (on == -1)
    {
        for (int i = 0; i < 10; i++)
        {
            driver.setOutput(speed);
            speed -= 0.1;
            delay(100);
        }

        // Ramp up to full reverse throttle
        for (int i = 0; i < 5; i++)
        {
            driver.setOutput(speed);
            speed -= 0.1;
            delay(100);
        }
    }
    if (on == 0)
    {
        if (speed < 0)
        {
            for (auto i{0}; i < 10; i++)
            {
                if (speed == 0)
                    break;
                driver.setOutput(speed);
                speed += 0.1;
                delay(100);
            }
            speed = 0;
            driver.setOutput(speed);
        }
        else
        {
            for (int i = 0; i < 10; i++)
            {
                if (speed == 0)
                    break;
                driver.setOutput(speed);
                speed -= 0.1;
                delay(100);
            }
            speed = 0;
            driver.setOutput(speed);
        }
    }
}

bool read_Barrier_Sens()
{
    if (analogRead(BARRIER) > 0)
    {
        return false;
    }
    else if (analogRead(BARRIER) == 0)
    {
        if (millis() - prev_time > 10)
        {
            prev_time = millis();
            Serial.println("Droped");
            return true;
        }
        else
        {
            prev_time = millis();
            return false;
        }
    }
}

void click()
{
    if (millis() - last_interrupt < DELAY) // Debounce
    {
        last_interrupt = millis();
        return;
    }
    last_interrupt = millis();
    Serial.println("button");
    if (state != 3 && state != 10)
        state = 401;
    else if (state == 3 && top_page)
    {
        state = 10; // Reload page
    }
    else if (state == 10 && (trees > 0))
    {
        state = 3;
        top_page = 0;
    }
    else if (state == 3 && top_page == 0)
        state = 4;
}
void right()
{
    if (millis() - last_interrupt < DELAY) // Debounce
    {
        last_interrupt = millis();
        return;
    }
    last_interrupt = millis();
    Serial.println("right");
    if (page != 0)
        top_page = 1;
    else if (page == 0)
        trees += (trees < TREE_LIMIT);
}
void left()
{
    if (millis() - last_interrupt < DELAY) // Debounce
    {
        last_interrupt = millis();
        return;
    }
    last_interrupt = millis();
    Serial.println("left");
    if (page != 0)
        top_page = 0;
    else if (page == 0)
        trees -= (trees > 0);
}

void loop()
{

    bool treeDrop = read_Barrier_Sens();

    //LCD functions
    u8g.firstPage();
    do
    {
        lcd.display_LCD(page, u8g, top_page, trees);
    } while (u8g.nextPage());

    //Serial Communications Functions
    if (prev_state != state)
    {
        prev_state = state;
        if (state < 10)
        {
            Serial.print("St: 00");
            Serial.println(state);
        }
        else if (state < 100)
        {
            Serial.print("St: 0");
            Serial.println(state);
        }
        else
        {
            Serial.print("St: ");
            Serial.println(state);
        }
        Serial.print("Tr: 00");
        Serial.println(trees);
    }
    while (Serial.available() > 0)
    {
        char res = (char)Serial.read();
        Serial.print(res);
        if (state == 3 && res == 'R')
            state = 10;
        if ((state == 3 || state == 10) && trees > 0 && res == 'B')
            state = 4;
        if (state != 3 && res == 'S')
            state = 401;
        else if (state == 3 || state == 10)
        {
            trees = (int)res - 48;
            Serial.println(trees);
        }
    }

    //States to Stop Manipulator Movement
    if (state < 3)
        movement = manipulator_control(state);
    if (state > 4)
        movement = manipulator_control(state);

    //Error State
    if (state == 401) // infinite tree not found
        page = 101;

    if (state == 7 && millis() - treeWait > TREE_DELAY){
      treeDrop.write(45);
      treeWait = millis();
    }

    //Reload State
    if (state == 3 && trees == 0)
        state = 10;

    //Testing
    if (state >= 0 && state <= 7)
    {
        delay(1000);
        state++;
    }

    switch (state)
    {
    case 0: // Calibrate
        page = 100;
        break;
    case 1: // Calibrate
        page = 100;
        break;
    case 2: // Calibrate
        page = 100;
        break;
    case 3: // Waiting for start
        page = 1;
        distance = read_Distance();
        break;
    case 4: // Moving drill
        page = 2;
        break;
    case 5: // Drilling
        page = 3;
        break;
    case 6: // Moving Drill
        page = 2;
        break;
    case 7: // Drop tree
        if (treeDrop)
        {
            trees--;
            // Stop Servo
            // Continue Machine State
        }
        page = 4;
        break;
    case 8: // Sweepers
        page = 5;
        break;
    case 10: // Reload
        page = 0;
        break;
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

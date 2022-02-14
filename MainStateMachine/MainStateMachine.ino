#include <TB9051FTGMotorCarrier.h>

#include "Servo.h"
#include "Arduino.h"
#include "LCD_display.h"
#include "Gcode.h"
#include "U8glib.h"
#include <SharpIR.h>

#define TREE_LIMIT 6
#define TREE_DELAY 1000
#define DELAY 250

#define BUZZERPIN 7

#define DIST_SENS A1
#define DIST_SENS_MODEL 1080
#define BARRIER 19

#define ENC_BUTTON 18
#define ENC_RIGHT 2
#define ENC_LEFT 3

#define DRILL_EN 10
#define DRILL_PWM1 5
#define DRILL_PWM2 6
#define DRILL_MOTOR 5

#define UP 0
#define DOWN 180
#define TIMER_MS 1500

#define ServoPin 9

SharpIR detect(SharpIR::GP2Y0A41SK0F, A1);


//Servo
Servo sweeper_left, sweeper_right;
int pin_left = 11, pin_right = 12;
int servo_state = 0, rotator_state = 0;
unsigned long timer_1 = 0;
unsigned long timer_2 = 0;

int trees = 0, pos = 0;
bool treeDropped = false;
float distance = 0;
int page = 1;
int movement, manipulator_state = 0, state = 0, res, top_page = 0, reload = 0, prev_state = 1;
static unsigned long last_interrupt = 0, prev_time = 0, treeWait = 0;

float speed = 0;
static TB9051FTGMotorCarrier driver{DRILL_PWM1, DRILL_PWM2};
U8GLIB_ST7920_128X64_1X u8g(23, 17, 16);
display lcd;
Servo treeDrop;
void setup()
{
    treeDrop.attach(ServoPin);
    driver.enable();
    gcode_configuration();
    lcd.setup_LCD(u8g);
    Serial.begin(115200);

    pinMode(ENC_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENC_BUTTON), click, FALLING);
    pinMode(ENC_RIGHT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENC_RIGHT), right, FALLING);
    pinMode(ENC_LEFT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENC_LEFT), left, FALLING);

    pinMode(BARRIER,INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BARRIER),read_Barrier_Sens,LOW);
    //attachInterrupt(digitalPinToInterrupt(BARRIER),read_Barrier_Sens,
    // while (!calibrate_Manipulator());

    //Servo
    sweeper_left.attach(pin_left);
    sweeper_right.attach(pin_right);
}

void start_timer(unsigned long &timer){
    timer = millis();
}

bool check_timer(unsigned long timer, unsigned long interval){
  return ((millis()-timer) >= interval);
}

void sweeper_sweep(){
  sweeper_left.write(UP);
  sweeper_right.write(DOWN);
}

void sweeper_home(){
  sweeper_left.write(DOWN);
  sweeper_right.write(UP);
}

float read_Distance()
{
    return detect.getDistance();
}

void turn_Drill(int on)
{
    /*if (on)
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
    }*/

    if (on) 
      digitalWrite(DRILL_MOTOR,HIGH);
    else 
      digitalWrite(DRILL_MOTOR,LOW);
}

bool read_Barrier_Sens()
{
  /*if(millis() - last_interrupt < DELAY || treeDropped == true){
    return;
  }*/
  if (state == 7 && rotator_state == 0)
  {
    treeDropped = true;
    //Serial.println("caiou Crl");
  }
    
  else 
    treeDropped = false;
  
    
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
    if (state != 3 && state != 10){
      //state = 401;
    }
        //state = 401;
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

    
    //treeDropped = read_Barrier_Sens();
    // LCD functions
    u8g.firstPage();
    do
    {
        lcd.display_LCD(page, u8g, top_page, trees);
    } while (u8g.nextPage());

    // Serial Communications Functions
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
    // States to Stop Manipulator Movement
    if (state < 3 && manipulator_state < 3)
    {
        manipulator_control(manipulator_state, int(distance));
        state = manipulator_state;
    }
    if (state >= 4 && state != 7&& state != 10 && state != 401)
    {
        manipulator_control(manipulator_state, int(distance));
        switch (manipulator_state)
        {
        case 3:
            state = 4;
            break;
        case 4:
            state = 4;
            break;
        case 5:
            state = 4;
            break;
        case 6:
            state = 5;
            break;
        case 8:
            state = 7;
            break;
        default:
            break;
        }
    }
    // Recicle State
    if (state == 8 && manipulator_control(manipulator_state, int(distance)) == DONE)
    {
        manipulator_state = 3;
        state = 3;
    }

    // Error State
    if (state == 401){
      // infinite tree not found
      manipulator_state = 401;
      page = 101;
    }
        

    // Retry to deploy a tree from the magazine if tree is not detected within TREE_DELAY ms
    if (state == 7 && (millis() - treeWait > TREE_DELAY))
    {
      //Starts the rotator machine state
      
      rotator_state = 1;
        
      treeWait = millis();
    }

    // Reload State
    if (state == 3 && trees == 0){
        state = 10;
        distance = read_Distance()*20;
        if (distance > 120){
          distance = 12;
        }
        //Serial.println((int)distance);
    }
    // Testing
    /*if (state >= 0 && state <= 6)
    {
        delay(1000);
        state++;
    }*/

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
        distance = read_Distance()*20;
        if (distance > 120){
          distance = 12;
        }
        //Serial.println((int)distance);
        break;
    case 4: // Moving drill
        page = 2;
        break;
    case 5: // Drilling
        turn_Drill(1);
        page = 3;
        break;
    case 6: // Moving Drill
        page = 2;
        treeDropped = false;
        break;
    case 7: // Drop tree
        turn_Drill(0);
        if (treeDropped)
        {
            trees--;
            state++;
            // Stop Servo
            // Continue Machine State
            // Comment this line when sweepers are built and
            // system to drop dirt is built
            manipulator_state = 8;
            servo_state = 1;
        }
        page = 4;
        break;
    case 8: // Sweepers (not built)
        page = 5;
        treeDropped = false;
        
        break;
    case 10: // Reload
        page = 0;
        break;
    default:
        break;
    }

    //Servo state machine
    switch(servo_state){
      
    case 0:
      //Check if this line is not fucking everything up
        
      break;
    case 1:
        start_timer(timer_1);
       
        sweeper_sweep();
        servo_state = 2;
        
        //Serial.println("Sweeping");
      break;
    case 2:
      if(check_timer(timer_1,TIMER_MS)){

        sweeper_home();
        servo_state = 0;
        //Serial.println("Returning home");
      }
      break;
  }

  //Rotator state
  switch(rotator_state){
      case 0:
        break;
      case 1:
        treeDrop.write(45);
        start_timer(timer_2);
        rotator_state = 2;
        break;
      case 2:
        if(check_timer(timer_2,124)){
          
          treeDrop.write(90);
          rotator_state = 0;
          //Serial.println("Rota");
        }
        break;
    }


    
}

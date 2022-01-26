#ifndef Gcode_h
#define Gcode_h

// LIBRARIES
#include <Arduino.h>
#include <Wire.h> //I2C communication

// DEFINES
#define I2CADDRESS 8
#define WIDTH 200
#define HEIGHT 200
#define X_SIDE 5      // distance from the middle to the side (f)
#define Y_SIDE 30     // distance from the bottom (j)
#define X_OBSTACLE 20 // position to avoid the tree storage on the right
#define MOV_SPEED 100 // movemente speed mm/min
#define HOLESIZE 150

// Slave messages
#define FREE "fre"     // the slave (RAMPS) is free to receive another message
#define OCCUPIED "ocp" // the slave(RAMPS) is occupied and cannot receive another message
#define MESSAGE_SIZE 3 // the size of the message the master is expected to receive

// Manipulator returns messages
#define MOVING 1      // Manipulator is moving
#define PERFURATING 2 // Manipulator is ready to, or perfurating
#define LIFTING 3     // Manipulator is ready to, or lifting after the perfuration
#define SOIL 4        // Manipulator is ready to, or dropping the soil
#define DONE 5        // Manipulator is done

// Setup needed to run
#define SETUP_1 "G17"
#define SETUP_2 "G21"
#define SETUP_3 "G91"
#define GO_HOME "G28"

// RUN THIS ON SETUP
// Will configure the i2c communication responsable to send the gcode commands
// Address used is the defined in I2CADDRESS
void gcode_configuration();

// Send the gcode command to the slave (the RAMPS)
// Uses the defined I2CADDRESS
// gcode[] -> The message to send (max size of single message is 32 bytes)
void sendGcode(char gcode[]);

// Checks if the slave (RAMPS) is ready to receive another message or not
// Uses the defined I2CADDRESS and the message requested has size of defined MESSAGE_SIZE
// In the current configuration, RAMPS sends the same messages as the defined FREE and OCCUPIED
// char *message_received -> The message received by the master (arduino) from the slave (RAMPS)
void updateState(char *message_received);

// Controls the manipulator
// This function will send the gcode commands by i2c to the Ramps
// int &manipulator_state internal state machine of manipulator control. This variable is auto-updated by this machine itself.
// In case of a restart needed, set manipulator_state to zero
// Return integer specifying the state of the manipulator, that being:
// MOVING 1 -> Manipulator is moving
// PERFURATING 2 -> Manipulator is ready to, or perfurating
// LIFTING 3 -> Manipulator is ready to, or lifting after the perfuration
// SOIL 4 -> Manipulator is ready to, or dropping the soil
// DONE 5 -> Manipulator is done
int manipulator_control(int &manipulator_state, int height);

// Controls the sweeper the drag the dirt back to the hole
// int &sweeper_state internal state machine of sweeper control. This variable is auto-updated by this machine itself.
// In case of a restart needed, set sweeper_state to zero
// Return integer specifying the state of the sweeper, that being:
// MOVING 1 -> Sweeper is moving
// DONE 5 -> Sweeper is done
int sweeper_control(int &sweeper_state);

#endif

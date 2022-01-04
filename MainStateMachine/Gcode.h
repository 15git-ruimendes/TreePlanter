#ifndef Gcode_h
#define Gcode_h

void setup_Wire(void);
int send_GCODE(char *buff);
int receive_Data(char *buff);
void create_Manipulator_GCODE(int x_axis, int y_axis, char *buff);
void create_Magazine_GCODE(char *buff);
void create_Sweeper_GCODE(int close_Open, char *buff);
int manipulator_control(int &state);

#endif

#define button 1

int drill_X = 0;
int drill_Y = 0;

int send_GCODE(char *GCODE)
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

void display_LCD(int page_Number)
{
}

void calibrate_Manipulator()
{
}

void update_Position()
{
}

void setup()
{

    int state = 0;
    int trees = 0;
    float distance = 0;
    char GCODE[256];
}

void loop()
{
    if (state == 0)
    {
        display_LCD(0);
    }
    else if (state == 0 && (digitalRead(button)))
    {
        display_LCD(1);
        trees = get_NumberOfTrees();
        state = 1;
    }
    else if (state == 1 && trees != 0)
    {
        distance = read_Distance();
        create_Manipulator_GCODE(distance, 0, GCODE);
    }
}

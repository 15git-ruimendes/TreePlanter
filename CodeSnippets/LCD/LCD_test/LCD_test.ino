#include "U8glib.h"
#include "LCD_Display.h"

U8GLIB_ST7920_128X64_1X u8g(21,17,16);
display lcd;

int page[10] = {100,0,1,2,3,4,5,6,7,8,101};
int trees = 8;
int top_page = 1;

int i = 0;

void setup() {
  // put your setup code here, to run once:
 lcd.setup_lcd(u8g);
}

void loop() {
  // put your main code here, to run repeatedly:
  u8g.firstPage();
  do{
    lcd.display_LCD(page[i],u8g,top_page,trees);
  }while(u8g.nextPage());
  
  delay(500);
  if (i < 11)
    i = 0;
  else
    i++;
}

#include "LCD_display.h"

void display::setup_LCD(U8GLIB_ST7920_128X64_1X u8g)
{
    previous_page = 0;
    u8g.setColorIndex(1);
}



void display::page_0(U8GLIB_ST7920_128X64_1X u8g)
{
}

void display::page_1(U8GLIB_ST7920_128X64_1X u8g,int top_page, int num_trees)
{ 
  char trees[3];
  trees[0] = num_trees/10 + '0';
  trees[1] = num_trees%10 + '0';
    
  if(top_page == 0){
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(0, 10, "Tree Planter!");
    u8g.drawLine(0, 15, 80, 15);
    u8g.drawLine(80, 0, 80, 64);
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(85, 10, "# Trees");
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(100, 25, trees);
    u8g.drawFrame(80, 28, 48, 18);
    u8g.setColorIndex(1);
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
  else{
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(0, 10, "Tree Planter!");
    u8g.drawLine(0, 15, 80, 15);
    u8g.drawLine(80, 0, 80, 64);
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(85, 10, "# Trees");
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(100, 25, trees);
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
}

void display::page_2(U8GLIB_ST7920_128X64_1X u8g)
{
    /*u8g.setFont(u8g_font_6x10);
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
    u8g.drawStr(5, 60, "Start...");*/
}

void display::page_3(U8GLIB_ST7920_128X64_1X u8g)
{
}

void display::page_4(U8GLIB_ST7920_128X64_1X u8g)
{
}

void display::page_5(U8GLIB_ST7920_128X64_1X u8g)
{
}

void display::page_100(U8GLIB_ST7920_128X64_1X u8g)
{
}

void display::display_LCD(int page,U8GLIB_ST7920_128X64_1X u8g,int top_page, int num_trees)
{
    switch (page)
    {
    case 0:
        page_0(u8g);
        break;
    case 1:
        page_1(u8g,top_page,num_trees);
        break;
    case 2:
        page_2(u8g);
        break;
    case 3:
        page_3(u8g);
        break;
    case 4:
        page_4(u8g);
        break;
    case 5:
        page_5(u8g);
        break;
    default:
        page_100(u8g);
        break;
    }
}

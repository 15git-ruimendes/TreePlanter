#ifndef LCD_display_h
#define LCD_display_h

#include "U8glib.h"

class display
{
public:
    void setup_LCD(U8GLIB_ST7920_128X64_1X u8g);
    void display_LCD(int page,U8GLIB_ST7920_128X64_1X u8g,int top_page, int num_trees);

private:
    int previous_page;
    void pick_page(int page,U8GLIB_ST7920_128X64_1X u8g,int top_page, int num_trees);
    void page_0(U8GLIB_ST7920_128X64_1X u8g);
    void page_1(U8GLIB_ST7920_128X64_1X u8g,int top_page, int num_trees);
    void page_2(U8GLIB_ST7920_128X64_1X u8g);
    void page_3(U8GLIB_ST7920_128X64_1X u8g);
    void page_4(U8GLIB_ST7920_128X64_1X u8g);
    void page_5(U8GLIB_ST7920_128X64_1X u8g);
    void page_6(U8GLIB_ST7920_128X64_1X u8g);
    void page_7(U8GLIB_ST7920_128X64_1X u8g);
    void page_8(U8GLIB_ST7920_128X64_1X u8g);
    void page_100(U8GLIB_ST7920_128X64_1X u8g);
};

#endif

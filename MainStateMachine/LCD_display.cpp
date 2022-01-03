#include "U8glib.h"
#include "LCD_display.h"

void setup_LCD(void)
{
    U8GLIB_ST7920_128X64_1X u8g(23, 17, 16);
    u8g.setColorIndex(1);
}

void display_LCD(int page)
{
    u8g.firstPage();
    do
    {
        pick_page(page)
    } while (u8g.nextPage());
}

void pick_page(int page)
{
    switch (page)
    {
    case page == 0:
        page_0();
        break;
    case page == 1:
        page_1();
        break;
    case page == 2:
        page_2();
        break;
    case page == 3:
        page_3();
        break;
    case page == 4:
        page_4();
        break;
    case page == 5:
        page_5();
        break;
    default:
        page_100();
        break;
    }
}

// mandar o número de arvores sempre para aqui para se conseguir atualizar sempre

void page_0() // Please Reload and/or Enter Number of Trees in Magazine
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
    u8g.drawStr(81, 40, "Reload?");
    u8g.drawBox(80, 46, 48, 18);
    u8g.setColorIndex(0);
    u8g.drawStr(87, 55, "Start?"); // penso que não será necessário
    u8g.setColorIndex(1);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(5, 30, "Enter");
    u8g.drawStr(5, 45, "The");
    u8g.drawStr(5, 60, "Number");
    u8g.drawStr(5, 60, "Of");
    u8g.drawStr(5, 60, "Trees");
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
    u8g.drawStr(81, 40, "Reload?");
    u8g.drawBox(80, 46, 48, 18);
    u8g.setColorIndex(0);
    u8g.drawStr(87, 55, "Start?");
    u8g.setColorIndex(1);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(5, 30, "Waiting");
    u8g.drawStr(5, 45, "For");
    u8g.drawStr(5, 60, "Start...");
}

void page_2() // moving
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
    u8g.drawStr(81, 40, "Reload?");
    u8g.setColorIndex(1);
    u8g.drawFrame(80, 46, 48, 18);
    u8g.drawStr(87, 55, "Start?");
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(5, 30, "Moving");
    u8g.drawStr(5, 45, "The");
    u8g.drawStr(5, 60, "Manipulator");
}

void page_3() // drilling
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
    u8g.drawStr(81, 40, "Reload?");
    u8g.setColorIndex(1);
    u8g.drawFrame(80, 46, 48, 18);
    u8g.drawStr(87, 55, "Start?");
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(5, 30, "Drilling");
}

void page_4() // Planting
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
    u8g.drawStr(81, 40, "Reload?");
    u8g.setColorIndex(1);
    u8g.drawFrame(80, 46, 48, 18);
    u8g.drawStr(87, 55, "Start?");
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(5, 30, "Planting");
    u8g.drawStr(5, 45, "The");
    u8g.drawStr(5, 60, "Tree");
}

void page_5() // Sweeping
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
    u8g.drawStr(81, 40, "Reload?");
    u8g.setColorIndex(1);
    u8g.drawFrame(80, 46, 48, 18);
    u8g.drawStr(87, 55, "Start?");
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(5, 30, "Filling");
    u8g.drawStr(5, 45, "With");
    u8g.drawStr(5, 60, "Dirt");
}

void page_100() // set up
{
}
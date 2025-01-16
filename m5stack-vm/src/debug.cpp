

#include "debug.hpp"
vstring message_list = {};
int message_all_count = 0;

void output_debug(String message, vint message_int)
{

    for (int i = 0; i < message_int.size(); i++)
    {
        message += " ";
        message += String(message_int[i]);
    }

    message_list.push_back(message);
    if (message_list.size() > 18)
    {
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.fillScreen(BLACK);
        message_list.erase(message_list.begin());
    }

    for (int i = 0; i < message_list.size(); i++)
    {
        M5.Lcd.printf("%d: %s\n", i, message_list[i].c_str());
    }

    message_all_count++;
}
void output_debug(String message, int message_int)
{

    message += String(message_int);
    message += " ";
    message_list.push_back(message);
    if (message_list.size() > 18)
    {
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.fillScreen(BLACK);
        message_list.erase(message_list.begin());
    }

    for (int i = 0; i < message_list.size(); i++)
    {
        M5.Lcd.printf("%d: %s\n", i, message_list[i].c_str());
    }

    message_all_count++;
}

void output_debug(String message)
{

    message_list.push_back(message);
    if (message_list.size() > 18)
    {
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.fillScreen(BLACK);
        message_list.erase(message_list.begin());
    }

    for (int i = 0; i < message_list.size(); i++)
    {
        M5.Lcd.printf("%d: %s\n", i, message_list[i].c_str());
    }

    message_all_count++;
}
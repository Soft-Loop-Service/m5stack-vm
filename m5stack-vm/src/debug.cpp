

#include "debug.hpp"
vstring message_list = {};
int message_all_count = 0;

bool is_debug_cui_mode = true;

void output_debug(String message, vint message_int)
{
    for (int i = 0; i < message_int.size(); i++)
    {
        message += " ";
        message += String(message_int[i]);
    }
    message_list.push_back(message);
    send_debug_message(message);
    output_debug_common();
}
void output_debug(String message, int message_int)
{
    message += " ";
    message += String(message_int);
    message += " ";
    message_list.push_back(message);
    send_debug_message(message);
    output_debug_common();
}

void output_debug(String message, vstring message_vstring)
{
    for (int i = 0; i < message_vstring.size(); i++)
    {
        message += " ";
        message += message_vstring[i];
    }
    message_list.push_back(message);
    send_debug_message(message);
    output_debug_common();
}

void output_debug(String message)
{

    message_list.push_back(message);
    send_debug_message(message);
    output_debug_common();
}

void output_debug(vstring v_message)
{
    // 連結して送信
    String message = "";
    for (int i = 0; i < v_message.size(); i++)
    {
        message += v_message[i];
        message += " ";
    }
    output_debug(message);
}

void output_debug_clear()
{
    message_list = {};
    message_all_count = 0;
}
void output_lcd_clear()
{
    message_list = {};
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.fillScreen(BLACK);
}

void output_debug_common()
{
    if (message_list.size() > 28)
    {
        // delay(100);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.fillScreen(BLACK);
        message_list.erase(message_list.begin());
    }

    if (is_debug_cui_mode)
    {
        for (int i = 0; i < message_list.size(); i++)
        {
            M5.Lcd.printf("%d: %s\n", message_all_count + i, message_list[i].c_str());
        }
    }

    message_all_count++;
}

void send_debug_message(String message)
{
    Serial.println(message);
}

void output_mode_gui()
{
    output_lcd_clear();
    is_debug_cui_mode = false;
    output_debug_common();
}

void output_mode_cli()
{
    output_lcd_clear();
    is_debug_cui_mode = true;
    output_debug_common();
}
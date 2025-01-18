

#include "debug.hpp"
vstring message_list = {};
int message_all_count = 0;

// 0 : GUI
// 1 : minimal CLI
// 2 : full CLI
int is_debug_mode_level = 2;

void output_debug(String message, vint message_int)
{
    if (is_debug_mode_level != 2)
    {
        return;
    }

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
    if (is_debug_mode_level != 2)
    {
        return;
    }
    message += " ";
    message += String(message_int);
    message += " ";
    message_list.push_back(message);
    send_debug_message(message);
    output_debug_common();
}

void output_debug(String message, vstring message_vstring)
{
    if (is_debug_mode_level != 2)
    {
        return;
    }
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
    if (is_debug_mode_level != 2)
    {
        return;
    }
    message_list.push_back(message);
    send_debug_message(message);
    output_debug_common();
}

void output_debug(vstring v_message)
{
    if (is_debug_mode_level != 2)
    {
        return;
    }
    // 連結して送信
    String message = "";
    for (int i = 0; i < v_message.size(); i++)
    {
        message += v_message[i];
        message += " ";
    }
    output_debug(message);
}

void output_message(String message, vint message_int)
{
    if (is_debug_mode_level < 1)
    {
        return;
    }

    for (int i = 0; i < message_int.size(); i++)
    {
        message += " ";
        message += String(message_int[i]);
    }
    message_list.push_back(message);
    send_debug_message(message);
    output_debug_common();
}
void output_message(String message, int message_int)
{
    if (is_debug_mode_level < 1)
    {
        return;
    }
    message += " ";
    message += String(message_int);
    message += " ";
    message_list.push_back(message);
    send_debug_message(message);
    output_debug_common();
}

void output_message(String message, vstring message_vstring)
{
    if (is_debug_mode_level < 1)
    {
        return;
    }
    for (int i = 0; i < message_vstring.size(); i++)
    {
        message += " ";
        message += message_vstring[i];
    }
    message_list.push_back(message);
    send_debug_message(message);
    output_debug_common();
}

void output_message(String message)
{
    if (is_debug_mode_level < 1)
    {
        return;
    }
    message_list.push_back(message);
    send_debug_message(message);
    output_debug_common();
}

void output_message(vstring v_message)
{
    if (is_debug_mode_level < 1)
    {
        return;
    }
    // 連結して送信
    String message = "";
    for (int i = 0; i < v_message.size(); i++)
    {
        message += v_message[i];
        message += " ";
    }
    output_message(message);
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

    if (is_debug_mode_level > 0)
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

void output_debug_mode(int level)
{
    is_debug_mode_level = level;
    if (level == 0)
    {
        output_lcd_clear();
        output_debug_clear();
    }
    else
    {
        output_lcd_clear();
        output_debug_clear();
        output_message("Debug mode is changed.");
        output_debug_common();
    }
}

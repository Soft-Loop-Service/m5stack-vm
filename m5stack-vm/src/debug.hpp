
#ifndef __DEBUG
#define __DEBUG

#include "./definition.hpp"
#include <stdexcept>

void output_debug(String, vint);
void output_debug(String, int);
void output_debug(String);
void output_debug(vstring);
void output_debug(String, vstring);

void output_message(String, vint);
void output_message(String, int);
void output_message(String);
void output_message(vstring);
void output_message(String, vstring);

void output_debug_clear();
void output_debug_common();
void output_lcd_clear();
void send_debug_message(String);

void output_debug_mode(int);
#endif
#include "./parser.hpp"
#include "M5Core2.h"
namespace Parser
{
    void parser(SourceCode receivedData)
    {
        bool start_flag = false;
        bool end_flag = false;

        // 不完全ではないか確認
        for (int i = 0; i < receivedData.size(); i++)
        {
            if (receivedData[i][0] == String(Bytecode::Opecode::program_start))
            {
                start_flag = true;
            }
            if (receivedData[i][0] == String(Bytecode::Opecode::program_start))
            {
                end_flag = true;
            }
        }

        if (!start_flag || !end_flag)
        {
            M5.Lcd.println("Error: Program is not complete.");
            return;
        }
        else
        {
            M5.Lcd.println("Program is complete.");
        }
    }
}
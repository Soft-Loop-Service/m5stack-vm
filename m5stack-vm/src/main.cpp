#include <M5Core2.h>
#include <vector>
#include <string>
#include "./definition.hpp"
#include "./parser/parser.hpp"
#include "./debug.hpp"
// データを記録する配列
SourceCode receivedData;
Parser::ParserSystem *parser;

void setup()
{
  M5.begin();
  Serial.begin(115200);
  Serial.println("Ready to receive data...");
  receivedData = {};
  parser = new Parser::ParserSystem(receivedData);
}

void try_parse()
{
  try
  {
    output_debug("try_parse");
    parser->refresh(receivedData);
    output_debug("try_parse2");
    parser->parser();
    output_debug("try_parse3");
  }
  catch (const std::runtime_error &e)
  {
    std::ostringstream *errer_message = new std::ostringstream();
    *errer_message << "Caught a runtime_error: " << e.what() << std::endl;

    output_message(String(errer_message->str().c_str()));
    parser->all_output_local_scope();
    parser->all_output_call_stack_system();
  }
  catch (const std::exception &e)
  {
    std::ostringstream *errer_message = new std::ostringstream();
    *errer_message << "Caught a standard exception: " << e.what() << std::endl;
    output_message(String(errer_message->str().c_str()));
  }
  catch (...)
  {
    std::ostringstream *errer_message = new std::ostringstream();
    *errer_message << "Caught an unknown exception" << std::endl;
    output_message(String(errer_message->str().c_str()));
  }
}

// 1行分のデータを処理して配列に記録
void processLine(String line)
{
  std::vector<String> tokens;
  String token = "";
  for (size_t i = 0; i < line.length(); i++)
  {
    if (line[i] == ' ')
    {
      // 空白でトークンを分割
      if (token.length() > 0)
      {
        tokens.push_back(token);
        token = "";
      }
    }
    else
    {
      token += line[i];
    }
  }
  // 最後のトークンを追加
  if (token.length() > 0)
  {
    tokens.push_back(token);
  }

  output_debug("pl ", tokens);

  if (tokens.size() > 0)
  {
    if (tokens[0] == "##parser")
    {
      try_parse();

      output_message("Parser is completed.");
      return;
    }
    else if (tokens[0] == "##lcd_clear")
    {
      output_lcd_clear();
      output_message("lcd_clear is completed.");
      return;
    }
    else if (tokens[0] == "##code_clear")
    {
      receivedData.clear();
      output_message("code_clear is completed.");
      return;
    }
    else if (tokens[0] == "##debug_mode")
    {
      output_message("debug_mode mode is implemented.");
      output_debug_mode(tokens[1].toInt());
      return;
    }

    else if (tokens[0] == "##all_output_local_scope")
    {
      parser->all_output_local_scope();
      return;
    }
    else if (tokens[0] == "##all_output_call_stack_system")
    {
      parser->all_output_call_stack_system();
      return;
    }
    else if (tokens[0] == "##all_output_opecode_stack_system")
    {
      parser->all_output_opecode_stack_system();
      return;
    }

    else if (tokens[0] == "##debug")
    {
      for (const auto &line : receivedData)
      {
        String output = "";
        for (const auto &item : line)
        {
          output += item + " ";
        }
        output_debug(output);
      }
      return;
    }
    else if (tokens[0] == "##")
    {
      output_message("Command is not found.");
      return;
    };
  }

  // 行ごとに記録
  receivedData.push_back(tokens);
}
void loop()
{
  static String buffer = ""; // 受信データのバッファ

  // シリアル通信でデータを受信
  while (Serial.available())
  {
    char c = Serial.read();
    if (c == '\n')
    {
      // 改行を検出したらバッファ内のデータを処理
      processLine(buffer);
      buffer = ""; // バッファをクリア
    }
    else
    {
      buffer += c; // バッファに文字を追加
    }
  }

  // 必要に応じて、記録されたデータを表示する
  if (M5.BtnA.wasPressed())
  {
    M5.Lcd.printf("Recorded Data:");
    for (const auto &line : receivedData)
    {
      String output = "";
      for (const auto &item : line)
      {
        output += item + " ";
      }
      output_debug(output);
    }
  }
  if (M5.BtnB.wasPressed())
  {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    receivedData.clear(); // データをクリア
    output_debug_clear(); // デバッグ情報をクリア
  }
  if (M5.BtnC.wasPressed())
  {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);

    // パーサーを実行
    parser->parser();
  }
  M5.update(); // M5Stackのボタン処理
}

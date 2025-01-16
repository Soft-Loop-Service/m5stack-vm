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
      parser->refresh(receivedData);
      parser->parser();

      output_debug("Parser is completed.");
      return;
    }
    else if (tokens[0] == "##lcd_clear")
    {
      output_lcd_clear();
      output_debug("lcd_clear is completed.");
      return;
    }
    else if (tokens[0] == "##code_clear")
    {
      receivedData.clear();
      output_debug("code_clear is completed.");
      return;
    }
    else if (tokens[0] == "##modeCLI" || tokens[0] == "##modeCUI")
    {
      output_mode_cli();
      output_debug("CLI mode is implemented.");
      return;
    }
    else if (tokens[0] == "##modeGUI")
    {
      output_mode_gui();
      output_debug("GUI mode is implemented.");
      return;
    }
    else if (tokens[0] == "##all_output_local_scope")
    {
      parser->all_output_local_scope();
      return;
    }
    else if (tokens[0] == "##all_output_stack_system")
    {
      parser->all_output_stack_system();
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
      output_debug("Command is not found.");
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
    parser->refresh(receivedData);
    parser->parser();
  }
  M5.update(); // M5Stackのボタン処理
}

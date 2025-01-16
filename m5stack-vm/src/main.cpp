#include <M5Core2.h>
#include <vector>
#include <string>
#include "./definition.hpp"
#include "./parser/parser.hpp"
#include "./debug.hpp"
// データを記録する配列
SourceCode receivedData = {};

void setup()
{
  M5.begin();
  Serial.begin(115200);
  Serial.println("Ready to receive data...");
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
  }
  if (M5.BtnC.wasPressed())
  {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);

    // パーサーを実行
    Parser::ParserSystem *parser = new Parser::ParserSystem(receivedData);
  }
  M5.update(); // M5Stackのボタン処理
}

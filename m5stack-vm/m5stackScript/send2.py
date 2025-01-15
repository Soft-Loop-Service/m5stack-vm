import serial
import time

# ファイル内の改行コードをドル記号に変える関数
def replace_newlines_with_dollar(input_file, output_file):
    with open(input_file, 'r', encoding='utf-8') as infile:
        data = infile.read().replace('\n', '$').replace(' ', '_')  # 改行をドル記号に置き換え

    with open(output_file, 'w', encoding='utf-8') as outfile:
        outfile.write(data + "\n")  # 変換したデータを書き込み

# 入力ファイルと出力ファイルのパスを指定
input_path = '.\\m5stackScript\\test\\9.txt'
output_path = '.\\m5stackScript\\test\\9_modified.txt'

# 改行をドル記号に置き換え
replace_newlines_with_dollar(input_path, output_path)

# シリアルポートとボーレートを設定
ser = serial.Serial('COM4', 115200)  # 'COM4'は使用するポートに応じて変更

# 改行がドル記号に置き換わったファイルをシリアル送信
with open(output_path, 'rb') as file:
    while chunk := file.read(1024):  # 1024バイトずつ送信
        ser.write(chunk)
        print(chunk.decode('utf-8'))  # 表示用にデコード
        time.sleep(0.1)  # 必要に応じて調整

ser.close()
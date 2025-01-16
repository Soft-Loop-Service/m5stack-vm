
import serial
import time

# シリアルポートとボーレートを設定
ser = serial.Serial('COM4', 115200)  # 'COM3'は使用するポートに応じて変更
input_path = '.\\m5stackScript\\test\\fibonacci.txt'

# ファイルを開いてデータを読み込み、シリアルポートに送信
with open(input_path, 'rb') as file:
    while chunk := file.read(1024):  # 1024バイトずつ送信
        ser.write(chunk)
        time.sleep(0.1)  # 必要に応じて調整

ser.close()
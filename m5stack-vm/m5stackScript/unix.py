import serial
import time
import os
import sys
import select

# シリアルポートとボーレートの設定
PORT = 'COM4'  # M5Stackが接続されているポートを指定
BAUDRATE = 115200

# シリアルポートを初期化
try:
    ser = serial.Serial(PORT, BAUDRATE, timeout=1)
    print(f"Connected to {PORT}")
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit()

print("Type 'exit' to quit.")
print("Enter a command (e.g., 'input filename') or wait for messages:")

# メインループ
try:
    while True:
        # 非ブロッキングでシリアルデータを受信
        if ser.in_waiting > 0:
            message = ser.readline().decode('utf-8').strip()  # シリアルからメッセージ受信
            print(f"Received from M5Stack: {message}")

        # ユーザー入力を確認（ブロッキング）
        if sys.stdin in select.select([sys.stdin], [], [], 0.1)[0]:  # 0.1秒待つ
            user_input = input("Command: ").strip()

            # "exit" コマンドで終了
            if user_input.lower() == "exit":
                print("Exiting...")
                break

            if user_input.startswith("##"):
                ser.write(user_input.encode('utf-8'))
                ser.write("\n".encode('utf-8'))
                print(f"Sent directly: {user_input}")
                continue

            # "input" コマンドの処理
            if user_input.startswith("input "):
                # ファイル名を抽出
                filename = user_input.split(" ", 1)[1]
                input_path = f'.\\m5stackScript\\test\\{filename}.txt'

                # ファイルの存在を確認
                if not os.path.exists(input_path):
                    print(f"File not found: {input_path}")
                    continue

                # ファイルを開いてデータを読み込み、シリアルポートに送信
                try:
                    with open(input_path, 'rb') as file:
                        while chunk := file.read(1024):  # 1024バイトずつ送信
                            ser.write(chunk)
                            print(f"Sent: {chunk.decode('utf-8', errors='ignore')}")  # 送信内容を表示
                            time.sleep(0.1)  # 必要に応じて調整
                    print(f"File '{filename}' sent successfully.")
                except Exception as e:
                    print(f"Error reading or sending file: {e}")
            else:
                print("Invalid command. Use 'input filename' or 'exit'.")

except KeyboardInterrupt:
    print("\nExiting...")

finally:
    ser.close()
    print("Serial port closed.")
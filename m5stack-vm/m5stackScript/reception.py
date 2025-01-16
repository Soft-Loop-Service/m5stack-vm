import serial

# シリアルポートとボーレートの設定
PORT = 'COM4'  # M5Stackが接続されているポート (Windowsの場合)
BAUDRATE = 115200  # M5Stackと同じボーレート

# シリアルポートを初期化
try:
    ser = serial.Serial(PORT, BAUDRATE, timeout=1)
    print(f"Connected to {PORT}")
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit()

# データの受信ループ
try:
    while True:
        if ser.in_waiting > 0:  # 受信データがあるか確認
            message = ser.readline().decode('utf-8').strip()  # 1行読み取り
            print(f"Received: {message}")
except KeyboardInterrupt:
    print("\nExiting...")
finally:
    ser.close()
    print("Serial port closed.")
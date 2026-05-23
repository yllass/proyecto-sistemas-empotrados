import serial
import websocket
import threading
import time

PUERTO_ESPLORA = "COM11"
IP_ESP32       = "192.168.100.10"

def conectar():
    while True:
        try:
            ws = websocket.WebSocket()
            ws.connect(f"ws://{IP_ESP32}:81")
            print("Conectado al ESP32")

            ser = serial.Serial(PUERTO_ESPLORA, 9600, timeout=1)
            print("Conectado al Esplora")

            # Hilo que manda datos del Esplora al ESP32
            def esplora_a_esp32():
                while True:
                    try:
                        linea = ser.readline().decode("utf-8").strip()
                        if linea:
                            print("Esplora → ESP32:", linea)
                            ws.send(linea)
                    except:
                        break

            t = threading.Thread(target=esplora_a_esp32, daemon=True)
            t.start()

            # Hilo principal: manda datos del ESP32 al Esplora
            while True:
                try:
                    msg = ws.recv()
                    if msg and not msg.startswith('{"x":'):
                        print("ESP32 → Esplora:", msg)
                        ser.write((msg + "\n").encode("utf-8"))
                except:
                    break

        except Exception as e:
            print(f"Error: {e} — reintentando en 3s...")
            time.sleep(3)

conectar()
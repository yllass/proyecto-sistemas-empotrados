#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// 📶 WIFI
const char* ssid     = "Totalplay-2.4G-f240";
const char* password = "44HX98ACNT6RztQN";

// 🌐 WebSocket
WebSocketsServer webSocket = WebSocketsServer(81);

// 🔴 LEDs
#define LED1 25
#define LED2 26
#define LED3 27

// 🎉 Estado ganar
bool efectoVictoria = false;

void setup() {

  Serial.begin(115200);

  // LEDs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);

  // WIFI
  WiFi.begin(ssid, password);

  Serial.println("Conectando WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // WEBSOCKET
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {

  webSocket.loop();

  // 🎉 EFECTO VICTORIA
  if (efectoVictoria) {

    for (int i = 0; i < 10; i++) {

      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
      delay(150);

      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      delay(150);
    }

    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);

    efectoVictoria = false;
  }
}

// 🌐 EVENTOS WEBSOCKET
void webSocketEvent(uint8_t num,
                    WStype_t type,
                    uint8_t* payload,
                    size_t length) {

  // Cliente conectado
  if (type == WStype_CONNECTED) {
    Serial.println("Cliente conectado");
  }

  // Mensaje recibido
  if (type == WStype_TEXT) {

    String mensaje = String((char*)payload);

    Serial.print("Mensaje: ");
    Serial.println(mensaje);

    // 🎉 GANAR
    if (mensaje == "GANAR") {
      efectoVictoria = true;
    }

    // Reenviar mensajes a todos
    webSocket.broadcastTXT(payload, length);
  }
}
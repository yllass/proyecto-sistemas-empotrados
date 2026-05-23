#include <Esplora.h>

int ultimoDx = 0, ultimoDy = 0;
bool ultimoBtn = false, ultimoBtn4 = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int joyX = Esplora.readJoystickX();
  int joyY = Esplora.readJoystickY();
  bool btn1 = !Esplora.readButton(SWITCH_1);
  bool btn4 = !Esplora.readButton(SWITCH_4);

  int dx = 0, dy = 0;
  if (joyX >  100) dx = -1;
  if (joyX < -100) dx =  1;
  if (joyY >  100) dy =  1;
  if (joyY < -100) dy = -1;

  if (dx != ultimoDx || dy != ultimoDy || btn1 != ultimoBtn || btn4 != ultimoBtn4) {
    Serial.print("{\"x\":");
    Serial.print(dx);
    Serial.print(",\"y\":");
    Serial.print(dy);
    Serial.print(",\"btn\":");
    Serial.print(btn1 ? "true" : "false");
    Serial.print(",\"btn4\":");
    Serial.print(btn4 ? "true" : "false");
    Serial.println("}");

    ultimoDx   = dx;
    ultimoDy   = dy;
    ultimoBtn  = btn1;
    ultimoBtn4 = btn4;
  }

  delay(50);
}
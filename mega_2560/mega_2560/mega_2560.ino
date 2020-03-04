
#define USE_ARDUINO_INTERRUPTS true

#include "WaterSensor.h"
#include "PulseSensor.h"

#include "TScreen.h"
#include "SecuritySystem.h"
#include "Scenes.h"

long lastSenseTime = millis();
WaterSensor ws;

//Arduino SETUP
void setup() {
  uint16_t ID;

  Serial.begin(9600);
  Serial3.begin(9600);

  Serial.println(F(">> Iniciando arduino"));

  ID = tft.readID();

  Serial.print(F(">> Pantalla TFT ID:0x"));
  Serial.println(ID, HEX);

  tft.begin(ID);
  tft.setTextColor(0xFFFF, 0x0000);
  tft.setRotation(Orientation);
}
long lastSensedTime = 0;
//Arduino LOOP @16MHz
void loop() {

  if (Serial.available()) {
    String value = Serial.readString();
    Serial.println(value);

    if (value.indexOf("secure") >= 0) {
      changeCurrentScene(SECURE);
    }
    else if (value.indexOf("start") >= 0) {
      changeCurrentScene(START);
    }
    else if (value.indexOf("bpm") >= 0){
      changeCurrentScene(BPM);  
    }
  }
  
  //Tomar los datos de los sensores cada 5 segundos
  if (millis() - lastSensedTime >= 5000){
    lastSensedTime = millis();
    //ws.sense();
  }
  
  if (hasChanged) {
    //repaint based in current scene
    switch (getCurrentScene()) {
      case START:
        printFirstScene();
        break;

      case SECURE:
        printSecureScene();
        break;

      case BPM:
        printBPMScene();
        break;
    }
  }
}

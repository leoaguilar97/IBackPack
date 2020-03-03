
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Touch screen functionality
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include "TScreen.h"

MCUFRIEND_kbv tft;

int touch_x, touch_y;
int pixel_x, pixel_y;
boolean pressed = false;

//
// Functions headers
//
void setup();
void loop();

bool Touch_getXY(void);
bool update_touch(void);

void start_menu(void);

void printFirstScene(void);
void printSecureScene(void);

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

//Arduino LOOP @16MHz
void loop() {

  if (Serial.available()) {
    String value = Serial.readString();
    Serial.println(value);

    if (value.indexOf("go_secure") >= 0) {
      changeCurrentScene(SECURE);
    }
    else if (value.indexOf("go_start") >= 0) {
      changeCurrentScene(START);
    }
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
    }
  }
}

bool Touch_getXY(void)
{
  TSPoint p = screen.getPoint();

  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);
  digitalWrite(XM, HIGH);

  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);

  if (pressed) {
    pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
    pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }

  return pressed;
}

bool update_touch(void)
{
  TSPoint p = screen.getPoint();

  pinMode(YP, OUTPUT);      //restore shared pins
  pinMode(XM, OUTPUT);

  digitalWrite(YP, HIGH);   //because TFT control pins
  digitalWrite(XM, HIGH);

  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    touch_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
    touch_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }

  return pressed;
}

void printFirstScene(void)
{
  tft.fillScreen(BLACK);
  tft.setCursor(15, 75);

  tft.setTextSize(4);
  tft.setTextColor(CYAN);
  tft.println("iBackPack");

  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.setCursor(15, 125);
  tft.println(F("Grupo #1"));

  tft.setCursor(15, 175);
  tft.setTextColor(RED);
  tft.setTextSize(1);

  tft.println("Esperando modo seguro...");
}

struct Point {
  int X = 0;
  int Y = 0;
};

struct PwdPoint {
  Point TopLeft;
  Point BotRight;
  boolean touched = false;
};

struct TouchedPoint {
  int pwdPointIndex = -1;
  int order = -1;
  boolean checked = false;
};

PwdPoint pwdPoints[9];
TouchedPoint tPs[9];

int tpIndex = 0;
int tpOrder = 0;

//revisar si un punto del sistema de seguridad
//ya fue tocado, true si ya fue tocado, false si aun no
bool checkIfAlreadyPressed(int pressedIndex) {
  for (int i = 0; i < 9; i++) {
    if (tPs[i].pwdPointIndex == pressedIndex) {
      return true;
    }
  }

  return false;
}

bool resetTouchedPoints() {
  for (int i = 0; i < 9; i++) {
    tPs[i].checked = false;
  }
}

//Revisar si el punto actual esta dentro de un punto
void checkIfInside() {

  TouchedPoint& tP = tPs[tpIndex];

  if (tP.checked) {
    return;
  }

  for (int i = 0; i < 9; i++) {
    PwdPoint& pP = pwdPoints[i];

    if (pP.touched) {
      continue;
    }

    //revisar si el punto se encuentra entre los extremos del punto del patron
    //si esta entre los posibles valores, significa que toco el punto
    if ((pixel_x >= pP.TopLeft.X && pixel_x <= pP.BotRight.X)
        && (pixel_y >= pP.TopLeft.Y && pixel_y <= pP.BotRight.Y)) {

      if (tpIndex >= 9) {
        return;
      }

      //agregar el punto que acaba de tocar a la lista de puntos
      tP.pwdPointIndex = i;
      tP.order = tpOrder++;
      tP.checked = true;

      pP.touched = true;

      tpIndex++;

      Serial.println("Tocado: " + String(i));
      Serial.println("Indice: " + String(tpIndex));
      Serial.println("Orden: " + String(tP.order));

      //de cualquier forma, ya se detecto que toco un punto
      //de seguridad, por lo que no es necesario seguir comprobando
      break;
    }
  }
}

//Revisar si la contrase;a ingresada es correcta
boolean checkOkPwd() {
  int indices[] = { 6, 3, 0, 4, 7, 5, 2 };

  if (tpIndex != 7) {
    return false;
  }

  for (int i = 0; i < 7; i++) {
    if (tPs[i].pwdPointIndex != indices[i]) {
      return false;
    }
  }
  return true;
}

//Dibujar los circulos que se utilizaran para
//ingresar el patron
int printPatronCircles(int cw, boolean checkIfTouched = false) {
  //cw = Circle Width
  int left = 0;
  int top = -cw;
  int radius = 0;

  tft.fillRect(left, top + cw, tft.width(), (top + cw * 2) * 3 + cw, BLACK);

  boolean correctPwd = checkIfTouched ? checkOkPwd() : false;

  for (int i = 0; i < 9; i++) {
    if (i % 3 == 0) {
      top += cw * 1.5;
      left = 0;
    }

    left = left == 0 ?
           cw / 2 + 2 :
           left + cw * 1.45;

    radius = cw / 2 - 10;

    tft.fillCircle(left, top, radius + 10, WHITE);
    tft.fillCircle(left, top, radius, 
      checkIfTouched ? pwdPoints[i].touched ? correctPwd ? GREEN : RED : WHITE : BLACK);

    pwdPoints[i].TopLeft.X = left - radius;
    pwdPoints[i].TopLeft.Y = top - radius;

    pwdPoints[i].BotRight.X = left + radius;
    pwdPoints[i].BotRight.Y = top + radius;

    pwdPoints[i].touched = false;
  }

  if (checkIfTouched) {
    for (int i = 1; i < tpIndex; i++) {

      int fIndex = tPs[i].pwdPointIndex;
      int lIndex = tPs[i - 1].pwdPointIndex;

      Serial.println("Conectando: " + String(fIndex) + " -> " + String(lIndex));

      int fpx = (pwdPoints[fIndex].TopLeft.X + pwdPoints[fIndex].BotRight.X) * 0.5;
      int fpy = (pwdPoints[fIndex].TopLeft.Y + pwdPoints[fIndex].BotRight.Y) * 0.5;

      int spx = (pwdPoints[lIndex].TopLeft.X + pwdPoints[lIndex].BotRight.X) * 0.5;
      int spy = (pwdPoints[lIndex].TopLeft.Y + pwdPoints[lIndex].BotRight.Y) * 0.5;

      tft.drawLine(fpx, fpy, spx, spy, correctPwd ? GREEN: RED);

      for (int i = -5; i < 5; i++) {
        tft.drawLine(fpx + i, fpy, spx + i, spy, correctPwd ? GREEN: RED);
        tft.drawLine(fpx, fpy + i, spx, spy + i, correctPwd ? GREEN: RED);
      }
    }
  }

  return top;
}


int allowPrint(void) {

  boolean checkedPwd = false;
  while (!Touch_getXY()) {
    ;
  }

  long clickedAt = millis();

  tpIndex = 0;
  tpOrder = 0;
  resetTouchedPoints();

  while (millis() - clickedAt <= 200) {
    while (Touch_getXY()) {
      int boxSize = tft.width() / 12;
      tft.fillRect(pixel_x - boxSize * 0.5, pixel_y - boxSize * 0.5, boxSize, boxSize, BLUE);
      checkIfInside();
      clickedAt = millis();
    }
  }

  printPatronCircles(tft.width() / 4, true);

  delay(1000);
}

void printSecureScene(void) {
  tft.fillScreen(BLUE);
  tft.setCursor(5, 5);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);


  bool finish = false;

  while (!finish) {
    int cw = tft.width() / 4;
    int top = printPatronCircles(cw);

    allowPrint();
    /*
      checkPwdBtn.initButton(&tft, tft.width() / 2, top + cw * 1.5, 150, 30, WHITE, BLUE , WHITE, "COMPROBAR", 2);
      checkPwdBtn.drawButton(false);

      bool checkedPwd = false;
      while (!checkedPwd) {
      checkedPwd = Touch_getXY();
      checkPwdBtn.press(checkedPwd && checkPwdBtn.contains(pixel_x, pixel_y));
      }
    */
  }
}

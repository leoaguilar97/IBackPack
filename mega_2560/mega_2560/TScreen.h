// Touch screen functionality
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#define MINPRESSURE 200
#define MAXPRESSURE 1000
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//Representar un punto en la pantalla
struct Point {
  int X = 0;
  int Y = 0;
};

//configuracion de la pantalla touch
const int XP = 8, XM = A2, YP = A3, YM = 9; //240x320 ID=0x9341
const int TS_LEFT = 112, TS_RT = 921, TS_TOP = 67, TS_BOT = 899;

TouchScreen screen = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;
MCUFRIEND_kbv tft;
uint8_t Orientation = 0;

//Configuracion de la pantalla
int touch_x, touch_y;
int pixel_x, pixel_y;

boolean pressed = false;

//Obtener y refrescar la posicion en la cual fue tocada la pantalla
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

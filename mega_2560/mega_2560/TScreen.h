
//configuracion de la pantalla touch
const int XP = 8, XM = A2, YP = A3, YM = 9; //240x320 ID=0x9341
const int TS_LEFT = 112, TS_RT = 921, TS_TOP = 67, TS_BOT = 899;

TouchScreen screen = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;

#define MINPRESSURE 200
#define MAXPRESSURE 1000

int16_t BOXSIZE;
int16_t PENRADIUS = 1;
uint16_t ID, oldcolor, currentcolor;
uint8_t Orientation = 0;

enum TftScene {
  START,
  SECURE
};

//escena actual, en la que se encuentra el sistema
TftScene CURRENT_SCENE = SECURE;
//revisa si la escena actual cambio desde la ultima lectura
boolean hasChanged = true;

//Obtener la escena actual de la pantalla
TftScene getCurrentScene() {
  if (hasChanged) {
    hasChanged = false;
  }
  return CURRENT_SCENE;
}

//cambiar la escena actual a una nueva, enviada como parametro
void changeCurrentScene(TftScene newScene) {
  hasChanged = true;
  CURRENT_SCENE = newScene;
}

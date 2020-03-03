
//Control de escenas de la pantalla
enum TftScene {
  START,
  SECURE
};

//escena actual, en la que se encuentra el sistema
TftScene currentScene = START;
//revisa si la escena actual cambio desde la ultima lectura
boolean hasChanged = true;

//Obtener la escena actual de la pantalla
TftScene getCurrentScene(void) {
  if (hasChanged) {
    hasChanged = false;
  }
  return currentScene;
}

//cambiar la escena actual a una nueva, enviada como parametro
void changeCurrentScene(TftScene newScene) {
  hasChanged = true;
  currentScene = newScene;
}

//Deja que el usuario pueda dibujar el patron en la pantalla
int allowPrint(void) {
  while (!Touch_getXY()) {
    ;
  }

  long clickedAt = millis();

  tpIndex = 0;
  tpOrder = 0;
  resetTouchedPoints();

  //revisar si el valor que se ingresa
  //es ingresado en menos de 200 ms de estar presionado
  while (millis() - clickedAt <= 200) {
    while (Touch_getXY()) {
      if (pixel_y > 230){
        continue;  
      }
      
      int boxSize = tft.width() / 12;
      tft.fillRect(pixel_x - boxSize * 0.5, pixel_y - boxSize * 0.5, boxSize, boxSize, BLUE);
      //revisar si el punto esta dentro de los circulos del patron
      checkIfInside();

      clickedAt = millis();
    }
  }
}

//Imprime la escena de la clave de seguridad
//Este no sale de aca hasta que se termina de
//realizar el patron
void printSecureScene(void) {
  tft.fillScreen(BLACK);
  tft.setCursor(5, 260);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.println("Ingrese el    patron");

  while (!isCorrect) {

    int cw = tft.width() / 4;
    int top = printPatronCircles(cw);

    //activar la pantalla touch para recibir el input del usuario
    allowPrint();

    //imprimir de nuevo, si gano, transicionar a la otra vista
    printPatronCircles(cw, true);
    delay(1500);
  }

  changeCurrentScene(START);
  isCorrect = false;
}


//Imprimir estado inicial del sistema
//el estado inicial muestra el nombre del producto
//los creadores
//y un mensaje que muestra que se esta esperando al modo seguro
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


//Representar un circulo, que se pintan en
//el modo seguro para representar el patron
struct PwdPoint {
  Point TopLeft;
  Point BotRight;
  boolean touched = false;
};

//Representar un punto que fue tocado en la pantalla
//y que corresponde a un circulo del patron
struct TouchedPoint {
  int pwdPointIndex = -1;
  int order = -1;
  boolean checked = false;
};

//Puntos a ser agregados al sistema, para contorlar la autenticacion
PwdPoint pwdPoints[9];
TouchedPoint tPs[9];

int tpIndex = 0;
int tpOrder = 0;
boolean isCorrect = false;

//Revisar si la contrase;a ingresada es correcta
boolean checkOkPwd(void) {
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

//Reiniciar los puntos tocados
bool resetTouchedPoints(void) {
  for (int i = 0; i < 9; i++) {
    tPs[i].checked = false;
  }
}

//Revisar si el punto actual esta dentro de un punto
void checkIfInside(void) {
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
        break;
      }

      //agregar el punto que acaba de tocar a la lista de puntos
      tP.pwdPointIndex = i;
      tP.order = tpOrder++;
      tP.checked = true;
      pP.touched = true;
      tpIndex++;
      break;
    }
  }
}
//Dibujar los circulos que se utilizaran para
//ingresar el patron
int printPatronCircles(int cw, boolean checkIfTouched = false) {
  //cw = Circle Width
  int left = 0;
  int top = -cw;
  int radius = 0;

  tft.fillRect(left, top + cw, tft.width(), (top + cw * 2) * 3 + cw, BLACK);

  isCorrect = checkIfTouched ? checkOkPwd() : false;

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
                   checkIfTouched ? pwdPoints[i].touched ? isCorrect ? GREEN : RED : WHITE : BLACK);

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
      int fpx = (pwdPoints[fIndex].TopLeft.X + pwdPoints[fIndex].BotRight.X) * 0.5;
      int fpy = (pwdPoints[fIndex].TopLeft.Y + pwdPoints[fIndex].BotRight.Y) * 0.5;
      int spx = (pwdPoints[lIndex].TopLeft.X + pwdPoints[lIndex].BotRight.X) * 0.5;
      int spy = (pwdPoints[lIndex].TopLeft.Y + pwdPoints[lIndex].BotRight.Y) * 0.5;

      for (int i = -5; i < 5; i++) {
        tft.drawLine(fpx + i, fpy, spx + i, spy, isCorrect ? GREEN : RED);
        tft.drawLine(fpx, fpy + i, spx, spy + i, isCorrect ? GREEN : RED);
      }
    }
  }

  return top;
}


class WaterSensor {
  private:
    const int ws_pin = A8;
    int waterValue = 0;
  public:
    //constructor del sensor
    WaterSensor() {
      pinMode(ws_pin, INPUT);
      Serial.println("Sensor de agua inicializado");
    }

    //metodo comun que realiza 
    sense() {
      waterValue = analogRead(ws_pin);
      Serial.println(">> Sensor de agua: " + waterValue);
    }
}


class WaterSensor {
  private:
    const int ws_pin = A8;
    int waterValue = 0;
  public:
    //constructor del sensor
    WaterSensor() {
      pinMode(ws_pin, INPUT);
    }

    //metodo comun que realiza 
    int sense() {
      waterValue = analogRead(ws_pin);
      Serial.println(">> Sensor de agua: " + waterValue);
      
      return waterValue;
    }
};

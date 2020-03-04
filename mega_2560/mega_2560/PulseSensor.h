
#include <PulseSensorPlayground.h>

class BPMCount {
  private:
    PulseSensorPlayground pulseSensor; 
    const int threshold = 500;
    int bpm = 0;
    
    
  public:
    //Son los milisegundos que se debe esperar para saber el bpm
    const int BPM_MS = 30000;
    const int INTERRUPT_PIN = 19;
    const int POWER_PIN = 22;

    //
    BPMCount() {
      pulseSensor.analogInput(INTERRUPT_PIN);
      pulseSensor.setThreshold(threshold);
    
      pulseSensor.begin();
      pinMode(POWER_PIN, OUTPUT);
    }

    //inicia el sensor de pulso
    void start(void){
      digitalWrite(POWER_PIN, HIGH);  
    }
    
    //sensar los datos del pulso
    int getBPM() {
      bpm = pulseSensor.getBeatsPerMinute();

      if (pulseSensor.sawStartOfBeat()){
        Serial.print("♥: "); 
        Serial.println(bpm);   
      }

      return bpm;
    }

    //finaliza el sensor de pulso
    void end(void){
      digitalWrite(POWER_PIN, LOW);  
    }

    //
    
};

BPMCount pulses;

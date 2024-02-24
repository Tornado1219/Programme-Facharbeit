
//Import von Bibliotheken
#include "FS.h"
#include "SD.h"
#include "SPI.h"


//Definieren von Konstanten und globalen Variablen
#define RI 51.2
#define Uteiler 1730.0 / 50.2

#define PlottTime 100

#define Grenzwert 150

#define Loops 8000
const int UPin = 32;
const int IPin = 13;

unsigned long FreqRawT[Loops];
unsigned long Traw[Loops];


unsigned int MessNum = 0;
unsigned long Tstart;

void setup() {
  //Intialisierung von Microkontroller
  Serial.begin(115200);
  pinMode(IPin, INPUT);
  pinMode(UPin, INPUT);
  SDsetup();
}



void loop() {
//Warten auf Start einer Messung durch Seriellen Monitor
  if (Serial.available()) {
    String s = Serial.readString();
    Serial.println(s);

    if (s.startsWith("messen")) {

      Serial.print("Messe....");
      int DataL = Loops;
      //Aufnhme der Messwerte
      while (analogRead(UPin) < Grenzwert) {}
      Tstart = millis();
      for (unsigned int i = 0; i < Loops; i++) {


        while (analogRead(UPin) > Grenzwert) {}
        unsigned long PTimeStart = micros();
        while (analogRead(UPin) < Grenzwert) {

        }

        if ((micros() - PTimeStart) > 1000000L) {
          FreqRawT[i] = 0;
        } else {
          FreqRawT[i] = micros() - PTimeStart;
        }
        
        Traw[i] = millis() - Tstart;

        if (Serial.available()) {
          String s = Serial.readString();
          Serial.println(s);

          if (s.startsWith("stop")) {
            DataL = i;
            break;
          }
        }
      }

      //Seicherung der Messwerte
      String path;
      path = "/PlottFreq-";
      path += MessNum;
      path += "-";
      path += (Traw[DataL] - Tstart) / 1000;
      path += ".csv";
      File file = SD.open(path, FILE_WRITE);

      Serial.println((String) "Schreibe in " + path + "\nSchreibe Zeile:");

      MessNum++;

      String txt;

      for (unsigned int i = 0; i < DataL; i++) {
        txt = String(Traw[i] - Tstart);
        txt += ";";
        txt += String(FreqRawT[i]);
        txt += ";";


        txt += "\n";
        file.print(txt);
        file.flush();
        if ((i % 500) == 0) {
          Serial.print(";");
          Serial.print(i);
        }
      }

      file.close();
      Serial.println("\nMessung gespeichert");
    }
  }
}


//Import von Bibliotheken
#include "FS.h"
#include "SD.h"
#include "SPI.h"


//Definieren von Konstanten und globalen Variablen
#define RI 51.2
#define Uteiler 1730.0 / 50.2

#define Loops 8000
const int UPin = 32;
const int IPin = 13;

unsigned int Iraw[Loops];
unsigned int Uraw[Loops];
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


      
      Serial.print("Messe....");

//Aufnhme der Messwerte
  Tstart = micros();
  for (unsigned int i = 0; i < Loops; i++) {
    Iraw[i] = analogRead(IPin);
    Uraw[i] = analogRead(UPin);
    Traw[i] = micros();

    if(Iraw[i] == 0 && Uraw[i] == 0 && Iraw[i - 1] == 0 && Uraw[i - 1] == 0){
      i--;
    } else if (Iraw[i - 1] == 0 && Uraw[i - 1] == 0) {
      int buffI = Iraw[i];
      int buffU = Uraw[i];
      int buffT = Traw[i];
      Iraw[i] = 0;
      Uraw[i] = 0;
      Traw[i] -= 10;
      i++;
      Iraw[i] = buffI;
      Uraw[i] = buffU;
      Traw[i] = buffT;
    }
  }

//Seicherung der Messwerte
  String path;
  path = "/NeuesMessgerät-";
  path += MessNum;
  path += ".csv";
  File file = SD.open(path, FILE_WRITE);

  Serial.println((String) "Schreibe in " + path + "\nSchreibe Zeile:");
  
  MessNum++;

  String txt;

  for (unsigned int i = 0; i < Loops; i++) {
float I = ((Iraw[i]/4095.0)*3.3)/RI;
float U = ((Uraw[i]/4095.0)*3.3)*Uteiler;
    txt = String(Traw[i] - Tstart);
    txt += ";";
    txt += String(I*1000.0);
    txt += ";";
    txt += String(Iraw[i]);
    txt += ";";
    txt += String(U);
    txt += ";";
    txt += String(Uraw[i]);
    txt += ";";

    txt += "\n";
    file.print(txt);
    file.flush();
    if((i % 500) == 0) {
      Serial.print(";");
      Serial.print(i);
    }
  }

  file.close();
  Serial.println("\nMessung gespeichert");





   
  }
  
}

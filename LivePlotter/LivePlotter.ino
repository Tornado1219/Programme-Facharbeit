
#define RI 51.2                //Definition Wert des Schand Widerstandes in Ohm
#define Uteiler 1730.0 / 50.2  //Definition des Faktors des Spannungsteilers


const int UPin = 32;  //Definition des ADC Messpins für die Spannung
const int IPin = 13;  //Definition des ADC Messpins für die Stromstärke

int Iraw[100];  //Speicher für Rohwerte des Stromstärken ADCs
int Uraw[100];  //Speicher für Rohwerte des Spannungs ADCs

void setup() {

  Serial.begin(115200);  //Initialisierung des Seriellen Ports
  pinMode(IPin, INPUT);  //Initialisierung des Eingangspins für die Strommessung als Eingang
  pinMode(UPin, INPUT);  //Initialisierung des Eingangspins für die Spannungsmessung als Eingang
}

void loop() {

  for (int i = 0; i < 100; i++) {  //Schleife um 100 Messungen am Stück zu aufzunehmen und zu speichern
    Iraw[i] = analogRead(IPin);    //Speichern der Rohdaten aus dem ADC für die Strommessung in den 'i' Speicherplatz des Arrays
    Uraw[i] = analogRead(UPin);    //Speichern der Rohdaten aus dem ADC für die Spannungsmessung in den 'i' Speicherplatz des Arrays
  }

  for (int i = 0; i < 100; i++) {                    //Schleife um 100 Messungen am Seriellen Port auszugeben
    float I = ((Iraw[i] / 4095.0) * 3.3) / RI;       //Umrechnung der Rohdaten an 'i' Stelle des in Ampere
    float U = ((Uraw[i] / 4095.0) * 3.3) * Uteiler;  //Umrechnung der Rohdaten an 'i' Stelle des in Volt
    if (I != 0 || U != 0) {                          //Ausortieren der uninteressanten Daten, also alle die nur 0 anzeigen würden
      Serial.print(I * 1000.0);                      //Ausgabe der Stromstärke in mA im Seriellen Port
      Serial.print(", ");                            //Ausgabe eines Kommas am Seriellen Port zur Trennung der Werte voneinander
      Serial.println(U);                             //Ausgabe des Spannungswertes und eines Absatzes zur Kennzeichnung als ein Wertepaar am Seriellen Port
    }
  }
}

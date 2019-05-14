#include <SoftwareSerial.h>

SoftwareSerial GPSModule(2, 3); // RX, TX

SoftwareSerial Genotronex(10, 11);//TX, RX

const int LED_PIN = 13; // led on D13 will show blink on / off
int BluetoothData;

int updates;
int failedUpdates;
int pos;
int stringplace = 0;

String timeUp;
String labels[12] {"Time: ", "Status: ", "Latitude: ", "Hemisphere: ", "Longitude: ", "Hemisphere: ", "Speed: ", "Track Angle: ", "Date: "};
String nmea[15];


void setup() {
  Serial.begin(9600);
  GPSModule.begin(9600);
  Genotronex.begin(9600);
  Genotronex.println("GPS Start");
  Genotronex.println("Bluetooth On please press 1 or 0 blink LED ..");
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  Serial.flush();
  GPSModule.flush();
  
  while (GPSModule.available() > 0)
  {
    GPSModule.read();
  }

  if (Genotronex.available())
  {
  BluetoothData = Genotronex.read();
   // Genotronex.println("LED  On D13 ON ! ");
    digitalWrite(LED_PIN, HIGH);

      String tempMsg = GPSModule.readStringUntil('\n');
      for (int i = 0; i < tempMsg.length(); i++) {
        if (tempMsg.substring(i, i + 1) == ",") {
          nmea[pos] = tempMsg.substring(stringplace, i);
          stringplace = i + 1;
          pos++;
        }
        if (i == tempMsg.length() - 1) {
          nmea[pos] = tempMsg.substring(stringplace, i);
        }
      }
      updates++;
      nmea[2] = ConvertLat();
      nmea[4] = ConvertLng();
   
    for (int i = 0; i < 9; i++) {
      // if number 1 pressed ....
      digitalWrite(LED_PIN, HIGH);
      
      Serial.println(labels[i]);
      Serial.print(nmea[i]);
      Serial.println("");
   
      Genotronex.println(labels[i]);
      Genotronex.println(nmea[i]);
      Genotronex.println("");
      delay(1000);
    }}
  else {
    failedUpdates++;
  }
  stringplace = 0;
  pos = 0;
}


String ConvertLat() {
  String posneg = "";
  if (nmea[3] == "S") {
    posneg = "-";
  }

  String latfirst;
  float latsecond;
  for (int i = 0; i < nmea[2].length(); i++) {
    if (nmea[2].substring(i, i + 1) == ".") {
      latfirst = nmea[2].substring(0, i - 2);
      latsecond = nmea[2].substring(i - 2).toFloat();
    }
  }
  latsecond = latsecond / 60;

  String CalcLat = "";

  char charVal[9];
  dtostrf(latsecond, 4, 6, charVal);
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLat += charVal[i];
  }
  latfirst += CalcLat.substring(1);
  latfirst = posneg += latfirst;
  return latfirst;
}



String ConvertLng() {
  String posneg = "";
  if (nmea[5] == "W") {
    posneg = "-";
  }

  String lngfirst;
  float lngsecond;
  for (int i = 0; i < nmea[4].length(); i++) {
    if (nmea[4].substring(i, i + 1) == ".") {
      lngfirst = nmea[4].substring(0, i - 2);
      Genotronex.println(lngfirst);
      lngsecond = nmea[4].substring(i - 2).toFloat();
      Genotronex.println(lngsecond);

    }
  }
  lngsecond = lngsecond / 60;
  String CalcLng = "";
  char charVal[9];
  dtostrf(lngsecond, 4, 6, charVal);
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLng += charVal[i];
  }
  lngfirst += CalcLng.substring(1);
  lngfirst = posneg += lngfirst;
  return lngfirst;

}

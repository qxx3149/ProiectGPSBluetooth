/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */
 
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
// The serial connection to the Bluetooth device
SoftwareSerial BlueTooth(11,10);

void setup()
{
  Serial.begin(9600);
  while (!Serial) 
  {
     // wait for serial port to connect. Needed for native USB port only
  }
  ss.begin(GPSBaud);
  BlueTooth.begin(9600);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  ss.listen();
  if (ss.available() > 0)
  {
    gps.encode(ss.read());
    if (gps.location.isUpdated())
    {
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
    }
  }
  //BlueTooth.listen();
  if(BlueTooth.available() > 0)
  {
    Serial.println(" Salut"); 
  }
}

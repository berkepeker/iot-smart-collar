#include <TinyGPS++.h>

// GPS modülünün bağlı olduğu seri port
#define GPS_SERIAL Serial1 // Arduino Uno'da, RX: pin 0, TX: pin 1

// GPS nesnesi oluşturuluyor
TinyGPSPlus gps;

void setup() {
  // Seri port başlatılıyor
  Serial.begin(9600); // Arduino Uno'da, USB seri portu
  GPS_SERIAL.begin(9600,SERIAL_8N1,D1,D0); // GPS modülü için seri port
}

void loop() {
  // GPS modülünden gelen veriyi oku
  while (GPS_SERIAL.available() > 0) {
    if (gps.encode(GPS_SERIAL.read())) {
      // GPS modülünden doğru bir şekilde veri alındıysa
      if (gps.location.isValid()) {
        // Konum geçerli ise
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
      } else {
        Serial.println("Konum bilgisi mevcut değil.");
      }
    }
  }
}
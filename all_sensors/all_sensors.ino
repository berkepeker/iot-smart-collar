#include <TinyGPS++.h>
#include "lsm6dsm.h"  // Deneyap Kart dahili IMU kütüphanesinin eklenmesi
#define GPS_SERIAL Serial1 // Arduino Uno'da, RX: pin 0, TX: pin 1
LSM6DSM IMU;  // IMU için class tanımlanması

const int pulsePin = A0; // Nabız sensörünün sinyal pinini buraya bağlayın
unsigned long previousMillis0 = 0;
unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
const long interval = 100; // Veri gönderme aralığı (milisaniye cinsinden)
int pulseValue = 0;


// GPS nesnesi oluşturuluyor
TinyGPSPlus gps;


void setup() {
  Serial.begin(38400);
  GPS_SERIAL.begin(9600, SERIAL_8N1, D1, D0);
  IMU.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  
  
  
  
  unsigned long currentMillis0 = millis();
  if (currentMillis0 - previousMillis0 >= 5000) {
    while (GPS_SERIAL.available() > 0) {
     
      if (gps.encode(GPS_SERIAL.read())) {
        // GPS modülünden doğru bir şekilde veri alındıysa
        if (gps.location.isValid()) {
          // Konum geçerli ise
          Serial.print("GPS: Latitude: ");
          Serial.println(gps.location.lat(), 6);
          Serial.print("GPS: Longitude: ");
          Serial.println(gps.location.lng(), 6);  
        } else {
            Serial.println("GPS: Konum bilgisi mevcut değil.");
        }
      }
    }
    previousMillis0 = currentMillis0; // Zaman damgasını güncelle
  }



  //NABIZ
   unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    pulseValue = analogRead(pulsePin); // Nabız sensöründen değeri oku
    Serial.println("NABIZ:"+ String(pulseValue) ); // Değeri seri porta yazdır
    previousMillis = currentMillis; // Zaman damgasını güncelle
  }

  //IMU
    unsigned long currentMillis2 = millis();
  if(currentMillis2 - previousMillis1 >= 2000 ) {
    Serial.println("Akselo X: "+ String(IMU.readFloatAccelX()));  // X-eksen akselerometre verisi okunması
    Serial.println("Akselo Y: "+ String(IMU.readFloatAccelY()));  // Y-eksen akselerometre verisi okunması
    Serial.println("Akselo Z: "+ String(IMU.readFloatAccelZ()));  // Z-eksen akselerometre verisi okunması

    Serial.println("Gyro X: "+ String(IMU.readFloatGyroX()));  // X-eksen gyro verisi okunması
    Serial.println("Gyro Y: "+ String(IMU.readFloatGyroY()));  // Y-eksen gyro verisi okunması
    Serial.println("Gyro Z: "+ String(IMU.readFloatGyroZ()));  // Z-eksen gyro verisi okunması

    Serial.println("Derece Celsius: "+ String(IMU.readTempC()));  // Sicaklik verisi okunması (Celsius)
    Serial.println("Derece Fahrenheit: "+String(IMU.readTempF()));  // Sicaklik verisi okunması (Fahrenheit)
    previousMillis1 = currentMillis2; // Zaman damgasını güncelle
  }


 
  




}

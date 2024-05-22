#include "lsm6dsm.h"

#define delayms 1000
#define activityThreshold 1.5 // Hareket kabul edilebilir eşik değeri (örneğin, bu değer 1.5'ten büyükse, hareket olarak kabul edilir)
#define restingThreshold 60000 // 1 dakika = 60,000 milisaniye

LSM6DSM IMU;

unsigned long lastLoopTime = 0; // Son döngü zamanı
unsigned long restingTime = 0;  // Dinlenme süresi
unsigned long activeTime = 0;   // Aktif hareket süresi
unsigned long totalDuration = 0; // Toplam süre

void setup() {
  Serial.begin(115200);
  IMU.begin();
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - lastLoopTime; // Son döngüden bu yana geçen süre
  totalDuration += elapsedTime; // Toplam süreyi güncelle

  float accelMagnitude = getAccelerationMagnitude(); // Anlık ivme büyüklüğünü al

  if (accelMagnitude > activityThreshold) {
    activeTime += elapsedTime; // Aktif hareket süresini güncelle
    Serial.println("Hayvan hareket ediyor...");
    restingTime = 0; // Eğer hayvan hareket ediyorsa dinlenme süresini sıfırla
  } else {
    restingTime += elapsedTime; // Dinlenme süresini güncelle
    if (restingTime >= restingThreshold) {
      Serial.println("Hayvan uyuyor...");
    } else {
      Serial.println("Hayvan dinleniyor...");
    }
  }

  // Hangi saatlerde daha aktif olduğunu kontrol etmek için zamanlamayı buraya ekleyebilirsiniz.

  lastLoopTime = currentTime; // Son döngü zamanını güncelle

  delay(delayms);
}

float getAccelerationMagnitude() {
  float accelX = IMU.readFloatAccelX();
  float accelY = IMU.readFloatAccelY();
  float accelZ = IMU.readFloatAccelZ();
  return sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ); // 3 eksenin büyüklüğünü hesapla
}

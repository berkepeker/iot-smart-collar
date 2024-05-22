const int pulsePin = A0; // Nabız sensörünün sinyal pinini buraya bağlayın
unsigned long previousMillis = 0;
const long interval = 60; // Veri gönderme aralığı (milisaniye cinsinden)
int pulseValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    pulseValue = analogRead(pulsePin); // Nabız sensöründen değeri oku
    Serial.println(pulseValue); // Değeri seri porta yazdır
    previousMillis = currentMillis; // Zaman damgasını güncelle
  }
}
#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* thingspeakApiKey = "76EL9MOEG7OM83SK"; 
const char* server = "http://api.thingspeak.com/update";

const int ledPin = 2;
bool lampuHidup = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Terhubung!");

  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov"); // UTC+7 untuk WIB
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Gagal mendapatkan waktu");
    return;
  }

  int hourNow = timeinfo.tm_hour;
  Serial.printf("Jam sekarang: %02d:%02d:%02d\n", hourNow, timeinfo.tm_min, timeinfo.tm_sec);


  if (hourNow >= 15 || hourNow < 6) {
    digitalWrite(ledPin, HIGH);
    lampuHidup = true;
  } else {
    digitalWrite(ledPin, LOW);
    lampuHidup = false;
  }

  kirimStatusKeThingspeak(lampuHidup ? 1 : 0);
  delay(15000); // Kirim data setiap 15 detik 
  Serial.println(lampuHidup ? "Lampu HIDUP" : "Lampu MATI");
}

void kirimStatusKeThingspeak(int status) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "?api_key=" + thingspeakApiKey + "&field1=" + status;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.println("Data terkirim ke ThingSpeak!");
    } else {
      Serial.println("Gagal mengirim data");
    }

    http.end();
  }
}

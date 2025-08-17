#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"
#include <Wire.h>
#include "RTClib.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* thingspeakApiKey = "76EL9MOEG7OM83SK"; 
const char* server = "http://api.thingspeak.com/update";

const int ledPins[] = {2, 4, 5, 18, 19};
const int ledCount = sizeof(ledPins) / sizeof(ledPins[0]);
bool lampuHidup = false;

RTC_DS1307 rtc;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Hubungkan ke WiFi
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Terhubung!");

  // Sinkronisasi waktu ke NTP
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov"); // UTC+7 WIB

  if (!rtc.begin()) {
    Serial.println("RTC tidak ditemukan!");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC tidak berjalan, set waktu sekarang...");
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      rtc.adjust(DateTime(1900 + timeinfo.tm_year, timeinfo.tm_mon + 1,
                          timeinfo.tm_mday, timeinfo.tm_hour,
                          timeinfo.tm_min, timeinfo.tm_sec));
    }
  }
}

void loop() {
  struct tm timeinfo;

  int hourNow;
  int minuteNow;
  int secondNow;

  if (getLocalTime(&timeinfo)) {
    // Ambil waktu dari NTP
    hourNow = timeinfo.tm_hour;
    minuteNow = timeinfo.tm_min;
    secondNow = timeinfo.tm_sec;
  } else {
    // Jika gagal, pakai RTC
    DateTime now = rtc.now();
    hourNow = now.hour();
    minuteNow = now.minute();
    secondNow = now.second();
  }

  Serial.printf("Jam sekarang: %02d:%02d:%02d\n", hourNow, minuteNow, secondNow);

  // Logika Lampu: ON dari jam 11 siang sampai jam 6 pagi
  if (hourNow >= 11 || hourNow < 6) {
    for (int i = 0; i < ledCount; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
    lampuHidup = true;
  } else {
    for (int i = 0; i < ledCount; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    lampuHidup = false;
  }

  kirimStatusKeThingspeak(lampuHidup ? 1 : 0);
  delay(15000); // kirim setiap 15 detik
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

# Lampu Otomatis Berbasis Waktu dan Internet menggunakan Wokwi Simulator dengan ESP32 dan Thingspeak

## Deskripsi
Proyek ini merupakan sistem lampu otomatis yang dikendalikan berdasarkan waktu dan dapat dipantau melalui internet menggunakan ESP32. Sistem ini memanfaatkan Wokwi Simulator untuk simulasi perangkat keras dan Thingspeak sebagai platform IoT untuk monitoring status lampu secara online.

## Fitur
- Lampu otomatis menyala pada jam 18:00 hingga 06:00 WIB, dan mati di luar jam tersebut.
- Status lampu dikirimkan secara periodik ke Thingspeak setiap 15 detik.
- Simulasi perangkat keras menggunakan Wokwi.
- Monitoring status lampu secara real-time melalui Thingspeak.

## Kebutuhan Hardware
- ESP32 DevKit v4 (simulasi di Wokwi)
- LED (sebagai indikator lampu)
- Resistor 220Ω

## Kebutuhan Software
- [Wokwi Simulator](https://wokwi.com/)
- Arduino IDE (atau editor yang mendukung file `.ino`)
- Library: `WiFi.h`, `HTTPClient.h`, `time.h`
- Akun Thingspeak (untuk mendapatkan API Key)

## Diagram Rangkaian
Simulasi rangkaian dapat dilihat pada file `diagram.json` atau langsung di Wokwi. Berikut gambaran koneksi:
- LED dihubungkan ke pin GPIO 2 pada ESP32 melalui resistor 220Ω ke GND.

```
ESP32 (Pin 2) ----> LED Anoda
LED Katoda ----> Resistor 220Ω ----> GND
```

## Cara Menjalankan Simulasi di Wokwi
1. Buka [Wokwi Simulator](https://wokwi.com/).
2. Upload file `sketch.ino` dan `diagram.json` ke proyek Wokwi Anda.
3. Jalankan simulasi. Pastikan koneksi internet aktif agar ESP32 dapat terhubung ke NTP dan Thingspeak.

## Konfigurasi Thingspeak
1. Daftar dan login ke [Thingspeak](https://thingspeak.com/).
2. Buat channel baru dan catat API Key-nya.
3. Ganti nilai `thingspeakApiKey` pada `sketch.ino` dengan API Key milik Anda jika ingin menggunakan channel sendiri.

## Penjelasan Kode Utama
- ESP32 terhubung ke WiFi dan sinkronisasi waktu menggunakan NTP.
- Lampu (LED) akan otomatis menyala pada jam 15:00 hingga 06:00 WIB.
- Status lampu dikirim ke Thingspeak melalui HTTP GET setiap 15 detik.

## Monitoring
- Buka channel Thingspeak Anda untuk melihat status lampu pada field yang telah dikonfigurasi.

## File
- `sketch.ino` : Kode utama ESP32
- `diagram.json` : Diagram rangkaian untuk simulasi Wokwi
- `wokwi-project.txt` : (Opsional) Konfigurasi proyek Wokwi

## Kredit
- Penulis: Agung Yuda Pratama
- Simulasi oleh: [Wokwi](https://wokwi.com/)
- IoT Platform: [Thingspeak](https://thingspeak.com/)

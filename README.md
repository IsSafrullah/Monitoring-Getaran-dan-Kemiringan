# Sistem Monitoring Getaran dan Kemiringan Berbasis ESP32 dan MPU6050

## Deskripsi Proyek
Proyek ini bertujuan untuk memantau kondisi **getaran** dan **kemiringan** suatu perangkat atau mesin secara _real-time_. Sistem ini sangat cocok digunakan pada alat atau struktur yang perlu diawasi dari potensi getaran berlebih atau kemiringan yang tidak wajar.

### Tujuan
- Mendeteksi perubahan sudut kemiringan (pitch & roll).
- Mendeteksi getaran berdasarkan percepatan linier.
- Menampilkan data secara lokal menggunakan OLED.
- Mengirimkan data ke aplikasi **Blynk** untuk pemantauan jarak jauh.

---

## Komponen yang Digunakan
- ESP32 (mikrokontroler utama)
- MPU6050 (accelerometer + gyroscope)
- OLED SSD1306 (resolusi 128x32)
- Modul WiFi (terintegrasi dalam ESP32)
- Power Supply (misalnya dari USB 5V)

---

## Fitur Utama
- Deteksi getaran berbasis percepatan sumbu XYZ dari MPU6050.
- Deteksi sudut kemiringan menggunakan perhitungan pitch dan roll.
- Tampilan data pada layar OLED secara lokal.
- Koneksi WiFi ke platform **Blynk** untuk pengawasan jarak jauh.
- Pengaturan ambang batas getaran dan kemiringan langsung dari aplikasi Blynk.
- Dashboard Blynk menampilkan data dan status kemiringan atau getaran.

---

## Diagram Blok, Flowchart, dan Schematic Sistem 
Lihat gambar lengkap di dokumentasi atau di folder `/assets` pada repositori.
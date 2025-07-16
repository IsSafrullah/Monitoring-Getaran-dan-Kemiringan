#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#define BLYNK_PRINT Serial

#define WIFI_SSID "AccessPoint_Kel1"
#define WIFI_PASSWORD "12345678"
#define BLYNK_TEMPLATE_ID "TMPL6VG7rR-nE"
#define BLYNK_TEMPLATE_NAME "Monitoring Vibration and Tilt"
#define BLYNK_AUTH_TOKEN "TmUXjvkS8W9jPyryVzX2zVzY30WgpbMm"

#include <BlynkSimpleEsp32.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

MPU6050 mpu;

bool dmpReady = false;
uint8_t devStatus;
uint16_t packetSize;
uint8_t fifoBuffer[64];

Quaternion q;
VectorFloat gravity;
float ypr[3];
VectorInt16 aa, aaReal;

String statusGetaran = "Normal";
String statusMiring = "Normal";

float batasGetaran = 0.00;
float batasMiring = 0.0;
float prevAccMag = 0;

BLYNK_WRITE(V6) {
  batasGetaran = param.asFloat();
}

BLYNK_WRITE(V7) {
  batasMiring = param.asFloat();
}

void setup() {
  Wire.begin();
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Initializing MPU...");
  display.display();

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  Serial.println("MPU6050 connected. Initializing DMP...");
  devStatus = mpu.dmpInitialize();

  // Custom offset (tune if needed)
  mpu.setXAccelOffset(0);
  mpu.setYAccelOffset(0);
  mpu.setZAccelOffset(0);
  mpu.setXGyroOffset(0);
  mpu.setYGyroOffset(0);
  mpu.setZGyroOffset(0);

  if (devStatus == 0) {
    mpu.CalibrateAccel(10);  // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateGyro(10);
    Serial.println("These are the Active offsets: ");
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
    Serial.println("DMP ready.");
  } else {
    Serial.print("DMP Init failed (code ");
    Serial.print(devStatus);
    Serial.println(")");
    while (1);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connect to Blynk");
  display.display();
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);
  delay(1000);
}

void loop() {
  Blynk.run();
  if (!dmpReady) return;

  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    mpu.dmpGetAccel(&aa, fifoBuffer);
    mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);

    // Hitung sudut derajat
    float yaw = ypr[0] * 180 / M_PI;
    float pitch = ypr[1] * 180 / M_PI;
    float roll = ypr[2] * 180 / M_PI;

    // Getaran → dari percepatan total (LSB/g = 16384)
    float accMag = sqrt(aaReal.x * aaReal.x + aaReal.y * aaReal.y + aaReal.z * aaReal.z) / 16384.0;

    float deltaAcc = abs(accMag - prevAccMag);
    prevAccMag = accMag;

    if (deltaAcc > batasGetaran) {
      statusGetaran = "Getar!";
    } else {
      statusGetaran = "Normal";
    }


    // Kemiringan (roll/pitch di luar batas)
    if (abs(pitch) > batasMiring || abs(roll) > batasMiring) {
      statusMiring = "Miring!";
    } else {
      statusMiring = "Normal";
    }

    // OLED Output
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Kemiringan: ");
    display.println(statusMiring);
    display.setCursor(0, 8);
    display.print("Getaran: ");
    display.println(statusGetaran);
    display.setCursor(0, 16);
    display.print("Yaw: ");
    display.println(yaw, 1);
    display.setCursor(0, 24);
    display.print("Pitch: ");
    display.println(pitch, 1);
    display.setCursor(0, 32);
    display.print("Roll: ");
    display.println(roll, 1);
    display.setCursor(0, 40);
    display.print("deltaAcc: ");
    display.println(deltaAcc, 2);
    display.setCursor(0, 48);
    display.print("Batas Getaran: ");
    display.println(batasGetaran, 2);
    display.setCursor(0, 56);
    display.print("Batas Miring: ");
    display.println(batasMiring, 1);
    display.display();

    Blynk.virtualWrite(V0, statusMiring);
    Blynk.virtualWrite(V1, statusGetaran);
    Blynk.virtualWrite(V2, yaw);
    Blynk.virtualWrite(V3, pitch);
    Blynk.virtualWrite(V4, roll);
    Blynk.virtualWrite(V5, deltaAcc);
  }
  delay(100);
}

//Linbreria del display
#include <LiquidCrystal.h>

//Librerias del acelerometro
#include <Wire.h>
#include <Adafruit_Sensor.h> 
#include <Adafruit_ADXL345_U.h>

//Librerias del lector rfid
#include <SPI.h>
#include <MFRC522.h>
#include <RFID.h>

//Libreria DHT11
#include <DHT.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

#define SS_PIN 10
#define RST_PIN 9

#define IR          A0
#define buttonIR    A1
#define buttonRFID  A2
#define buttonACC   A3
#define detector    2

#define DHTPIN 1
#define DHTTYPE DHT11

#define MQ2 0

int infrared_value;
int detector_value;

LiquidCrystal lcd(7, 8, 3, 4, 5, 6);
RFID rfid(SS_PIN, RST_PIN);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  accel.begin();
  SPI.begin();
  rfid.init();
  dht.begin();
  pinMode(detector, INPUT);
  pinMode(MQ2, INPUT);
}

void loop() {
  lcd.begin(16, 2);
  lcd.print("Select mode");
  if(analogRead(buttonIR)>50){
    while(analogRead(buttonIR)>50){
      detector_value = digitalRead(detector);
      infrared_value = analogRead(IR);
      lcd.begin(16, 2);
      lcd.print("Infrarojo: ");      
      lcd.print(infrared_value);    
      lcd.setCursor(0, 1);
      
      lcd.print("Distancia: ");
      lcd.print(detector_value);
      delay(1000);
      
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      lcd.begin(16, 2);
      lcd.print("Hum: ");
      lcd.print(h);
      lcd.setCursor(0, 1);
      lcd.print("Temp: ");
      lcd.print(t);
      lcd.print(" *C "); 
      delay(1000);

      lcd.begin(16, 2);
      int stat = digitalRead(MQ2);
      lcd.print("Sensor MQ2: ");
      lcd.print(stat);
      delay(1000);
    }
  }
  else if(analogRead(buttonRFID)>50){
    while(analogRead(buttonRFID)>50){
      lcd.begin(16, 2);
      lcd.print("Approach a card");
      if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
          lcd.begin(16, 2);
          lcd.print("The card's id is: ");
          lcd.setCursor(0, 1);
          lcd.print(rfid.serNum[0],HEX);
          lcd.print(rfid.serNum[1],HEX);
          lcd.print(rfid.serNum[2],HEX);
          lcd.print(rfid.serNum[3],HEX);
          lcd.print(rfid.serNum[4],HEX);
          delay(5000);
        }
        rfid.selectTag(rfid.serNum);
      }
      rfid.halt();
    }
  }
  else if(analogRead(buttonACC)>50){
    while(analogRead(buttonACC)>50){
      Serial.println(analogRead(buttonACC));
      sensors_event_t event; 
      accel.getEvent(&event);
      lcd.begin(16, 2);
      lcd.print("X:"); lcd.print(event.acceleration.x); lcd.print("");
      lcd.print(" Y:"); lcd.print(event.acceleration.y); lcd.print("");
      lcd.setCursor(0, 1);
      lcd.print("Z:"); lcd.print(event.acceleration.z); lcd.print("");
      lcd.print(" m/s^2");
      delay(500);      
    }
  }
  delay(500);
}
CODE:

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include "HX711.h"
 
// -------- WIFI --------

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";
 
// -------- RFID --------

#define SS_PIN 5

#define RST_PIN 4

MFRC522 rfid(SS_PIN, RST_PIN);
 
// -------- LCD --------

LiquidCrystal_I2C lcd(0x27, 16, 2);
 
// -------- DHT --------

#define DHTPIN 15

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
 
// -------- LED --------

#define GREEN_LED 13

#define RED_LED 14

#define BUILTIN_LED 2
 
// -------- HX711 --------

#define DT 32

#define SCK 33

HX711 scale;
 
float calibration_factor = 546.0;
 
// -------- SESSION --------

bool sessionActive = false;

String sessionUser = "";
 
unsigned long lastActivityTime = 0;

#define TIMEOUT 30000
 
// -------- TIMERS --------

unsigned long lcdTimer = 0;

unsigned long tempTimer = 0;

unsigned long weightTimer = 0;
 
// -------- DATA --------

float temperature = 0;

float weight = 0;
 
// -------- USER --------

String getUser(String uid){

  uid.toUpperCase();

if(uid == "YOUR_CARD_UID_1") return "User 1";
if(uid == "YOUR_CARD_UID_2") return "User 2";

  return "Unknown";

}
 
// -------- BLYNK CONNECT --------

BLYNK_CONNECTED() {

  digitalWrite(BUILTIN_LED, HIGH);
 
  lcd.clear();

  lcd.print("Blynk Connected");
 
  Serial.println("✅ Blynk Connected");
 
  delay(1200);

  lcd.clear();

  lcd.print("Scan Card...");

}
 
// -------- END SESSION --------

void endSession(){

  Serial.println("SESSION_END," + sessionUser);
 
  lcd.clear();

  lcd.print("Thank You!");
 
  sessionActive = false;

  sessionUser = "";
 
  delay(2000);

  lcd.clear();

  lcd.print("Scan Card...");

}
 
// -------- SETUP --------

void setup(){

  Serial.begin(115200);
 
  pinMode(GREEN_LED, OUTPUT);

  pinMode(RED_LED, OUTPUT);

  pinMode(BUILTIN_LED, OUTPUT);
 
  lcd.init();

  lcd.backlight();
 
  lcd.print("Connecting WiFi");
 
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {

    delay(300);

  }
 
  lcd.clear();

  lcd.print("WiFi Connected");

  Serial.println("✅ WiFi Connected");
 
  delay(1000);
 
  lcd.clear();

  lcd.print("Connecting Blynk");
 
  Blynk.config(BLYNK_AUTH_TOKEN);

  Blynk.connect();
 
  SPI.begin();

  rfid.PCD_Init();

  dht.begin();
 
  // -------- HX711 INIT --------

  scale.begin(DT, SCK);

  scale.set_scale(calibration_factor);

  scale.tare();  // RESET TO ZERO
 
  Serial.println("✅ HX711 Ready");
 
  lcd.clear();

  lcd.print("Scan Card...");

}
 
// -------- LOOP --------

void loop(){

  Blynk.run();
 
  // -------- TEMPERATURE --------

  if(millis() - tempTimer > 2000){

    float t = dht.readTemperature();

    if(!isnan(t)){

      temperature = t;

      Blynk.virtualWrite(V0, temperature);

      Serial.print("Temp: ");

      Serial.println(temperature);

    }

    tempTimer = millis();

  }
 
  // -------- WEIGHT --------

  if(millis() - weightTimer > 1000){

    weight = scale.get_units(5);
 
    if(weight < 0) weight = 0;
 
    Serial.print("Weight(g): ");

    Serial.println(weight);
 
    Blynk.virtualWrite(V4, weight);
 
    // ⚠️ ALERT

    if(weight < 250){

      Blynk.logEvent("weight_alert", "⚠️ Weight below 250g");

    }
 
    weightTimer = millis();

  }
 
  // -------- RFID --------

  if(rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
 
    String uid="";

    for(byte i=0;i<rfid.uid.size;i++){

      if(rfid.uid.uidByte[i]<0x10) uid+="0";

      uid+=String(rfid.uid.uidByte[i],HEX);

    }
 
    uid.toUpperCase();

    String user=getUser(uid);
 
    Serial.print("Card UID: ");

    Serial.println(uid);
 
    if(user!="Unknown"){
 
      if(!sessionActive){

        sessionActive=true;

        sessionUser=user;

        lastActivityTime=millis();
 
        lcd.clear();

        lcd.print("Welcome");

        lcd.setCursor(0,1);

        lcd.print(user);
 
        Serial.println("SESSION_STARTED,"+user);

      }
 
      else if(user.equalsIgnoreCase(sessionUser)){

          endSession();

      }
 
      else{

        lcd.clear();

        lcd.print("User Active!");
 
        Serial.println("❌ ACCESS DENIED");
 
        digitalWrite(RED_LED, HIGH);

        delay(1000);

        digitalWrite(RED_LED, LOW);

      }

    }
 
    rfid.PICC_HaltA();

  }
 
  // -------- SERIAL FROM PYTHON --------

  if(Serial.available()){

    String data = Serial.readStringUntil('\n');

    data.trim();
 
    Serial.print("Received: ");

    Serial.println(data);
 
    int i1 = data.indexOf(',');

    int i2 = data.indexOf(',', i1+1);
 
    if(i1>0 && i2>i1){

      String user = data.substring(0,i1);
 
      if(sessionActive && user.equalsIgnoreCase(sessionUser)){

        lastActivityTime = millis();
 
        digitalWrite(GREEN_LED, HIGH);

        delay(200);

        digitalWrite(GREEN_LED, LOW);

      }

    }

  }
 
  // -------- TIMEOUT --------

  if(sessionActive && millis() - lastActivityTime > TIMEOUT){

    endSession();

  }
 
  // -------- LCD DISPLAY --------

  if(millis() - lcdTimer > 1500){

    lcd.clear();
 
    if(sessionActive){

      lcd.print(sessionUser);

      lcd.setCursor(0,1);

      lcd.print("T:");

      lcd.print(temperature);

      lcd.print(" W:");

      lcd.print(weight);

    }

    else{

      lcd.print("T:");

      lcd.print(temperature);

      lcd.print(" W:");

      lcd.print(weight);
 
      lcd.setCursor(0,1);

      lcd.print("Scan Card...");

    }
 
    lcdTimer = millis();

  }

}

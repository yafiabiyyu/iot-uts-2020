#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR3W.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#define sensorLDR A0
#define DHTPIN D4
#define DHTTYPE DHT11 
#define SS_PIN 15  
#define RST_PIN 0 
#define lamputrue 16
#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);
int statuss = 0;
int out = 0;
DHT dht(DHTPIN, DHTTYPE); 
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7); 
float nilaiSensor;


void setup()
{
  lcd.begin (16,2); 
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  Serial.begin(9600);
  pinMode(DHTPIN,INPUT);
  dht.begin();
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(sensorLDR, INPUT);
  pinMode(lamputrue, OUTPUT);
}


void loop()
{ 
  digitalWrite(lamputrue, LOW); 
  statuss = 0;
  lcd.setCursor(0,0);
  lcd.print("Silahkan Tap");
  lcd.setCursor(0,1);
  lcd.print("Kartu Anda");
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if(statuss == 1){
    lcdPrint();
    lcd.clear();
  }else{
    if (content.substring(1) == "D1 EE F4 1D")
    {
      lcd.setCursor(0,0);
      lcd.print("Selamat Datang");
      lcd.setCursor(0,1);
      lcd.print("---------------");
      delay(2000);
      lcdPrint();
      statuss = 1;
      lcd.clear();
   }
    else{
      statuss = 0;
      lcd.setCursor(0,0);
      lcd.print("kartu anda    ");
      lcd.setCursor(0,1);
      lcd.print("tidak  falid");
      lampuOn();
      delay(3000);
      lcd.clear();
    }
  }
}

void lcdPrint(){
  lcd.home();
//  cahaya
  nilaiSensor = analogRead(sensorLDR);
  float voltage = nilaiSensor * (5.0 / 1023.0);
  Serial.println(voltage);
  lcd.setCursor(0,1);
  if(voltage > 1){
     lcd.print("Ruangan: ");
     lcd.print("Redup"); 
  }else{
     lcd.print("Ruangan: ");
     lcd.print("Terang");  
  }
//  end cahaya
//start suhu
  float h = dht.readHumidity(); 
  float t = dht.readTemperature(); 
  float f = (t*1.8)+32; 
  lcd.setCursor(0,0);
  lcd.print("KL:");
  lcd.print(h); 
  lcd.print(" SH:"); 
  lcd.print(t); 
  lampuBlip();
  //end suhu
}

void lampuBlip(){
  for(int i = 0 ; i < 10;i++){
    digitalWrite(lamputrue, HIGH);  
    delay(1000);   
    digitalWrite(lamputrue, LOW); 
    delay(1000);
  }
}

void lampuOn(){
  digitalWrite(lamputrue, HIGH);  
}

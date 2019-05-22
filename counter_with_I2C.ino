#include <ESP8266WiFi.h>
#include <time.h>

#include <SPI.h>
#include <Wire.h>

#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Arunjith";
const char* password = "asdfghjkl123456789";

int ledPin = 13;
int dst = 0;

unsigned long api_mtbs = 60000; //mean time between api requests
unsigned long api_lasttime;   //last time api request has been done


long subs = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
String payload = "";

void setup() {
  Serial.begin(115200);
  
  //lcd.begin(16, 2);  
  Wire.begin(D2, D1);
  lcd.begin();
  
  WiFi.begin(ssid,password);

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      lcd.clear();
      lcd.print("  Connecting..");
  }

  Serial.println("Wifi Connected!");
  Serial.println("IP:");
  Serial.println(WiFi.localIP() );

  configTime(5.5 * 3600, 1800, "pool.ntp.org","time.nist.gov");
  Serial.println("\nWaiting for NTP...");

  while(!time(nullptr)){
     Serial.print("*");

     delay(1000);
  }
  Serial.print("\nTime response....OK"); 

  delay(1000);
  DrawYoutube();
}

void loop() {

  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);

  if(p_tm) {

    DrawYoutube();
    
    Serial.print(p_tm->tm_mday);
    Serial.print("/");
    Serial.print(p_tm->tm_mon + 1);
    Serial.print("/");
    Serial.print(p_tm->tm_year + 1900);
    lcd.setCursor(4, 0);
    lcd.print(" ");
    
    lcd.setCursor(5, 0);
    if(p_tm->tm_hour < 12) {
      if( p_tm->tm_hour <10)
        lcd.print("0");
      lcd.print(p_tm->tm_hour);
    } else {
      if(p_tm->tm_hour - 12 == 0) {
        lcd.print("12");
      } else {
        if( p_tm->tm_hour - 12 < 10)
          lcd.print("0");
        lcd.print(p_tm->tm_hour - 12);
      }
    }
    
    lcd.print(":");
    if( p_tm->tm_min <10)
      lcd.print("0");
    lcd.print(p_tm->tm_min);
    lcd.print(":");
    if( p_tm->tm_sec <10)
      lcd.print("0"); 
    lcd.print(p_tm->tm_sec);

    if(p_tm->tm_hour < 12) {
      lcd.print(" AM");
    } else {
      lcd.print(" PM");
    }
    
    if(payload == "") {
      lcd.setCursor(5, 1);
      for(int i=0; i<=10; i++) {

        if(dst%2 == 0){
          lcd.print(" ");
          lcd.setCursor(5 + i, 1);
          delay(100);
          lcd.print(".");
        } else {
          lcd.print(".");
          lcd.setCursor(5 + i, 1);
          delay(100);
          lcd.print(" ");
        }
      }
      dst++;
    }
  
    delay(1000); // update every 1 sec
  
    if (millis() - api_lasttime > api_mtbs)  {
   
      if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
     
        HTTPClient http;  //Declare an object of class HTTPClient
     
        http.begin("http://sparkpost.000webhostapp.com");  //Specify request destination
        int httpCode = http.GET();                                                                  //Send the request
     
        if (httpCode > 0) { //Check the returning code
        
          payload = http.getString();   //Get the request response payload
          Serial.println(payload);  

          lcd.clear();
          lcd.setCursor(5, 1);
          lcd.print("Subs: ");
          lcd.setCursor(12, 1);
          lcd.print(payload);
     
        }
     
        http.end();   //Close connection
      } else {
        payload = "";
      }
      api_lasttime = millis();
    }
  }

}


//draw youtube logo
void DrawYoutube()
{
  //initiate custom char for youtube
  youtubelcdcharinitiate();
  //print youtube logo
  lcd.setCursor(0,0);
  lcd.write(byte(0));
  lcd.setCursor(0,1);
  lcd.write(byte(1));
  lcd.setCursor(1,0);
  lcd.write(byte(2));
  lcd.setCursor(1,1);
  lcd.write(byte(3));
  lcd.setCursor(2,0);
  lcd.write(byte(4));
  lcd.setCursor(2,1);
  lcd.write(byte(5));
  lcd.setCursor(3,0);
  lcd.write(byte(6));
  lcd.setCursor(3,1);
  lcd.write(byte(7));
  // subscrribers text
  lcd.setCursor(5,0);
  //clear previous number
  lcd.setCursor(5,1);
  //count
  lcd.setCursor(5,1);
}
//initiate the lcd custom charcters For Youtube Icon
void youtubelcdcharinitiate()
{
    //number 0 charaters 2x2 decimals
    byte C0[8] = {0x03,0x07,0x0F,0x1F,0x1F,0x1F,0x1F,0x1F};
    byte C1[8] = {0x1F,0x1F,0x1F,0x1F,0x1F,0x0F,0x07,0x03};
    byte C2[8] = {0x1F,0x1F,0x1B,0x19,0x18,0x18,0x18,0x18};
    byte C3[8] = {0x18,0x18,0x18,0x18,0x19,0x1B,0x1F,0x1F};
    byte C4[8] = {0x1F,0x1F,0x1F,0x1F,0x1F,0x0F,0x07,0x03};
    byte C5[8] = {0x03,0x07,0x0F,0x1F,0x1F,0x1F,0x1F,0x1F};
    byte C6[8] = {0x18,0x1C,0x1E,0x1F,0x1F,0x1F,0x1F,0x1F};
    byte C7[8] = {0x1F,0x1F,0x1F,0x1F,0x1F,0x1E,0x1C,0x18};
    
    lcd.createChar(0 , C0);
    lcd.createChar(1 , C1);
    lcd.createChar(2 , C2);
    lcd.createChar(3 , C3);
    lcd.createChar(4 , C4);
    lcd.createChar(5 , C5);
    lcd.createChar(6 , C6);
    lcd.createChar(7 , C7);
}

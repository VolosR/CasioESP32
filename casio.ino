#include <TFT_eSPI.h> 
#include "sat3.h"
#include "font.h"
TFT_eSPI    tft = TFT_eSPI(); 

#include <WiFi.h>
#include "time.h"

const char* ssid     = "IGK20";
const char* password = "18072019";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec =3600;
const int   daylightOffset_sec = 3600;


#define background 0xB635

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 5;


void setup() {

  Serial.begin(9600);
   WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(5, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, 250);
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 
 tft.init();
 tft.setRotation(2);
 tft.setSwapBytes(true);

 tft.fillScreen(TFT_WHITE); 

 tft.pushImage(0,0,240,240,sat3);
 tft.setTextColor(0x0081,background);
 tft.fillRoundRect(48,107,138,48,5,background);
 tft.setFreeFont(&DSEG7_Classic_Bold_30);
 tft.drawString("22:58",46,126);

 tft.setFreeFont(&DSEG7_Classic_Bold_20);
 tft.drawString("50",154,136);

  tft.setFreeFont(&DSEG14_Classic_Bold_18);
 tft.drawString("MO",94,96);
 tft.drawString("30",156,96);
 
// DSEG14_Classic_Bold_18
}

String tM="ww";


void loop() {  delay(1000);
  printLocalTime();

}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  /*Serial.println(&timeinfo);
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");
  */

  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  

  char timeMin[3];
  strftime(timeMin,3, "%M", &timeinfo);
  String m=String(timeMin);

  char timeSec[3];
  strftime(timeSec,3, "%S", &timeinfo);

   char timeWeekDay[3];
  strftime(timeWeekDay,10, "%A", &timeinfo);
   String d=String(timeWeekDay);

   char timeDay[3];
  strftime(timeDay,3, "%d", &timeinfo);


  if(tM!=m){
  tft.setFreeFont(&DSEG7_Classic_Bold_30);
  tft.drawString(String(timeHour)+":"+String(timeMin),46,126);
  tM=m;
  }

  tft.setFreeFont(&DSEG7_Classic_Bold_20);
  tft.drawString(String(timeSec),154,136);

 
   tft.setFreeFont(&DSEG14_Classic_Bold_18);
 tft.drawString(d.substring(0,2),94,96);
 tft.drawString(String(timeDay),156,96);
  
  
  
  Serial.println();
}

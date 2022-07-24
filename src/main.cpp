#include <Arduino.h>
#include <RTClib.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SSID "testnetwork" 
#define PSSWD "testpassword"

String serverName = "http://api.konnilol.tk";

TFT_eSPI tft = TFT_eSPI(); //initialize display library

byte omm = 99;
bool initial = 1;
byte xcolon = 0;
unsigned int color = 0;

static uint8_t conv2d(const char* p) {
  uint8_t v =  0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

RTC_DS3231 rtc; //initialize RTC

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


String getHTTP(String path) {

  String response;
  
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(path.c_str());

    int responseCode = http.GET();

    if(responseCode>0) {
      Serial.print("HTTP response code: ");
      Serial.println(responseCode);
      response = http.getString();
      Serial.println(response);
    }
    
    http.end();

  }
  int str_len = response.length() + 1;
  char char_array[str_len];
  response.toCharArray(char_array, str_len);
  return response;
}

void wifiConnect() {

  WiFi.begin(SSID, PSSWD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network. Your IP Address: ");
  Serial.println(WiFi.localIP());
}

void setup() {

  if(WiFi.status() != WL_CONNECTED) {
  
    Serial.begin(115200);
    Serial.print("entered void setup");
  
    wifiConnect();

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
  
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);

    tft.print("Setup loop");

    Serial.print("Initialized Screen");

  
    if (! rtc.begin()) {
      tft.setCursor(5, 5);
      while (1) delay(10);
    }
  
    if (rtc.lostPower()) {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    Serial.print("Initialed RTC");

//  delay(3000);
    tft.fillScreen(TFT_BLACK);
  }
}


void loop() {

  Serial.print("Entered loop \n");

  String response;
  String weather;


  if(WiFi.status() == WL_CONNECTED) {
    //Serial.println("Entered first if");

    String serverPath = serverName+"/";


    DateTime now =  rtc.now();
    //Serial.println("init rtc complete");
  
    //tft.fillScreen(TFT_BLACK);

    tft.setCursor(10, 64);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.printf("%02d %02d Temp: %.2f C\n", now.hour(), now.minute(), rtc.getTemperature());
    tft.print("Data from API: ");
    tft.println(getHTTP(serverPath));
    //Serial.println("completed wite to screen");
    delay(900);
  }
  else if (WiFi.status() != WL_CONNECTED){
    wifiConnect();
  }


}


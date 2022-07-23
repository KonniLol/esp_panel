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

void setup() {

  Serial.begin(115200);
  Serial.print("entered void setup");
  

  WiFi.begin(SSID, PSSWD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network. Your IP Address: ");
  Serial.println(WiFi.localIP());

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


void loop() {

  Serial.print("Entered loop \n");

  String response;

  //tft.fillScreen(TFT_BLACK);

  if(WiFi.status() == WL_CONNECTED) {
    //Serial.println("Entered first if");
    HTTPClient http;

    String serverPath = serverName+"/";

    http.begin(serverPath.c_str());

    int responseCode = http.GET();

    if(responseCode>0) {
      //Serial.println("Entered if responseCode");
      Serial.print("HTTP response code: ");
      Serial.println(responseCode);
      response = http.getString();
      Serial.println(response);
    }
    
    //Serial.println("Exited if responseCode");

    http.end();
    DateTime now =  rtc.now();
    //Serial.println("init rtc complete");
  

    tft.setCursor(10, 64);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.printf("%d %d Temp: %.2f C", now.hour(), now.minute(), rtc.getTemperature());
    tft.print("Data from API: ");
    tft.print(response);
    //Serial.println("completed wite to screen");
    delay(900);
  }


}

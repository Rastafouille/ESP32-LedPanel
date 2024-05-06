//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 01_Showing_Text
//----------------------------------------Including the libraries.
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFi.h>
#include "time.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Dhole_weather_icons32px.h"
#include "credentials.h"



//----------------------------------------Defines the connected PIN between P5 and ESP32.
#define R1_PIN 19
#define G1_PIN 13
#define B1_PIN 18
#define R2_PIN 5
#define G2_PIN 12
#define B2_PIN 17

#define A_PIN 16
#define B_PIN 14
#define C_PIN 4
#define D_PIN 27
#define E_PIN -1  //--> required for 1/32 scan panels, like 64x64px. Any available pin would do, i.e. IO32.

#define LAT_PIN 26
#define OE_PIN 15
#define CLK_PIN 2
//----------------------------------------

//----------------------------------------Defines the P5 Panel configuration.
#define PANEL_RES_X 64  //--> Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32  //--> Number of pixels tall of each INDIVIDUAL panel module.
#define NUM_ROWS 1
#define NUM_COLS 2
#define PANEL_CHAIN NUM_COLS*NUM_ROWS  //--> Total number of panels chained one to another
//----------------------------------------

// Initialize dma_displayPanel_I2S_DMA as "dma_display".
MatrixPanel_I2S_DMA *dma_display = nullptr;

//----------------------------------------Variable for color.
// color565(0, 0, 0); --> RGB color code. Use the "color picker" to use or find another color code.
uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 51, 51);
uint16_t myGREEN = dma_display->color565(115, 230, 0);
uint16_t myBLUE = dma_display->color565(51, 51, 255);
uint16_t myORANGE = dma_display->color565(255, 153, 51);
uint16_t myYELLOW = dma_display->color565(255, 209, 26);
uint16_t myGRAY = dma_display->color565(179, 179, 179);

uint16_t colortest;
//----------------------------------------

int power=0;
int conso=0;
int ErrCom=0;

String couleurJourJ;
String couleurJourJ1;

 //https://arduinojson.org/v6/assistant/
DynamicJsonDocument coindoc(1024);
DynamicJsonDocument weatherdoc(4096);
DynamicJsonDocument tempodoc(1024);

int count = 20;


uint8_t frameDelay = 10;  // default frame delay value
static time_t last_t;
uint8_t r = 0, g = 0, b = 0;
int h, m, s, d, yr;
uint8_t month, dow;
String text;
unsigned int NewRTCh = 24;
unsigned int NewRTCm = 60;
unsigned int NewRTCs = 10;
time_t t;
const char* wd[7] = {"Sunday", "Monday", "Tuesday", "Wednesday","Thursday", "Friday", "Saturday"};
const char* months []= {"Jan", "Feb", "Mar", "Apr", "May", "Jun","Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
String str_h;String str_m;String str_s;

//---------------------------------------- codes crypto
String code1 = "BTC";
String code2 = "ETH";
String code3 = "BNB";
String code4 = "POLS";
String code5 = "ALPH";


//---------------------------------------- serveur NTP
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 1;
const int   daylightOffset_sec = 3600 * 1;

const char wifi_image1bit[] PROGMEM   =  {
 0x00,0x00,0x00,0xf8,0x1f,0x00,0x00,0x00,0x00,0x00,0x80,0xff,0xff,0x01,0x00,
 0x00,0x00,0x00,0xf0,0xff,0xff,0x07,0x00,0x00,0x00,0x00,0xfc,0xff,0xff,0x1f,
 0x00,0x00,0x00,0x00,0xfe,0x07,0xe0,0x7f,0x00,0x00,0x00,0x80,0xff,0x00,0x00,
 0xff,0x01,0x00,0x00,0xc0,0x1f,0x00,0x00,0xf8,0x03,0x00,0x00,0xe0,0x0f,0x00,
 0x00,0xf0,0x07,0x00,0x00,0xf0,0x03,0xf0,0x0f,0xc0,0x0f,0x00,0x00,0xe0,0x01,
 0xff,0xff,0x80,0x07,0x00,0x00,0xc0,0xc0,0xff,0xff,0x03,0x03,0x00,0x00,0x00,
 0xe0,0xff,0xff,0x07,0x00,0x00,0x00,0x00,0xf8,0x0f,0xf0,0x1f,0x00,0x00,0x00,
 0x00,0xfc,0x01,0x80,0x3f,0x00,0x00,0x00,0x00,0x7c,0x00,0x00,0x3e,0x00,0x00,
 0x00,0x00,0x38,0x00,0x00,0x1c,0x00,0x00,0x00,0x00,0x10,0xe0,0x07,0x08,0x00,
 0x00,0x00,0x00,0x00,0xfc,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0x7f,0x00,
 0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x1f,0xf8,
 0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,
 0x00,0xc0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x07,0x00,0x00,0x00,0x00,
 0x00,0x00,0xe0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0x03,0x00,0x00,0x00,
 0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00  };
void drawXbm565(int x, int y, int width, int height, const char *xbm, uint16_t color = 0xffff) 
{
  if (width % 8 != 0) {
      width =  ((width / 8) + 1) * 8;
  }
    for (int i = 0; i < width * height / 8; i++ ) {
      unsigned char charColumn = pgm_read_byte(xbm + i);
      for (int j = 0; j < 8; j++) {
        int targetX = (i * 8 + j) % width + x;
        int targetY = (8 * i / (width)) + y;
        if (bitRead(charColumn, j)) {
          dma_display->drawPixel(targetX, targetY, color);
        }
      }
    }
}


//________________________________________________________________________________VOID SETUP()
void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  delay(1000);

  // Initialize the connected PIN between Panel P5 and ESP32.
  HUB75_I2S_CFG::i2s_pins _pins={R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};
  delay(10);

  //----------------------------------------Module configuration.
  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   //--> module width.
    PANEL_RES_Y,   //--> module height.
    PANEL_CHAIN,   //--> Chain length.
    _pins          //--> pin mapping.
  );
  delay(10);
  //----------------------------------------

  // Set I2S clock speed.
  mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_10M;  // I2S clock speed, better leave as-is unless you want to experiment.
  delay(10);

  //----------------------------------------Display Setup.
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(15); //--> 0-255.
  dma_display->clearScreen();

  Connecting_To_The_Network();
  delay(100);

  // On configure le seveur NTP
  configTzTime("CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", ntpServer);
  //configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

 /************** DISPLAY **************/
 //https://github.com/mrcodetastic/ESP32-HUB75-MatrixPanel-DMA/blob/master/examples/BitmapIcons/BitmapIcons.ino
  Serial.println("...Starting Display");
  
  //dma_display->fillScreen(dma_display->color444(0, 1, 0));  

  // Fade a Red Wifi Logo In
  //for (int r=0; r < 255; r++ )
 // {   drawXbm565(0,0,32,32, wifi_image1bit, myWHITE);dma_display->color565(r,0,0));  delay(10);  }

  //delay(2000);
  //dma_display->clearScreen();
}

void loop()
 {
  if (count==20)
  {
    count=0;
    //updateTempo();
    updateWeather();
    if (ErrCom>=10)
    {  Connecting_To_The_Network(); ErrCom=0;
}

  }

  updateCoin();
  printCoin();

  configTzTime("CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", ntpServer); //https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
  updateTime();
  printTime("Paris");

  configTzTime("EST5EDT,M3.2.0,M11.1.0", ntpServer);
  updateTime();
  printTime("New York");

  configTzTime("JST-9", ntpServer);
  updateTime();
  printTime("Tokyo");

  //printTempo();

  updateSolar();
  printSolar();

  printWeather();

  count+=1;
}

void Connecting_To_The_Network() {

  //scroll_text(22, frameDelay/2, "Connecting...",1,myORANGE);
  dma_display->clearScreen();
  dma_display->setTextWrap(false);
  dma_display->setTextSize(1);
  dma_display->setCursor(1,12);
  dma_display->setTextColor(myORANGE);
  dma_display->print("Connecting");
  drawXbm565(64,0,64,32, wifi_image1bit, myORANGE);

  Serial.println();
  Serial.print("Connecting to : ");
  WiFi.mode(WIFI_STA);
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");

  }
  Serial.println("WiFi connected.");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  //scroll_text(22, frameDelay/2, "Connected !!",1,myGREEN);
  dma_display->clearScreen();
  dma_display->setTextSize(1);
  dma_display->setCursor(1,7);
  dma_display->setTextColor(myGREEN);
  dma_display->print("Connected");
  dma_display->setCursor(1,16);
  dma_display->print(WiFi.localIP());
  drawXbm565(64,0,64,32, wifi_image1bit, myGREEN);


}


void scroll_text(uint16_t ypos, unsigned long scroll_delay, String text, int size, uint16_t color )
{
  int text_length = text.length();
  // Asuming size *5 pixel average character width
   int firstxpos=(128 - text_length*size*5.5)/2 ;
  for (int xpos = firstxpos; xpos > -(128 + text_length * size * 3.5); xpos--)
  {  
    dma_display->setTextSize(size);
    dma_display->setCursor(xpos, ypos);
    dma_display->fillRect(0, ypos, 128, 32, myBLACK);
    //dma_display->setFont(&kongtext4pt7b);
    dma_display->setTextColor(color);
  for (int x = 0; x < text_length; x++) { dma_display->print(text[x]); }
  if (xpos==firstxpos){delay(1500);}
  else {delay(scroll_delay);}
  }
}

void updateCoin()
{
HTTPClient http;

    http.begin(coinurl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("x-api-key", coinapiKey); 
    // Créer le corps de la requête
    String coinjson = "{\"currency\": \"USD\",\"codes\": [\"" + code1 + "\", \"" + code2 + "\", \"" + code3 + "\", \"" + code4 + "\", \"" + code5 + "\"],\"sort\": \"rank\",\"order\": \"ascending\",\"offset\": 0,\"limit\": 0,\"meta\": false}";
    // Envoyer la requête POST
    int httpCode = http.POST(coinjson);
    // Vérifier la réponse
    if (httpCode > 0)     {
      String payload = http.getString();
      Serial.println("HTTP POST Response Code: " + String(httpCode));
      Serial.println("HTTP POST Response: " + payload);
      // Analyser la réponse JSON
      
      DeserializationError error = deserializeJson(coindoc, payload);
      if (error) 
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
    }
      else 
    { Serial.println("COIN - Error on sending POST: " + String(httpCode)); 
    ErrCom++;Serial.print("ErrCom = ");Serial.println(ErrCom);}
    http.end();
}

void printCoin()
{
  for (JsonObject coin : coindoc.as<JsonArray>()) 
      {
        Serial.print("Code: ");
        Serial.println(coin["code"].as<String>()); 
        Serial.print("Rate: ");
        Serial.println(coin["rate"].as<float>()); 
        Serial.print("Day delta : ");
        Serial.println(coin["delta"]["day"].as<double>()); 

        //  Serial.print("Volume: ");
        //  Serial.println(coin["volume"].as<String>()); // Convertir en String

        if  (coin["delta"]["day"].as<float>()>=1){colortest=myGREEN;}
        else {colortest=myRED;} 
        float a =(coin["delta"]["day"].as<float>()-1)*coin["rate"].as<float>();
        scrollCoin(frameDelay, coin["code"].as<String>()+":"+coin["rate"].as<float>()+"$", "24h : "+ String(a) + "$",colortest );
      }
}

void scrollCoin(unsigned long scroll_delay, String text1, String text2,uint16_t color )
{
  uint16_t text_length = text1.length();
  // Asuming size *5 pixel average character width
  int firstxpos=0;
  for (int xpos = firstxpos; xpos > -(10 + text_length * 12); xpos--)
  {  
        dma_display->clearScreen();
        dma_display->setTextSize(2);
        dma_display->setCursor(xpos, 3);
        dma_display->setTextColor(color);
        for (int x = 0; x < text_length; x++) { dma_display->print(text1[x]);}

        dma_display->setTextSize(1);
        dma_display->setCursor(xpos, 21);
        for (int x = 0; x < text2.length(); x++) { dma_display->print(text2[x]);}
        if (xpos==firstxpos){delay(3000);}
        else {delay(scroll_delay);}
  }
}

void updateTime()
{
  t = time(NULL);
  struct tm *tm;
  tm = localtime(&t);
  h = tm->tm_hour;  m = tm->tm_min;  s = tm->tm_sec;
  if (d != tm->tm_mday || month != tm->tm_mon || yr != tm->tm_year + 1900) 
    {  d = tm->tm_mday;  dow = tm->tm_wday;  month = tm->tm_mon;  yr = tm->tm_year + 1900;   }
  yield();
}

void printTime(String text)
{
    if (NewRTCs != s / 10)
      { NewRTCs = s / 10;  } 
    if (NewRTCm != m)
      { NewRTCm = m;}
  if (NewRTCh != h)
      {  NewRTCh = h;}
  dma_display->clearScreen();
  dma_display->setTextSize(1);
  dma_display->setTextColor(myWHITE);
  dma_display->setCursor(1, 1);
  dma_display->print(text);
  if (h < 10) {str_h = "0" + String(h);} 
  else {str_h = String(h); }
  if (m < 10) {str_m = "0" + String(m);} 
  else {str_m = String(m); }
  if (s < 10) {   str_s = "0" + String(s);
    } else {str_s = String(s);}
  scroll_text(12, frameDelay, str_h+":"+str_m, 2, myWHITE); //+":"+str_s
  dma_display->clearScreen();
  return;
}

void updateTempo()
{ 
  // recup du jour de l'année format AAAA/MM/JJ
  configTzTime("CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", ntpServer);
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {Serial.println("Erreur de recup de l'heure");  }
  char strftime_buf[20];
  strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d", &timeinfo);
  //Serial.println(strftime_buf); // debug date du jour
  // Construire l'URL avec la date formatée
  String tempourl = "https://particulier.edf.fr/services/rest/referentiel/searchTempoStore?dateRelevant=" + String(strftime_buf);
  //Serial.println(tempourl); // debug url
  // Effectuer la requête HTTP
  HTTPClient http;
  http.begin(tempourl);
  int httpCodeTempo = http.GET();
  if (httpCodeTempo > 0) {
    if (httpCodeTempo == HTTP_CODE_OK) {
      String payload = http.getString();
      // Parse JSON avec ArduinoJSON
      //const size_t capacity = JSON_OBJECT_SIZE(2) + 100;
      //DynamicJsonDocument doc(capacity);
      deserializeJson(tempodoc, payload);
      // Extraire les valeurs
      couleurJourJ = tempodoc["couleurJourJ"].as<String>();
      couleurJourJ1 = tempodoc["couleurJourJ1"].as<String>();
      // DEBUG
      Serial.print("CouleurJourJ: ");
      Serial.println(couleurJourJ);
      Serial.print("CouleurJourJ1: ");
      Serial.println(couleurJourJ1);
    } // fin if (httpCode == HTTP_CODE_OK) {
  } else {
    Serial.println("TEMPO - Erreur de connexion au serveur EDF");
    ErrCom++;Serial.print("ErrCom = ");Serial.println(ErrCom);
  } //if (httpCode > 0) {
  http.end();

}

void printTempo()
{
  dma_display->clearScreen();
  dma_display->setTextSize(1);
  dma_display->setTextColor(myWHITE);
  dma_display->setCursor(1, 1);
  dma_display->print("EDF Tempo");
  
      Serial.print("CouleurJourJ: ");
      if (couleurJourJ=="TEMPO_BLEU") {
        Serial.println("bleu");
        scroll_text(12, frameDelay, "HOY", 2, myBLUE);
      }
      if (couleurJourJ=="TEMPO_BLANC") {
        Serial.println("blanc");
        scroll_text(12, frameDelay, "HOY", 2, myWHITE);
      }
      if (couleurJourJ=="TEMPO_ROUGE") {
        Serial.println("rouge");
        scroll_text(12, frameDelay, "HOY", 2, myRED);
      }
 
      // Affichage des LEDs du lendemain
      Serial.print("CouleurJourJ1: ");
      if (couleurJourJ1=="TEMPO_BLEU") {
        Serial.println("bleu");
        scroll_text(12, frameDelay, "MANANA", 2, myBLUE);
      }
      if (couleurJourJ1=="TEMPO_BLANC") {
        Serial.println("blanc");
        scroll_text(12, frameDelay, "MANANA", 2, myWHITE);
      }
      if (couleurJourJ1=="TEMPO_ROUGE") {
        Serial.println("rouge");
        scroll_text(12, frameDelay, "MANANA", 2, myRED);
      }
      dma_display->clearScreen();
      return;
}

void updateWeather()
{
  Serial.println("updateWeather");
  HTTPClient http;
 http.begin(weatherurl);
  int httpCodeweather = http.GET();
  if (httpCodeweather > 0) 
    if (httpCodeweather == HTTP_CODE_OK) {
      String weatherpayload = http.getString();
      Serial.println(weatherpayload);
      // Parse JSON avec ArduinoJSON
      //const size_t weathercapacity = JSON_OBJECT_SIZE(2) + 100;
      deserializeJson(weatherdoc, weatherpayload);
      DeserializationError weathererror = deserializeJson(weatherdoc, weatherpayload);
      if (weathererror) 
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(weathererror.c_str());
        return;
      }
    } 
   
  else {    
    Serial.println("Erreur de connexion au serveur Weather");
    ErrCom++; Serial.print("ErrCom = "); Serial.println(ErrCom);  } 

  http.end();
  return ;
}

void printWeather()
{
    Serial.println("printWeather");

  for (JsonObject list_item  : weatherdoc["list"].as<JsonArray>()) 
      {
        JsonObject list_item_main = list_item["main"];
        int list_item_main_temp = list_item_main["temp"]; // 12.82, 10.4, 8.82, 10.93
        String list_item_main_humidity = list_item_main["humidity"]; // 91, 90, 86, 77

        JsonObject list_item_weather_0 = list_item ["weather"][0];
        String list_item_weather_0_main = list_item_weather_0["main"];
        String list_item_weather_0_description = list_item_weather_0["description"]; // "overcast clouds", ...
        String list_item_weather_0_icon = list_item_weather_0["icon"];

        JsonObject list_item_wind = list_item["wind"];
        float list_item_wind_speed = list_item_wind["speed"]; // 4.01, 3.33, 1.4, 4.3
        list_item_wind_speed = 1.852*list_item_wind_speed;

        int list_item_wind_deg = list_item_wind["deg"]; // 151, 170, 224, 315
        String list_item_wind_gust = list_item_wind["gust"]; // 6.19, 5.34, 2.69, 6.92

        String list_item_dt_txt = list_item["dt_txt"]; // "2024-05-02 00:00:00", "2024-05-02 03:00:00", ...

      dma_display->clearScreen();
      dma_display->setTextSize(1);
      dma_display->setTextColor(myWHITE);
      dma_display->setCursor(1, 1);
      dma_display->print(list_item_dt_txt.substring(0,13)+"h");
      //dma_display->setCursor(5, 8);
      //dma_display->print(list_item_weather_0_description);
      dma_display->setCursor(3, 11);
      dma_display->print(String(list_item_wind_speed,0)+" km/h");
      dma_display->setCursor(8, 21);
      dma_display->print(String(list_item_main_temp)+" deg" );
      
      if (list_item_wind_speed>50){drawXbm565(90,0,32,32, wind_bits, myBLUE);}
      else 
      {
        if (list_item_weather_0_icon.substring(0,2)=="01"){drawXbm565(90,0,32,32, sun_bits, myYELLOW);} //myString.substring(from inclus, to exclu) indice commence a 0
        if (list_item_weather_0_icon.substring(0,2)=="02"){drawXbm565(90,3,32,32, cloud_sun_bits, myORANGE);}
        if (list_item_weather_0_icon.substring(0,2)=="03"){drawXbm565(90,-4,32,32, cloud_bits, myGRAY);}
        if (list_item_weather_0_icon.substring(0,2)=="04"){drawXbm565(90,0,32,32, clouds_bits, myGRAY);}
        if (list_item_weather_0_icon.substring(0,2)=="09"){drawXbm565(90,-4,32,32, rain2_bits, myBLUE);}
        if (list_item_weather_0_icon.substring(0,2)=="10"){drawXbm565(90,-4,32,32, rain1_bits, myBLUE);}
        if (list_item_weather_0_icon.substring(0,2)=="11"){drawXbm565(90,-4,32,32, rain_lightning_bits, myBLUE);}
        if (list_item_weather_0_icon.substring(0,2)=="13"){drawXbm565(90,0,32,32, snow_bits, myBLUE);}
        if (list_item_weather_0_icon.substring(0,2)=="50"){drawXbm565(90,0,32,32, clouds_bits, myGRAY);}
      }
        delay(2000);
      }
}

void updateSolar()
{
HTTPClient http;
 http.begin(solarurl);
  int httpCodeSolar = http.GET();
  if (httpCodeSolar > 0) {
    if (httpCodeSolar == HTTP_CODE_OK) {
      String Solarpayload = http.getString();
      // Parse JSON avec ArduinoJSON
      const size_t Solarcapacity = JSON_OBJECT_SIZE(2) + 100;
      DynamicJsonDocument Solardoc(Solarcapacity);
      deserializeJson(Solardoc, Solarpayload);
      // Extraire les valeurs
      power = Solardoc["result"]["acpower"];
      conso = Solardoc["result"]["feedinpower"];
      // DEBUG
      Serial.print("power: ");
      Serial.println(power);
      Serial.print("conso: ");
      Serial.println(conso);

    } // fin if (httpCode == HTTP_CODE_OK) {
  } else {
    Serial.println("Erreur de connexion au serveur Solax");
    ErrCom++;Serial.print("ErrCom = ");Serial.println(ErrCom);
  } //if (httpCode > 0) {
  http.end();
  return ;
}

void printSolar()
{
      dma_display->clearScreen();
      dma_display->setTextSize(1);
      dma_display->setTextColor(myWHITE);
      dma_display->setCursor(1, 1);
      dma_display->print("Production solaire");
      scroll_text(12, frameDelay,String(power)+" W", 2, myGREEN);

      dma_display->clearScreen();
      dma_display->setTextSize(1);
      dma_display->setTextColor(myWHITE);
      dma_display->setCursor(1, 1);
      dma_display->print("Conso maison");
      scroll_text(12, frameDelay,String(conso)+" W", 2, myRED);
}


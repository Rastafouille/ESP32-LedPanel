//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 01_Showing_Text
//----------------------------------------Including the libraries.
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFi.h>
#include "time.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Dhole_weather_icons32px.h"
#include "credentials.h"
#include <WiFiManager.h>
#include <ArduinoOTA.h>




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

uint16_t myBLACK, myWHITE, myRED, myGREEN, myBLUE, myORANGE, myYELLOW, myGRAY;

uint16_t colortest;
//----------------------------------------

int power=0;
int conso=0;
bool solarAvailable=false;
bool houseAvailable=false;
bool waterHeaterAvailable=false;
bool gridAvailable=false;
bool batteryAvailable=false;
bool batteryAcAvailable=false;
int waterHeaterPower=0;
int gridNetExport=0;
int batterySoc=0;
int batteryAcCharge=0;
int batteryAcDischarge=0;
String waterHeaterMode="unavailable";
bool otaStarted=false;
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

void serviceOTA() {
  if (WiFi.status() == WL_CONNECTED && !otaStarted) {
    ArduinoOTA.setHostname(otaHostname);
    ArduinoOTA.onStart([]() { Serial.println("Mise à jour OTA démarrée"); });
    ArduinoOTA.onEnd([]() { Serial.println("Mise à jour OTA terminée"); });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Erreur OTA [%u]\n", error);
    });
    ArduinoOTA.begin();
    otaStarted = true;
    Serial.print("OTA prêt : ");
    Serial.println(otaHostname);
  }
  if (otaStarted) ArduinoOTA.handle();
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

  //---------------------------Display Setup.
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(15); //--> 0-255.
  dma_display->clearScreen();

  myBLACK  = dma_display->color565(0, 0, 0);
  myWHITE  = dma_display->color565(255, 255, 255);
  myRED    = dma_display->color565(255, 51, 51);
  myGREEN  = dma_display->color565(115, 230, 0);
  myBLUE   = dma_display->color565(51, 51, 255);
  myORANGE = dma_display->color565(255, 153, 51);
  myYELLOW = dma_display->color565(255, 209, 26);
  myGRAY   = dma_display->color565(179, 179, 179);

  Connecting_To_The_Network();
  delay(100);

  serviceOTA();



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
  serviceOTA();
  if (WiFi.status() != WL_CONNECTED) {
    static uint32_t lastRetry = 0;
    if (millis() - lastRetry > 15000) {
      lastRetry = millis();
      Serial.println("WiFi perdu, tentative de reconnexion...");
      WiFi.reconnect();
    }
  }

  if (count==20)
  {
    count=0;
    updateTempoV2();
    updateWeather();
    updateSolar();

    if (ErrCom>=10)
    {  ESP.restart(); }

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
  
  printTempo();

  printSolar();

  printWeather();

  count+=1;
}

void Connecting_To_The_Network() {
  dma_display->clearScreen();
  dma_display->setTextWrap(false);
  dma_display->setTextSize(1);
  dma_display->setCursor(1, 7);
  dma_display->setTextColor(myORANGE);
  dma_display->print("Connecting...");

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);

  Serial.println("Tentative de connexion a : " + String(ssid));
  WiFi.begin(ssid, password);

  uint32_t start = millis();
  uint8_t dots = 0;
  while (WiFi.status() != WL_CONNECTED && millis() - start < 30000) {
    delay(500);
    Serial.print(".");
    dots = (dots + 1) % 4;
    dma_display->fillRect(1, 18, 120, 10, myBLACK);
    dma_display->setCursor(1, 18);
    dma_display->setTextColor(myWHITE);
    dma_display->print("Please wait");
    for (uint8_t i = 0; i < dots; i++) {
      dma_display->print(".");
    }
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Echec WiFi direct, ouverture portail WiFiManager...");
    WiFiManager wm;
    wm.setConnectTimeout(30);
    wm.setConfigPortalTimeout(180);
    bool res = wm.autoConnect("Solar-ConfigAP");
    if (!res) {
      Serial.println("WiFiManager: aucune connexion et timeout portail.");
      return;
    }
  }

  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  dma_display->clearScreen();
  dma_display->setTextWrap(false);
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
    serviceOTA();
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
        serviceOTA();
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

void updateTempoV2()
{ 
  // recup du jour de l'année format AAAA/MM/JJ
  //configTzTime("CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", ntpServer);
  //struct tm timeinfo;
  //if (!getLocalTime(&timeinfo)) {Serial.println("Erreur de recup de l'heure");  }
  //char strftime_buf[20];
  //strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d", &timeinfo);
  //Serial.println(strftime_buf); // debug date du jour
  // Construire l'URL avec la date formatée
  String urlToDay = "https://www.api-couleur-tempo.fr/api/jourTempo/today";
  //Serial.println(urlToDay); // debug url
  String urlTomorrow = "https://www.api-couleur-tempo.fr/api/jourTempo/tomorrow";
  //Serial.println(urlTomorrow); // debug url
  // Effectuer la requête HTTP

  HTTPClient http;
  http.begin(urlToDay);
  int httpCodeTempo = http.GET();
  if (httpCodeTempo > 0) {
    if (httpCodeTempo == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.print("payloadDe = ");Serial.println(payload);

       // Parse JSON avec ArduinoJSON
        const size_t capacity = JSON_OBJECT_SIZE(3) + 100;
        DynamicJsonDocument doc(capacity);
        DeserializationError error = deserializeJson(doc, payload);
 
        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
          return;
        }
 
        // Extraire les valeurs
        int codeJour = doc["codeJour"];
        Serial.print("codeJour = ");Serial.println(codeJour);
 
        // Afficher la couleur en fonction du code
        //Code couleur du tarif Tempo applicable: 
        // - 0: tarif inconnu (pas encore communiqué par RTE) 
        // - 1: tarif bleu 
        // - 2: tarif blanc 
        // - 3: tarif rouge
        switch(codeJour) {
          case 0:
            couleurJourJ = "INCONNU";
            break;
          case 1:
            couleurJourJ = "BLEU";
            break;
          case 2:
            couleurJourJ = "BLANC";
            break;
          case 3:
            couleurJourJ = "ROUGE";
            break;
          default:
            couleurJourJ = "INCONNU";
        }
    } // fin if (httpCode == HTTP_CODE_OK) {
  } else {
    Serial.println("TEMPO - Erreur de connexion au serveur EDF");
    ErrCom++;Serial.print("ErrCom = ");Serial.println(ErrCom);
  } //if (httpCode > 0) {
  http.end();


  http.begin(urlTomorrow);
  int httpCodeDemain = http.GET();
    if (httpCodeDemain > 0) {
      if (httpCodeDemain == HTTP_CODE_OK) {
        String payload = http.getString();
                Serial.print("payloadDemain = ");Serial.println(payload);

 
        // Parse JSON avec ArduinoJSON
        const size_t capacity = JSON_OBJECT_SIZE(3) + 100;
        DynamicJsonDocument doc(capacity);
        DeserializationError error = deserializeJson(doc, payload);
 
        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
          return;
        }
 
        // Extraire les valeurs
        int codeJour = doc["codeJour"];
        Serial.print("codeDemain = ");Serial.println(codeJour);

        switch(codeJour) {
          case 0:
            couleurJourJ1 = "INCONNU";
            break;
          case 1:
            couleurJourJ1 = "BLEU";
            break;
          case 2:
            couleurJourJ1 = "BLANC";
            break;
          case 3:
            couleurJourJ1 = "ROUGE";
            break;
          default:
            couleurJourJ1 = "INCONNU";
        }
      }
    } else {
      Serial.println("TEMPO - Erreur de connexion au serveur EDF");
      ErrCom++;Serial.print("ErrCom = ");Serial.println(ErrCom);   
     }
    http.end(); // Libérer les ressources

      // DEBUG
      Serial.print("CouleurJourJ: ");
      Serial.println(couleurJourJ);
      Serial.print("CouleurJourJ1: ");
      Serial.println(couleurJourJ1);
}

void printTempo()
{
  dma_display->clearScreen();
  dma_display->setTextSize(1);
  dma_display->setTextColor(myWHITE);
  dma_display->setCursor(1, 1);
  dma_display->print("EDF Tempo");
  
      Serial.print("CouleurJourJ: ");
      if (couleurJourJ=="BLEU") {
        Serial.println("bleu");
        scroll_text(12, frameDelay, "HOY", 2, myBLUE);
      }
      if (couleurJourJ=="BLANC") {
        Serial.println("blanc");
        scroll_text(12, frameDelay, "HOY", 2, myWHITE);
      }
      if (couleurJourJ=="ROUGE") {
        Serial.println("rouge");
        scroll_text(12, frameDelay, "HOY", 2, myRED);
      }
 
      // Affichage des LEDs du lendemain
      Serial.print("CouleurJourJ1: ");
      if (couleurJourJ1=="BLEU") {
        Serial.println("bleu");
        scroll_text(12, frameDelay, "MANANA", 2, myBLUE);
      }
      if (couleurJourJ1=="BLANC") {
        Serial.println("blanc");
        scroll_text(12, frameDelay, "MANANA", 2, myWHITE);
      }
      if (couleurJourJ1=="ROUGE") {
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

  //-------------------- JOUR 0 = AUJOURD'HUI --------------------
  JsonObject day0 = weatherdoc["daily"][0];
  float tempMin0 = day0["temp"]["min"].as<float>();
  float tempMax0 = day0["temp"]["max"].as<float>();
  float windDay0 = day0["wind_speed"].as<float>() * 1.852;
  const char* iconDay0 = day0["weather"][0]["icon"];

  dma_display->clearScreen();
  dma_display->setTextSize(1);
  dma_display->setTextColor(myWHITE);
  dma_display->setCursor(0, 0);
  dma_display->print("Aujourd'hui");
  dma_display->setCursor(0, 12);
  dma_display->print(String(tempMin0, 0) + "/" + String(tempMax0, 0) + " C");
  dma_display->setCursor(0, 22);
  dma_display->print(String(windDay0, 0) + " km/h");

  String s0(iconDay0);
  if (s0.startsWith("01")) drawXbm565(90,0,32,32, sun_bits, myYELLOW);
  if (s0.startsWith("02")) drawXbm565(90,3,32,32, cloud_sun_bits, myORANGE);
  if (s0.startsWith("03")) drawXbm565(90,-4,32,32, cloud_bits, myGRAY);
  if (s0.startsWith("04")) drawXbm565(90,0,32,32, clouds_bits, myGRAY);
  if (s0.startsWith("09")) drawXbm565(90,-4,32,32, rain2_bits, myBLUE);
  if (s0.startsWith("10")) drawXbm565(90,-4,32,32, rain1_bits, myBLUE);
  if (s0.startsWith("11")) drawXbm565(90,-4,32,32, rain_lightning_bits, myBLUE);
  if (s0.startsWith("13")) drawXbm565(90,0,32,32, snow_bits, myBLUE);
  if (s0.startsWith("50")) drawXbm565(90,0,32,32, clouds_bits, myGRAY);

delay(3000);


  //-------------------- JOUR 1 = DEMAIN --------------------
  JsonObject day1 = weatherdoc["daily"][1];
  float tempMin1 = day1["temp"]["min"].as<float>();
  float tempMax1 = day1["temp"]["max"].as<float>();
  float windDay1 = day1["wind_speed"].as<float>() * 1.852;
  const char* iconDay1 = day1["weather"][0]["icon"];

  dma_display->clearScreen();
  dma_display->setCursor(0, 0);
  dma_display->print("Demain");
  dma_display->setCursor(0, 12);
  dma_display->print(String(tempMin1, 0) + "/" + String(tempMax1, 0) + " C");
  dma_display->setCursor(0, 22);
  dma_display->print(String(windDay1, 0) + " km/h");

  String s1(iconDay1);
  if (s1.startsWith("01")) drawXbm565(90,0,32,32, sun_bits, myYELLOW);
  if (s1.startsWith("02")) drawXbm565(90,3,32,32, cloud_sun_bits, myORANGE);
  if (s1.startsWith("03")) drawXbm565(90,-4,32,32, cloud_bits, myGRAY);
  if (s1.startsWith("04")) drawXbm565(90,0,32,32, clouds_bits, myGRAY);
  if (s1.startsWith("09")) drawXbm565(90,-4,32,32, rain2_bits, myBLUE);
  if (s1.startsWith("10")) drawXbm565(90,-4,32,32, rain1_bits, myBLUE);
  if (s1.startsWith("11")) drawXbm565(90,-4,32,32, rain_lightning_bits, myBLUE);
  if (s1.startsWith("13")) drawXbm565(90,0,32,32, snow_bits, myBLUE);
  if (s1.startsWith("50")) drawXbm565(90,0,32,32, clouds_bits, myGRAY);

delay(3000);


  //-------------------- JOUR 2 = APRES-DEMAIN --------------------
  JsonObject day2 = weatherdoc["daily"][2];
  float tempMin2 = day2["temp"]["min"].as<float>();
  float tempMax2 = day2["temp"]["max"].as<float>();
  float windDay2 = day2["wind_speed"].as<float>() * 1.852;
  const char* iconDay2 = day2["weather"][0]["icon"];

  dma_display->clearScreen();
  dma_display->setCursor(0, 0);
  dma_display->print("Apres-demain");
  dma_display->setCursor(0, 12);
  dma_display->print(String(tempMin2, 0) + "/" + String(tempMax2, 0) + " C");
  dma_display->setCursor(0, 22);
  dma_display->print(String(windDay2, 0) + " km/h");

  String s2(iconDay2);
  if (s2.startsWith("01")) drawXbm565(90,0,32,32, sun_bits, myYELLOW);
  if (s2.startsWith("02")) drawXbm565(90,3,32,32, cloud_sun_bits, myORANGE);
  if (s2.startsWith("03")) drawXbm565(90,-4,32,32, cloud_bits, myGRAY);
  if (s2.startsWith("04")) drawXbm565(90,0,32,32, clouds_bits, myGRAY);
  if (s2.startsWith("09")) drawXbm565(90,-4,32,32, rain2_bits, myBLUE);
  if (s2.startsWith("10")) drawXbm565(90,-4,32,32, rain1_bits, myBLUE);
  if (s2.startsWith("11")) drawXbm565(90,-4,32,32, rain_lightning_bits, myBLUE);
  if (s2.startsWith("13")) drawXbm565(90,0,32,32, snow_bits, myBLUE);
  if (s2.startsWith("50")) drawXbm565(90,0,32,32, clouds_bits, myGRAY);

delay(3000);}


void updateSolar() {
  HTTPClient http;
  http.begin(homeStatusUrl);
  const int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    const String payload = http.getString();
    StaticJsonDocument<4096> doc;
    const DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print("HOMESTATUS - Erreur parsing JSON: ");
      Serial.println(error.c_str());
      ErrCom++;
    } else {
      // /api/homestatus fournit explicitement disponibilité et mesures.
      JsonObjectConst solar = doc["solar"].as<JsonObjectConst>();
      JsonObjectConst house = doc["house"].as<JsonObjectConst>();
      JsonObjectConst waterHeater = doc["waterHeater"].as<JsonObjectConst>();
      JsonObjectConst grid = doc["grid"].as<JsonObjectConst>();
      JsonObjectConst battery = doc["battery"].as<JsonObjectConst>();
      solarAvailable = solar["available"] | false;
      houseAvailable = house["available"] | false;
      waterHeaterAvailable = waterHeater["available"] | false;
      gridAvailable = grid["available"] | false;
      batteryAvailable = battery["available"] | false;
      batteryAcAvailable = batteryAvailable &&
        !battery["acChargePowerW"].isNull() && !battery["acDischargePowerW"].isNull();
      if (solarAvailable && !solar["powerW"].isNull()) {
        power = solar["powerW"].as<int>();
      } else solarAvailable = false;
      if (houseAvailable && !house["powerIncludingWaterHeaterW"].isNull()) {
        conso = house["powerIncludingWaterHeaterW"].as<int>();
      } else {
        houseAvailable = false;
      }
      if (waterHeaterAvailable && !waterHeater["powerW"].isNull()) {
        waterHeaterPower = waterHeater["powerW"].as<int>();
      } else waterHeaterAvailable = false;
      waterHeaterMode = waterHeater["mode"] | "unavailable";
      if (gridAvailable && !grid["netExportW"].isNull()) {
        gridNetExport = grid["netExportW"].as<int>();
      } else gridAvailable = false;
      if (batteryAvailable && !battery["socPercent"].isNull()) {
        batterySoc = battery["socPercent"].as<int>();
      } else batteryAvailable = false;
      if (batteryAcAvailable) {
        batteryAcCharge = battery["acChargePowerW"].as<int>();
        batteryAcDischarge = battery["acDischargePowerW"].as<int>();
      }
      Serial.println("Données reçues depuis /api/homestatus");
      Serial.print("solar.powerW: ");
      if (solarAvailable) Serial.println(power); else Serial.println("indisponible");
      Serial.print("house.powerIncludingWaterHeaterW: ");
      if (houseAvailable) Serial.println(conso); else Serial.println("indisponible");
    }
  } else {
    Serial.printf("HOMESTATUS - GET en échec (HTTP %d)\n", httpCode);
    ErrCom++;
  }
  http.end();
}
void printSolar()
{
  // Pages courtes et colorées pour rendre l'instantané HomeSolarPilot lisible.
  dma_display->clearScreen();
  dma_display->setTextSize(1);
  dma_display->setTextColor(myWHITE);
  dma_display->setCursor(1, 1);
  dma_display->print("PRODUCTION PV");
  if (solarAvailable) scroll_text(12, frameDelay, String(power) + " W", 2, myGREEN);
  else scroll_text(12, frameDelay, "Indisponible", 1, myORANGE);

  dma_display->clearScreen();
  dma_display->setTextSize(1);
  dma_display->setTextColor(myWHITE);
  dma_display->setCursor(1, 1);
  dma_display->print("MAISON + CE");
  if (houseAvailable) scroll_text(12, frameDelay, String(conso) + " W", 2, myRED);
  else scroll_text(12, frameDelay, "Indisponible", 1, myORANGE);

  dma_display->clearScreen();
  dma_display->setTextSize(1);
  dma_display->setTextColor(myWHITE);
  dma_display->setCursor(1, 1);
  dma_display->print("CHAUFFE-EAU ");
  dma_display->setTextColor(waterHeaterMode == "stopped" ? myGRAY : myORANGE);
  dma_display->print(waterHeaterMode);
  if (waterHeaterAvailable) scroll_text(12, frameDelay, String(waterHeaterPower) + " W", 2, myORANGE);
  else scroll_text(12, frameDelay, "Indisponible", 1, myORANGE);

  dma_display->clearScreen();
  dma_display->setTextSize(1);
  dma_display->setTextColor(myWHITE);
  dma_display->setCursor(1, 1);
  dma_display->print("RESEAU");
  if (gridAvailable) {
    const bool exporting = gridNetExport >= 0;
    scroll_text(12, frameDelay,
      String(exporting ? "Injection " : "Achat ") + String(abs(gridNetExport)) + " W",
      1, exporting ? myGREEN : myRED);
  } else scroll_text(12, frameDelay, "Indisponible", 1, myORANGE);

  dma_display->clearScreen();
  dma_display->setTextSize(1);
  dma_display->setTextColor(myWHITE);
  dma_display->setCursor(1, 1);
  dma_display->print("BATTERIE");
  if (batteryAvailable) {
    String batteryText = String(batterySoc) + "%";
    if (batteryAcAvailable) {
      batteryText += "  +" + String(batteryAcDischarge) + " -" + String(batteryAcCharge) + "W";
    }
    scroll_text(12, frameDelay, batteryText, 1, myBLUE);
  } else scroll_text(12, frameDelay, "Indisponible", 1, myORANGE);
}

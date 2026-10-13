// Provide all the crdentials here

const char* ssid = "YOUR_SSID";  //--> Your wifi name
const char* password = "YOUR_PASSWORD"; //--> Your wifi password

const char* coinurl = "https://api.livecoinwatch.com/coins/map";
const char* coinapiKey = "YOUR_livecoinwath_API";

const char* weatherurl = "https://api.openweathermap.org/data/3.0/onecall?lat=43.956&lon=4.779&exclude=hourly&appid=YOUR_API&units=metric";//&lang=fr";

// Endpoint du projet qui centralise les données de la maison.
// Exemple : http://192.168.1.168:8787/api/homestatus
const char* homeStatusUrl = "http://192.168.1.168:8787/api/homestatus";

// Nom de l'ESP32 affiché dans le réseau lors du téléversement OTA.
const char* otaHostname = "esp32-led-panel";

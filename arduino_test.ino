#include "WiFiClient.h"
#include "ESP8266WebServer.h"
#include "ESP8266mDNS.h"
#include "DHT.h"

#define DHTPIN 2        // Der Pin vom Wetter-Sensor
#define DHTTYPE DHT22   // Welcher Wettersonsor ? -> DHT 22  (AM2302), AM2321

const char* ssid = "";
const char* password = "";
const int led = 13;




DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);




void handleRoot() {
  // Luftfeuchtigkeit lesen
  float h = dht.readHumidity();
  // Temperatur auslesen
  float t = dht.readTemperature();

 
  String INDEX_HTML = "";
  INDEX_HTML += "<!DOCTYPE html>"; 
  INDEX_HTML +=  "<html>"; 
  INDEX_HTML +=  "<head>"; 
  INDEX_HTML +=    "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />"; 
  INDEX_HTML +=    "<meta charset=\"UTF-8\">"; 
  INDEX_HTML +=    "<title>Hauswächter</title>"; 
  INDEX_HTML +=    "<script id=\"sap-ui-bootstrap\" src=\"https://openui5.hana.ondemand.com/resources/sap-ui-core.js\" data-sap-ui-theme=\"sap_bluecrystal\" data-sap-ui-libs=\"sap.m\" data-sap-ui-preload=\"async\">"; 
  INDEX_HTML +=    "</script>"; 
  INDEX_HTML +=    "<script type=\"text/javascript\">"; 
  INDEX_HTML +=      "sap.ui.getCore().attachInit(function() {"; 
      // create a mobile app and display page initially
  INDEX_HTML +=        "var app = new sap.m.App(\"myApp\", {"; 
  INDEX_HTML +=          "initialPage: \"page1\""; 
  INDEX_HTML +=        "});"; 
      // Add one or more tiles
  INDEX_HTML +=        "var tiles = new Array();"; 
  INDEX_HTML +=        "var oTile = new sap.m.StandardTile(\"worst_case\", {"; 
  INDEX_HTML +=          "icon: sap.ui.core.IconPool.getIconURI(\"cloud\"),"; 
  INDEX_HTML +=          "number: \"";
  INDEX_HTML += t;
  INDEX_HTML +=          " °C\",";
  INDEX_HTML +=          "numberUnit: \"Carport\","; 
  INDEX_HTML +=          "title: \"Aussensensor\","; 
  INDEX_HTML +=          "info: \"Luft:";
  INDEX_HTML += h,
  INDEX_HTML +=          " %\",";
  INDEX_HTML +=          "infoState: \"Success\""; 
  INDEX_HTML +=        "});"; 
  INDEX_HTML +=        "tiles.push(oTile);"; 
      // create the page
  INDEX_HTML +=        "var page1 = new sap.m.Page(\"page1\", {"; 
  INDEX_HTML +=          "title: \"Messwerte vom ...\","; 
  INDEX_HTML +=          "showNavButton: false,"; 
  INDEX_HTML +=          "content: ["; 
  INDEX_HTML +=            "new sap.m.TileContainer({"; 
  INDEX_HTML +=              "tiles: tiles"; 
  INDEX_HTML +=            "})"; 
  INDEX_HTML +=          "]"; 
  INDEX_HTML +=        "});"; 
      // add page to the app
  INDEX_HTML +=        "app.addPage(page1);"; 
      // place the app into the HTML document" 
  INDEX_HTML +=        "app.placeAt(\"content\");"; 
  INDEX_HTML +=      "});"; 
  INDEX_HTML +=    "</script>"; 
  INDEX_HTML +=  "</head>"; 
  INDEX_HTML +=  "<body class=\"sapUiBody\" id=\"content\">"; 
  INDEX_HTML +=  "</body>"; 
  INDEX_HTML +=  "</html>"; 
 
 

  
  digitalWrite(led, 1);
  server.send(200, "text/html", INDEX_HTML);
  digitalWrite(led, 0);
}




void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}




void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  dht.begin();

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}

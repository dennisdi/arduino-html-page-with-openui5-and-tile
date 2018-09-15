/******************************************************************
* FIRST TRY WITHOUT SENSORS! ONLY FOR TRYING OUT CONSUMING UI5!!!
* THIS CODE IS NOT CLEAND UP FINALLY!
* YOU WILL FIND SOME USELESS FRAGMENTS! I´m SORRY... 
* With part´s from: Usman Ali Butt - microcontroller-project.com
 * ****************************************************************/
#include <ESP8266WiFi.h>
 
const char* ssid = "------  YOUR WLAN NAME HERE !!! ------";
const char* password = "------  YOUR PASSWORD HERE !!! ------";
int Raw       = A0;      //Analog channel A0 as used to measure temperature
int threshold = 13;      //Nodemcu digital pin water sensor read
const int regensensorPin = 2; // Pin vom Regensensor
int buttonState = 0;          // variable for reading the pushbutton status
WiFiServer server(80);
 
void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(regensensorPin, INPUT);
  Serial.begin(115200);
  delay(10);
  // Connect to WiFi network
  pinMode(threshold,INPUT_PULLUP); //Pin#12 as output-Activate pullup at pin 13
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);     //Begin WiFi
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();



 
  // Return the response
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
    client.println("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />");
    client.println("<meta charset=\"UTF-8\">");
    client.println("<title>Hello World App</title>");
    client.println("<script id=\"sap-ui-bootstrap\" src=\"https://openui5.hana.ondemand.com/resources/sap-ui-core.js\" data-sap-ui-theme=\"sap_bluecrystal\" data-sap-ui-libs=\"sap.m\" data-sap-ui-preload=\"async\">");
    client.println("</script>");
    client.println("<script type=\"text/javascript\">");
      client.println("sap.ui.getCore().attachInit(function() {");
      // create a mobile app and display page initially
        client.println("var app = new sap.m.App(\"myApp\", {");
          client.println("initialPage: \"page1\"");
        client.println("});");
      // Add one or more tiles
        client.println("var tiles = new Array();");
        client.println("var oTile = new sap.m.StandardTile(\"worst_case\", {");
          client.println("icon: sap.ui.core.IconPool.getIconURI(\"cloud\"),");
          client.println("number: \"-18,5°C\",");
          client.println("numberUnit: \"Gefriertruhe\",");
          client.println("title: \"Keller\",");
          client.println("info: \"Temperatur: stabil\",");
          client.println("infoState: \"Success\"");
        client.println("});");
        client.println("tiles.push(oTile);");
      // create the page
        client.println("var page1 = new sap.m.Page(\"page1\", {");
          client.println("title: \"Messwerte vom 16.09.2018 - 17.13 Uhr\",");
          client.println("showNavButton: false,");
          client.println("content: [");
            client.println("new sap.m.TileContainer({");
              client.println("tiles: tiles");
            client.println("})");
          client.println("]");
        client.println("});");
      // add page to the app
        client.println("app.addPage(page1);");
      // place the app into the HTML document");
        client.println("app.placeAt(\"content\");");
      client.println("});");
    client.println("</script>");
  client.println("</head>");
  client.println("<body class=\"sapUiBody\" id=\"content\">");
  client.println("</body>");
  client.println("</html>");
 
}


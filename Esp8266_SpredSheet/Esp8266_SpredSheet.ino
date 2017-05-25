#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
#include "DHT.h"

DHT dht;

// Replace with your network credentials
const char* ssid = "MyWifi";  //------------------------------------
const char* password = "abcd1234";  //------------------------------------------

const char* host = "script.google.com";
const char* googleRedirHost = "script.googleusercontent.com";
//https://script.google.com/macros/s/AKfycbw-JhJ3nqlNW79LiZUSDRyVZW_AsZF6xIsHkRvfq0CBloGr0pOn/exec
const char *GScriptId = "AKfycbw-JhJ3nqlNW79LiZUSDRyVZW_AsZF6xIsHkRvfq0CBloGr0pOn";  //-------------------------------------------

const int httpsPort = 443;


// echo | openssl s_client -connect script.google.com:443 |& openssl x509 -fingerprint -noout

const char* fingerprint = "D1 58 71 3D 71 54 34 86 50 2A DF FE 2C 74 85 50 82 20 95 C3";  //--------------------------------------------------------

//String url_x = String("/macros/s/") + GScriptId + "/exec?value=Hello";
String url_x = String("/macros/s/") + GScriptId + "/exec";
String url3 = String("/macros/s/") + GScriptId + "/exec?read";
float a,tag;
//String url = url_x + "?value=" + String(a);

String url = url_x + "?tag=" + String(tag) + "&value=" + String(a);

void setup() {

  dht.setup(D3);  
  pinMode(D0, OUTPUT);  //LED
  
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  HTTPSRedirect client(httpsPort);
  Serial.print("Connecting to ");
  Serial.println(host);

  bool flag = false;
  for (int i=0; i<5; i++){
    int retval = client.connect(host, httpsPort);
    if (retval == 1) {
       flag = true;
       break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }
  
  Serial.flush();
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    Serial.println("Exiting...");
    return;
  }
  
  Serial.flush();
  if (client.verify(fingerprint, host)) {
    Serial.println("Certificate match.");
  } else {
    Serial.println("Certificate mis-match");
  }

 // client.printRedir(url, host, googleRedirHost);
  Serial.println("==============================================================================");
  
 
}

void loop() {

  delay(dht.getMinimumSamplingPeriod());

  float tag = dht.getHumidity();
  float a = dht.getTemperature();
  Serial.println("Humid : \t"); Serial.println(tag);
  Serial.println("Temp : \t"); Serial.println(a);

  String url = url_x + "?tag=" + String(tag) + "&value=" + String(a);
  
  HTTPSRedirect client(httpsPort);
  if (!client.connected())
    client.connect(host, httpsPort);
  
  client.printRedir(url3, host, googleRedirHost);

  client.printRedir(url, host, googleRedirHost);


  delay(1500);
}

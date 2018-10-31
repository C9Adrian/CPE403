#include <ESP8266WiFi.h>

#define period 15     //15 secs for Thingspeak

//My SSID information
const char* server = "api.thingspeak.com";
String apikey = "S5WR2DBEPDZ2UB0L";
const char* ssid = "********";
const char* password = "******";

//holds lux value
String luxValue;

//character read in
char readIn;

int sent = 0;


void sendValue(String lux)
{
    //Creates a client that can connect to to a specified internet IP address and port
   WiFiClient client;
  
   if (client.connect(server, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
   Serial.println("WiFi Client connected ");

   //String to be sent off to server
   String postStr = apikey;
   postStr += "&field1=";
   postStr +=  lux;
   postStr += "\r\n\r\n";

   //Make an HTTP request
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   //Transmit Information once request has been approved 
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apikey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(1000);
   
   }//end if
   sent++;
 client.stop();
  
}

void connectwifi()
{
  Serial.print("Connecting to "+*ssid);
  //Initializes the WiFi library's network settings and provides the current status
  WiFi.begin(ssid, password);

  //Keep trying to successfuly connect
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }

  //Successful connections
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  connectwifi();
  
}

void loop() {
  // put your main code here, to run repeatedly:

  if(Serial.available())
  {
    //
  }

    Serial.print(String(sent)+" Luminosity Sensor: ");
    Serial.println(luxValue);

    //Send Lux value to client
    sendValue(luxValue);
    int count = period;
    while(count--){
      luxValue = "";
      delay(1000);
    }

    
  Serial.flush();
}

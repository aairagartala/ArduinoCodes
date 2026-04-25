 #include <WiFi.h>
#include <WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 15 on the ESP32
#define ONE_WIRE_BUS 15

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

uint8_t sensor1[8] = { 0x28, 0x54, 0x42, 0x78, 0x00, 0x00, 0x00, 0x32   };
uint8_t sensor2[8] = { 0x28, 0xDE, 0xA4, 0x7C, 0x00, 0x00, 0x00, 0x5F   };
uint8_t sensor3[8] = { 0x28, 0x22, 0x7C, 0x79, 0x00, 0x00, 0x00, 0xB1   };

/*Put your SSID & Password*/
const char* ssid = "AUTOMATION";  // Enter SSID here
const char* password = "pass@1234";  //Enter Password here

WebServer server(80);             

float tempSensor1, tempSensor2, tempSensor3;
 
void setup() {
  Serial.begin(115200);
  delay(100);
  
  sensors.begin();              

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

}
void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  sensors.requestTemperatures();
  tempSensor1 = sensors.getTempC(sensor1); // Gets the values of the temperature
  tempSensor2 = sensors.getTempC(sensor2); // Gets the values of the temperature
  tempSensor3 = sensors.getTempC(sensor3); // Gets the values of the temperature
  server.send(200, "text/html", SendHTML(tempSensor1,tempSensor2,tempSensor3)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float tempSensor1,float tempSensor2,float tempSensor3){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP32 Temperature Monitor</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP32 Temperature Monitor</h1>\n";
  ptr +="<p>Living Room: ";
  ptr +=tempSensor1;
  ptr +="&deg;C</p>";
  ptr +="<p>Bedroom: ";
  ptr +=tempSensor2;
  ptr +="&deg;C</p>";
  ptr +="<p>Kitchen: ";
  ptr +=tempSensor3;
  ptr +="&deg;C</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}






//0x28, 0x54, 0x42, 0x78, 0x00, 0x00, 0x00, 0x32
//0x28, 0xDE, 0xA4, 0x7C, 0x00, 0x00, 0x00, 0x5F
// 0x28, 0x22, 0x7C, 0x79, 0x00, 0x00, 0x00, 0xB1 

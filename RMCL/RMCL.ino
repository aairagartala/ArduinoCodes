
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include<EEPROM.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

 
#include"AcRemote.h"

#include <OneWire.h>
 
#include "DHT.h"

const uint16_t kIrLed = 4;  

IRsend irsend(kIrLed);  

 int stam=0;

 int stae=0;

 float tm;
 float hm;

 String tmpt;
 String hmdt;

 String tmpf;

 String mnad;
 String stag;
 
 uint16_t rawData[211] = {5200, 4810,  670, 1330,  666, 332,  662, 1356,  630, 370,  610, 1392,  578, 424,  576, 1424,  574, 1426,  574, 1424,  576, 1426,
   576, 1422,  576, 1426,  576, 1424,  574, 426,  574, 1426,  574, 1426,  574, 1426,  574, 1448,  552, 426,  574, 446,  552, 1426,  574, 1446,  554, 446,  
   554, 1446,  552, 446,  552, 1448,  552, 448,  552, 1448,  552, 1448,  552, 448,  552, 448,  552, 1448,  552, 1448,  552, 1448,  552, 1448,  552, 1448,  
   552, 1448,  550, 1448,  552, 1448,  552, 1448,  552, 1448,  552, 1448,  550, 1450,  550, 1450,  552, 1448,  552, 1448,  550, 1450,  552, 1448,  550, 1450,
     550, 1450,  550, 1450,  550, 1448,  552, 1448,  552, 1450,  534, 1466,  550, 1450,  550, 1450,  550, 1450,  526, 1474,  526, 1472,  528, 1472,  528, 1472, 
      526, 1472,  528, 1474,  526, 472,  526, 472,  528, 472,  526, 474,  526, 474,  526, 1474,  526, 1472,  528, 1474,  526, 472,  528, 1472,  528, 1472,  526, 
      1474,  526, 1474,  526, 1474,  526, 1474,  526, 1474,  526, 472,  526, 1474,  526, 1474,  524, 1474,  526, 474,  526, 474,  526, 474,  526, 474,  526, 1476, 
       524, 1474,  526, 1474,  526, 1476,  524, 1474,  526, 1474,  526, 1476,  524, 1476,  524, 474,  526, 474,  526, 1474,  526, 1474,  526, 1474,  526, 474,  524, 1476,  524, 1476,  5026};


uint16_t rawDatan[211] = {5166, 4834,  668, 1332,  666, 334,  658, 1358,  614, 386,  608, 1394,  578, 424,  574, 1428,  572, 1426,  574, 1446,  552, 1428,  574, 1446, 
 554, 1426,  574, 1448,  552, 426,  574, 1448,  552, 446,  552, 1448,  552, 1448,  552, 448,  552, 448,  552, 1448,  552, 1448,  552, 448,  550, 1450,  552, 448,  552, 1450, 
  550, 450,  550, 1448,  552, 1450,  550, 448,  550, 450,  552, 1448,  552, 1448,  552, 1450,  552, 1448,  552, 1448,  550, 1452,  550, 1450,  550, 1450,  550, 1450,  552, 1448,
    550, 1450,  550, 1450,  550, 1448,  550, 1450,  550, 1448,  550, 1450,  550, 1448,  550, 1450,  548, 1452,  550, 1450,  550, 1450,  550, 1452,  548, 1452,  550, 1450,  548, 1452, 
     550, 1450,  550, 1452,  548, 1452,  550, 1450,  550, 1452,  526, 1474,  526, 1474,  524, 1476,  526, 474,  526, 472,  526, 474,  526, 474,  526, 472,  526, 1474,  526, 1474,  524,
      1474,  526, 474,  526, 1474,  526, 1474,  526, 1476,  526, 1474,  526, 1474,  526, 1474,  526, 1476,  524, 474,  524, 1476,  524, 1476,  524, 1476,  524, 476,  524, 476,  526, 474,
        526, 1474,  526, 474,  524, 1476,  524, 1476,  524, 1476,  524, 1476,  524, 1476,  524, 1474,  524, 1476,  524, 476,  526, 474,  526, 1474,  524, 1476,  524, 1476,  522, 476,  524, 1476,  524, 1476,  5026};




#define DHTPIN 2      

#define DHTTYPE DHT11 




 DHT dht(DHTPIN, DHTTYPE);

 





WebServer server(80);       



IPAddress    apIP(192, 168, 1,242);  
                                   
const char *ssid = "RMCLC";
const char *password = "12345678";

 
WiFiClient client;


TaskHandle_t Task1;

 
 

  



void handleRoot() {
 String s = MAIN_page; 
 server.send(200, "text/html", s); 
}





 
 


 

  


void blnc5() 

{

String sbln5="";
 sbln5 = tmpt ;


 server.send(200, "text/plane", sbln5);

}



void blnc8() 

{

String sbln8="";
 sbln8 = tmpf ;


 server.send(200, "text/plane", sbln8);

}

void blnc14() 

{

String sbln14="";
 
 sbln14=hmdt;


 server.send(200, "text/plane", sbln14);

}





 void handleph() {
 String phState = "OFF";
 String t_state = server.arg("phstate"); 
 
  
if(t_state=="5")
 {
  
  irsend.sendRaw(rawData, 211, 38);   
  delay(6);

 }

 if(t_state=="6")
 {
  
  irsend.sendRaw(rawDatan, 211, 38);   
  delay(6);

 }

}
 

 
void handleph6() {
   String stage;
 stag = server.arg("stage");


 if(stag=="24")
 {
  stae=1;
 }
 
 if(stag=="25")
 {
  stae=2;
 }

 if(stag=="26")
 {
  stae=3;
 }

  if(stag=="27")
 {
  stae=4;
 }

 if(stag=="28")
 {
  stae=5;
 }

 if(stag=="29")
 {
  stae=6;
 }

if(stag=="30")
 {
  stae=7;
 }

 if(stag=="31")
 {
  stae=8;
 }

 if(stag=="32")
 {
  stae=9;
 }


  EEPROM.begin(100);
  EEPROM.write(0,stae);
  EEPROM.commit();
  EEPROM.end();

Serial.println(stag);

}

 










void saveSettingsToEEPPROM(char* ssid_, char* pass_,char* ipaddressd,char* gatewayd){

  
  writeEEPROM(1 ,16  , ssid_);
  writeEEPROM(1* 16 , 16,  pass_);
  writeEEPROM(2* 16 , 16 , ipaddressd);
  writeEEPROM(3* 16 , 16 , gatewayd);
 
}

void readSettingsFromEEPROM(char* ssid_, char* pass_,char* ipaddressd,char* gatewayd){
  readEEPROM( 1 , 16 , ssid_);
  readEEPROM(( 1* 16 ), 16 , pass_);
  readEEPROM(( 2* 16) , 16 , ipaddressd);
  readEEPROM(( 3* 16), 16 , gatewayd);

   Serial.println(ssid_);
  Serial.println(ipaddressd);
}


void writeEEPROM(int startAdr, int length, char* writeString) {
  EEPROM.begin(400);
  yield();
  for (int i = 0; i < length; i++) EEPROM.write(startAdr + i, writeString[i]);
  EEPROM.commit();
  EEPROM.end();           
}

void readEEPROM(int startAdr, int maxLength, char* dest) {
  EEPROM.begin(400);
  delay(10);
  for (int i = 0; i < maxLength; i++) dest[i] = char(EEPROM.read(startAdr + i));
  dest[maxLength-1]=0;
  EEPROM.end();
  }
 

















void setup() {
  Serial.begin(9600);
 
   

 xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &Task1,  0); 

  delay(500); 
  


  

  
 
 


  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  delay(100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   
    
 /*

if(flag==21 || flag==22 || flag==23)
{

  readSettingsFromEEPROM(ssidd,passwordd,ipadrs,gatway); 
  IPAddress ipdd,gata;
  ipdd.fromString(ipadrs);
  gata.fromString(gatway);
  IPAddress sbnt(255,255,255,0);
  WiFi.config(ipdd,gata,sbnt,gata);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
 Serial.println(ssidd);
 Serial.println(sbnt);
 
 
 delay(1000);

  Serial.println("");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);

    //break;
  }

  */
  Serial.println("");
  Serial.print("Connected to ");
  
  Serial.println(ssid);
  Serial.print("IP address: ");
  delay(2000);
  Serial.println(WiFi.localIP());
  
 
 
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  


 
   
  // rtc.adjust(DateTime(2025, 18, 23, 7, 15, 36));


 


  
  server.on("/", handleRoot);      
 // server.on("/action_page", handleForm); 

   server.on("/b4", blnc5);
   
   server.on("/b6", blnc8);

   server.on("/b16", blnc14);

   
  
   
   
   server.on("/setstage", handleph6);
  
  
  server.on("/setph", handleph);

  
  server.begin();                 
  Serial.println("HTTP server started");

 
   dht.begin();
 

 
 irsend.begin();




 
 


 

}









void loop(void){


EEPROM.begin(100);
 
stam=EEPROM.read(0);
 EEPROM.end();

 if(stam==1)
 {
  tmpf="24";
 }

 if(stam==2)
 {
  tmpf="25";
 }
 
 if(stam==3)
 {
  tmpf="26";
 }
 
 if(stam==4)
 {
  tmpf="27";
 }
 
 if(stam==5)
 {
  tmpf="28";
 }

if(stam==6)
 {
  tmpf="29";
 }

if(stam==7)
 {
  tmpf="30";
 }

 if(stam==8)
 {
  tmpf="31";
 }

 if(stam==9)
 {
  tmpf="32";
 }


/*
 if(digitalRead(14)==LOW)
 {

  flagg=EEPROM.read(0);
 EEPROM.end();
 r=flagg%10;
 flagg=(flagg-r)+1;
  EEPROM.begin(100);
  EEPROM.write(0,flagg);
  EEPROM.commit();
  EEPROM.end();

  EEPROM.begin(100);
 
 flag=EEPROM.read(0);
 EEPROM.end();
   
 }
  
  if(digitalRead(12)==LOW)
 {
  flagg=EEPROM.read(0);
 EEPROM.end();
 r=flagg%10;
 flagg=(flagg-r)+2;

  EEPROM.begin(100);
  EEPROM.write(0,flagg);
  EEPROM.commit();
  EEPROM.end();
  EEPROM.begin(100);
 
 flag=EEPROM.read(0);
 EEPROM.end();
  
 }

 if(digitalRead(13)==LOW)
 {

  flagg=EEPROM.read(0);
 EEPROM.end();
 r=flagg%10;
 flagg=(flagg-r)+3;
  EEPROM.begin(100);
  EEPROM.write(0,flagg);
  EEPROM.commit();
  EEPROM.end();
   
   EEPROM.begin(100);
 
 flag=EEPROM.read(0);
 EEPROM.end();
 }


if(flag%10==1)
{5
  
  digitalWrite(5,HIGH);
  digitalWrite(18,LOW);
  digitalWrite(19,LOW);

   digitalWrite(15,HIGH);
  digitalWrite(2,LOW);
  digitalWrite(4,LOW);
}

if(flag%10==2)
{
   
  digitalWrite(5,LOW);
  digitalWrite(18,HIGH);
  digitalWrite(19,LOW);

  digitalWrite(15,LOW);
  digitalWrite(2,HIGH);
  digitalWrite(4,LOW);
}


if(flag%10==3)
{
   
  digitalWrite(5,LOW);
  digitalWrite(18,LOW);
  digitalWrite(19,HIGH);

  digitalWrite(15,LOW);
  digitalWrite(2,LOW);
  digitalWrite(4,HIGH);
}
*/

 
/*
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
*/
   


/*
if(mnth==1)
{
   
    if(hr>=16 && mn>=50 && mn<=59)
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);

    }
      
   if(hr>=17 && hr<21)
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}

if(mnth==1)
{
   
    if(hr>=16 && (mn>=50 && mn<=59))
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);
    }
      
   if(hr>=17 && hr<21)
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}



if(mnth==2)
{
   
    if(hr>=17 && (mn>=20 && mn<=59))
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);
    }
      
   if(hr>=18 && hr<21)
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}



if(mnth==3)
{
   
    if(hr>=17 && (mn>=34 && mn<=59))
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);
    }
      
   if(hr>=18 && hr<21)
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}


if(mnth==4)
{
   
    if(hr>=17 && (mn>=46 && mn<=59))
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);
    }
      
   if(hr>=18 && hr<21)
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}


if(mnth==5)
{
   
    if(hr>=17 &&  mn==59)
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);
    }
      
   if(hr>=18 && hr<21 )
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}


if(mnth==6)
{
   
    if(hr>=18 && (mn>=12 && mn<=59))
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);
    }
      
   if(hr>=19 && hr<21)
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}


if(mnth==7)
{
   
    if(hr>=18 && (mn>=14 && mn<=59))
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);
    }
      
   if(hr>=17 && hr<21)
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}


if(mnth==8)
{
   
    if(hr>=17 && (mn>=57 && mn<=59))
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);
    }
      
   if(hr>=18 && hr<21)
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}



if(mnth==9)
{
   
    if(hr>=17 && (mn>=28 && mn<=59))
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);
    }
      
   if(hr>=18 && hr<21)
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}


if(mnth==10)
{
   
    if(hr>=16 && mn==59)
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);
    }
      
   if(hr>=17 && hr<21)
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}


if(mnth==11)
{
   
    if(hr>=16 && (mn>=39 && mn<=59))
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);
    }
      
   if(hr>=17 && hr<21)
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}


if(mnth==12)
{
   
    if(hr>=16 && (mn>=41 && mn<=59))
    {
           digitalWrite(15,HIGH);
           digitalWrite(18,HIGH);
    }
      
   if(hr>=17 && hr<21)
   {
    digitalWrite(15,HIGH);
    digitalWrite(18,HIGH);
   } 

   
}

*/




/*

  if(hr>=21)
    {
        digitalWrite(15,LOW);
        digitalWrite(18,LOW);
    }
*/
     
     


     

        
      
         

          
  

  float hm = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float tm = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenhei

  if (isnan(hm) || isnan(tm) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

   
  // Compute heat index in Celsius (isFahreheit = false)
   

  Serial.println(F("Humidity: "));
  Serial.println(hm);
  Serial.println(F("%  Temperature: "));
  Serial.println(tm);
  
  tmpt=String(tm);
  hmdt=String(hm);

   delay(2000); 
}
 
  void Task1code( void * pvParameters ){

  for(;;)
  {

     server.handleClient();   
     
  }

}
 

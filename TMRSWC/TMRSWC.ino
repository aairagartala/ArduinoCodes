
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include<EEPROM.h>


#include "RTClib.h"

#include "index.h"




 

 



const byte address[6] = "00001";

const byte addr[6] = "00002";

char st[16]="";

char stem[16]="";

char strm[16]="";

//String sr[16] = "Data recv";


RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


int flag=0;
int flagsa=1;
int flagsb=0;
int flagsc=0;
int flagg=0;
int sflag=0;
int stln=0;

 


String si;

String pd;

String id;

String ga;

int i;

int j;

int cntp=0;

int cntn=0;

int r=0;

int yr=0;
int mnth=0;
int dt=0;
int hr=0;
int mn=0;
int scnd=0;


String yra;
String mntha;
String dta;
String hra;
String mna;
String scnda;



String yrad;
String mnthad;
String dtad;
String hrad;
String mnad;
String scndad;

WebServer server(80);       



IPAddress    apIP(192, 168, 1,146);  
                                   
const char *ssid = "LWTTMR";
const char *password = "12345678";

 
WiFiClient client;


TaskHandle_t Task1;

char ssidd[16]="";
char passwordd[16]="";
char ipadrs[16]="";
char gatway[16]="";

char sii[16]="";
char pdd[16]="";
char ida[16]="";
char gaa[16]="";


int cflag=1;
int stflag=0;
int dflag=0;
 

  



void handleRoot() {
 String s = MAIN_page; 
 server.send(200, "text/html", s); 
}





void blnc() 

{

String sbln1="";
 
if(mnth==1)
{
  sbln1="16:59";
}


if(mnth==2)
{
  sbln1="17:20";
}

if(mnth==3)
{
  sbln1="17:34";
}

if(mnth==4)
{
  sbln1="17:46";
}

if(mnth==5)
{
  sbln1="17:59";
}

if(mnth==6)
{
  sbln1="18:12";
}

if(mnth==7)
{
  sbln1="18:14";
}

if(mnth==8)
{
  sbln1="17:57";
}

if(mnth==9)
{
  sbln1="17:28";
}

if(mnth==10)
{
  sbln1="16:59";
}

if(mnth==11)
{
  sbln1="16:39";
}

if(mnth==12)
{
  sbln1="16:41";
}




 server.send(200, "text/plane", sbln1);

}

void blnc1() 

{

 
String sbln2="";
  
sbln2=hra+":"+mna+":"+scnda+"-"+dta+"/"+mntha+"/"+yra;

 server.send(200, "text/plane", sbln2);

}



void blnc2() 

{

String sbln3="";
 sbln3=strm;


 server.send(200, "text/plane", sbln3);

}


void blnc3() 

{

String sbln4="";
 sbln4=String(hr);


 server.send(200, "text/plane", sbln4);

}

void blnc4() 

{

String sbln5="";
 sbln5=String(mn);


 server.send(200, "text/plane", sbln5);

}

void blnc5() 

{

String sbln6="";
 sbln6=String(scnd);


 server.send(200, "text/plane", sbln6);

}





 

void handleph() {
   String phstate1;
 yrad = server.arg("phstate1");
 

 
}

void handleph1() {
   String phstate2;
 mnthad = server.arg("phstate2");
 

 
}

void handleph2() {
   String phstate3;
 dtad = server.arg("phstate3");
 

 
}

void handleph3() {
   String phstate4;
 hrad = server.arg("phstate4");
 


}

void handleph4() {
   String phstate5;
 mnad = server.arg("phstate5");
 

 
}

void handleph5() {
   String phstate6;
 scndad = server.arg("phstate6");
 
 int ya=0;
 int ma=0;
 int da=0;
 int ha=0;
 int mh=0;
 int sa=0;
 
 ya=yrad.toInt();
 ma=mnthad.toInt();
 da=dtad.toInt();
 ha=hrad.toInt();
 mh=mnad.toInt();
 sa=scndad.toInt();


  rtc.adjust(DateTime(ya, ma, da, ha, mh, sa));

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
 
 if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

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
  
  Serial.println(ssidd);
  Serial.print("IP address: ");
  delay(2000);
  Serial.println(WiFi.localIP());
  
 
 
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  


 
   
  // rtc.adjust(DateTime(2025, 18, 23, 7, 15, 36));


 


  
  server.on("/", handleRoot);      
 // server.on("/action_page", handleForm); 

  server.on("/bl", blnc);
  server.on("/getrtc", blnc1);
  server.on("/b3", blnc2);
  server.on("/b4", blnc3);
  server.on("/b5", blnc4);
  server.on("/b6", blnc5);
   
  
 









   
  
  server.on("/setph1", handleph);
  server.on("/setph2", handleph1);
  server.on("/setph3", handleph2);
  server.on("/setph4", handleph3);
  server.on("/setph5", handleph4);
  server.on("/setph6", handleph5);
  


  
  server.begin();                 
  Serial.println("HTTP server started");

 

 

pinMode(15,OUTPUT);
pinMode(18,OUTPUT);
pinMode(19,OUTPUT);

digitalWrite(15,LOW);
digitalWrite(18,LOW);
digitalWrite(19,LOW);
 

}









void loop(void){

  
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
{
  
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

digitalWrite(19,HIGH);

DateTime now = rtc.now();
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
  hr=now.hour();
  hra=String(hr);

  mn=now.minute();
  mna=String(mn);

  scnd=now.second();
  scnda=String(scnd);

  yr=now.year();
  yra=String(yr);

  mnth=now.month();
  mntha=String(mnth);

  dt=now.day();
  dta=String(dt);

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








  if(hr>=21)
    {
        digitalWrite(15,LOW);
        digitalWrite(18,LOW);
    } 
}
 
  void Task1code( void * pvParameters ){

  for(;;)
  {

     server.handleClient();   
     
  }

}
 

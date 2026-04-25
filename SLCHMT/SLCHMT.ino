#include <WiFi.h>
 #include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <WebServer.h>

#include "ACS712.h"
 
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

LiquidCrystal_I2C lcd(0x27,20,4);


#define WLAN_SSID       "realme 14x 5G"
#define WLAN_PASS       "12345678"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "Akesha_iot"
#define AIO_KEY         "aio_wJYt38iScOTLNuKDL4nuoYUcmdaH"

WiFiClient client;
 
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe evnc = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/evnc");

Adafruit_MQTT_Subscribe prcs = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/prcs");
 
 ACS712  ACS(39, 3.3, 4095, 185);

float sv=0;
float solv=0;


float bv=0;
float btvl=0;

float ct=0;
float ctnt=0;

float ct1=0;

float ctnt1=0;

 

float vl=0;
float vlg=0;

float vola=0;
float volb=0;

float cmxa=0;
float cmxb=0;

float pr=0;

int cntt=1;


int flagn=0;

int flagc=0;

int flagpe=0;

int flagir=0;

int i=0;




 


void setup() {
  Serial.begin(9600);

   
    lcd.init();                       
  lcd.init();
  
  lcd.backlight();
   
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

 WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }   
Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
   

   mqtt.subscribe(&evnc);

   mqtt.subscribe(&prcs);

   pinMode(2,OUTPUT);
   pinMode(4,OUTPUT);
   pinMode(5,OUTPUT);
   pinMode(18,OUTPUT);

  digitalWrite(2,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(18,HIGH);

   digitalWrite(5,LOW);
        digitalWrite(18,LOW);

         ACS.autoMidPoint();



}

void loop() {



  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;

  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &evnc) {
      Serial.print(F("Got: "));
      Serial.println((char *)evnc.lastread);

      if (!strcmp((char*) evnc.lastread, "ON")) {

        flagn=1;
        
      }
      else {
             
             flagn=0;

      }
    }

    if (subscription == &prcs) {
      Serial.print(F("Got: "));
      Serial.println((char *)prcs.lastread);

      if (!strcmp((char*) prcs.lastread, "ON")) {
        
        flagc=1;

      }
      else {
        
        flagc=0;

      }
    }




  }

  sv=analogRead(35);

  solv=sv/82.07;

  bv=analogRead(32);

  btvl=bv/95.65;
 
 
 for(i=1;i<=16;i++)
 {
   cmxa=ct;
  ct=analogRead(33);
   if(ct>cmxa)
   {
    cmxa=ct;
   }
   delay(2);
 }

 ct=cmxa;


 
 
  ctnt=(ct-798)*0.0048;

  vola=analogRead(36);

  volb=vola/4.74;

  if(volb>180)
  {
    flagpe=1;
  }

  if(volb<170)
  {
    flagpe=0;
  }



 
   
   ct1=ACS.mA_AC_sampling();

    ct1=(ct1-36)/271;

   
   if(flagir==1)
   {

    pr=ct1*240;
   }
   
   if(flagir==0)
   {
    pr=ct1*volb;
  
   }

   







  
   
  Serial.println(solv);

  Serial.println(btvl);

  Serial.println(ctnt);

  Serial.println(volb);

  Serial.println(ct1);

  

  if(cntt>3)
  {
    cntt=1;
  }
  if(cntt==1)
  {

  lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("SV:");

  lcd.print(solv);
    lcd.setCursor(8,0);
lcd.print("BV:");
     lcd.print(btvl);
  }
  if(cntt==2)
  {
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("SC:");
    lcd.print(ctnt);

    if(flagn==1)
    {
      lcd.setCursor(1,1);
      lcd.print("Night");
    }
  
   if(flagn==0)
    {
      lcd.setCursor(1,1);
      lcd.print("Day");
    }
  
  if(flagpe==1)
  {
    if(flagc==1)
    {
      lcd.setCursor(8,1);
      lcd.print("LOW");
    }

   if(flagc==0)
    {
      lcd.setCursor(8,1);
      lcd.print("HIGH");
    }


  }
  


  if(flagpe==0)
  {
      lcd.setCursor(8,1);
      lcd.print("NO PWR");
    
  }
  }



  if(cntt==3)
  {

  lcd.clear();
   
   if(flagir==0)
   {
    lcd.setCursor(0,0);
    lcd.print("IV:");
    
  lcd.setCursor(3,0);
  lcd.print(volb);

  lcd.setCursor(9,0);
  lcd.print("PW:");

  lcd.print(pr);

  lcd.setCursor(0,1);
  lcd.print("Curr:");
  lcd.setCursor(6,1);
  lcd.print(ct1);
    
 
   }


   if(flagir==1)
   {
    lcd.setCursor(0,0);
    lcd.print("IV:");
    
  lcd.setCursor(3,0);
  lcd.print("210");

   lcd.setCursor(9,0);
  lcd.print("PW:");

  lcd.print(pr);

   lcd.setCursor(0,1);
  lcd.print("Curr:");
  lcd.setCursor(6,1);
  lcd.print(ct1);
  
    
 
   }
  
  }


if(flagpe==1)
{

   if(flagn==0 && flagc==0)
   {
      if(btvl>=12.7)
      {
        
        

        
        digitalWrite(2,HIGH);
        digitalWrite(4,HIGH);

        delay(1000);

        digitalWrite(5,LOW);
        digitalWrite(18,LOW);

        flagir=1;
      }
   
   
     if(btvl<12.5)
      {
         
       
        digitalWrite(5,HIGH);
        digitalWrite(18,HIGH);
        
        delay(1000);

        digitalWrite(2,LOW);
        digitalWrite(4,LOW);

        flagir=0;

       
      }
   }
     if(flagn==0 && flagc==1)
     {

       digitalWrite(5,HIGH);
        digitalWrite(18,HIGH);

        delay(1000);

        
        digitalWrite(2,LOW);
        digitalWrite(4,LOW);

        flagir=0;
       

     }
   
     if(flagn==1 && flagc==0)
     {
       
         if(btvl>=12.5)
      {
        
        

        
        digitalWrite(2,HIGH);
        digitalWrite(4,HIGH);

        delay(1000);

        digitalWrite(5,LOW);
        digitalWrite(18,LOW);

        flagir=1;
      }
   
   
     if(btvl<12.3)
      {
         
       
        digitalWrite(5,HIGH);
        digitalWrite(18,HIGH);
        
        delay(1000);

        digitalWrite(2,LOW);
        digitalWrite(4,LOW);

        flagir=0;

       
      }

     } 


   if(flagn==1 && flagc==1)
     {

       
   digitalWrite(5,HIGH);
        digitalWrite(18,HIGH);

        delay(1000);

        
        digitalWrite(2,LOW);
        digitalWrite(4,LOW);

        flagir=0;

   } 

   
  
}

  if(flagpe==0)
  {

     if(btvl>=12.5
     )
      {
        
        

        
        digitalWrite(2,HIGH);
        digitalWrite(4,HIGH);

        delay(1000);

        digitalWrite(5,LOW);
        digitalWrite(18,LOW);

        flagir=1;
      }
   
   
     if(btvl<12.3)
      {
         
       
        digitalWrite(5,HIGH);
        digitalWrite(18,HIGH);
        
        delay(1000);

        digitalWrite(2,HIGH);
        digitalWrite(4,HIGH);

        flagir=0;

       
      }


  }


cntt=cntt+1;
  delay(2000);


}





 

void MQTT_connect()
{
  int8_t ret;

   
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) {  
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();

    delay(5000);  

    retries--;
    if (retries == 0) {
       
      while (1);
    }
  }

  Serial.println("MQTT Connected!");
}


 
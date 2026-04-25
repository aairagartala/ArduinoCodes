
#include <Wire.h>
#include <TEA5767N.h>

TEA5767N radio = TEA5767N();
float station[] = {101.6,95.0,92.7};
int i=0, numStations = *(&station+1)-station;
boolean powerOnOff = false, muteAudio = false;

void setup()
{ 
  Wire.begin();
  //radio.setFrequency(station[0]);
}

void loop()
{
  if(powerOnOff == true)
  {
    /*lcd.setCursor(0, 0);
    lcd.print("Power ON");
    lcd.setCursor(0, 1);
    lcd.print("Select Station");*/
    radio.setStandByOff();
    //radio.mute();
  }
  //-------------------------------------------
  while(powerOnOff)
  {
    if(digitalRead(10) == HIGH) changeStation();
    //if(digitalRead(13) == HIGH) muteStatus();
  }
}


void changeStation()
{
  radio.turnTheSoundBackOn();
  radio.selectFrequency(station[i]);
  delay(1000);
  i++;
  if(i == numStations) 
  i = 0;
}
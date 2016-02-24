//For connection to WLAN//
const char* ssid = "eduroam";
const char* password = "80005350";

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "4f60a4abb5e744f395a39f877c5877a"; //Blynk key

void setup() 
{
  Blynk.begin(auth, ssid, password); //Connecting to blynk
}

void loop() 
{
    //****Blynk communication****//
  Blynk.run();
  Blynk.virtualWrite(1,tempC);
  Blynk.virtualWrite(2,tempC);
  //********//
}

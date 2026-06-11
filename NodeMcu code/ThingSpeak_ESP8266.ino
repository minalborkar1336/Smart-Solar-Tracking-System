#include <ESP8266WiFi.h>
#include <ThingSpeak.h>


// Add your details here

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_PASSWORD";


unsigned long myChannelNumber = YOUR_CHANNEL_NUMBER;

const char* myWriteAPIKey = "YOUR_API_KEY";


WiFiClient client;


String buffer="";

bool packetStarted=false;



void setup()
{

Serial.begin(9600);


WiFi.begin(ssid,password);


Serial.print("Connecting");


while(WiFi.status()!=WL_CONNECTED)
{
 delay(500);
 Serial.print(".");
}


Serial.println();
Serial.println("WiFi Connected");


ThingSpeak.begin(client);


pinMode(D4,OUTPUT);

digitalWrite(D4,HIGH);

}



void loop()
{


while(Serial.available())
{


char c = Serial.read();



if(c=='<')
{

packetStarted=true;
buffer="";

}



else if(c=='>')
{


packetStarted=false;


float voltage;
float rain;
float ldrL;
float ldrR;



int parsed = sscanf(
buffer.c_str(),
"%f,%f,%f,%f",
&voltage,
&rain,
&ldrL,
&ldrR
);



if(parsed==4)
{


ThingSpeak.setField(1,voltage);

ThingSpeak.setField(2,rain);

ThingSpeak.setField(3,ldrL);

ThingSpeak.setField(4,ldrR);



int status = ThingSpeak.writeFields(
myChannelNumber,
myWriteAPIKey
);



if(status==200)
{

digitalWrite(D4,LOW);
delay(200);
digitalWrite(D4,HIGH);

Serial.println("Uploaded");

}


else
{

Serial.println("Upload Failed");

}


}


buffer="";


delay(16000);


}



else if(packetStarted)
{

buffer += c;

}


}


}
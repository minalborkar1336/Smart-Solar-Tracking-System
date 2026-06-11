#include <Servo.h>

Servo horizontal;
Servo wiper;

int servoh = 90;
int servohLimitHigh = 155;
int servohLimitLow = 35;

int ldrlt = A2;
int ldrrt = A1;
int rainPin = A0;
int voltPin = A3;

int rainThreshold = 600;

bool wiperUp = false;

void setup()
{
  Serial.begin(9600);

  horizontal.attach(9);
  wiper.attach(6);

  wiper.write(0);

  Serial.println("SMART SOLAR TRACKER SYSTEM");
  delay(1000);
}


void loop()
{

  // Solar tracking

  int lt = analogRead(ldrlt);
  int rt = analogRead(ldrrt);

  int tol = 50;

  int dhoriz = lt - rt;


  if(dhoriz > tol)
  {
    servoh++;

    if(servoh > servohLimitHigh)
      servoh = servohLimitHigh;
  }

  else if(dhoriz < -tol)
  {
    servoh--;

    if(servoh < servohLimitLow)
      servoh = servohLimitLow;
  }


  horizontal.write(servoh);



  // Rain detection + wiper

  int rainValue = analogRead(rainPin);


  if(rainValue < rainThreshold)
  {

    if(wiperUp)
    {
      wiper.write(0);
      wiperUp = false;
    }

    else
    {
      wiper.write(90);
      wiperUp = true;
    }

    delay(400);

  }

  else
  {
    wiper.write(0);
    wiperUp=false;
  }



  // Voltage measurement

  int rawV = analogRead(voltPin);

  float Vin = (rawV * 5.0) / 1023.0;



  // Send data to NodeMCU

  Serial.print("<");

  Serial.print(Vin,2);
  Serial.print(",");

  Serial.print(rainValue);
  Serial.print(",");

  Serial.print(lt);
  Serial.print(",");

  Serial.print(rt);

  Serial.println(">");


  delay(100);

}
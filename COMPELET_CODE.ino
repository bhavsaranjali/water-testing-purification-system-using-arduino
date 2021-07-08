float calibration = 20.57; //change this value to calibrate
const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;
#include <EEPROM.h>
#include "GravityTDS.h"

#define TdsSensorPin A1
GravityTDS gravityTds;

float temperature = 25,tdsValue = 0;

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int sensor_pin = A5;

int output_value ;
int phValue;

void setup()
{
  Serial.begin(9600);
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization
    sensors.begin();
}
void loop()
{
  /* START PH */
  for(int i=0;i<10;i++) 
 { 
 buf[i]=analogRead(analogInPin);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buf[i]>buf[j])
 {
 temp=buf[i];
 buf[i]=buf[j];
 buf[j]=temp;
 }
 }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = -5.70 * pHVol + calibration;
 Serial.print("pH = ");
 Serial.println(phValue);
 delay(1000);
   /* END PH */
   
  /* START TDS*/
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue()/10;  // then get the value
     Serial.print("TDS = ");
    Serial.print(tdsValue,0);
     Serial.println("ppm ");
    delay(1000);
   /*END TDS*/

   /* START TEMPRATURE */
sensors.requestTemperatures();  
  Serial.print("Temperature = ");
  Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  //Update value every 1 sec.
  delay(1000);
  
   /* END TEMPRATURE*/
   /* START CONDCUTIVITY*/
   output_value= analogRead(sensor_pin);
   output_value = map(output_value,550,0,0,100);
   Serial.print("counductivity = ");
   Serial.print(output_value);
   Serial.println("%");
   delay(1000);

   /* END */
   /* START TURBUDITY*/
     int senValue = analogRead(A3);// read the input on analog pin 0:
  float voltage = senValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.print("Turbidity = ");
  Serial.println(voltage); // print out the value you read:
  delay(1000);
   /*END*/
   
}

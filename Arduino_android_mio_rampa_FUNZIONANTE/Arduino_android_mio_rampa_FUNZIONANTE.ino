

 //Includo libreria SoftwareSerial
#include <SoftwareSerial.h>
#define BT_TX_PIN 12
#define BT_RX_PIN 11 
int led = 13;
int button = 10;
// Pins used for inputs and outputs********************************************************
const int analogInPin0 = A0;// Analog input pins
const int analogInPin1 = A1;
const int analogInPin2 = A2;
const int analogInPin3 = A3;
 
//Arrays for the 4 inputs**********************************************
float sensorValue[4] = {0,0,0,0};
float voltageValue[4] = {0,0,0,0};
 
//Char used for reading in Serial characters
byte inbyte = 0;
//*******************************************************************************************
 //istanzio oggetto SoftwareSerial (il nostro futuro bluetooth)
    SoftwareSerial bt =  SoftwareSerial(BT_RX_PIN, BT_TX_PIN);
void setup() {
  // initialise serial communications at 9600 bps:
  Serial.begin(9600);
  //definisco modalità pin
      pinMode(BT_RX_PIN, INPUT);
      pinMode(BT_TX_PIN, OUTPUT);
 // lcd.begin(20, 4); //change to 16, 2 for smaller 16x2 screens
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(led, INPUT);
  //inizializzo comunicazione Bluetooth
      bt.begin(9600);
}
 
void loop() {
  readSensors();
  getVoltageValue();
  String scritta=sendAndroidValues();
  bt.print(scritta);
  //when serial values have been received this will be true
  //FUNZIONA
   while (bt.available() > 0) {
        //mostrali nel Serial Monitor
        inbyte = bt.read();
        Serial.println(inbyte);
        
    if (inbyte == 49)//'1' IN CODICE ASCII
    {
      Serial.println("\t Led ON");
      //LED off
      digitalWrite(led, LOW);
      
    }
    if (inbyte == 48)//'0' IN CODICE ASCII
    {
      Serial.println("\t Led OFF");
      //LED on
      digitalWrite(led, HIGH);
    }
        
   }
  //delay by 2s. Meaning we will be sent values every 2s approx
  //also means that it can take up to 2 seconds to change LED state
  delay(100);
}
 
void readSensors()
{
  // read the analog in value to the sensor array
  sensorValue[0] = sensorValue[0]+10;
  if (sensorValue[0]>1023){
    sensorValue[0]=0;
    }
}
//sends the values from the sensor over serial to BT module
String sendAndroidValues()
 {
  //puts # before the values so our app knows what to do with the data
  String scritta= "#";
  //Serial.print('#');
  //for loop cycles through 4 sensors and sends values via serial
  for(int k=0; k<4; k++)
  {
    scritta = scritta + voltageValue[k];
    scritta = scritta + "+";
    //technically not needed but I prefer to break up data values
    //so they are easier to see when debugging
  }
  scritta = scritta + "~";
  //Serial.println(scritta);
  return scritta;
}
 
void getVoltageValue()
{
    voltageValue[0] = ((sensorValue[0]/1023)*5);
}

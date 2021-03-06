#include <SD.h>         //SD Card Library
#include <SPI.h>           
#include <Wire.h>       //I2C Library
#include <max6675.h>     // max Library

//SPI SD Card Pins
//MOSI = Pin 11
// MISO = Pin 12
// SCLK = PIN 13
  int CS_pin = 10;
  int pow_pin = 8;

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO); //Max6657 board
int vccPin = 3;
int grdPin = 2;


//I2C Temperature Pins
//SDA = Analog Pin 4
//SCL = Analog Pin 5
 
//IR Distance Sensor Pins
//  int IR1_pin = 2;
//  int IR2_pin = 3;
 
//Light Sensor Pins
// int light_pin = 1;

float refresh_rate = 0.0;  //Datalogger Refresh Rate
//int temp_address = 72;     //Address of the I2C Temp Sensor
long id = 1;                //Use this to store the id # of our reading.

void setup()
{
  //Wire.begin();
  Serial.begin(9600);
  Serial.println("Initializing Card");
  //CS Pin is an output
  pinMode(CS_pin, OUTPUT);
 
  //SD Card will Draw Power from Pin 8, so set it high
  pinMode(pow_pin, OUTPUT); 
  digitalWrite(pow_pin, HIGH);
  pinMode(vccPin,OUTPUT); digitalWrite(vccPin, HIGH); //Max Power pin 3
  pinMode(grdPin, OUTPUT); digitalWrite(grdPin, LOW);  // Max Grd pin 2
  //*{Serialprintln("MAX6657 test")  //wait for Max Chip to stablize
  //*delay(500)}
 
  //Initialize Card
  if (!SD.begin(CS_pin))
  {
      Serial.println("Card Failure");
      return;
  }
  Serial.println("Card Ready");
 
  //Read the Configuration information (COMMANDS.txt)
  File commandFile = SD.open("COMMANDS.txt");
  if (commandFile)
  {
    Serial.println("Reading Command File");
   
    float decade = pow(10, (commandFile.available() - 1));
    while(commandFile.available())
    {
      float temp = (commandFile.read() - '0');
      refresh_rate = temp*decade+refresh_rate;
      decade = decade/10;
    }
    Serial.print("Refresh Rate = ");
    Serial.print(refresh_rate);
    Serial.println("ms");
    commandFile.close();
  }
  else
  {
    Serial.println("Could not read command file.");
    return;
  }
 
  //Write Log File Header
  File logFile = SD.open("LOG.csv", FILE_WRITE);
  if (logFile)
  {
    logFile.println(", , , ,"); //Just a leading blank line, incase there was previous data
    String header = " Temp ";
    logFile.println(header);
    logFile.close();
    Serial.println(header);
  }
  else
  {
    Serial.println("Couldn't open log file");
  }
 
}

void loop()
{
  //Check Light Level
// int light_level = analogRead(light_pin);
 
  //Read Temperature
  //Wire.beginTransmission(temp_address); //Start talking
  Wire.write(0); //Ask for Register zero
  Wire.endTransmission(); //Complete Transmission
// Wire.requestFrom(temp_address, 1); //Request 1 Byte
  //while(Wire.available() == 0); //wait for response
   int temp_c = Wire.read(); // Get the temp
  //int temp_f = round(temp_c*9.0/5.0 +32.0);  //Convert to stupid American units
 
  //Read Distances
  //int IR1_val = analogRead(IR1_pin);
  //int IR2_val = analogRead(IR2_pin);
 
  //Create Data string for storing to SD card
  //We will use CSV Format 
  String dataString = String(id) + ", " + String(temp_c);
 
  //Open a file to write to
  //Only one file can be open at a time
  File logFile = SD.open("LOG.csv", FILE_WRITE);
  if (logFile)
  {
    logFile.println(dataString);
    logFile.close();
    Serial.println(dataString);
  }
  else
  {
    Serial.println("Couldn't open log file");
  }
 
  //Increment ID number
  id++;
 
  delay(refresh_rate);
}



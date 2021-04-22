/*
Save LUX readings using the BH1750 in SD/microSD card every 30 seconds...  
UW Stout Spring 2021  
Impacts of Engineering (Group 4).  
by: Chris Thummel
Date Finished: 4/21/2021 
References: 
SD Card Module with Arduino: How to Read/Write Data - Arduino Project Hub 
DIY Light Lux Meter - Arduino Project Hub 
GitHub:  it2515/Lux-Meter-With-SD-Card (github.com) 
*/
#include <SPI.h>
#include <SD.h>
#include<LiquidCrystal.h>
#include<Wire.h>

// Global Variabules

File myFile;
LiquidCrystal lcd (2, 3, 4, 5, 6, 7); 

int data[150];
int BH1750address = 0x23;
byte buff[2];
File file; 

// counters and number variabules  
int z = 0;
int x = 0;
int value;
int count = 0;
int Count1 = 0;
int y = 0;


// sets button state
bool buttonState = LOW;
bool buttonState2 = LOW;

// declaires button pins 
const int BUTTON_PIN = 8;
const int BUTTON_PIN2 = 9;

 // set up 
void setup() {
 
 pinMode(BUTTON_PIN, INPUT);// Sets the button pins as input
 pinMode(BUTTON_PIN2, INPUT);

 Wire.begin();
 lcd.begin(16,2);
 lcd.print("  BH1750 Light  ");
 lcd.setCursor(0,1);
 lcd.print("Intensity Sensor");
 delay(2000);
 lcd.clear();

// Open serial communications and wait for port to open:
 Serial.begin(9600);

 Serial.print("Initializing SD card...");
 if (!SD.begin(10)) {
  Serial.println("initialization failed!");
  while (1);
  }
 Serial.println("initialization done.");
}
 

// Sets up the adressing for the sensor
int BH1750_Read(int address){
int i=0;
Wire.beginTransmission(address);
Wire.requestFrom(address, 2);
while(Wire.available()) 
{
 buff[i] = Wire.read();
 i++;
}
  Wire.endTransmission();  
  return i;
}
void BH1750_Init(int address) {
  Wire.beginTransmission(address);
  Wire.write(0x10);
  Wire.endTransmission();
}

// What goes into the SD card is in this function.
void writeFile()
{
 y = y + 1;
 z = x;
 // Compares the current Value to the highest 
 if (z < value)
 {
  x = value; // stores the highest value. 
 }
 // Stores The data and displays it on the Serial moniter.
 Serial.print(y);
 Serial.print(",");
 Serial.print(value);
 file = SD.open("file.txt", FILE_WRITE); // open "file.txt" 
 if (file) { 
  file.print(y);
  file.print(" , ");
  file.print(value);
  file.print(" \n ");
  file.close(); // close file
  Serial.begin(9600);
  Serial.print(" ");
//Serial.print(value); // debug output: show written number in serial monitor
//Serial.println();  
    
  } 
}


// Main Loop Everything that is not setup is based here...
void loop()
{
 int i;
 buttonState = digitalRead(BUTTON_PIN);
 buttonState2 = digitalRead(BUTTON_PIN2);
 //Serial.println(buttonState);

    
 BH1750_Init(BH1750address);
 delay(250);

 if(2==BH1750_Read(BH1750address)){
  value=((buff[0]<<8)|buff[1])/1.2;// <<< converts value to LUX... (Value) is the Lux reading kinda important 
  lcd.setCursor(0,0);
  lcd.print("Intensity in LUX");
  lcd.setCursor(6,1);
  lcd.print(value);
  lcd.print("   ");
  }
  delay(250);

  // When this counter reaches 60 it will store the current value into the SD card this is so that we don't over whelm the SD card with usless data
  count = count + 1; 
  //Serial.println(count);


// sets the counter for button 1 
 if (buttonState == HIGH) {
    Count1 = Count1 + 1;  
 }else{
      Count1 = 0;// resets counter for button 1
 }

 if ((Count1 < 10) && (Count1 > 0))
 {
   count = 0; // resets count
 }

 // This will pause the data collection by settin the counter higher than the inital value so that it 
 // never reaches 60... To reset this all you need to do is press button one for more than 3 sec
 if ((Count1 > 10) && (Count1< 12)){
  count = 61;
  lcd.begin(16,2);
  lcd.print("Paused Data");
  delay(2000);
  lcd.clear();
  lcd.begin(16,2);
  lcd.print("Hold to Erase");
  delay(2000);
  lcd.clear();
 } 
// this erases the file ALL DATA COLLECTED WILL BE DELETED if this is true count is longer due to delays
 if (Count1 > 12){
  SD.remove("file.txt");
  //Serial.print("deleated");
  Count1 = 0;
  lcd.begin(16,2);
  lcd.print(" File Erased");
  delay(2000);
  lcd.clear();
  y = 0;
 }

// This will display the highest reading 
 if (buttonState2 == HIGH)
 {
  lcd.begin(16,2);
  lcd.print("Highest Reading");
  lcd.setCursor(6,1);
  lcd.print(x);
  delay(2000);
  lcd.clear();
 }
// resets count and then will go to write function
 if (count == 60){ 
  writeFile();
  count = 0;
 }

// Pause Display
 if(count > 61){
   lcd.begin(16,2);
   lcd.print("Collection Paused");
   delay(500);
   lcd.clear(); 
 }


// Easter egg Displays names of group
 if ((buttonState2 == HIGH)&& (buttonState == HIGH)){
  lcd.setCursor(0,0);
  lcd.print("Made By:");
  lcd.setCursor(6,1);
  lcd.print("Christian");
  delay (3000);
  lcd.clear(); 
  
  lcd.setCursor(6,1);
  lcd.print("Patrick");
  delay (3000);
  lcd.clear();
   
  lcd.setCursor(6,1);
  lcd.print("Morgan");
  delay (3000);
  lcd.clear(); 

  lcd.setCursor(6,1);
  lcd.print("Sara");
  delay (3000);
  lcd.clear(); 

  }
 //Serial.print(Count1);
}





  





   

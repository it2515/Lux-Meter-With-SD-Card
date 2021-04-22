/*
Save LUX readings using the BH1750 in SD/microSD card every half 30 seconds 
Made: 4/21/2021
by: Chris Thummel
*/
#include <SPI.h>
#include <SD.h>
#include<LiquidCrystal.h>
#include<Wire.h>

File myFile;
LiquidCrystal lcd (2, 3, 4, 5, 6, 7); 


int data[150];
int BH1750address = 0x23;
byte buff[2];
File file; 


int z = 0;
int x = 0;
int value;
int count = 0;
int Count1 = 0;
int y = 0;

bool buttonState = LOW;
bool buttonState2 = LOW;

const int BUTTON_PIN = 8;
const int BUTTON_PIN2 = 9;

 
void setup() {
 pinMode(BUTTON_PIN, INPUT);
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

void writeFile()
{
 y = y + 1;
 z = x;
 if (z < value)
 {
  x = value;
 }
 
 Serial.print(y);
 Serial.print(",");
 Serial.print(value);
 file = SD.open("file.txt", FILE_WRITE); // open "file.txt" to write data; make sure that you want to write in the same file that you created in the setup()
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
    //you can write as much as you want, just make sure that you have a consistent formatting!
  } 
}



void loop()
{
 int i;
 buttonState = digitalRead(BUTTON_PIN);
 buttonState2 = digitalRead(BUTTON_PIN2);
 Serial.println(buttonState);

    
 BH1750_Init(BH1750address);
 delay(250);

 if(2==BH1750_Read(BH1750address)){
  value=((buff[0]<<8)|buff[1])/1.2;
  lcd.setCursor(0,0);
  lcd.print("Intensity in LUX");
  lcd.setCursor(6,1);
  lcd.print(value);
  lcd.print("   ");
  }
  delay(250);
  count = count + 1;
  //Serial.println(count);

 if (buttonState == HIGH) {
    Count1 = Count1 + 1;  
 }else{
      Count1 = 0;
 }

 if ((Count1 < 10) && (Count1 > 0))
 {
   count = 0; 
 }
 if ((Count1 > 10) && (Count1< 20)){
 
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

 if (Count1 > 12){
   SD.remove("file.txt");
  Serial.print("deleated555555");
  Count1 = 0;
  lcd.begin(16,2);
  lcd.print(" File Erased");
  delay(2000);
  lcd.clear();
  y = 0;
 }


 if (buttonState2 == HIGH)
 {
  lcd.begin(16,2);
  lcd.print("Highest Reading");
  lcd.setCursor(6,1);
  lcd.print(x);
  delay(2000);
  lcd.clear();
 }

 if (count == 4){ 
  writeFile();
  count = 0;
 }

 if(count > 61){
   lcd.begin(16,2);
   lcd.print("Collection Paused");
   delay(500);
   lcd.clear(); 
 }

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
 Serial.print(Count1);
}






  





   

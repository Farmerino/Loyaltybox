/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <uRTCLib.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 13, en = 12, d4 = 14, d5 = 15, d6 = 16, d7 = 17;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 5, 4, 3, 2 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 8, 9, 10 }; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define beeppin 11

SoftwareSerial RFID(6, 7); // RX and TX

int i = 0;
long previousmillis = 0;
byte shorttimer = 100;
word midtimer = 5000;
long longtimer = 15000;
unsigned int enteredvalue = 0;
boolean setupmode = false;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.setCursor(4,0);
  lcd.print("LOYALTY BOX");
  
  pinMode(beeppin,OUTPUT);
  digitalWrite(beeppin, LOW);
  
  RFID.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);

  unsigned long currentmillis = millis();
  if (currentmillis - previousmillis > shorttimer)
  {
    digitalWrite(beeppin,LOW);
  }
  if (currentmillis - previousmillis > midtimer)
  {
    lcd.setCursor(10,2);
    lcd.print("     ");
    enteredvalue = 0;
  }

  
  char key = kpd.getKey();


  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      case '*':
        digitalWrite(beeppin, HIGH);
        previousmillis = currentmillis;
        if (enteredvalue == 0 && setupmode == true)
        {
          setupmode = false;
          lcd.setCursor(9,2);
          lcd.print(" ");
          break;
        }
        else if (enteredvalue == 0 && setupmode == false)
        {
          setupmode = true;
          lcd.setCursor(9,2);
          lcd.print("*");
          break;
        }
        else 
        {
        enteredvalue = enteredvalue / 10;
        lcd.setCursor(10,2);
        lcd.print("     ");
        lcd.setCursor(10,2);
        lcd.print(enteredvalue);
        break;
        }
        
        
      case '#':
        digitalWrite(beeppin, HIGH);
        previousmillis = currentmillis;
        if (setupmode == true)
        {
          switch (enteredvalue)
          {
            case 65432:             //set time
            break;
    
            case 61234:             //do something else
            digitalWrite(beeppin, HIGH);
            delay(1000);
            digitalWrite(beeppin, LOW);
            break;
          }
          setupmode = false;
          lcd.setCursor(9,2);
          lcd.print("      ");
          enteredvalue = 0;
          break;
        }
        else
        {
          lcd.setCursor(10,2);
          lcd.print("OK   ");
          enteredvalue = 0;
          break;
        }
        
      default:
        lcd.setCursor(10, 2);
        if (enteredvalue < 6553)
          {
            enteredvalue = enteredvalue * 10 + (key - '0');
          }
          if (enteredvalue < 10) //to erase the "K" from "OK", because it is ugly
          {
            lcd.print("  ");     //if left unerased
            lcd.setCursor(10,2); //reposition the cursor before printing
          }
        lcd.print(enteredvalue);
        previousmillis = currentmillis;
    }
  

  if (RFID.available() > 0) 
  {
     i = RFID.read();
     Serial.print(i, DEC);
     Serial.print(" ");
  }
    }
  }




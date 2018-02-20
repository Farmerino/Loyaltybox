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

uRTCLib rtc(0x68, 0x57);

int i = 0;
long previousmillis = 0;
byte shorttimer = 100;
word midtimer = 8000;
long longtimer = 15000;
long enteredvalue = 0;
boolean syscommandmode = false;
boolean datetimeenter = false;


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.setCursor(4,0);
  lcd.print("LOYALTY BOX");
  
  pinMode(beeppin,OUTPUT);
  digitalWrite(beeppin, LOW);
  Wire.begin();
  RFID.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
}

void loop() {
  
  unsigned long currentmillis = millis();
  if (currentmillis - previousmillis > shorttimer)
  {
    digitalWrite(beeppin,LOW);
  }
  if (currentmillis - previousmillis > midtimer && datetimeenter == false)
  {
    lcd.setCursor(10,2);
    lcd.print("     ");
    enteredvalue = 0;
  }

  
  char key = kpd.getKey();
  byte day,month,year,hour,minute,sec;

  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      case '*':
        digitalWrite(beeppin, HIGH);
        previousmillis = currentmillis;
        if (enteredvalue == 0 && syscommandmode == true && datetimeenter == false)
        {
          syscommandmode = false;
          lcd.setCursor(9,2);
          lcd.print(" ");
          break;
        }
        else if (enteredvalue == 0 && syscommandmode == false && datetimeenter == false)
        {
          syscommandmode = true;
          lcd.setCursor(9,2);
          lcd.print("*");
          break;
        }
        if (enteredvalue > 0 && datetimeenter == false)
        {
          enteredvalue = enteredvalue / 10;
          lcd.setCursor(10,2);
          lcd.print("     ");
          lcd.setCursor(10,2);
          lcd.print(enteredvalue);
          break;
        }
        else if (enteredvalue > 0 && datetimeenter == true)
        {
          enteredvalue = enteredvalue / 10;
          lcd.setCursor(1,3);
          lcd.print("            ");
          lcd.setCursor(1,3);
          lcd.print(enteredvalue);
          break;
        }
        
      case '#':
        digitalWrite(beeppin, HIGH);
        previousmillis = currentmillis;
        if (syscommandmode == true && datetimeenter == false)
        {
          switch (enteredvalue)
          {
            case 65432:             //set time
            datetimeenter = true;
            enteredvalue = 0;
            rtc.refresh();
            
            
    /*        Serial.print("RTC DateTime: ");
            Serial.print(rtc.year());
            Serial.print('/');
            Serial.print(rtc.month());
            Serial.print('/');
            Serial.print(rtc.day());
            Serial.print(' ');
            Serial.print(rtc.hour());
            Serial.print(':');
            Serial.print(rtc.minute());
            Serial.print(':');
            Serial.print(rtc.second());
            Serial.println(); 
    */    
            day = rtc.day();
            month = rtc.month();
            year = rtc.year();
            hour = rtc.hour();
            minute = rtc.minute();
            sec = rtc.second();

            lcd.clear();
            lcd.setCursor(1, 0);
            lcd.print("Set DATE and TIME");
            lcd.setCursor(1,1);
            lcd.print("YY/MM/DD/hh/mm/ss");
            lcd.setCursor(1,2);
            if (year < 10)
            {
              lcd.print("0");
              lcd.print(year);
            }
            else
            {
              lcd.print(year);
            }
            if (month < 10)
            {
              lcd.print("0");
              lcd.print(month);
            }
            else
            {
              lcd.print(month);
            }
            if (day < 10)
            {
              lcd.print("0");
              lcd.print(day);
            }
            else
            {
              lcd.print(day);
            }
            if (hour < 10)
            {
              lcd.print("0");
              lcd.print(hour);
            }
            else
            {
              lcd.print(hour);
            }
            if (minute < 10)
            {
              lcd.print("0");
              lcd.print(minute);
            }
            else
            {
              lcd.print(minute);
            }
            if (sec < 10)
            {
              lcd.print("0");
              lcd.print(sec);
            }
            lcd.setCursor(1,3);
            
            break;
    
            case 61234:             //do something else
            digitalWrite(beeppin, HIGH);
            delay(1000);
            digitalWrite(beeppin, LOW);
            break;
          }
          if (datetimeenter == false)
          {
            syscommandmode = false;   //undefined syscommand received
            lcd.setCursor(9,2);       //deleting it, clearing this field of the lcd
            lcd.print("      ");
            enteredvalue = 0;
            break;
          }
          else
          {
            datetimeenter = false;
            sec = enteredvalue % 100;
            enteredvalue = enteredvalue / 100;
            minute = enteredvalue % 100;
            enteredvalue = enteredvalue / 100;
            hour = enteredvalue % 100;
            enteredvalue = enteredvalue / 100;
            day = enteredvalue % 100;
            enteredvalue = enteredvalue / 100;
            month = enteredvalue % 100;
            enteredvalue = enteredvalue / 100;
            year = enteredvalue;
            if (day < 32 && day > 0 && month < 13 && month >0 && year > 17 && sec < 60 && minute < 60 && hour < 24 )
            {
              rtc.set(sec, minute, hour, 1, day, month, year);
              //  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
            }
            break;
          }
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
        if (datetimeenter == false)     // we are entering a price
        {
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
        }
        else        // we are entering a yy/mm/dd/hh/mm/ss value
        {
          enteredvalue = enteredvalue * 10 + (key - '0');
          lcd.print(enteredvalue);
        }
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




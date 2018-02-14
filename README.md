# Loyaltybox
Arduino-based gadget to track customers and reward them for buying my vegetables.

Background:
I am a farmer who try to make ends meet by producing different vegetables and selling them on the local and nearby town's markets.
Selling directly to end-users (mostly housewifes) is the best. I would like to build a loyal customer base to make my life more predictable because the weather, the pests and everyday life provide me more than enough uncertainty.

I like tinkering with electronics and I thought I'd put together a small electronic "loyaltybox" to help me achieving my goal!
Putting together the hardware was not too hard - thanks to the inventors of Arduino and many chinese module-makers. The hardware is on breadboard now and has the following parts:

- Arduino Uno R3
- 4x20 LCD module
- 3x4 numeric keypad
- RFID reader module
- Real-time clock module
- 64kB EEPROM
- a piezo "beeper".

Expected behaviour:
I envision giving away nice RFID plastic cards to my customers who want to have some fun. Then every time they visit me at the market and buy something I would key in the end price of purchase via the keypad before they paid. Then they would "beep" their cards on the loyaltybox. The box would read the identifier of the card and based on their previous purchases (total/average money spent, day of last purchase, frequency of previous purchases, etc.) would calculate a personal loyalty discount. The keyed in price, the discount and the calculated final fee would then be displayed on the LCD.

And everyone would be happy: they would spend less and less the more they used my services. I would get more and more recurrent customers because the buzz of this new gadget would spread among local people like wildfire. (Well, not exactly like that but would eventually reach everyone who visit the farmer's market.)

The challenge:
Writing the software what makes the whole show spinning. :-)
The hardware is ready to take its share but my programming skills are very limited. I have some AVR assembly background but I would rather develop the code in Arduino C in this case. I need help with the code writing and would appreciate any help I could get.

Hardware description / pinout:

4x20 LCD module:
- RS: Arduino pin 13
- RW: Ground
- E: Arduino pin 12
- D4-D7: Arduino pins A0 - A3

3x4 numeric keypad:
- 3 columns to Arduino pins 8,9,10
- 4 rows to Arduino pins 5,4,3,2

RFID reader (type RDM6300 125kHz reader):
- TX to Arduino pin 6
- RX is floating (but in software is mapped to Arduino pin 7 due to SoftwareSerial requirements)

Real time clock (DS3231 module with 32kbit EEPROM and 200 Ohm resistor removed to save the CR 2032 button cell):
- I2C clock chip read address: 0xD0
- I2C clock chip write address: 0xD1
- I2C 32kb EEPROM read address: 0xAE
- I2C 32kb EEPROM write address: 0xAF

64kByte EEPROM (24LC512):
- I2C read address: 0xA8
- I2C write address: 0xA9

Beeper (fix frequency beeper, so no tone() is required/can be used):
- Arduino pin 11

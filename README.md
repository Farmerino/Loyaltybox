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

Software layout (crude version):

Setup() should do what setup() does best: variable definition/initialization. After it's done it should read and display the actual date and time from the clock module and ask the user whether he/she would like to setup/correct the time or not. Setup() should wait for operator answer (keying say # on the keypad) for a few secs then assume that the date/time is OK if no keypress was detected during this time. Then it should refresh the LCD with a greeting or something and allow program flow to enter the main loop(). In case the "#" key was pressed then should call the clock setup routine and after the setup was completed should enter the main loop().

The loop() should check the RFID module and the keys.

If the RFID module would output a valid card ID without a previously keyed-in end price (say "1234#") then it should look this ID up in the database (external EEPROM). If there was a match (the card is already known by the system) then it should display a few useful things associated to this particular ID. E.g.: the ID of the card, the date it was last seen, etc. but otherwise do nothing.)

If the card ID was not found in the database then only display this ID.

If a price was entered (keyed-in say "12345#" where "#"after the digits means "end of input") it should be converted to unsigned integer (without the "#" of course, holding capacity of an uint should be plenty, unlikely I would sell more than 65535 or more bucks worth of vegetables at a time to any of my customers). After this conversion there is a time window open say for 10-15 secs while the system waits for the customer to get her/his card read by the RFID module. If it time-outs and no card had been read then the display should be cleared and the loop() should resume normal operation.

If a tag was read its ID should be matched against the database. If a match found: known card, recurrent customer, calculate the oh-so-awaited discount, display it on the LCD, refresh the database. After a few more secs clear the display and show the default greetings and wait for a new customer.

If a match was not found then this is a first time use of the given loyalty card. Give the customer the first-time-use bonus (if there's any) then refresh the database, etc.


If I make a typo in entering the end price there should be a mechanisms for correcting the entered value. Pressing "*" after a digit should mean "backspace". Repeated pressing should delete another digit, and so on, until there's nothing left.

Starting the keying with "*" should mean "entering special system command". Such a command should have a general syntax as "*1234#", where 1234 is an arbitrary four digit code for a command and # is the end of input char.

Such a system command could be used for updating the clock at anytime or initiate a database dump via the USB cable, or something even more useful act.


/*
   Stubo's RTC Clockduino 
   Copyright (c) 2015 Stuart W. MacDonald
   based on Doomsday clock example - Ticking HH:MM:SS
   Copyright (c) 2012 Devon Sean McCullough

Features:

* Includes RTC code for Maxim DS3231 that is used on Samari Circutis "MAMA'S EXTREMELY ACCURATE REAL TIME CLOCK" module that uses a Maxim DS3231 RTC chip datasheet is here: http://datasheets.maximintegrated.com/en/ds/DS3231.pdf
* Every Minute it displays the date
* Every Minute it displays the ambiant temp of the DS3231 chip temp sensor
* Cool Cylone LED's to get your attention
* On the hour it "bongs" the current hour in 12 hour format 
* 




   This example is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.

   See file LICENSE.txt for further informations on licensing terms.
*/

#include <Doomsday.h>
#include <Time.h>  
#include <Wire.h>  
#include <DS3232RTC.h>        // RTC Library for DS3231/3232 http://github.com/JChristensen/DS3232RTC


Doomsday doom;
int  buzzPin1 = A1; // This is the pin for the Buzzer
        // Set old information to absurd values to trigger new second/minute/hour/day actions
  int oldHour = 255;      // These three are set to absurd values to trigger
  int oldMinute = 255;    // the change-sensitive bits in loop().
  int oldSecond = 255;    // 
  int oldDay = 255;       //
int dots =0; 

void setup ()
{

  Wire.begin();  //initialize the I2C library
  doom.begin (72);   // 72 ms ~~ 14 Hz
  pinMode (buzzPin1, OUTPUT); //sets the digital pin in variable buzzPin1 as output
  Serial.begin(9600);
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  setSyncInterval(300);       // set the number of seconds between re-sync 300 sec = 5 mins 
  
  while (!Serial) ; // wait until Arduino Serial Monitor opens
  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");  
  digitalClockDisplay();  //serial print the time
  doom.setClock (second() , minute(), hour(), day(), month(), year());  
}

void loop ()
{

  int bongs =0;
  doom.secondsTick ();
    doom.printd ("%H%(:%)%M%(:%)%S");  // print time ":" dim

  // These are the things that happen every minute:
    if (minute() != oldMinute) {
        oldMinute = minute();
        digitalClockDisplay();  // prints time to serial
        cylonMonth();     //prints the date with Cylon dots cycle
        cylonTemp();
        // float temp_C =  RTC.temperature() / 4.;  //gets the temp from the rtc chip DS3231
        // float temp_F = temp_C *9. / 5. + 32. ;
              
    }
 
  // These are the hourly events:
    if (hour() != oldHour) {
        oldHour = hour(); bongs=0;
        RTC.get();  //refresh time from RTC
        doom.setClock (second() , minute(), hour(), day(), month(), year());  // refresh the ardunio doom clock with the RTC which is synced every 5 mins. 
        // play tone every hour for the number of bongs
        if (hour() <=12){   //for hours 12 and below
            bongs = hour();
            playHRbongs(bongs);
        } 
        // play tone every hour for the number of bongs
        if (hour() >12){    //only play up to 12 bongs for AM or PM
            //bongs = convert24to12hour(hour());
            bongs = hour() - 12;
            playHRbongs(bongs);
            }
     }

     
     // These are the daily events:
    if (day() != oldDay) {
        oldDay = day(); 
          setSyncProvider(RTC.get);   // the function to get the time from the RTC 
    }
    
}

void digitalClockDisplay(void)
{
    // digital clock display of the time
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(' ');
    Serial.print(day());
    Serial.print(' ');
    Serial.print(month());
    Serial.print(' ');
    Serial.print(year()); 
    Serial.println(); 
}


void printDigits(int digits){
  // utility function for serial digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

    
void cylonMonth(){
          //prints the date with Cylon dots cycle
        doom.printd ("%m,%d,%y", dots); 
        dots = (256 * B00000000) + B00000001;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00000010;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00000100;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00001000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00010000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00100000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B01000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B10000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000001) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000010) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000100) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00001000) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000100) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000010) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000001) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B10000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B01000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00100000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00010000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00001000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00000100;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00000010;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00000001;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00000010;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00000100;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00001000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00010000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00100000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B01000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B10000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000001) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000010) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000100) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00001000) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000100) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000010) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000001) + B00000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B10000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B01000000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00100000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00010000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00001000;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00000100;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00000010;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00000001;  doom.printd ("%m,%d,%y", dots);
        dots = (256 * B00000000) + B00000000;  doom.printd ("%m,%d,%y", dots);
}

void playHRbongs(int dings){
     while (dings >=1){
        dots = (256 * B00001000) + B00001000;
        doom.begin (600);
        tone(buzzPin1, 150, 100); delay(100); doom.printd ("%H:%M", dots);
        dings--;
        }
     doom.begin (72);
}

void cylonTemp(){
          //prints the temperature with Cylon dots cycle
          // doom.printd only excepts a 7 char text to display so you have to be creative to parse the data you want to display
          // into a displayable char variable.
          
        float temp_C =  RTC.temperature() / 4.;  //gets the temp from the rtc chip DS3231
        float temp_F = temp_C *9. / 5. + 32. ;  
        Serial.print(temp_C); Serial.println(" degrees C");
        Serial.print(temp_F); Serial.println(" degrees F");
      
        char stemp_f[5];
        char junk[5] = "\033F" ;  // \033 is the degree symbol
        char otemp_f[20];
        dtostrf(temp_F,4,1, stemp_f);  // float to char conversion
        otemp_f[0] ='\0';
        strcat(otemp_f, stemp_f);  // concat the two char arrays
        strcat(otemp_f,junk);

        dots = (256 * B00000000) + B00000000;  doom.printd (otemp_f, dots);
        dots = (256 * B00000000) + B00000000;  doom.printd (otemp_f, dots);
        dots = (256 * B00001000) + B00000001;  doom.printd (otemp_f, dots);
        dots = (256 * B00001000) + B00000001;  doom.printd (otemp_f, dots);
        dots = (256 * B00000100) + B00000010;  doom.printd (otemp_f, dots);
        dots = (256 * B00000010) + B00000100;  doom.printd (otemp_f, dots);
        dots = (256 * B00000001) + B00001000;  doom.printd (otemp_f, dots);
        dots = (256 * B00000000) + B10010000;  doom.printd (otemp_f, dots);
        dots = (256 * B00000000) + B01100000;  doom.printd (otemp_f, dots);
        //dots = (256 * B00000000) + B01100000;  doom.printd (otemp_f, dots);
        dots = (256 * B00000000) + B10010000;  doom.printd (otemp_f, dots);
        dots = (256 * B00000001) + B00001000;  doom.printd (otemp_f, dots);
        dots = (256 * B00000010) + B00000100;  doom.printd (otemp_f, dots);
        dots = (256 * B00000100) + B00000010;  doom.printd (otemp_f, dots);       
        dots = (256 * B00001000) + B00000001;  doom.printd (otemp_f, dots);
        dots = (256 * B00001000) + B00000001;  doom.printd (otemp_f, dots);
        dots = (256 * B00000100) + B00000010;  doom.printd (otemp_f, dots);
        dots = (256 * B00000010) + B00000100;  doom.printd (otemp_f, dots);
        dots = (256 * B00000001) + B00001000;  doom.printd (otemp_f, dots);
        dots = (256 * B00000000) + B10010000;  doom.printd (otemp_f, dots);
        dots = (256 * B00000000) + B01100000;  doom.printd (otemp_f, dots);
        //dots = (256 * B00000000) + B01100000;  doom.printd (otemp_f, dots);
        dots = (256 * B00000000) + B10010000;  doom.printd (otemp_f, dots);
        dots = (256 * B00000001) + B00001000;  doom.printd (otemp_f, dots);
        dots = (256 * B00000010) + B00000100;  doom.printd (otemp_f, dots);
        dots = (256 * B00000100) + B00000010;  doom.printd (otemp_f, dots);       
        dots = (256 * B00001000) + B00000001;  doom.printd (otemp_f, dots);        
        dots = (256 * B00001000) + B00000001;  doom.printd (otemp_f, dots);
        dots = (256 * B00000000) + B00000000;  doom.printd (otemp_f, dots);
        dots = (256 * B00000000) + B00000000;  doom.printd (otemp_f, dots);
 
        
}


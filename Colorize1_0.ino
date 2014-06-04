// v. 1.0 21.5.2014
//////////////////////////////////////////////////////////
/* Teensyduino++ 2.0 project
Colorize - A game where you try to match the color of two RGB leds
by adjusting the red, green and blue values with three potentiometers.
When colors match, a sound is played and new color randimized.

Created 21.5.2014 @ Aalto University Electronics Crash Course session
Team Colorize: Ville Kukko-Liedes, Simo Lahdenne, Jooel Friman
Contact: ville.kukko-liedes (a) aalto.fi
*/
/////////////////////////////////////////////////////////
byte debug = 1;


#define analogR A0 // pins for input potentiometers
#define analogG A1
#define analogB A2

#define masterR 14 // PWM pins for the led to match ("master")
#define masterG 15
#define masterB 16

#define slaveR 27 // PWM pins for the led that is adjusted ("slave")
#define slaveG 0
#define slaveB 1

#define buzzPin 26 // Pin for the piezo buzzer

#define MINCOLOR 20 // Minimun value for an individual color
#define TRESHOLD 20 // Value of allowed difference in color (+/-)

byte r = 0; // values sent to the master led
byte g = 0;
byte b = 0;

byte rIn = 0; // values read from the potentiometers and sent to the slave led
byte gIn = 0;
byte bIn = 0;


//used for serial communication:
unsigned long time = 0; // used to keep track of time
  unsigned long serialTime = 0; // last time serial info sent
  int serialDelay = 500; // interval of serial updates
  


void setup(){
  Serial.begin(9600);
  randomSeed(analogRead(A7)); // this helps to truly randomize values
  analogWrite(buzzPin, 0);
  randomize(); // randomize initial values for the master led
}

void loop(){
  
  getRGB(); // read potentiometers
  updateLeds(); // send new values to both leds
  
  if(((rIn - r)-(gIn - g)) < TRESHOLD && ((rIn - r)-(gIn - g)) > -TRESHOLD){ // check if colors match == Point!
    if(((rIn - r)-(bIn - b)) < TRESHOLD && ((rIn - r)-(bIn - b)) > -TRESHOLD){
      if(((gIn - g)-(bIn - b)) < TRESHOLD && ((gIn - g)-(bIn - b)) > -TRESHOLD){
        win();
      }
    }
  }
  if(debug){
  delay(10); // some delay because no need to run the loop too fast
  
   // Serial communication to computer used for debugging and calibration:
  time = millis(); // current time in milliseconds since start
  if((time - serialTime) > serialDelay){ // check if more than min interval lasted since last update
    serialTime = millis(); // reset timer and sen data
    Serial.print("R: ");
    Serial.print(rIn - r);
    Serial.print(" G: ");
    Serial.print(gIn - g);
    Serial.print(" B: ");
    Serial.println(bIn - b);
  }
  }
  
}


void randomize(){ // create new values for the master led
  r = random(MINCOLOR, 256);
  g = random(MINCOLOR, 256);
  b = random(MINCOLOR, 256);
  // check that new color isn't too close to the previous:
  if(((rIn - r)-(gIn - g)) < 2*TRESHOLD && ((rIn - r)-(gIn - g)) > -2*TRESHOLD){
    if(((rIn - r)-(bIn - b)) < 2*TRESHOLD && ((rIn - r)-(bIn - b)) > -2*TRESHOLD){
      if(((gIn - g)-(bIn - b)) < 2*TRESHOLD && ((gIn - g)-(bIn - b)) > -2*TRESHOLD){
        randomize();
      }
    }
  }
}

void getRGB(){ // read potentiometers potition and remap to values approprite for the leds
  rIn = map(analogRead(analogR), 0, 1024, MINCOLOR, 255);
  gIn = map(analogRead(analogG), 0, 1024, MINCOLOR, 255);
  bIn = map(analogRead(analogB), 0, 1024, MINCOLOR, 255);
}

void updateLeds(){ // update led pins
  analogWrite(masterR, (255-r));
  analogWrite(masterG, (255-g));
  analogWrite(masterB, (255-b));
  
  analogWrite(slaveR, (255-rIn));
  analogWrite(slaveG, (255-gIn));
  analogWrite(slaveB, (255-bIn));
}

void buzz(){ // make some noise
  analogWrite(buzzPin, 150);
  delay(200); // duration of soubd
  digitalWrite(buzzPin, 0);
}

void shutAll(){
  analogWrite(masterR, 255);
  analogWrite(masterG, 255);
  analogWrite(masterB, 255);
  
  analogWrite(slaveR, 255);
  analogWrite(slaveG, 255);
  analogWrite(slaveB, 255);
}

void win(){
  int t = 110;
  buzz(); // make a sound
  shutAll();
  delay(t);
  updateLeds();
  delay(2*t);
  shutAll();
  delay(t);
  updateLeds();
  delay(2*t);
  shutAll();
  delay(t);
  updateLeds();
  delay(2*t);
    
  randomize(); // create new color

}

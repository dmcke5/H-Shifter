#include "HID-Project.h"

//Shifter Variables

int gearUp = 2; //Pin 2 for top row of gears
int gearDn = 3; //Pin 3 for bottom row of gears

//Change these values to match the values shown in debug for each gate.
int gateR = 222;
int gate12 = 183;
int gate34 = 145;
int gate56 = 106;

int gateDeadband = 10; //Adjust higher if analog signal is too noisey

int gateSelected;

int currentGear = 0;

//Handbrake Variables

int handbrakePin = A1; //Pin to connect Handbrake Load Cell

int handbrakeReading; 

int handbrakeDigitalThreshold = 100; //Adjust value to control when digital button is triggered.

int handbrakeUpperLimit = 300; //Used for scaling handbrake output. Increase if handbrake output wraps back around to 0 at full load.

bool digitalOutput = false; //Enables handbrake digital output for games that don't support analog.

bool debug = false; //Set to true for testing. Adds 100ms delay to every CPU cycle so don't leave running!

void setup() {
  if(debug){
    Serial.begin(9600);
  }
  pinMode(gearUp, INPUT_PULLUP);
  pinMode(gearDn, INPUT_PULLUP);

  Gamepad.begin();
}

void loop() {
  gateSelected = analogRead(A3); //Read Potentiometer
  handbrakeReading = analogRead(handbrakePin);

  if(!digitalRead(gearUp)){ //Reverse,First,Third or fifth selected
    if(gateSelected < gate56 + gateDeadband){
      Gamepad.press(5);
      currentGear = 5;
    } else if(gateSelected < gate34 + gateDeadband && gateSelected > gate34 - gateDeadband){
      Gamepad.press(3);
      currentGear = 3;
    } else if(gateSelected < gate12 + gateDeadband && gateSelected > gate12 - gateDeadband){
      Gamepad.press(1);
      currentGear = 1;
    } else if(gateSelected < gateR + gateDeadband && gateSelected > gateR - gateDeadband){
      Gamepad.press(7);
      currentGear = 7;
    }
  } else if(!digitalRead(gearDn)){ //Second, fourth or sixth selected
    if(gateSelected < gate56 + gateDeadband){
      Gamepad.press(6);
      currentGear = 6;
    } else if(gateSelected < gate34 + gateDeadband && gateSelected > gate34 - gateDeadband){
      Gamepad.press(4);
      currentGear = 4;
    } else if(gateSelected < gate12 + gateDeadband && gateSelected > gate12 - gateDeadband){
      Gamepad.press(2);
      currentGear = 2;
    } 
  } else {
    Gamepad.releaseAll();
  }

  if(digitalOutput){
    if(handbrakeReading > handbrakeDigitalThreshold){
      Gamepad.press(8);
    } else {
      Gamepad.release(8);
    }
  }
  Gamepad.zAxis(map(handbrakeReading, 0, handbrakeUpperLimit, -127, 128));

  if(debug){
    Serial.print("Current Potentiometer Value: ");
    Serial.println(gateSelected);
    Serial.print("Gear Selected: ");
    Serial.println(currentGear);
    Serial.print("Handbrake Reading: ");
    Serial.println(handbrakeReading);

    delay(100);
  }

  Gamepad.write();
}

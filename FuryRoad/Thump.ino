/*
 * This is the bleeding edge version of our robot navigation code.
 * So naturally a lot of it is not tested and sometimes coded entirely away
 * from our robot, Yamaraz.
 * 
 * This file is free to copy, modify, and redistribute under the
 * terms specified in the COPYING file (its MIT).
 * 
 * Use at your own discretion.
 * Don't come to me if your robot is on fire!
 * 
 * Sincerely,
 * Nishan Rayamajhee
 */
 
// the flags representing solid, wired and hollow state.
const int SOLID = 0; // represented by green
const int WIRED = 1;                // red
const int HOLLOW = 2;               // blue

// thumper pinout
const int thumper = 29;

// microphonw pinour
// note this is a analog pin
const int micro = 8;

// thumper threashold
const int posThresh = 550;
const int negThresh = 400;



// get thumper detection

int thump() {
  int counter = 0;
  int arrayIndex = 20000;
  int getData[arrayIndex];

  digitalWrite(thumper, HIGH);        //hit
  delay(25);                          //wait for the thumper to be close to the ground
  for (int i = 0; i < arrayIndex; i++) {
    getData[i] = analogRead(8);       //sample data
    if (getData[i] > posThresh|| getData[i] < negThresh)         //if the sampled data is greater than threshold increase counter
    counter++;
  }
  delay(100);
  
  digitalWrite(thumper,LOW);     //take it up
  delay(500);
  Serial.print("Counter = ");
  Serial.println(counter);          //Display counter. It is different for hollow and foam

  if (counter < 2000) {
    
    Serial.println("Its solid babe!");
    return SOLID;
    
  } else if (counter < 2500) {
    
     Serial.println("Don't kill me!");
     return WIRED;
     
  } else {
    
     Serial.println("It smells fishy in here!");
     return HOLLOW;
  }
}

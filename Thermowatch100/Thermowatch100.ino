/*
Thermowatch

Control an H-bridge to drive a peltier hot or cold. 
Allows user input on 3 buttons.
Gives feedback through 5 LEDs
*/

const int led0 = 5;     // Coldest LED
const int led1 = 6;
const int led2 = 9;     // Neutral LED
const int led3 = 10;
const int led4 = 11;     // Warmest LED

const int btndown = 7;  // Down button
const int btnmid = 8;   // Neutral button
const int btnup = 12;    // Up button

const int drive = 4;    // Pin that drives H-bridge
const int enable = 3;   // Pin that turns H-bridge on/off

const long debouncedelay = 50;  // Debounce threshold in ms
long lastpress = 0;     // holds last button press time
int peltier = 0;        // -1 is cold, 0 is off, 1 is hot
int strength = 25;       // PWM strength for drive signal
int led0_value;
int led1_value;
int led2_value;
int led3_value;
int led4_value;


boolean validpress = true;  // for valid button presses

// the setup routine runs once when you press reset
void setup()  {
  //declare pins appropriately
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(btndown, INPUT);
  pinMode(btnmid, INPUT);
  pinMode(btnup, INPUT);
  pinMode(enable, OUTPUT);
  pinMode(drive, OUTPUT);
  
  //set initial levels
}


// the loop routine runs in perpetuity
void loop() {
  //Respond to  button press and update LED feedback
  
  // Check button presses
  int btnup_state = digitalRead(btnup);
  int btnmid_state = digitalRead(btnmid);
  int btndown_state = digitalRead(btndown);
  
 /*  
  // Is  button press valid? Only allow button presses every so often
  if ((millis() - debouncedelay) > lastpress) {
    validpress = true;
  }
  else {
     validpress = false;
  }
  */
  
  if (validpress == true) {
     // Ready to receive button press, Check button states
     if (btnmid_state == HIGH) {
     // Handle Mid button press
     // Return system to neutral or off state
       strength = 0;
       digitalWrite(enable, LOW);
       peltier = 0;
     }
     else if (btnup_state == HIGH) {
     // Handle Up button press
       if (peltier == 1) {
         // If already in Hot state, increase the power!
         if (strength < 250) {
             strength = strength + 5;
         }
         else {
             strength = 255;
         }
       }
       else {
         // Not in Hot state, decrease power
         if (strength > 5) {
           strength = strength - 5;
         }
         else {
           strength = 0;
           peltier = 1;
         }
       }
     }
      else if (btndown_state == HIGH) {
        // Down button pressed
        if (peltier == -1) {
          // Already in cold state, increase power!
          if (strength < 250) {
            strength = strength + 5;
          }
          else {
             strength = 255; 
          }
        }
        else {
          // Not in cold state, decrease power or flip states
          if (strength > 5) {
           strength = strength - 5;
          }
         else {
          strength = 0;
          peltier = -1;
         } 
        }
      }     
     }
     
  
  // Update LED values    
  if (peltier == 1) {
   // Hot direction
     led2_value = 255;
     led0_value = 0;
     led1_value = 0;
     
     if (strength > 127) {
       led3_value = 255; 
       led4_value = (strength - 128)*2;
     }
     else {
       led3_value = strength * 2;
       led4_value = 0;
     }
  }
  else if (peltier == -1){
    // Cold direction
     led2_value = 255;
     led3_value = 0;
     led4_value = 0;
     
     if (strength > 127) {
       led1_value = 255; 
       led0_value = (strength - 128)*2;
     }
     else {
       led1_value = strength * 2;
       led0_value = 0;
     }
  }
  else {
    led0_value = 0;
    led1_value = 0;
    led2_value = 125;
    led3_value = 0;
    led4_value = 0;    
  }
  // update system
  analogWrite(led0, 255 - led0_value);
  analogWrite(led1, 255 - led1_value);
  analogWrite(led2, 255 - led2_value);
  analogWrite(led3, 255 - led3_value);
  analogWrite(led4, 255 - led4_value);
  analogWrite(enable, strength);
  if (peltier == 1) {
    digitalWrite(drive, HIGH);
  }
  else {
    digitalWrite(drive, LOW);
  }   

delay(100);

}

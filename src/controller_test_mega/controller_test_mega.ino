//

/*
  Scroll
  - Brake
  - Accel to Target
  - Hold Target for X ms
  - Brake

  Swipe
  - Accel to Target
  - Hold Target for X ms
  - Coast

  Pause
  - Brake
  - Hold Stopped for X ms

*/

// Number of motors
#define CHANNELS 7

// Actions
#define LOOK  0
#define SCROLL 1
#define SWIPE 2

// Action States
#define ACCEL  0
#define HOLD 1
#define DECEL 2
#define DONE 3

// Motor states
#define BRAKE 0x11
#define FORWARD 0x01
#define REVERSE 0x10
#define COAST 0x00

// PWM pins used for controlling motors
int PWM_PINS[] = {
  2,
  3,
  4,
  5,
  6,
  7,
  8
};

// Pins connected to INA1 on motor driver boards
int INA1_PINS[] = {
  22,
  24,
  26,
  28,
  30,
  32,
  34
};

// Pins connected to INA2 on motor driver boards
int INA2_PINS[] = {
  23,
  25,
  27,
  29,
  31,
  33,
  35
};

// Used for setting refresh interval
long current_time = 0;
int refresh_interval = 33; //ms => roughly 30 times a second
long last_refresh = 0;

// variables used for motor control
float drag[] = {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25};
int target_speed[] = {0, 0, 0, 0, 0, 0, 0};
int motor_speed[] = {0, 0, 0, 0, 0, 0, 0};
int motor_direction[] = {1, 1, 1, 1, 1, 1, 1};
int current_direction[] = {0, 0, 0, 0, 0, 0, 0};
int last_direction[] = {0, 0, 0, 0, 0, 0, 0};

// variables used for actions
boolean action_is_complete[] = {true, true, true, true, true, true, true};
int action_state[] = {0, 0, 0, 0, 0, 0, 0};
int current_action[] = {0, 0, 0, 0, 0, 0, 0};
int last_action[] = {0, 0, 0, 0, 0, 0, 0};
long action_duration[] = {5000, 5000, 5000, 5000, 5000, 5000, 5000};
long action_start[] = {0, 0, 0, 0, 0, 0, 0};


void setup() {
  // Initialize pins
  for (int i = 0; i < CHANNELS; i++) {
    pinMode(PWM_PINS[i], OUTPUT);
    pinMode(INA1_PINS[i], OUTPUT);
    pinMode(INA2_PINS[i], OUTPUT);
  }
  // Open serial port (for debugging)
  //Serial.begin(19200);
}

void loop() {

  //capture start of the loop
  current_time = millis();

  //check if it's time to update motors and actions
  if (current_time - last_refresh >= refresh_interval ) {
    
    for (int i = 0 ; i < CHANNELS; i++) {
      last_refresh = current_time;

      if (action_is_complete[i]) {
        last_action[i] = current_action[i];
        action_is_complete[i] = false;

        // Markov chain to determine next action
        switch (last_action[i]) {
          case LOOK:
            current_action[i] = random(1000) < 300 ? SCROLL : SWIPE;
            break;
          case SCROLL:
            current_action[i] = random(1000) < 500 ? SCROLL : random(1000) < 250 ? SWIPE : LOOK;
            break;
          case SWIPE:
            current_action[i] = random(1000) < 500 ? SWIPE : random(1000) < 500 ? SCROLL : LOOK;
            break;
          default:
            current_action[i] = random(3);
            break;
        }

        // initialize action
        switch (current_action[i]) {
          case LOOK:
            initLook(i);
            break;
          case SCROLL:
            initScroll(i);
            break;
          case SWIPE:
            initSwipe(i);
            break;
        }

      } else { // action is not finished
        switch (current_action[i]) {
          case LOOK:
            doLook(i);
            break;
          case SCROLL:
            doScroll(i);
            break;
          case SWIPE:
            doSwipe(i);
            break;
        }
      }

      // update motor speed and direction value
      if (motor_speed[i] > target_speed[i]) {
        motor_speed[i] -= max( round(drag[i] * float(motor_speed[i] - target_speed[i])), 1);
      } else if (motor_speed[i] < target_speed[i]) {
        motor_speed[i] += max(round(drag[i] * float(target_speed[i] - motor_speed[i])), 1);
      }

      // apply the speed to the motor
      set_speed(i, motor_speed[i]);
    }

  }
}


//----------------------------------------------------------------
// MOTOR CONTROL

void set_brake(int _index) {
  digitalWrite(INA1_PINS[_index], HIGH);
  digitalWrite(INA2_PINS[_index], HIGH);
}

void set_coast(int _index) {
  digitalWrite(INA1_PINS[_index], LOW);
  digitalWrite(INA2_PINS[_index], LOW);
}

void set_forward(int _index) {
  digitalWrite(INA1_PINS[_index], HIGH);
  digitalWrite(INA2_PINS[_index], LOW);
}

void set_reverse(int _index) {
  digitalWrite(INA1_PINS[_index], LOW);
  digitalWrite(INA2_PINS[_index], HIGH);
}

void set_speed(int _index, int _motor_speed) {
  analogWrite( PWM_PINS[_index], abs(_motor_speed));

  //check motor direction and update if necessary
  current_direction[_index] = motor_speed[_index] > 0 ? 1 : -1;
  if (last_direction[_index] != current_direction[_index]) {
    motor_speed[_index] > 0 ? set_forward(_index) : set_reverse(_index);
    last_direction[_index] = current_direction[_index];
  }

}

void stopMotor(int _index) {
  motor_speed[_index] = 0;
  target_speed[_index] = 0;
  set_speed(_index, motor_speed[_index]);
}

//----------------------------------------------------------------
// LOOK ACTION

void initLook(int _index) {

  action_start[_index] = millis();
  stopMotor(_index);

  // How long to LOOK based on last_action
  switch (last_action[_index]) {
    case LOOK:
      action_duration[_index] = random(500, 1500);
      break;
    case SCROLL:
      action_duration[_index] = random(500, 1500);
      break;
    case SWIPE:
      action_duration[_index] = random(750, 1500);
      break;
    default:
      action_duration[_index] = random(500, 2000);
      break;
  }

  target_speed[_index] = 0;
  action_state[_index] = HOLD;
}

void doLook(int _index) {
  if (millis() - action_start[_index] >= action_duration[_index]) {
    action_state[_index] = DONE;
    action_is_complete[_index] = true;
  }
}

//----------------------------------------------------------------
// SWIPE ACTION

void initSwipe(int _index) {

  switch (last_action[_index]) {
    case LOOK:
      action_duration[_index] = random(750, 1500);
      break;
    case SCROLL:
      action_duration[_index] = random(500, 750);
      break;
    case SWIPE:
      action_duration[_index] += random(-250 , 250);
      action_duration[_index] = max(action_duration[_index], 250);
      break;
    default:
      action_duration[_index] = random(750, 1750);
      break;
  }

  switch (motor_direction[_index]) {
    case 1:
      motor_direction[_index] = random(1000) < 125 ? -1 : 1;
      break;
    case -1:
      motor_direction[_index] = random(1000) < 250 ? -1 : 1;
      break;
  }

  target_speed[_index] = motor_direction[_index] * random(225, 255);

  drag[_index] = 0.5;
  action_state[_index] = ACCEL;
}

void doSwipe(int _index) {
  switch (action_state[_index]) {
    case ACCEL:
      if (motor_speed[_index] == target_speed[_index]) {
        action_start[_index] = millis();
        target_speed[_index] = 0;
        drag[_index] = 0.025;
        action_state[_index] = DECEL;
      }
      break;
    case DECEL:
      if (millis() - action_start[_index] >= action_duration[_index]) {
        action_state[_index] = DONE;
        action_is_complete[_index] = true;
      }
      break;
  }
}

//----------------------------------------------------------------
// SCROLL ACTION

void initScroll(int _index) {

  switch (last_action[_index]) {
    case LOOK:
      action_duration[_index] = random(750, 1500);
      break;
    case SCROLL:
      action_duration[_index] += random(-250 , 250);
      action_duration[_index] = max(action_duration[_index], 330);
      break;
    case SWIPE:
      action_duration[_index] += random(-250 , 250);
      break;
    default:
      action_duration[_index] = random(500, 1500);
      break;
  }

  switch (motor_direction[_index]) {
    case 1:
      motor_direction[_index] = random(1000) < 125 ? -1 : 1;
      break;
    case -1:
      motor_direction[_index] = random(1000) < 250 ? -1 : 1;
      break;
  }

  target_speed[_index] = motor_direction[_index] * random(100, 150);
  drag[_index] = 0.25;
  action_state[_index] = ACCEL;
}

void doScroll(int _index) {
  switch (action_state[_index]) {
    case ACCEL:
      if (motor_speed[_index] == target_speed[_index]) {
        action_start[_index] = millis();
        action_state[_index] = HOLD;
      }
      break;
    case HOLD:
      if (millis() - action_start[_index] >= action_duration[_index]) {
        target_speed[_index] = 0;
        action_state[_index] = DECEL;
      }
      break;
    case DECEL:
      if (motor_speed[_index] == target_speed[_index]) {
        action_is_complete[_index] = true;
        set_brake(_index);
        action_state[_index] = DONE;
      }
      break;
  }
}

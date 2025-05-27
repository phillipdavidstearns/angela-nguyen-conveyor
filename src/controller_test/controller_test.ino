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

#define LOOK  0
#define SCROLL 1
#define SWIPE 2

#define ACCEL  0
#define HOLD 1
#define DECEL 2
#define DONE 3

#define BRAKE 0x11
#define FORWARD 0x01
#define REVERSE 0x10
#define COAST 0x00

#define PWM_PIN 3
#define INA_PIN 7
#define INB_PIN 8

boolean action_is_complete = true;

float drag = 0.25;

int target_speed = 0;
int motor_speed = 0;
int motor_direction = 1;
int current_direction = 0;
int last_direction = 0;

long current_time = 0;

int refresh_interval = 33;
long last_refresh = 0;

int action_state = 0;
int current_action = 0;
int last_action = 0;
long action_duration = 5000;
long action_start = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(PWM_PIN, OUTPUT);
  pinMode(INA_PIN, OUTPUT);
  pinMode(INB_PIN, OUTPUT);
  Serial.begin(19200);
}

void loop() {

  //capture start of the loop
  current_time = millis();

  if (current_time - last_refresh >= refresh_interval ) {
    last_refresh = current_time;

    if (action_is_complete) {

      action_is_complete = false;
      current_action = random(3);

      switch (current_action) {
        case LOOK:
          Serial.println("Starting LOOK action.");
          initLook();
          break;
        case SCROLL:
          Serial.println("Starting SCROLL action.");
          initScroll();
          break;
        case SWIPE:
          Serial.println("Starting SWIPE action.");
          initSwipe();
          break;
      }

    } else {
      switch (current_action) {
        case LOOK:
          doLook();
          break;
        case SCROLL:
          doScroll();
          break;
        case SWIPE:
          doSwipe();
          break;
      }
    }

    if (motor_speed > target_speed) {
      motor_speed -= max( round(drag * float(motor_speed - target_speed)), 1);
    } else if (motor_speed < target_speed) {
      motor_speed += max(round(drag * float(target_speed - motor_speed)), 1);
    }

    set_speed(motor_speed);

  }
}


//----------------------------------------------------------------
// MOTOR CONTROL

void set_brake() {
  digitalWrite(INA_PIN, HIGH);
  digitalWrite(INB_PIN, HIGH);
}

void set_coast() {
  digitalWrite(INA_PIN, LOW);
  digitalWrite(INB_PIN, LOW);
}

void set_forward() {
  digitalWrite(INA_PIN, HIGH);
  digitalWrite(INB_PIN, LOW);
}

void set_reverse() {
  digitalWrite(INA_PIN, LOW);
  digitalWrite(INB_PIN, HIGH);
}

void set_speed(int motor_speed) {
  analogWrite( PWM_PIN, abs(motor_speed));
  current_direction = motor_speed > 0 ? 1 : -1;

  if (last_direction != current_direction) {
    motor_speed > 0 ? set_forward() : set_reverse();
    last_direction = current_direction;
  }

}

void stopMotor() {
  motor_speed = 0;
  target_speed = 0;
  set_speed(motor_speed);
}

//----------------------------------------------------------------
// LOOK ACTION

void initLook() {
  action_start = millis();
  stopMotor();
  action_duration = random(500, 1500);
  target_speed = 0;
  action_state = HOLD;
}

void doLook() {
  if (millis() - action_start >= action_duration) {
    action_state = DONE;
    action_is_complete = true;
  }
}

//----------------------------------------------------------------
// SWIPE ACTION

void initSwipe() {
  action_duration = random(500, 2000);
  motor_direction = random(1000) < 125 ? -1 : 1;
  target_speed = motor_direction * random(200, 255);
  drag = 0.5;
  action_state = ACCEL;
}

void doSwipe() {
  switch (action_state) {
    case ACCEL:
      if (motor_speed == target_speed) {
        action_start = millis();
        target_speed = 0;
        drag = 0.025;
        action_state = DECEL;
      }
      break;
    case DECEL:
      if (millis() - action_start >= action_duration) {
        action_state = DONE;
        action_is_complete = true;
      }
      break;
  }
}

//----------------------------------------------------------------
// SCROLL ACTION

void initScroll() {

  action_duration = random(500, 2000);
  motor_direction = random(1000) < 125 ? -1 : 1;
  target_speed = motor_direction * random(200, 255);
  drag = 0.25;
  action_state = ACCEL;
}

void doScroll() {
  switch (action_state) {
    case ACCEL:
      if (motor_speed == target_speed) {
        action_start = millis();
        action_state = HOLD;
      }
      break;
    case HOLD:
      if (millis() - action_start >= action_duration) {
        target_speed = 0;
        action_state = DECEL;
      }
      break;
    case DECEL:
      if (motor_speed == target_speed) {
        action_is_complete = true;
        set_brake();
        action_state = DONE;
      }
      break;
  }
}

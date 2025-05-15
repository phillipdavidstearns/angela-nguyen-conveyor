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

int current_time = 0;

int refresh_interval = 33;
int last_refresh = 0;

int action_state = 0;
int current_action = 0;
int last_action = 0;
int action_duration = 5000;
int action_start = 0;


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

    if (motor_speed > target_speed) {
      motor_speed -= max( round(drag * float(motor_speed - target_speed)), 1);
    } else if (motor_speed < target_speed) {
      motor_speed += max(round(drag * float(target_speed - motor_speed)), 1);
    }

    set_speed(abs(motor_speed));

    if (motor_speed > 0) {
      set_forward();
    } else if (motor_speed < 0) {
      set_reverse();
    }

  }

  if (action_is_complete) {
    current_action = random(3);
    switch (current_action) {
      case LOOK:
        Serial.println("Starting LOOK action.");
        action_duration = random(250, 2000);
        set_brake();
        target_speed = 0;
        motor_speed = 0;
        action_state = HOLD;
        break;
      case SCROLL:
        Serial.println("Starting SCROLL action.");
        set_brake();
        motor_speed = 0;
        action_duration = random(500, 1500);
        target_speed = random(100, 255);
        drag = 0.25;
        if ( random(1000) < 253 ) target_speed *= -1;
        action_state = ACCEL;
        break;
      case SWIPE:
        Serial.println("Starting SWIPE action.");
        set_brake();
        motor_speed = 0;
        action_duration = random(300, 1250);
        target_speed = random(200, 255);
        drag = 0.5;
        if ( random(1000) < 150 ) target_speed *= -1;
        action_state = ACCEL;
        break;
    }
    action_start = millis();
    action_is_complete = false;
  } else {
    switch (current_action) {
      case LOOK:
        look();
        break;
      case SCROLL:
        scroll();
        break;
      case SWIPE:
        swipe();
        break;
    }
  }
}

void look() {

  if (millis() - action_start >= action_duration){
    Serial.println("LOOK action complete.");
    action_is_complete = true;
  }
}

void swipe() {
    switch(action_state){
    case ACCEL:
      if(motor_speed == target_speed){
        Serial.println("SWIPE decelerating...");
        target_speed = 0;
        drag = 0.025;
        action_state = DECEL;
      }
      break;
    case DECEL:
      if(millis() - action_start >= action_duration){
        Serial.println("SWIPE action complete.");
        action_is_complete = true;
      }
      break;
  }
}

void scroll() {
  switch(action_state){
    case ACCEL:
      if(motor_speed == target_speed){
        Serial.println("Holding SCROLL...");
        action_state = HOLD;
      }
      break;
    case HOLD:
      if(millis() - action_start >= action_duration){
        Serial.println("SCROLL decelerating...");
        target_speed = 0;
        action_state = DECEL;
      }
      break;
    case DECEL:
      if(motor_speed == target_speed){
        action_is_complete = true;
        set_brake();
        Serial.println("SCROLL action complete.");
      }
      break;
  }
}

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

void set_speed(int target) {
  analogWrite( PWM_PIN, target );
}

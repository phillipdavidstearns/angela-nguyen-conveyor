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

#define BRAKE 0x11
#define FORWARD 0x01
#define REVERSE 0x10
#define COAST 0x00

#define PWM_PIN 3
#define INA_PIN 7
#define INB_PIN 8

int motor_speed = 0;
int interval = 3000;
int current_time = 0;
int last_time = 0;

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

  //check against last time the interval ran and run again if enough time has elapsed
  if (current_time - last_time >= interval) {
    //save the start time of the interval
    last_time = current_time;

    //randomize the motor speed and set it
    motor_speed = random(0, 255);
    set_speed(motor_speed);
    
    //randomly select a setting for the "direction"
    switch (random(4)) {
      case 0:
        //hard shorts the motor to gound
        set_brake();
        break;
      case 1:
        //floats the motor
        set_coast();
        break;
      case 2:
        set_forward();
        break;
      case 3:
        set_reverse();
        break;
    }
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

# Journal Angela Nguyen

## Sept 26th 4:30a - 6:00p (1.5 hours)

### Tasks:
* Wiring motor boards to Arduino.

## Sept 25th 12:30a - 1:00p (0.5 hours)

### Tasks:
* Installed power inlet on enclosure.

## Sept 23rd 6:50a - 7:20p (0.5 hours)

### Tasks:
* Revised code for use on Arduino Mega with 7 PWM outputs.
* Preliminary tests seem OK.
* Uploaded to main board.


## Sept 23rd 2:00a - 5:00p (2 hours)

### Tasks:

* Tested Motor Driver Boards (100% OK)
* Soldered Driver Boards to Protoboard
* Soldered common ground for driver boards to connect to Arduino, though it may be superfluous. After testing the ground connections, all components so far appear to be grounded.
* Prepared power connections for the driver boards and attached wires to terminal blocks.


## Sept 23rd 2:00a - 5:00p (2 hours)

### Tasks:

* Assembled Motor Driver Boards
* Secured power supply to mounting panel

## Sept 22nd 11:00a - 2:00p (3 hours)

### Tasks:

* Assembled Arduino Mega shield
* Positioned parts on enclosure mounting panel:
	* Power supply
	* Terminals
	* Protoboard
	* Voltage converter
	* Arduino
* Tested 5.5mm x 2.5mm cables with existing connectors. Will be returning them and ordering 5.5mm x 2.1mm parts:
	* Power cables
	* Power jacks
* Wired up:
	* power inlet to power supply
	* power supply to terminal block
	* power supply to voltage converter
	* voltage converter to Arduino VIN terminal blocks on shield

### Notes:

* Failed delivery of Motor driver boards to RedLine

## Sept 17th 11:00a - 1:30p (2.5 hours)

### Tasks:

* Compiled shoppings lists for individual and single control box options: [Project Budget Spreadsheet](https://docs.google.com/spreadsheets/d/1Jn1ExP9aKBmZd6r0uzt52_Y1YAMKufn_YkM_hZGr3w4/edit?usp=sharing)
* Completed renders of different configuration options

### Notes:

* Half project total paid: $1176.00

## August 14th 11:00a - 1:00p (2 hours)

### Tasks:

* Shopping for 7x control units:
	* 4 x \$17 (~\$80 incl. tax & shipping) - [Junction Box IP65 Waterproof Dustproof ABS Plastic Universal Electric Project Enclosure with Fixed Ear Black 6.2x3.5x2.5 inch (2pc)](https://www.amazon.com/LMioEtool-Waterproof-Dustproof-Universal-Enclosure/dp/B0BFF3B162/) (look out for clear containers)
	* 3 x \$16 (~\$60 incl. tax & shipping) - [Nano V3.0, Nano Board ATmega328P 5V 16M Micro-Controller Board Compatible with Arduino IDE (Nano x 3 with USB Cable)](https://www.amazon.com/LAFVIN-Board-ATmega328P-Micro-Controller-Arduino/dp/B07G99NNXL/)
	* 7 x \$12 (~\$100 incl. tax & shipping) - [TB67H420FTG Dual/Single Motor Driver Carrier](https://www.pololu.com/product/2999)
	* 1 x \$17 (~\$20 incl. tax & shipping) - [10 Pcs Solderable Breadboard PCB Board Gold-Plated Solder Breadboard for DIY Electronics Projects Apply to Soldering Projects 3.5"x2.05" (Green)](https://www.amazon.com/EPLZON-Solderable-Breadboard-Gold-Plated-Electronics/dp/B0D5XFJLFM/)
	* 4 x \$7 (~\$35 incl. tax & shipping) - [DC 5525 Power Pigtails Cable, 5.5MM x 2.5MM Male Plug to Bare Wire Open End Power Wire Supply Repair Cable, 18AWG Barrel Connector for CCTV Camera,DVR,LED Strip Light Etc (2 Pack) 4FT](https://www.amazon.com/Pigtails-Supply-Repair-Barrel-Connector/dp/B0D46LBVVS/)
	* 1 x \$7 (~\$8.50 incl. tax & shipping) - [2.5mm DC Power Jack Socket, Panel Mount DC Power Jack Socket Female Panel Mounting Connector 10Pcs](https://www.amazon.com/DIYhz-Socket-Female-Mounting-Connector/dp/B09W9SJ1B6/)
* Discussed synchronization:
	* All start up at the same time.
	* Every 10m search for zero point.
	* Wait for sync signal.
	* Roll through until zero point.
	* Resume random behaviors
* This would entail Arduino Connections not previously discussed:
	* Output for "waiting"
	* Input for "synced"
* Perhaps a central control unit would be desirable... then extending the PWM outputs using [this technique](https://howtomechatronics.com/tutorials/arduino/how-to-extend-arduino-pwm-outputs-tlc5940-tutorial/)?
	* This would eliminate the need for multiple Arduinos and therefore the need to communicate between them. We'd only need 7 inputs for the switches on each box.
	* Would have to consider power. Centralized or not?


## June 30th 10:45a - 11:45a (1 hrs)

### Tasks:

* Mounted conveyor on the wall and taped to simulate a social media feed.
* Recorded and shared video

## May 27th 1:00 - 3:00p (2 hrs)

### Completed:

* Unexpected erratic motion was observed.
	* Observed when code was changing directions.
	* Removed direction changes.
	* Reviewed logic to make sure variables and pin values were being set when desired and in the correct sequence.
* Erratic motion resolved. The culprit was:

```
#define FORWARD 1
#define REVERSE -1  <---
```

Apparently you can't use `#define` set negative keyword values...

### ToDo:

* Markov chains!

## May 14th 7:00 - 9:00p (2 hrs)

### Completed:

* Rough modeling of gestures:
	* LOOK - stop and hold
	* SCROLL - stop, accelerate to a set speed, hold speed, decelerate to stop
	* SWIPE - stop, accelerate to speed, then decelerate

### ToDo:

* Refine gestures
* Build Markov Chain to shape how the next gestures is randomly chosen based on the previous gesture.

## May 13th 3:30p - 6:00p + 7:30 - 8:30p (3.5 hrs)

### Completed:

* Checked power supply for PWM. It's not PWM... possibly a simple variable regulated voltage output.
* Connected bench power supply to Motor Driver Board.
* Connected Motor to Motor Driver Board
* Setup basic Arduino PMW test code to verify that the motor can be controlled from a 24V source using the Motor Driver Board controlled by the Arduino.
* Wired conveyor power supply to Motor Driver board, powered Arduino from VCC (+5V) out, all control connections for forward, reverse, pwm.
* Setup Github Repo at [https://github.com/phillipdavidstearns/angela-nguyen-conveyor](https://github.com/phillipdavidstearns/angela-nguyen-conveyor)
* Started Guide
	* README.md
	* Wiring Diagram

### ToDo:

* Assemble the conveyor belt and see how the motor+driver+arduino combo runs.
* Start coding behaviors:
	* Pause/Stop+Hold
	* Scroll
	* Swipe
	* Markov Chain for direction and actions, etc.

## May 9th 4:30p - 6:00p (1.5hrs)

Preliminary assessment following opening the conveyor and inspecting the motor:

* Seems 100% compatible with [Pololu 37D Gearmotors](https://www.pololu.com/category/116/37d-metal-gearmotors). Looks like a simple swap.
* Motor Power supply looks like a variable voltage output range from 4V to 26V. Would have to hook up to a scope to determine whether it's PWM or some other form of voltage control. Seems like it should be PWM, but does not appear to be, given the performance of the motor at lower voltages (extreme loss of torque).
* Assembled a spare [TB67H420FTG Dual/Single Motor Driver Carrier](https://www.pololu.com/product/2999) for testing before committing to purchasing the higher power versions.

### ToDo:

* Setup Github and add photos to this document.
* Check the output of the provided power supply for PWM and V/A performance characteristics.
* If not PWM, setup the bench supply and measure current loads of the motor to check that it's in spec for the driver.
* Establish basic connections between motor, driver, and Arduino.
* Begin writing test sequence code.
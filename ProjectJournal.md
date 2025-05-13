# Journal Angela Nguyen

## May 13th 3:30p - 

Completed:

* Checked power supply for PWM. It's not PWM... possibly a simple variable regulated voltage output.


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
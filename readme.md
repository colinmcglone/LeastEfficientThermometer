# Automated Kettle

## Physical Description

The modified parts of this kettle consist of a Serial 7 Segment Display, which has four 7 segment digits and an ATMega328, a potentiometer, a waterproof thermometer, and a piezo speaker.

The board is powered from the kettle by tapping into the source for the power indicator LED using a DC-DC up converter to bring the 1.3v to 5v. This means that the kettle power controls the board power.


## Features

Displays current temperature
An alarm goes off at an adjustable target temperature


## Use

1. Turn on kettle.
2. Set alarm temperature using dial. (The alarm temperature is displayed once the dial is turned, current temperature display is resumed after 2 seconds)
3. Wait for alarm to sound (if no alarm is desired, set to 100 and wait for your prefered temperature to be displayed)
4. Pour hot water.


## Future Plans

Add PID control to allow water to rest at alarm temperature


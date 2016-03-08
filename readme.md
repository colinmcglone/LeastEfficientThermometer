# Automated Kettle

This project merges the simplicity of an electric kettle with the complexity of a microcontroller to create a project which can expand to fit the spare time of it's creator.

This project uses the arduino system for ease of use, the libraries for the thermometer and displays are well done and do almost all the work for us.

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


## Issues and Limitations

The reading of the thermometer lags about 10s behind the actual temperature of the water, so the alarm attempts to take this into account. The heating of the water is also fairly quick in the operational range so the response time of the user will effect the temperature of the water greatly. The kettle and board are powered by the same circuit, the heating element cannot be turned off seperately from the microcontroller. This means you cannot overheat and wait for the temperature to come down while seeing the temperature (turning on the heater and quickly shutting it off can give you a quick idea of the temperature without heating the water significantly, but that is a pretty crap solution).


## Future Plans

Add PID control to allow water to rest at alarm temperature.


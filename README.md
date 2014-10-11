VL6180-Driver
=============

a generic driver for the VL6180 proximity-and-als detector

[Datasheet](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00112632.pdf)

## The Sensor
The VL6180 is both an ambient light sensor (ALS) and a Proximity Sensor for
short ranges of about 0 to 100 [cm]. ST has implemented a time-of-flight 
based sensor measurement technique, which theoretically implies that the
distance measurment is constant regardless of the ambient light returned
by the sensor.

### Basic Sensor Characteristics (summarized from the datasheet)
* Field of View for ALS sensing is ~42 [deg]
* Spectral response is most prominent between the 500 and 650 [nm] range
* ALS Gain: 1, 1.25, 1.67, 2.5, 5, 10, 20, 40

* Abs max voltage for any pin: 3.6 [V]
* avg current consumption 300 [uA]

## The Driver Interface



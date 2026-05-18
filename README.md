# Monitoring Smartwatch for Patient Care

An Arduino-based wearable environmental monitoring prototype that tracks ambient temperature, sound, and brightness levels using Grove sensors. The system displays real-time readings on an I2C OLED screen and shows alert messages when environmental values remain outside safe threshold ranges.

This prototype was designed to explore how wearable monitoring systems could support patient-care environments by identifying environmental factors such as excessive noise, bright lighting, or uncomfortable temperature.

## Features

- Real-time temperature, sound, and light monitoring
- I2C OLED display for live sensor readings
- Threshold-based alert system
- Timing filters to reduce false warnings from brief environmental changes
- Grove button acknowledgment to return from alert mode to the sensor monitor screen
- Wearable prototype format for technical symposium demonstration

## Hardware Used

- Arduino-compatible board
- Grove Base Shield
- Grove Temperature Sensor
- Grove Sound Sensor
- Grove Light Sensor
- Grove Button
- I2C OLED Display
- Jumper/Grove cables
- Prototype wrist strap

## Demo

<!-- Put your BEST OLED close-up photo here -->
<!-- Recommended: normal Sensor Monitor screen photo -->

![Sensor Monitor OLED]

The OLED display shows real-time environmental readings for temperature, sound, and light.

![Alert OLED](images/alert-screen.jpg)

When a sensor value stays above or below the threshold for a set amount of time, the OLED switches to an alert screen.

This early hardware prototype uses Grove modules and an Arduino sensor shield to validate the sensing logic, OLED interface, and button-based alert acknowledgment before miniaturization.

## How It Works

The Arduino reads analog values from the temperature, sound, and light sensors. These readings are converted into display values and compared against preset thresholds.

If an environmental condition remains outside the threshold range for a sustained period, the system displays an alert message on the OLED screen. Short spikes are ignored using a timing filter, reducing false warnings.

The Grove button allows the user to acknowledge an alert and return to the live sensor monitor screen.

## Alert Examples

```text
ALERT!
TEMP TOO HIGH

# CalibraSense Pro
### ESP32-Based Analog Monitoring and Alert System

## Overview

CalibraSense Pro is an embedded measurement and monitoring prototype built using an ESP32 microcontroller. It reads an analog input from a potentiometer, samples the ADC multiple times, calculates a filtered measurement, and uses threshold-based logic with hysteresis to control visual and audible alerts.

The system displays measurement information on an I²C LCD and transmits structured readings through the serial interface for CSV logging and further analysis.

The project combines embedded firmware development, analog signal acquisition, basic signal processing, hardware interfacing, event logging, and experimental data analysis.

## Objectives

- Acquire analog measurements using the ESP32 ADC.
- Reduce short-term measurement variation through multi-sample averaging.
- Implement hysteresis-based state transitions to reduce rapid switching near a threshold.
- Display readings and system status on an LCD.
- Control an LED and buzzer according to the detected state.
- Record measurements in CSV format for statistical analysis and visualization.
- Evaluate system behavior using collected experimental data.

## System Features

- Analog input acquisition
- Multi-sample averaging
- Threshold-based state detection
- Hysteresis for state stability
- LCD-based measurement display
- LED and buzzer alerts
- Serial CSV data logging
- Data analysis using Python and Excel

## Hardware Components

| Component | Purpose |
|---|---|
| ESP32 development board | Main controller and ADC acquisition |
| Potentiometer | Adjustable analog input for testing |
| LED | Visual state indication |
| Buzzer | Audible state indication |
| I²C LCD | Displays measurements and system status |
| Breadboard and jumper wires | Circuit assembly |

## Software and Tools

- Arduino IDE
- ESP32 Arduino Core
- C/C++ firmware
- `Wire` library for I²C communication
- `LiquidCrystal_I2C` library for LCD interfacing
- Python / Google Colab for data analysis
- Microsoft Excel for CSV inspection and visualization
- Git and GitHub for version control and documentation

## System Architecture

```text
Potentiometer
     |
     v
ESP32 Analog Input
     |
     v
ADC Sampling
     |
     v
Multi-Sample Averaging
     |
     v
Hysteresis-Based State Logic
     |
     +----------+-----------+
     |          |           |
     v          v           v
    LCD        LED        Buzzer
     |
     v
Serial CSV Logging
     |
     v
Python / Excel Data Analysis
```

## Measurement and State Logic

The ESP32 acquires ADC samples from the potentiometer input. Multiple samples are averaged to obtain a filtered measurement for state evaluation.

The system uses two threshold values:

- **Upper threshold:** A transition from NORMAL to HIGH is requested when the filtered reading reaches or exceeds this value.
- **Lower threshold:** A transition from HIGH to NORMAL is requested when the filtered reading falls to or below this value.

Keeping the two thresholds separate implements hysteresis and helps prevent repeated switching when the input fluctuates near a boundary.

The initial candidate thresholds are 2565 ADC counts for the upper threshold and 2450 ADC counts for the lower threshold. These values are provisional and require controlled testing before being treated as validated operating limits.

## Data Logging

The firmware produces structured CSV records containing:

- Timestamp in milliseconds
- Raw ADC reading
- Filtered ADC reading
- System state
- LED status
- Buzzer status
- Event count
- State duration
- Event marker

The collected data can be imported into Python or Excel to examine measurement distributions, state transitions, output behavior, and potential anomalies.

## Data Analysis and Validation

The initial dataset has been examined using descriptive statistics and visualizations.

The preliminary analysis includes:

- Mean, minimum, maximum, and range of ADC readings
- Comparison of raw and filtered measurements
- Distribution of NORMAL and HIGH states
- Inspection of state transitions
- Review of ADC saturation at the lower and upper limits

The initial dataset contains many readings at ADC counts 0 and 4095. These endpoint readings require further investigation to distinguish deliberate potentiometer extremes from possible input saturation or measurement setup issues.

Filtering and stable state transitions do not, by themselves, establish absolute measurement accuracy. Reference-voltage checks and repeatable controlled tests are needed for stronger validation.

## Repository Structure

```text
CalibraSense-Pro/
├── README.md
├── firmware/
│   └── CalibraSense.ino
├── data/
│   └── calibrasense_log.csv
├── analysis/
│   └── CalibraSense_Analysis.ipynb
├── images/
│   ├── hardware_setup.jpg
│   └── analysis_plot.png
└── docs/
    └── Validation_Report.md
```

*Files will be added as they are prepared and verified.*

## Current Status

- Hardware prototype assembled
- Analog acquisition and output control implemented
- LCD display integrated
- CSV data collected
- Initial statistical analysis and plots prepared
- Firmware recovery and final verification in progress
- Threshold validation and reference-based accuracy testing pending

## Future Improvements

- Calibrate ADC readings against a trusted voltage reference.
- Investigate endpoint saturation and input behavior.
- Test repeatability across multiple controlled input levels.
- Quantify the effect of filtering on measurement variation.
- Validate hysteresis behavior with repeated rising and falling input sweeps.
- Improve data visualization and produce a formal validation report.

## Learning Outcomes

This project provides practical experience with microcontroller ADC acquisition, analog input interfacing, basic digital filtering, hysteresis, peripheral integration, serial data logging, and experimental data analysis.

## Disclaimer

CalibraSense Pro is currently an educational prototype. The potentiometer is used as a controllable analog input for testing; the system has not yet been established as a calibrated instrument for safety-critical or industrial measurements.

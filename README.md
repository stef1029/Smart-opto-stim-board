# Head-sensor-experiment-control: Arduino Laser Controller

This repository contains an Arduino script that works in conjunction with `laser_control.py` to provide precise control over laser stimulation patterns. The system coordinates between a Cobolt laser device and an Arduino to deliver timed laser pulses with configurable parameters.

## System Components

- Arduino script (`laser_controller.ino`)
- Python control script (`laser_control.py`)
- Cobolt06MLD laser
- Arduino (connected to laser trigger and LED indicator)

## Hardware Setup

The Arduino requires the following pin connections:
- Pin 7: Laser trigger output
- Pin 13: LED indicator (for visual feedback)
- USB connection to computer for serial communication

## Communication Protocol

The Arduino accepts the following serial commands:

- `p`: Parameter configuration
- `s`: Start sequence
- `e`: Emergency stop

### Parameter Format
Parameters are sent in the following format:
```
numDurations#duration1,duration2,...,numCycles,stimInterval,pulseOnTime,pulseOffTime
```

Example:
```
6#50,100,250,500,1000,2000,20,5000,50,50
```
This configures:
- 6 different duration values
- Durations: 50ms, 100ms, 250ms, 500ms, 1000ms, 2000ms
- 20 cycles
- 5000ms interval between stimulations
- 50ms pulse on time
- 50ms pulse off time

## Features

- Configurable stimulation durations (up to 10 different durations)
- Adjustable number of cycles
- Configurable interval between stimulations
- Pulse train mode with adjustable on/off times
- Solid pulse mode (when pulse off time is 0)
- LED indicator matching laser trigger output
- Serial feedback for sequence completion and parameter reception

## Operation Modes

### Solid Pulse Mode
When `pulseOffTime` is set to 0, the system operates in solid pulse mode, delivering continuous stimulation for the specified duration.

### Pulse Train Mode
When `pulseOffTime` is greater than 0, the system delivers a train of pulses for the specified duration, with configurable on and off times.

## Integration with Python Control

The Arduino script is designed to work with `laser_control.py`, which provides:
- High-level control interface
- Multiple power level sequences
- Safety checks and monitoring
- Command-line parameter configuration
- Automated cleanup on completion or interruption

## Safety Features

- Emergency stop command (`e`)
- Parameter validation
- LED indicator for visual feedback
- Serial communication confirmation
- Automatic shutdown after sequence completion

## Usage

1. Upload the Arduino script to your Arduino board
2. Connect the Arduino to your laser setup
3. Run the Python control script with desired parameters:

```bash
python laser_control.py --laser_port COM11 --arduino_port COM23 --powers 5.0 10.0 15.0 --stim_times 50 100 250 500 1000 2000 --num_cycles 20 --stim_delay 5000 --pulse_freq 10.0 --pulse_on_time 50
```

See `laser_control.py` for complete command-line options and additional documentation.

## Error Handling

The Arduino script includes error checking for:
- Invalid parameter formats
- Out-of-range duration counts
- Serial communication errors

## Dependencies

### Arduino
- Arduino Standard Library

### Python Control Script
- pycobolt
- pyserial
- keyboard
- colorama

## Monitoring

The system provides real-time feedback through:
- Serial communication (57600 baud rate)
- LED indicator on pin 13
- Detailed console output from Python control script

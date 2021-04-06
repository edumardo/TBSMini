# TBSMini

Arduino library to control the TBS Mini sound unit (Benenidi or Dasmikro clon). This library wouldn't exist without the fantastic job of [Open Panzer TCB](https://github.com/OpenPanzerProject/TCB) and its [OP_TBS class](https://github.com/OpenPanzerProject/TCB/tree/master/OpenPanzerTCB/src/OP_TBS).

You can see in action in the [arduino-panzer](https://github.com/edumardo/arduino-panzer) repository.

## Features

* Start/stop engine.
* Proportional sound engine.
* Up to 14 different sounds to play it individually.
* Volume up/down.

## Installation

## Dependencies

TBS Mini inputs are designed to work with servos and microsecond pulses, therefore it uses the arduino standard [servo](https://github.com/arduino-libraries/Servo) library to communicate with the unit. Also depends on the [arduino-timer](https://github.com/contrem/arduino-timer) for sending control signals.

## How it works

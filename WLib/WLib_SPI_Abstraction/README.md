# WLib-SPI-Abstraction

## Motivation

Assume that you are given the task of writing a driver for the chip XY connected via SPI.
To ensure the reusability of the driver it should be as platform independent as possible.
However, the feature set and the operation of the SPI hardware is a strongly platform dependent matter.
It is therefore recommended to abstract the SPI communication as a whole.
The operation of the SPI hardware is delegated to a specialized SPI hardware driver.

WLib-SPI-Abstraction represents such an interface.
It is the starting point for the Chip-XY driver, which now has no dependency on the concrete SPI-HW.
In addition, this interface specifies the interface of the specialized SPI hardware drivers.

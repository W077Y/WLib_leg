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


## Modell

### Channel
The SPI channel represents an inactive communication connection to the chip.
To open a channel means that the hardware gets locked and configured but the chip is not yet selected.
The channel lifetime is managed via the ```Channel_Handle```.

### Connection
The SPI connection represents an active communication link to the chip.
This means that the hardware is configured and the chip is selected.
The connection lifetime is managed via the ```Connection_Handle```.

### The ```Channel_Provider```
The ```Channel_Provider``` provides a way to request a channel via the request_channel memberfunction.

### Example
``` cpp
void func(WLib::SPI::Channel_Provider& device)
{
    std::byte tx[5];
    std::byte rx[5];

    // fill tx with data
    // ...
    
    // lock and configure the hardware
    // by opening a channel
    auto && channel_h = device.request_channel(spi_cfg);
    // channel_h keeps the channel "alive"
    // as long as channel_h exists the channel stays open

    // select the target chip
    auto && conn_h = channel_h.select_chip();
    // conn_h keeps the connection "alive"
    // as long as conn_h exists the chip stays selected



    // transceive data
    conn_h.transceive(tx, rx, 5);

    // use the recieved data in rx
    // ...
}
```

## How to use the tranceive function

### Clock ```len``` bytes:
``` cpp
    connection.transceive(nullptr, nullptr, len);
```
Both outgoing and incoming data are irrelevant

Example:
+ Clocking dummy bytes for eeprom

### Transmit ```len``` bytes
``` cpp
    connection.transceive(tx, nullptr, len);
```
The outgoing data are in the tx-buffer, the incoming data can be discarded because they are not relevant.

Example:
+ Sending a command without response 

### Receive ```len``` bytes
``` cpp
    connection.transceive(nullptr, rx, len);
```
Receiving data into the rx-buffer the outgoing data is not relevant

Example:
+ Reading data from a device

### Transceive ```len``` bytes 
``` cpp
    connection.transceive(tx, rx, len);
```
The outgoing data is in the tx-buffer, the incoming data is to be written to the rx-buffer.

### Transceive with replacement ```len``` bytes 
``` cpp
    connection.transceive(txrx, txrx, len);
```
The outgoing data is in the tx-buffer, the incoming data is to be written to the rx-buffer.

PiHWCtrl
========

The PiHWCtrl (Pi Hardware Control) library is an object oriented approach of
controlling modules connected to the Raspberry Pi via the 40 GPIO pins. The
library itself is implemented in C++14 but its functionality is fully accessible
from Python via the provided Swig interfaces.

The following sections describe what is currently implemented in the library.
For examples of how to use the classes check the directories `src/examples` (for
C++ examples) and `swig/examples` (for Python examples)

**WARNING:** This library is still in its infancy and it is not (yet) containing
much functionality.


HWInterfaces
------------

The `HWInterfaces` package contains the interfaces used by the library to
abstract the different implementations and help for the modularization.

- `Switch` : Represents a switch that can be turned ON and OFF
- `BinaryInput` : Represents an input which has two states, ON and OFF
- `AnalogInput<T>` : Represents an input which provides values of type T
- `Observer<T>` : Object that can be notified for events of type T
- `Observable<T>` : Object that generates events of type T


gpio
----

The `gpio` package contains classes for directly controlling the GPIO pins,
using the sysfs drivers.

- `GpioSwitch` : Controls a GPIO pin as an output
- `GpioBinaryInput` : Controls a GPIO pin as an input


modules
-------

The `modules` package contains classes for controlling the different peripheral
modules connected to the GPIO pins.

- `BMP180` : Pressure, temperature and altitude sensor

controls
--------

The `controls` package contains classes for further manipulating the values
retrieved by the different modules.

- `StateChangeFilter<T>` : Observer decorator to filter out repetitive events
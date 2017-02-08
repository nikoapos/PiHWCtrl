%module PiHWCtrl

// The PiHWCtrl interfaces
%include PiHWCtrl/HWInterfaces/BinaryInput.h
%include PiHWCtrl/HWInterfaces/Switch.h

// The implementations using the pigpio library
%{
#include <PiHWCtrl/pigpio/PigpioBinaryInput.h>
#include <PiHWCtrl/pigpio/PigpioSwitch.h>
%}
%include PiHWCtrl/pigpio/PigpioBinaryInput.h
%include PiHWCtrl/pigpio/PigpioSwitch.h
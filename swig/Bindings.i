%module PiHWCtrl

%{
#include <PiHWCtrl/pigpio/PigpioSwitch.h>
%}

%include PiHWCtrl/HWInterfaces/Switch.h
%include PiHWCtrl/HWInterfaces/BinaryInput.h
%include PiHWCtrl/pigpio/PigpioSwitch.h
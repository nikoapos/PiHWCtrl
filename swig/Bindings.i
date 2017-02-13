%module PiHWCtrl

// Handle the exceptions
%{
#include <typeinfo>
%}
%include exception.i
%exception {
    try {
        $function
    } catch(const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}

// The PiHWCtrl interfaces
%{
#include <PiHWCtrl/HWInterfaces/BinaryInput.h>
#include <PiHWCtrl/HWInterfaces/Switch.h>
#include <PiHWCtrl/HWInterfaces/PWM.h>
%}
%include PiHWCtrl/HWInterfaces/BinaryInput.h
%include PiHWCtrl/HWInterfaces/Switch.h
%include PiHWCtrl/HWInterfaces/PWM.h

// The implementations using the pigpio library
%{
#include <PiHWCtrl/pigpio/PigpioBinaryInput.h>
#include <PiHWCtrl/pigpio/PigpioSwitch.h>
#include <PiHWCtrl/pigpio/PigpioPWM.h>
#include <PiHWCtrl/pigpio/exceptions.h>
%}
%include PiHWCtrl/pigpio/PigpioBinaryInput.h
%include PiHWCtrl/pigpio/PigpioSwitch.h
%include PiHWCtrl/pigpio/PigpioPWM.h
        
// I2C Bus
%{
#include <PiHWCtrl/i2c/I2CBus.h>
%}
%include PiHWCtrl/i2c/I2CBus.h
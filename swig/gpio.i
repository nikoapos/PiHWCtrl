%module(package="PiHWCtrl", directors="1") gpio

%include HWInterfaces.i
    
%{ 
#include <PiHWCtrl/gpio/GpioSwitch.h>
%}
%include PiHWCtrl/gpio/GpioSwitch.h
    
%{ 
#include <PiHWCtrl/gpio/GpioBinaryInput.h>
%}
%include PiHWCtrl/gpio/GpioBinaryInput.h
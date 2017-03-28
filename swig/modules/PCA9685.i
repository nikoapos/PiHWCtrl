%module(package="PiHWCtrl", directors="1") modules

%include HWInterfaces.i
%include <stdint.i>
    
%{ 
#include <PiHWCtrl/modules/PCA9685.h>
%}
// Ignore the methods that use unique_ptr
%ignore PiHWCtrl::PCA9685::factory;
%ignore PiHWCtrl::PCA9685::getAsPWM(int);

%include PiHWCtrl/modules/PCA9685.h
        
// Create alternatives for the methods that use unique_ptr
%{
PiHWCtrl::PCA9685* PCA9685_factory(std::uint8_t address, int pwm_frequency=200) {
    return PiHWCtrl::PCA9685::factory(address, pwm_frequency).release();
}
%}
PiHWCtrl::PCA9685* PCA9685_factory(std::uint8_t address, int pwm_frequency=200);

%extend PiHWCtrl::PCA9685 { 
    PWM* getAsPWM(long led) {
        return $self->getAsPWM(led).release();
    }
}
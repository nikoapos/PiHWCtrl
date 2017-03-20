%module(package="PiHWCtrl", directors="1") modules

%include HWInterfaces.i
%include <stdint.i>
    
%{ 
#include <PiHWCtrl/modules/ADS1115.h>
%}
// Ignore the methods that use unique_ptr
%ignore PiHWCtrl::ADS1115::factory;
%ignore PiHWCtrl::ADS1115::conversionAnalogInput(Input);

%include PiHWCtrl/modules/ADS1115.h
        
// Create alternatives for the methods that use unique_ptr
%{
PiHWCtrl::ADS1115* ADS1115_factory(PiHWCtrl::ADS1115::AddressPin addr=PiHWCtrl::ADS1115::AddressPin::GND,
                                   PiHWCtrl::ADS1115::DataRate data_rate=PiHWCtrl::ADS1115::DataRate::DR_128_SPS) {
    return PiHWCtrl::ADS1115::factory(addr, data_rate).release();
}
%}
PiHWCtrl::ADS1115* ADS1115_factory(PiHWCtrl::ADS1115::AddressPin addr=PiHWCtrl::ADS1115::AddressPin::GND,
                                   PiHWCtrl::ADS1115::DataRate data_rate=PiHWCtrl::ADS1115::DataRate::DR_128_SPS);

%extend PiHWCtrl::ADS1115 { 
    AnalogInput<float>* conversionAnalogInput(PiHWCtrl::ADS1115::Input input, int dummy=0) {
        return $self->conversionAnalogInput(input).release();
    }
}
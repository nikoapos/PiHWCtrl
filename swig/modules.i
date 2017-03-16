%module(package="PiHWCtrl", directors="1") modules

%include HWInterfaces.i
%include <stdint.i>
    
%{ 
#include <PiHWCtrl/modules/BMP180.h>
%}
// Ignore the methods that use unique_ptr
%ignore PiHWCtrl::BMP180::factory;
%ignore PiHWCtrl::BMP180::rawTemperatureAnalogInput();
%ignore PiHWCtrl::BMP180::temperatureAnalogInput();
%ignore PiHWCtrl::BMP180::rawPressureAnalogInput();
%ignore PiHWCtrl::BMP180::pressureAnalogInput();
%ignore PiHWCtrl::BMP180::altitudeAnalogInput();

%include PiHWCtrl/modules/BMP180.h
        
// Create alternatives for the methods that use unique_ptr
%{
PiHWCtrl::BMP180* BMP180_factory(PiHWCtrl::BMP180::PressureMode mode=PiHWCtrl::BMP180::PressureMode::STANDARD,
                                 float sea_level_pressure=1020) {
    return PiHWCtrl::BMP180::factory(mode, sea_level_pressure).release();
}
%}
PiHWCtrl::BMP180* BMP180_factory(PiHWCtrl::BMP180::PressureMode mode=PiHWCtrl::BMP180::PressureMode::STANDARD,
                                 float sea_level_pressure=1020);

%extend PiHWCtrl::BMP180 { 
    AnalogInput<std::uint16_t>* rawTemperatureAnalogInput(int dummy=0) {
        return $self->rawTemperatureAnalogInput().release();
    }
    AnalogInput<float>* temperatureAnalogInput(int dummy=0) {
        return $self->temperatureAnalogInput().release();
    }
    AnalogInput<std::uint32_t>* rawPressureAnalogInput(int dummy=0) {
        return $self->rawPressureAnalogInput().release();
    }
    AnalogInput<float>* pressureAnalogInput(int dummy=0) {
        return $self->pressureAnalogInput().release();
    }
    AnalogInput<float>* altitudeAnalogInput(int dummy=0) {
        return $self->altitudeAnalogInput().release();
    }
}
        
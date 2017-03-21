%module(package="PiHWCtrl", directors="1") modules

%include HWInterfaces.i
    
%{ 
#include <PiHWCtrl/modules/HCSR04.h>
%}
%ignore PiHWCtrl::HCSR04::HCSR04(std::unique_ptr<Switch> trigger, std::unique_ptr<BinaryInput> echo,
         float max_distance=4., float sound_speed=343);

%include PiHWCtrl/modules/HCSR04.h

%{
PiHWCtrl::HCSR04* HCSR04_factory(PiHWCtrl::Switch* trigger, PiHWCtrl::BinaryInput* echo,
                                 float max_distance=4., float sound_speed=343) {
    return new PiHWCtrl::HCSR04(std::unique_ptr<PiHWCtrl::Switch>(trigger),
                                std::unique_ptr<PiHWCtrl::BinaryInput>(echo),
                                max_distance, sound_speed);
}
%}
PiHWCtrl::HCSR04* HCSR04_factory(PiHWCtrl::Switch* trigger, PiHWCtrl::BinaryInput* echo,
                                 float max_distance=4., float sound_speed=343);
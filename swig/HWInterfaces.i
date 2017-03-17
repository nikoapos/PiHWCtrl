%module(package="PiHWCtrl", directors="1") HWInterfaces

%include <std_shared_ptr.i>
%include <stdint.i>
        
%{
#include <PiHWCtrl/HWInterfaces/BinaryInput.h>
%}
%feature("director") PiHWCtrl::BinaryInput;
%feature("nodirector") PiHWCtrl::BinaryInput::isOff() const;
%include PiHWCtrl/HWInterfaces/BinaryInput.h

%{
#include <PiHWCtrl/HWInterfaces/Switch.h>
%}
%feature("director") PiHWCtrl::Switch;
%feature("nodirector") PiHWCtrl::Switch::turnOn();
%feature("nodirector") PiHWCtrl::Switch::turnOff();
%include PiHWCtrl/HWInterfaces/Switch.h

%{
#include <PiHWCtrl/HWInterfaces/PWM.h>
%}
%feature("director") PiHWCtrl::PWM;
%include PiHWCtrl/HWInterfaces/PWM.h

%{
#include <PiHWCtrl/HWInterfaces/AnalogInput.h>
%}
%feature("director") PiHWCtrl::AnalogInput<std::uint16_t>;
%feature("director") PiHWCtrl::AnalogInput<std::uint32_t>;
%feature("director") PiHWCtrl::AnalogInput<float>;
%include PiHWCtrl/HWInterfaces/AnalogInput.h
%template(AnalogInputUInt16) PiHWCtrl::AnalogInput<std::uint16_t>;
%template(AnalogInputUInt32) PiHWCtrl::AnalogInput<std::uint32_t>;
%template(AnalogInputFloat) PiHWCtrl::AnalogInput<float>;

%shared_ptr(PiHWCtrl::Observer<bool>)
%shared_ptr(PiHWCtrl::Observer<std::uint16_t>)
%shared_ptr(PiHWCtrl::Observer<std::uint32_t>)
%shared_ptr(PiHWCtrl::Observer<float>)
%{
#include <PiHWCtrl/HWInterfaces/Observer.h>
%}
%feature("director") PiHWCtrl::Observer<bool>;
%feature("director") PiHWCtrl::Observer<std::uint16_t>;
%feature("director") PiHWCtrl::Observer<std::uint32_t>;
%feature("director") PiHWCtrl::Observer<float>;
%include PiHWCtrl/HWInterfaces/Observer.h
%template(ObserverBool) PiHWCtrl::Observer<bool>;
%template(ObserverUInt16) PiHWCtrl::Observer<std::uint16_t>;
%template(ObserverUInt32) PiHWCtrl::Observer<std::uint32_t>;
%template(ObserverFloat) PiHWCtrl::Observer<float>;

%{
#include <PiHWCtrl/HWInterfaces/Observable.h>
%}
%feature("director") PiHWCtrl::Observable<bool>;
%feature("director") PiHWCtrl::Observable<std::uint16_t>;
%feature("director") PiHWCtrl::Observable<std::uint32_t>;
%feature("director") PiHWCtrl::Observable<float>;
%feature("nodirector") PiHWCtrl::Observable<bool>::notifyObservers;
%feature("nodirector") PiHWCtrl::Observable<std::uint16_t>::notifyObservers;
%feature("nodirector") PiHWCtrl::Observable<std::uint32_t>::notifyObservers;
%feature("nodirector") PiHWCtrl::Observable<float>::notifyObservers;
%include PiHWCtrl/HWInterfaces/Observable.h
%template(ObservableBool) PiHWCtrl::Observable<bool>;
%template(ObservableUInt16) PiHWCtrl::Observable<std::uint16_t>;
%template(ObservableUInt32) PiHWCtrl::Observable<std::uint32_t>;
%template(ObservableFloat) PiHWCtrl::Observable<float>;
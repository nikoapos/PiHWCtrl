%module(package="PiHWCtrl", directors="1") HWInterfaces

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
%feature("director") PiHWCtrl::AnalogInput;
%include PiHWCtrl/HWInterfaces/AnalogInput.h

%{
#include <PiHWCtrl/HWInterfaces/Observer.h>
%}
%include PiHWCtrl/HWInterfaces/Observer.h
%feature("director") PiHWCtrl::Observer<bool>;
%template(BoolObserver) PiHWCtrl::Observer<bool>;

%{
#include <PiHWCtrl/HWInterfaces/Observable.h>
%}
%include PiHWCtrl/HWInterfaces/Observable.h
%feature("director") PiHWCtrl::Observable<bool>;
%template(BoolObservable) PiHWCtrl::Observable<bool>;
%module(package="PiHWCtrl") controls

%include HWInterfaces.i
        
%shared_ptr(PiHWCtrl::StateChangeFilter<bool>)
%{
#include <PiHWCtrl/controls/StateChangeFilter.h>
%}
%include PiHWCtrl/controls/StateChangeFilter.h
%template(BoolStateChangeFilter) PiHWCtrl::StateChangeFilter<bool>;
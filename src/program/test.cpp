#include <iostream>
#include <PiHWCtrl/pigpio/SmartPigpio.h>

#include<pigpio.h>
 
int main(int argc, char *argv[]){
  
  PiHWCtrl::SmartPigpio a {};
  
  for (unsigned int i = 0; i < 32; ++i) {
    std::cout << "GPIO " << i << " mode: " << gpioGetMode(i) << '\n';
  }
  
  return 0;
}

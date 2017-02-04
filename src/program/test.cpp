#include <iostream>
#include <PiHWCtrl/pigpio/PigpioBinaryInput.h>

#include<pigpio.h>
 
using namespace PiHWCtrl;

int main(int argc, char *argv[]) {
  
  PigpioBinaryInput a {23};
  PigpioBinaryInput b {21};
  
  gpioWrite(21, true);
  
  std::cout << "a is " << a.isOn() << '\n';
  std::cout << "b is " << b.isOn() << '\n';
  
  return 0;
}

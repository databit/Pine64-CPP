# Pine64-CPP
C++ library for Pine64. Based over work of Eric Ptak &lt;trouch@trouch.com>, Stefan Mavrodiev &lt;support@olimex.com>, Kamil Trzcinski &lt;ayufan@ayufan.eu>

Pine64-CPP is object oriented library for access to GPIO, I2C and SPI. Moreover all GPIO pin is mapped.

Example

<pre>
#include "pinout.h"
#include "gpio.h"

using namespace std;
using namespace Pine64;

int main (void) {
  GPIO* man = new GPIO();

  if (man->setup()) {
    cout << "Failed to configure GPIO\n" << endl;
    return 1;
  }

  cout << " Set GPIO pin directions" << endl;
  man->pinMode(PI_GPIO_18, OUTPUT);

  for(int i = 0; i < 20; i++) {
    cout << " Turn Led ON" << endl;
    man->digitalWrite(PI_GPIO_18, HIGH);
    man->delay(1000);
    cout << " Turn Led OFF" << endl;
    man->digitalWrite(PI_GPIO_18, LOW);
    man->delay(1000);
  }

  cout << "Exiting....." << endl;
  return 0;
}
</pre>

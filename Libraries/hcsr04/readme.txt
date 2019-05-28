Arduino C library for HC-SR04 ultrosonic distance sensor.

Installation
--------------------------------------------------------------------------------

To install this library, just place this entire folder as a subfolder in your
Arduino/lib/targets/libraries folder.

When installed, this library should look like:

Arduino/lib/targets/libraries/hcsr04              (this library's folder)
Arduino/lib/targets/libraries/hcsr04/hcsr04.c     (the library implementation file)
Arduino/lib/targets/libraries/hcsr04/hcsr04.h     (the library description file)
Arduino/lib/targets/libraries/hcsr04/keywords.txt (the syntax coloring file)
Arduino/lib/targets/libraries/hcsr04/readme.txt   (this file)

Building
--------------------------------------------------------------------------------

After this library is installed, you just have to start the Arduino application.
You may see a few warning messages as it's built.

To use this library in a sketch, go to the Sketch | Import Library menu and
select hcsr04.  This will add a corresponding line to the top of your sketch:
#include <hcsr04.h>

To stop using this library, delete that line from your sketch.

Geeky information:
After a successful build of this library, a new file named "hcsr04.o" will appear
in "Arduino/lib/targets/libraries/hcsr04". This file is the built/compiled library
code.

If you choose to modify the code for this library (i.e. "hcsr04.c" or "hcsr04.h"),
then you must first 'unbuild' this library by deleting the "hcsr04.o" file. The
new "hcsr04.o" with your code will appear after the next press of "verify"

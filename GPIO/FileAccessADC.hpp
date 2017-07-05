/**
 * Reads the value of an analog sensor by means of reading the current value in
 * it's corresponding file in the Linux file system.
 *
 * This is basically a straight-up copy of the file "readLDR.cpp" from Derek Molloy.
 */
/** Simple LDR Reading Application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#define ADC_PREFIX "/sys/bus/iio/devices/iio:device0/in_voltage"
#define ADC_SUFFIX "_raw"

int readNumberFromFile(std::string filename);
int readAnalog(int number);

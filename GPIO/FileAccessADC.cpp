#include "FileAccessADC.hpp"

using namespace std;

int readNumberFromFile(string filename)
{
	int number;
	fstream fs;
	fs.open(filename.c_str(), fstream::in);
	fs >> number;
	fs.close();
	return number;
}

int readAnalog(int number)
{
	stringstream ss;
	ss << ADC_PREFIX << number << ADC_SUFFIX;
	return readNumberFromFile(ss.str());
}

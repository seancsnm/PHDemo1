#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include "FileAccessADC.hpp"

using namespace std;

int main()
{
	while (true)
	{
		cout << "Waiting for start button to be pressed" << endl;
		cout << "(currently just the file /root/start.txt)" << endl;

		int startVal = 0;
		while ((startVal = readNumberFromFile("/root/start.txt")) == 0)
		{
			this_thread::sleep_for(chrono::milliseconds(100));
		}

		int endVal = 0;
		while ((endVal = readNumberFromFile("/root/end.txt")) == 0)
		{
			int analogValue0 = readAnalog(0);
			cout << analogValue0 << endl;
			this_thread::sleep_for(chrono::milliseconds(100));
		}

		this_thread::sleep_for(chrono::milliseconds(200));
	}

	return 0;
}

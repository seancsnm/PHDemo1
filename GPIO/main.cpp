#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include "FileAccessADC.hpp"
extern "C" {
#include <stdio.h>
#include "BBBio/BBBiolib.h"
#include "BBBio/BBBiolib_ADCTSC.h"
}

#define BTN_HDR 9
#define P_START 11
#define P_STOP 12
#define BUFFER_SIZE 1
#define SAMPLE_SIZE 1 
#define EMG_ADC 0
#define FSR_ADC 1


using namespace std;

int main()
{
	int exitCode;
	int startVal = 0;
	int endVal = 0; 
	int start_btn_val, stop_btn_val;
	unsigned int ADC0_val[BUFFER_SIZE], ADC1_val[BUFFER_SIZE];

	//Initialize GPIO library
	if ((exitCode = iolib_init())) {
		cout << "Error code " << exitCode << endl;
		exit(1);
	}
	cout << "Started iolib\n" << endl;
	
	//Set GPIO inputs
	iolib_setdir(BTN_HDR, P_START, BBBIO_DIR_IN);
	iolib_setdir(BTN_HDR, P_STOP, BBBIO_DIR_IN);
	cout << "button directions set" << endl;
	
	//Initialize ADC library (note that GPIO library must be initialized prior to ADC library)
	if (!(exitCode = BBBIO_ADCTSC_Init())) {
		cout << "ADC initialize error code" << exitCode << endl;
		exit(1);
	}
	
	//Setup ADC device
	BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_TIMER_INT, 160);
	
	//Setup control for ADC channels
	BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN0, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, 0, 1, BBBIO_ADC_STEP_AVG_1, ADC0_val, BUFFER_SIZE);
	BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN1, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, 0, 1, BBBIO_ADC_STEP_AVG_1, ADC1_val, BUFFER_SIZE);
	
	//Enable ADC channels
	BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN0);
	BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN1);

	cout << "Waiting for start button to be pressed" << endl;
	
	while ((startVal = is_low(BTN_HDR, P_START)) == 0) { // poll for start button press
		this_thread::sleep_for(chrono::milliseconds(10));
	}
	cout << "Start button pushed; begin data stream" << endl;
	cout << "FSR \t EMG" << endl;
	
	while ((endVal = is_low(BTN_HDR, P_STOP)) == 0) { // poll for stop button press
		//Collect samples from ADC
		BBBIO_ADCTSC_work(SAMPLE_SIZE);
		printf("%04d \t %04d\n", ADC0_val[0],  ADC1_val[0]);
		//cout << ADC0_val[0] << "\t" << ADC1_val[0] << endl;
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	cout << "Stop button pushed; end data stream" << endl;
	
	//Close GPIO mapping
	if (iolib_free()) exit(-1);
	cout << "iolib closed successfully" << endl;
	
	return 0;
		
	
  /* //Old code
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
	*/
}

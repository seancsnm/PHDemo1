#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include "benutil/range.hpp"
extern "C" {
#include "BBBio/BBBiolib.h"
#include "BBBio/BBBiolib_ADCTSC.h"
#include "argparse/argparse.h"
}

#define BTN_HDR 9
#define P_START 11
#define P_STOP 12
#define BUFFER_SIZE 1
#define SAMPLE_SIZE 1 
#define EMG_ADC 0
#define FSR_ADC 1

using namespace std;

static const string BLOCKING_MODE_BUSY = "busy";
static const string BLOCKING_MODE_TIMED = "timed";
static const char *const usage[] = {
	"analogReadTest [options]",
	NULL
};

class ADC_Val
{
public:
	unsigned int* buffer;
	UIntRange range;

	ADC_Val()
	{
		buffer = (unsigned int*) malloc(sizeof(unsigned int) * BUFFER_SIZE);
		range = UIntRange();
	}

	~ADC_Val()
	{
		free(buffer);
		buffer = NULL;
	}
};

ADC_Val* initAdcInputs(int numInputs, const char *blockingMode, int clockDivider)
{
	int exitCode;
	ADC_Val *ADC_vals = new ADC_Val[numInputs]();

	//Initialize GPIO library
	if ((exitCode = iolib_init())) {
		cout << "Error code " << exitCode << endl;
		exit(1);
	}
	cout << "Started iolib\n" << endl;

	//Initialize ADC library (note that GPIO library must be initialized prior to ADC library)
	if (!(exitCode = BBBIO_ADCTSC_Init())) {
		cout << "ADC initialize error code " << exitCode << endl;
		exit(1);
	}

	//Setup ADC device
	int blockingModeFlag = (BLOCKING_MODE_BUSY.compare(blockingMode) == 0)
		? BBBIO_ADC_WORK_MODE_BUSY_POLLING
		: BBBIO_ADC_WORK_MODE_TIMER_INT;
	BBBIO_ADCTSC_module_ctrl(blockingModeFlag, clockDivider);

	//Setup each device
	for (int i = 0; i < numInputs; i++)
	{
		int BBBIO_ADC_AIN = ADCTSC_getAIN(i);

		//Setup control for ADC channels
		BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, 0, 1, BBBIO_ADC_STEP_AVG_1, ADC_vals[i].buffer, BUFFER_SIZE);

		//Enable ADC channels
		BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN);
	}

	return ADC_vals;
}

ADC_Val* freeAdcInputs(ADC_Val *ADC_vals, int count)
{
	if (ADC_vals)
		delete[] ADC_vals;
	return NULL;
}

chrono::milliseconds getMills()
{
	return chrono::duration_cast< chrono::milliseconds >(
		chrono::system_clock::now().time_since_epoch()
	);
}

int main(int argc, const char *argv[])
{
	// set up and parse arguments
	int numInputs = 1;
	int numSamples = 10000;
	bool measureRange = false;
	const char *blockingMode = NULL;
	int clockDivider = 1;
	int printInterval = 1000;
	struct argparse_option options[] = {
		OPT_HELP(),
		OPT_STRING('b', "blocking", &blockingMode, string("mode of blocking while waiting for analog reads, " + 
			string("can be either \"") + BLOCKING_MODE_BUSY + "\" or \"" + BLOCKING_MODE_TIMED + "\"").c_str()),
		OPT_INTEGER('c', "count", &numInputs, "numer of analog inputs to test"),
		OPT_INTEGER('d', "divider", &clockDivider, "clock divider to the ADC, default is 1"),
		OPT_INTEGER('p', "printInterval", &printInterval, "interval of reads to print a message at, for progress updates"),
		OPT_BOOLEAN('r', "range", &measureRange, "record the range of measured values per each input"),
		OPT_INTEGER('s', "samples", &numSamples, "number of samples to read per analog input"),
		OPT_END()
	};
	struct argparse argparse;
	argparse_init(&argparse, options, usage, 0);
	argparse_describe(&argparse, "Tests analog read speeds on the BeagleBone Black.", "");
	argc = argparse_parse(&argparse, argc, argv);

	// verify arguments are valid
	if (numInputs < 1 || numInputs > 7)
	{
		cerr << "Can't create " << numInputs << " inputs. Must specify between 1 and 7 inputs." << endl;
		exit(1);
	}
	if (numSamples < 1)
	{
		cerr << "There must be at least one sample per input." << endl;
		exit(1);
	}
	blockingMode = (blockingMode == NULL) ? BLOCKING_MODE_BUSY.c_str() : blockingMode;
	if (BLOCKING_MODE_BUSY.compare(blockingMode) != 0 &&
		BLOCKING_MODE_TIMED.compare(blockingMode) != 0)
	{
		cerr << "Blocking mode can only be one of \"" + BLOCKING_MODE_BUSY + "\" or \"" + BLOCKING_MODE_TIMED + "\"!" << endl;
		exit(1);
	}
	if (clockDivider < 1)
	{
		cerr << "Clock divider must be a positive integer." << endl;
		exit(1);
	}
	if (printInterval < 1)
	{
		cerr << "Print interval must be a positive integer." << endl;
		exit(1);
	}

	// initialize the analog inputs and set up their buffers
	ADC_Val* ADC_vals = initAdcInputs(numInputs, blockingMode, clockDivider);

	//////////////////////////
	// Read values!
	//////////////////////////
	
	// read in numSamples samples for each input in numInputs
	cout << "Starting read..." << endl;
	chrono::milliseconds startMills = getMills();
	for (int i = 0; i < numSamples; i++)
	{
		BBBIO_ADCTSC_work(SAMPLE_SIZE);

		if (measureRange)
		{
			for (int ain = 0; ain < numInputs; ain++)
			{
				ADC_vals[ain].range.apply(*ADC_vals[ain].buffer);
			}
		}

		if (i % printInterval == 0 && i > 0)
		{
			cout << "Read " << i << "/" << numSamples << " samples" << endl;
		}
	}
	chrono::milliseconds endMills = getMills();

	//////////////////////////
	// Announce the findings!
	//////////////////////////

	// announce read speed statistics
	double millsEllapsed = (double) (endMills - startMills).count();
	int secs = chrono::duration_cast<chrono::seconds>(endMills - startMills).count();
	double averageReadSpeed = ((double)numSamples * (double)numInputs) / millsEllapsed * (double) 1000;
	cout << "Finished reading " << numSamples << " samples in " << secs << " seconds." << endl;
	cout << "    (" << (endMills - startMills).count() << " mills)" << endl;
	cout << "Average read speed: " << averageReadSpeed << " samples/second" << endl;

	// announce measured ranges
	if (measureRange)
	{
		for (int ain = 0; ain < numInputs; ain++)
		{
			cout << "    measured range for input " << (ain + 1) << ": " << ADC_vals[ain].range << endl;
		}
	}
	
	//Close GPIO mapping
	ADC_vals = freeAdcInputs(ADC_vals, numInputs);
	if (iolib_free()) exit(-1);
	cout << "iolib closed successfully" << endl;
	
	return 0;
}
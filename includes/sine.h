#ifndef _SINE_H_
#define _SINE_H_
#include "oscillator.h"
using namespace std;

class Sine : public Oscillator {
	public:
		Sine(double samplerate, double frequency);
		~Sine();

		void calculate();
};

#endif

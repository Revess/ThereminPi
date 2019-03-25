#ifndef _OSCILLATOR_H_
#define _OSCILLATOR_H_
#include "math.h"
#include <iostream>
using namespace std;

#define TWOPI 6.28318530717959
#define DELTA_TIME 8

class Oscillator {
	public:
		Oscillator(double samplerate, double frequency);
		virtual ~Oscillator();

		double getSample();
		void setSample();
		
		virtual void calculate();

		void setFrequency(double frequency);
		void setAmplitude(double amplitude);

	protected:
		double freqArr [DELTA_TIME];
		int readHead = 0;
		int writeHead;
		double samplerate;
		double amplitude;
		double frequency;
		double phase;
		double sample;
		double amp;
};

#endif

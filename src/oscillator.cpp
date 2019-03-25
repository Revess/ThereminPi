#include "../includes/oscillator.h"
using namespace std;

Oscillator::Oscillator(double samplerate, double frequency) :
	samplerate(samplerate), 
	frequency(frequency), 
	phase(0.0), 
	sample(0.0),
	amp(0.0){
	for(int i = 0; i < DELTA_TIME; i++){
		freqArr[i] = 0;
	}
}

Oscillator::~Oscillator(){
}

double Oscillator::getSample(){
	return sample * amp;
}

void Oscillator::setSample(){
	phase+=frequency/samplerate;
	if(phase>=1) {
		phase-=1.0;
	}
	calculate();
}

void Oscillator::setFrequency(double freq){
	if(freq > 0.0 && freq < 0.5 * samplerate){
		writeHead = (((readHead-1)%DELTA_TIME)+DELTA_TIME)%DELTA_TIME;
		this->frequency = (freq + freqArr[readHead]) / 2.0;
		freqArr[writeHead] = freq;
		readHead++;
		readHead = readHead % DELTA_TIME;
	}
}

void Oscillator::calculate(){
}

void Oscillator::setAmplitude(double amplitude){
	this->amp = amplitude;
}

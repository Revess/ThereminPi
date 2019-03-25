#include "../includes/sine.h"

Sine::Sine(double samplerate, double frequency) : 
	Oscillator(samplerate, frequency){	
}

Sine::~Sine(){

}

void Sine::calculate(){
	sample = sin(phase * TWOPI);
}

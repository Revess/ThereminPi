#include <thread>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <unistd.h>
#include "../includes/jack_module.h"
#include "../includes/flick.h"
#include "../includes/sine.h"

using namespace std;

#define FREQ_MAP (1250.0/65000.0)
#define AMP_MAP (1.0/65000.0)

double frequency = 65;
double offsetPitch = 65;
double zPos;
double zPosOld = 0;
double xPos;
double xPosOld = 0;
double offsetVol = 1.0;
long int angle;
bool tap = false;
bool changeFreq = false;
bool timeSet = false;
bool touchCenter;
bool editPitch = false;
bool editVol = false;
bool isTapped = false;
time_t tappedst;
time_t tappedct;
time_t ct;
time_t st;

void onTouchCb( FlickTouch_t touch, uint16_t period ){
	switch (touch){
	case TOUCH_SOUTH_ELECTRODE:
		break;
	case TOUCH_WEST_ELECTRODE:
		break;
	case TOUCH_NORTH_ELECTRODE:
		touchCenter = true;
		break;
	case TOUCH_EAST_ELECTRODE:
		break;
	case TOUCH_CENTER_ELECTRODE:
		break;
	case TAP_SOUTH_ELECTRODE:
		break;
	case TAP_WEST_ELECTRODE:
		break;
	case TAP_NORTH_ELECTRODE:
		if(!isTapped){
			if(tap){
				tap = false;
			} else {
				tap = true;
			}
			time(&tappedst);
			isTapped = true;
		}
		break;
	case TAP_EAST_ELECTRODE:
		break;
	case TAP_CENTER_ELECTRODE:
		break;
	case DOUBLE_TAP_SOUTH_ELECTRODE:
		break;
	case DOUBLE_TAP_WEST_ELECTRODE:
		break;
	case DOUBLE_TAP_NORTH_ELECTRODE:
		break;
	case DOUBLE_TAP_EAST_ELECTRODE:
		break;
	case DOUBLE_TAP_CENTER_ELECTRODE:
		break;
	default:
		break;
		}
}

void airwheelCb(int32_t a) {
	angle = a;
}

void onGestureCb(FlickGesture_t gesture, FlickGestureClass_t gestClass, bool isEdgeFlick, bool inProgress){
	switch (gesture){ 
	case GESTURE_GARBAGE_MODEL:
		break;
	case FLICK_WEST_TO_EAST:
		break;
	case FLICK_EAST_TO_WEST:
		break;
	case FLICK_SOUTH_TO_NORTH:
		editPitch = true;
		editVol = false;
		break;
	case FLICK_NORTH_TO_SOUTH:
		editVol = true;
		editPitch = false;
		break;
	default:
		break;
	}
}

void onXYZCb(uint16_t x, uint16_t y, uint16_t z){
	zPos = z;
	xPos = x;
}

int main(int argc,char **argv)
{
	system("jackd -d alsa -P");
	usleep(500000);
	Flick flick(27,17);
	flick.touchCallback = onTouchCb;
	flick.airWheelCallback = airwheelCb;
	flick.xyzCallback = onXYZCb;
	flick.gestureCallback = onGestureCb;

	int ret = flick.SetRuntimeParameter(0xa1, 0x1f, 0xFFFFFFFF);
	ret = flick.SetRuntimeParameter(0x80, 0x1b, 0x1F);

	JackModule jack;

	jack.init("threminPi");
	double samplerate = jack.getSamplerate();

	Sine sine(samplerate, frequency);

	Oscillator *oscillator = &sine;

	jack.onProcess = [&oscillator](jack_default_audio_sample_t *outBuf, 
						jack_nframes_t nframes) {
		double amplitude = 0.5;
		for(unsigned int i = 0; i < nframes; i++) {
			outBuf[i] = amplitude * oscillator->getSample();
			oscillator->setSample();
		}

		return 0;
	};

	jack.autoConnect();

	while (true){
		flick.Poll();
		if(!tap){
			if(zPos != zPosOld){
				frequency = zPos * FREQ_MAP + offsetPitch; 
				oscillator->setFrequency(frequency);
				zPosOld = zPos;
			}
			if(xPos != xPosOld){
				oscillator->setAmplitude((xPos * AMP_MAP) * offsetVol);
				xPosOld = xPos;
			}
		} else {
			if(zPos > 0){
				if(touchCenter == true || timeSet == true){
					touchCenter = false;
					timeSet = false;
				}
				if(editVol){
					offsetVol = fmod((fabs(angle) / 500.0), 2.0);
					oscillator->setFrequency(220);
					oscillator->setAmplitude(offsetVol);
				} else if(editPitch){
					offsetPitch = fmod(fabs(angle), 600) + 65;
					frequency = offsetPitch;
					oscillator->setFrequency(frequency);
					oscillator->setAmplitude(1.0);
				}
			} else if(touchCenter){
				oscillator->setAmplitude(0);
				if(!timeSet){
					timeSet = true;
					time(&st);
				} 
				if(difftime(ct,st) >= 3){
					jack.end();
					break;
				} else {
					time(&ct);
				}
			} else {
				oscillator->setAmplitude(0);
			}
		} 
		if(isTapped){
			if(difftime(tappedct,tappedst) > 1){
				isTapped = false;	
			}
			time(&tappedct);
		}
		usleep(1);
	}
	system("pkill jackd");
	usleep(250000);
	system("sudo shutdown now");

	return 0;
}

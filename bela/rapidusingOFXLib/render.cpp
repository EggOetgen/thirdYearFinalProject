#include <Bela.h>
#include "maximilian.h"


//#include "rapidLib/regression.h"
#include "drumSynth.h"
#include "rapidBrain.h"
#include "drumBrain.h"
//maximilian stuff
maxiClock benKlock;
int playHead;

drumSynth test;
drumBrain testBrain;
//---------------

//sensor stuff---
int pitchPot = 0;
int tonePot  = 1;
int volPot  = 3;


float frequency;
float tone;

float gPhase;
float gInverseSampleRate;
int gAudioFramesPerAnalogFrame;
//---------------

bool setup(BelaContext *context, void *userData)
{
	
	// Check if analog channels are enabled
	if(context->analogFrames == 0 || context->analogFrames > context->audioFrames) {
		rt_printf("Error: this example needs analog enabled, with 4 or 8 channels\n");
		return false;
	}

	// Check that we have the same number of inputs and outputs.
	if(context->audioInChannels != context->audioOutChannels ||
			context->analogInChannels != context-> analogOutChannels){
		printf("Error: for this project, you need the same number of input and output channels.\n");
		return false;
	}
	
		// Useful calculations
	gAudioFramesPerAnalogFrame = context->audioFrames / context->analogFrames;
	gInverseSampleRate = 1.0 / context->audioSampleRate;
	gPhase = 0.0;

	pinMode(context, 0, P8_08, INPUT);
	pinMode(context, 0, P8_10, INPUT);
	pinMode(context, 0, P8_07, OUTPUT);

	playHead = 0;
	
	tone = 1.0;
	frequency = 0.5;
	
    benKlock.setTempo(128      );
    benKlock.setTicksPerBeat(4);
	
	return true;
}

void render(BelaContext *context, void *userData)
{
	
for(unsigned int n = 0; n < context->audioFrames; n++) {
	
	int status1=digitalRead(context, 0, P8_08); //read the value of the button
	int status2=digitalRead(context, 0, P8_10); //read the value of the button
		digitalWriteOnce(context, n, P8_07, 0);
	if(status2 ==0 || status1 ==0){
		
	digitalWriteOnce(context, n, P8_07, 1); //write the status to the LED
	}
		
		if(!(n % gAudioFramesPerAnalogFrame)) {
			// On even audio samples: read analog inputs and update frequency and amplitude
		//	if(analogRead(context, n/gAudioFramesPerAnalogFrame, pitchPot) != frequency){
			frequency = analogRead(context, n/gAudioFramesPerAnalogFrame, pitchPot);
			//testBrain.voices[0].frequency 
			benKlock.setTempo( ::map(frequency, 0, 1, 30, 400));
		//	}
			//tone = map(analogRead(context, n/gAudioFramesPerAnalogFrame, pitchPot), 0, 1, , 100);
		//	testBrain.voices[0].frequency = ::map(frequency, 0, 1, 30, 100);;
		//	tone = analogRead(context, n/gAudioFramesPerAnalogFrame, tonePot);
			
		//	testBrain.voices[0].oscMix = tone;
		}
	  benKlock.ticker();

        if (benKlock.tick) {
            playHead++;
            playHead%=16;
            testBrain.step(playHead);
		//	if(test.sequence[playHead] == 1)		
        //	  test.trigger();
        }

   float out = testBrain.play() * analogRead(context, n/gAudioFramesPerAnalogFrame, volPot); ;
	
		for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
			// Two equivalent ways to write this code

			// The long way, using the buffers directly:
			// context->audioOut[n * context->audioOutChannels + channel] = out;

			// Or using the macros:
			audioWrite(context, n, channel, out);
		}
	}
}

void cleanup(BelaContext *context, void *userData)
{

}
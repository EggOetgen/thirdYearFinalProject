#include <Bela.h>
#include "maximilian.h"

//#include "rapidLib/regression.h"
#include "drumSynth.h"
#include "rapidBrain.h"
#include "drumBrain.h"

#define NUM_OF_BUTTONS 2;


//maximilian stuff
maxiClock benKlock;
int playHead;

drumSynth test;
drumBrain testBrain;
//---------------

//sensor stuff---
int pitchPot = 0;
//int tonePot  = 1;
int volPot  = 3;
int pulsePot  = 5;
int lengthPot  = 1;

int play;
uint8_t pulse;
uint8_t length;
float frequency;
float tone;

int buttonStates[4];
int buttonInputs[4];
float gPhase;
float gInverseSampleRate;
int gAudioFramesPerAnalogFrame;
//---------------

AuxiliaryTask gGenerateEuclideanSequence;
void generateEuclideanSequence(void*);

AuxiliaryTask gCheckButtons;
void checkButtons(void*);
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
	
	pinMode(context, 0, P8_07, INPUT);
	pinMode(context, 0, P8_08, INPUT);
	pinMode(context, 0, P8_09, INPUT);
	pinMode(context, 0, P8_10, INPUT);
	
	
	playHead = 0;
	play = 0;
	
	tone = 1.0;
	frequency = 0.5;
	
    benKlock.setTempo(128      );
    benKlock.setTicksPerBeat(4);
	
	if((gGenerateEuclideanSequence= Bela_createAuxiliaryTask(&generateEuclideanSequence, 85, "generating-sequences")) == 0)
		return false;
		
	if((gCheckButtons= Bela_createAuxiliaryTask(&checkButtons, 85, "checking-buttons")) == 0)
		return false;

	for (uint8_t i = 0; i < 4; i++){
		
		buttonInputs[i] = 0;
		buttonStates[i] = 0;
	}
	
	return true;
}

void render(BelaContext *context, void *userData)
{
 int debounce = 0;
 
 int debounceDelay = 50;
for(unsigned int n = 0; n < context->audioFrames; n++) {
	//around five millisecond samplerate for inpiuts
	if(!(n % 220)){
		if(digitalRead(context, 0, P8_07) ==1)
			debounce = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounce > debounceDelay){
	    		buttonInputs[0]=digitalRead(context, 0, P8_07); //read the value of the button
	    }
		
		

	buttonInputs[1]=digitalRead(context, 0, P8_08); //read the value of the button
	buttonInputs[2]=digitalRead(context, 0, P8_09); //read the value of the button
	buttonInputs[3]=digitalRead(context, 0, P8_10); //read the value of the button
	Bela_scheduleAuxiliaryTask(gCheckButtons);
		
	if(buttonStates[0] == 1){
	//	debounce = milis();
	
	// 		//	Bela_scheduleAuxiliaryTask(gGenerateEuclideanSequence);
	 			if(play == 1)
	 				play = 0;
	 			else{
	 				play = 1;
	 				playHead = 0;
	 				
	 			}
	 	 }
	 	 
		if(!play){
			for(uint8_t i = 1; i < 4; i++){
				if(buttonStates[i] == 1){
					//	Bela_scheduleAuxiliaryTask(gGenerateEuclideanSequence);
					testBrain.voices[i-1].trigger();
		
				}
			}
		}
	}
	// if(buttonStates[2] == 1){
	// 		//	Bela_scheduleAuxiliaryTask(gGenerateEuclideanSequence);
	// 			testBrain.voices[1].trigger();
//		 }
		//digitalWriteOnce(context, n, P8_07, 0);
//	if(status2 ==0 || status1 ==0){
		
//	digitalWriteOnce(context, n, P8_07, 1); //write the status to the LED
//	}
		
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
	
		// pulse = map(analogRead(context, n/gAudioFramesPerAnalogFrame, pulsePot), 0, 1, 0, 8);
		// length = map(analogRead(context, n/gAudioFramesPerAnalogFrame, lengthPot), 0, 1, 8, 16);
		// int status=digitalRead(context, 0, P8_08);
		 //if(status != 0){
			// //	Bela_scheduleAuxiliaryTask(gGenerateEuclideanSequence);
			// 	play = !play;
		 //}
		//testBrain.euclidSeq(length, pulse);
		 //int status=digitalRead(context, 0, P8_1o);
		}
	 
  if(play == 1){
	  benKlock.ticker();

        if (benKlock.tick) {
           
          
            testBrain.step(playHead);
             playHead++;
            playHead%=16;
		//	if(test.sequence[playHead] == 1)		
        //	  test.trigger();
        }
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

void checkButtons(void*){

for( int i = 0; i < 4; i++){

	if(buttonInputs[i] == 1 && buttonStates[i] ==0){
		
		buttonStates[i] = 1;
	}else if (buttonInputs[i] == 1 && buttonStates[i] ==1)
		buttonStates[i] = 2;
	else if( buttonInputs[i] == 0)
		buttonStates[i] = 0;
//	int status1=digitalRead(context, 0, P8_08); //read the value of the button

}

}
void generateEuclideanSequence (void*){
	
		uint8_t l = uint8_t(length);
		uint8_t p = uint8_t(pulse);
		
		testBrain.euclidSeq(l, p);
	
}

void cleanup(BelaContext *context, void *userData)
{

}
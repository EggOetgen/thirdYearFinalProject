#include <Bela.h>
#include "maximilian.h"

#define NUM_OF_BUTTONS 2;
//#include "rapidLib/regression.h"
#include "drumSynth.h"
#include "rapidBrain.h"
#include "drumBrain.h"
#include <math_neon.h>

maxiFilter inputfilter;

//maximilian stuff
maxiClock benKlock;
int playHead;

drumSynth test;
drumBrain testBrain;
rapidBrain testRapid;

rapidBrain enveRapid, timbreRapid;

//---------------

//sensor stuff---
int pitchPot = 0;
//int tonePot  = 1;
int volPot  = 2;
int pulsePot  = 3;
int lengthPot  = 1;

int buttonsPinOneToFour = 4;
int buttonsPinFiveToEight = 5;


int play;
bool shift;
bool selectMode;
uint8_t pulse;
uint8_t length;
float frequency;
float tone;
float pfrequency;
float ptone;
 
 float	buttonPressOneToFour = 0.0;
 float	buttonPressFiveToEight = 0.0;
int buttonStates[11];
int buttonInputs[11];
float gPhase;
float gInverseSampleRate;
int gAudioFramesPerAnalogFrame;
//---------------

AuxiliaryTask gGenerateEuclideanSequence;
void generateEuclideanSequence(void*);

AuxiliaryTask gCheckButtons;
void checkButtons(void*);
AuxiliaryTask gPrintData;
void printData(void*);

AuxiliaryTask gRunRegression;
void runRegression(void*);


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
	pinMode(context, 0, P8_16, OUTPUT);
	// pinMode(context, 0, P8_07, INPUT);
	// pinMode(context, 0, P8_08, INPUT);
	// pinMode(context, 0, P8_09, INPUT);
	// pinMode(context, 0, P8_10, INPUT);
	// pinMode(context, 0, P8_11, INPUT);
	// pinMode(context, 0, P8_12, INPUT);
	// pinMode(context, 0, P8_15, INPUT);
	
	
	playHead = 0;
	play = 0;
	shift = false;
//	selectMode = false
	tone = 1.0;
	frequency = 0.5;
	ptone = 1.0;
	pfrequency = 0.5;
    benKlock.setTempo(128      );
    benKlock.setTicksPerBeat(4);
	
	if((gGenerateEuclideanSequence= Bela_createAuxiliaryTask(&generateEuclideanSequence, 99, "generating-sequences")) == 0)
		return false;
		
	if((gCheckButtons= Bela_createAuxiliaryTask(&checkButtons, 85, "checking-buttons")) == 0)
		return false;
	
		if((gPrintData= Bela_createAuxiliaryTask(&printData, 86, "printing")) == 0)
		return false;
		
		if((gRunRegression= Bela_createAuxiliaryTask(&runRegression, 50, "regressing")) == 0)
		return false;	

	for (uint8_t i = 0; i < 4; i++){
		
		buttonInputs[i] = 0;
		buttonStates[i] = 0;
	}
	// for(int i = 0; i < 10; i++){
	// float r =	(rand()*(1.0 / (RAND_MAX + 1.0))*0.1);
	// testRapid.addTrainingExample(0, 0.81-r, 0.81-r, 60, 0.2, 1.5, 0, 400);
	// testRapid.addTrainingExample(0, 0.0+r, 0.0+r, 70, 1.0,2, 400, 1000);
	// testRapid.addTrainingExample(0, 0.81-r, 0.0+r, 40, 0.0, 4.5, 0, 2000);
	// testRapid.addTrainingExample(0, 0.0+r, 0.81-r, 60, 0.5,1.4,50,   150);
	// testRapid.addTrainingExample(0, 0.5+r, 0.81-r, 55, 1,0.01,0, 80);
	// testRapid.addTrainingExample(0, 0.0, 0.5, 30, 0.2, 1.5, 0,  100);
	// testRapid.addTrainingExample(0, 0.81-r, 0.5-r, 80, 0.25, 1.3, 20, 500);
	// testRapid.addTrainingExample(0, 0.5, 0.5, 333, 0.2, 1.5, 0.0, 100);
	// testRapid.myRegression.train(testRapid.trainingSet);
	
	// std::cout << "completed " << i << " Rounds of training" <<std::endl; 
	// }
	
// 	for(int i = 0; i < 10; i++){
// 	float r =	(rand()*(1.0 / (RAND_MAX + 1.0))*0.1);
// 					//freq, pma, gain_, om, tm, nm, oa, oh, oR, pa, pr, na, nr
// 	// voices[0].setAll(40., 4.184, 1., 0.692, 1., 0., 0., 1., 504., 0., 84., 0., 0. );
// 	// updateVoice(0, 40., 4.184, 1., 0.692, 1., 0., 0., 1., 504., 0., 84., 0., 0. );
// 	// updateVoice(1, 200., 3.6, 1., 0.14, 0.14, 0.85, 0., 1., 428., 0., 100., 0., 228. );
// 	// updateVoice(2, 0., 0, 1., 0., 0., 1., 0., 1., 504., 0., 84., 0., 29. );
// 	// updateVoice(3, 80., 4.184, 1., 0.692, 1., 0., 0., 1., 504., 0., 84., 0., 0. );
		
// 	//f1, t1,osc1, nm1, pm1, a1, r1, na1, nr1
// 	testRapid.addTrainingExampleWholeKit(0, 0.81-r, 0.81-r, 
// 										61.74, 0.2, 0.001, 1.5, 0, 400, 0.0, 10,
// 										392, 0.2,  0.7, 1.5, 0, 100, 0, 200,
// 										98, 0.2,  0.1, 4.0, 100, 100, 0, 2,
// 										98, 0.1,  1.0, 1, 0, 51, 0, 50);
// 	// //f1, t1, nm1, pm1, a1, r1, na1, nr1
// 	testRapid.addTrainingExampleWholeKit(0, 0.0+r, 0.0+r, 
// 										61.75, 0.8,  0.0, 0.5, 50, 500, 0.1, 1,
// 										69.3, 0.8, 0.0, 0.5, 50, 500, 0.1, 1,
// 										73.42, 0.8, 0, 0.5, 50, 500, 0.1, 1,
// 										82.41, 0.8,  0.1, 0.5, 50, 500, 0.1, 100);

// 	// 									//f1, t1, nm1, pm1, a1, r1, na1, nr1
// // //	testRapid.addTrainingExampleWholeKit(0, 0.0+r, 0.00+r,
// // 										61.74, 0.2, 0.0, 1.5, 0, 200, 0, 0,
// // 										69.3, 0.2, 0.0, 1.5, 0, 200, 0, 0,
// // 										73.42, 0.2, 0.0, 1.5, 0, 200, 0, 0,
// // 										82.41, 0.2, 0.0, 1.5, 0, 200, 0, 0);
// 																			//f1, t1, nm1, pm1, a1, r1, na1, nr1
// 	// testRapid.addTrainingExampleWholeKit(0, 0.5+r, 0.5+r,
// 	// 									61.74, 0.2, 0.0, 1.5, 0, 200, 0, 0,
// 	// 									69.3, 0.2, 0.0, 1.5, 0, 200, 0, 0,
// 	// 									73.42, 0.2, 0.0, 1.5, 0, 200, 0, 0,
// 	// 									82.41, 0.2, 0.0, 1.5, 0, 200, 0, 0);
// 	// testRapid.addTrainingExample(0, 0.0+r, 0.0+r, 70, 1.0,2, 400, 1000);
// 	// testRapid.addTrainingExample(0, 0.81-r, 0.0+r, 40, 0.0, 4.5, 0, 2000);
// 	// testRapid.addTrainingExample(0, 0.0+r, 0.81-r, 60, 0.5,1.4,50,   150);
// 	// testRapid.addTrainingExample(0, 0.5+r, 0.81-r, 55, 1,0.01,0, 80);
// 	// testRapid.addTrainingExample(0, 0.0, 0.5, 30, 0.2, 1.5, 0,  100);
// 	// testRapid.addTrainingExample(0, 0.81-r, 0.5-r, 80, 0.25, 1.3, 20, 500);
// 	// testRapid.addTrainingExample(0, 0.5, 0.5, 333, 0.2, 1.5, 0.0, 100);
// 	testRapid.myRegression.train(testRapid.trainingSet);
	
// 	std::cout << "completed " << i+1 << " Rounds of training" <<std::endl; 
// 	}
// 		std::cout << "training complete, device running" <<std::endl; 
// 	return true;
vector<double> drumKitLong = { 32.7, 4.184,  1.,  0.692,     0.9, 0.00001,   1,50., 5000., 30,  200., 0.1,   400.,
						  174.615,    5, 0.9,       0.5,    0.14,    0.85, 0.5, 1.5,   200., 0.1,  100.,  0.,   228.,
					 	     98.,     2,  0.,       0.2,    0.2,  0.001, 0.0, 1., 504., 0.,  84.5, 0.5,    500.,
						   130.81, 4.184, 1.,    0.692,    0.4,     0.99, 0.1, 1., 504., 0.,  84.5, 0.5,   22. 
				  
};
vector<double> drumKit = { 32.7,  4.184, 1.,   0.2,  0.0, 0.00002, 0.1,   2., 100., 0.,  84., 0.,   228.,
						  174.61,   3.6, 1.,   0.14, 0.14,    0.85, 0.1, 1., 428., 0., 100., 0.,   28.,
					 	     98.6,     0, 1.,     1.,  0.5,  0.0001, 0.0, 1., 504., 0.,  84., 0.,    50.,
						   130.81, 4.184, 1., 0.2,  0.0,     1.0, 0.1, 1., 504., 0.,  84., 0.,   228. 
				  
};
	//freq, pma, gain_, om, tm, nm, oa, oh, oR, pa, pr, na, nr
vector<double> major = { 261.63,   0.1, 0.9, 0, 1., 0, 50, 100, 500, 50, 500, 50, 500,
						 349.23,   0.1, 0.9, 0, 1., 0, 50, 100, 500, 50, 500, 50, 500,
				     	  	392,   0.1, 0.9, 0, 1., 0, 50, 100, 500, 50, 500, 50, 500,
						523.25,    0.1, 0.9, 0, 1., 0, 50, 100, 500, 50, 500, 50, 500
				  
};

// vector<double> drumKitEnvs = {  0.1,   1., 10000.,   0.,84., 0.,   228.,
// 							 	0., 1., 428.,     0., 100., 0., 228.,
// 					 	        0., 1., 504.,     0.,  84., 0.,    50.,
// 						        0., 1., 504.,     0.,  84., 0.,   228. 
				  
// };
// vector<double> drumKitEnvs2 = {  100,   1., 100., 200,  150., 0.,   20.,
// 								 200., 428., 0., 100., 0.,   228.,
// 					 	    	 300, 504., 0.,  84., 0.,    50.,
// 						   		 400, 504., 0.,  84., 0.,   228. 
				  
// };
// vector<double> drumKitTimbres = { 32.7, 4.184, 1., 0.2,  0.9, 0.00001
// 						  174.61,   3.6, 1.,  0.14, 0.14,    0.85, 
// 					 	     98.,     0, 1.,    1.,  0.5,  0.0001, 
// 						   130.81, 4.184, 1., 0.692,  0.0,     1.0
				  
// };

	//freq, pma, gain_, om, tm, nm, oa, oh, oR, pa, pr, na, nr

	for(int i = 0; i < 10; i++){
	float r =	(rand()*(1.0 / (RAND_MAX + 1.0))*0.1);
	
	testRapid.addTrainingExampleVector(0.81-r, 0.81-r, drumKitLong);
	testRapid.addTrainingExampleVector(0.41-r, 0.41-r, drumKit);
	// //f1, t1, nm1, pm1, a1, r1, na1, nr1
	testRapid.addTrainingExampleVector(0.0+r, 0.0+r, major);


	testRapid.myRegression.train(testRapid.trainingSet);
	
	std::cout << "completed " << i+1 << " Rounds of training" <<std::endl; 
	}
		std::cout << "training complete, device running" <<std::endl; 
	return true;
}

void render(BelaContext *context, void *userData)
{
 int debouncePLAY =  0;
 int debounceSHIFT = 0;
 int debounceSELECT = 0;
 int debounces[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 int debounceDelay = 50;
for(unsigned int n = 0; n < context->audioFrames; n++) {


	
		if(!(n % gAudioFramesPerAnalogFrame)) {
			// On even audio samples: read analog inputs and update frequency and amplitude
		//	if(analogRead(context, n/gAudioFramesPerAnalogFrame, pitchPot) != frequency){
			frequency = analogRead(context, n/gAudioFramesPerAnalogFrame, 1);
			frequency =	floorf_neon(frequency * 100.) / 100;
			//testBrain.voices[0].frequency 
			benKlock.setTempo( ::map(analogRead(context, n/gAudioFramesPerAnalogFrame, 0), 0, 1, 30, 400));
		//	}
			//tone = map(analogRead(context, n/gAudioFramesPerAnalogFrame, pitchPot), 0, 1, , 100);
		//	testBrain.voices[0].frequency = ::map(frequency = analogRead(context, n/gAudioFramesPerAnalogFrame, 1), 0, 1, 30, 100);
			tone = analogRead(context, n/gAudioFramesPerAnalogFrame, 3);
		tone =	floorf_neon(tone * 100.) / 100;
	if((tone != ptone) || (frequency != pfrequency)){
				std::vector<double> regressionOutput;
				
	//	regressionOutput=	testRapid.addRunExample(frequency, tone);
		Bela_scheduleAuxiliaryTask(gRunRegression);
	//	cout  <<"poo" <<endl;
	//freq, pma, gain_, om, tm, nm, oa, oh, oR, pa, pr, na, nr
		 //testBrain.voices[testBrain.selectedVoice].frequency =regressionOutput[0];
		 //testBrain.voices[testBrain.selectedVoice].oscMix =regressionOutput[1];
		 //testBrain.voices[testBrain.selectedVoice].pitchModAmount = regressionOutput[2];
		 //testBrain.voices[testBrain.selectedVoice].setOscEnv( regressionOutput[3], 1, 1, regressionOutput[4]);
//	f1, t1, nm1, pm1, a1, r1, na1, nr1
		//   testBrain.voices[0].frequency =regressionOutput[0];
		// // testBrain.voices[0].oscMix =regressionOutput[1];
		//  //testBrain.voices[0].noiseMix =regressionOutput[2];
		//  //testBrain.voices[0].pitchModAmount = regressionOutput[3];
		//  //testBrain.voices[0].setOscEnv( regressionOutput[4], 1, 1, regressionOutput[5]);
		//  //testBrain.voices[0].setNoiseEnv( regressionOutput[6], 1, 1, regressionOutput[7]);
		
		//  testBrain.voices[1].frequency =regressionOutput[8];
		//  //testBrain.voices[1].oscMix =regressionOutput[9];
		//  //testBrain.voices[1].noiseMix =regressionOutput[10];
		//  //testBrain.voices[1].pitchModAmount = regressionOutput[11];
		//  //testBrain.voices[1].setOscEnv( regressionOutput[12], 1, 1, regressionOutput[13]);
		//  //testBrain.voices[1].setNoiseEnv( regressionOutput[14], 1, 1, regressionOutput[15]);
		
		//  testBrain.voices[2].frequency =regressionOutput[16];
		//  testBrain.voices[2].oscMix =regressionOutput[1];
		//  testBrain.voices[2].noiseMix =regressionOutput[2];
		//  testBrain.voices[0].pitchModAmount = regressionOutput[3];
		//  testBrain.voices[0].setOscEnv( regressionOutput[4], 1, 1, regressionOutput[5]);
		//  testBrain.voices[0].setNoiseEnv( regressionOutput[6], 1, 1, regressionOutput[7]);
	}
		//	Bela_scheduleAuxiliaryTask(gRunRegression);
		//	testBrain.voices[0].oscMix = tone;
		// testBrain.voices[0].frequency =regressionOutput[0];
		// testBrain.voices[0].oscMix =regressionOutput[1];
	
		// pulse = map(analogRead(context, n/gAudioFramesPerAnalogFrame, pulsePot), 0, 1, 0, 8);
		// length = map(analogRead(context, n/gAudioFramesPerAnalogFrame, lengthPot), 0, 1, 8, 16);
		// int status=digitalRead(context, 0, P8_08);
		 //if(status != 0){
			// //	Bela_scheduleAuxiliaryTask(gGenerateEuclideanSequence);
			// 	play = !play;
		 //}
		//testBrain.euclidSeq(length, pulse);
		 //int status=digitalRead(context, 0, P8_1o);
		
		buttonPressOneToFour = analogRead(context, n/gAudioFramesPerAnalogFrame, buttonsPinOneToFour);
	
	 	 if (buttonPressOneToFour >0.209){
			debounces[0] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[0] > debounceDelay){
	    	buttonInputs[3] = 1;
	    	buttonInputs[4] = 1;
	    	buttonInputs[5] = 1;
	    	buttonInputs[6] = 1;
	    }
		//	buttonInputs[4] = 1;
			
		}else if(buttonPressOneToFour >0.204&& buttonPressOneToFour <0.209){
		 
			debounces[1] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[1] > debounceDelay){
	    	buttonInputs[3] = 1;
	    	buttonInputs[4] = 1;
	    	buttonInputs[5] = 1;
	    }
	    }else if(buttonPressOneToFour >0.195&& buttonPressOneToFour <0.2){
		 
			debounces[2] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[2] > debounceDelay){
	    	buttonInputs[3] = 1;
	    	buttonInputs[4] = 1;
	    	
	    }
	    }else if(buttonPressOneToFour >0.15&& buttonPressOneToFour <0.18){
		 
			debounces[3] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[3] > debounceDelay){
	    	buttonInputs[3] = 1;
	    	buttonInputs[5] = 1;
	    	buttonInputs[6] = 1;
	    }
	    }else if(buttonPressOneToFour >0.154&& buttonPressOneToFour < 0.16){
		 
			debounces[4] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[4] > debounceDelay){
	    	
	    	buttonInputs[3] = 1;
	    	buttonInputs[5] = 1;
	    }
	    }else if(buttonPressOneToFour >0.148&& buttonPressOneToFour <0.152){
		 
			debounces[5] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[5] > debounceDelay){
	    	buttonInputs[3] = 1;
	    	
	    	buttonInputs[6] = 1;
	    }
	    }else if(buttonPressOneToFour >0.14&& buttonPressOneToFour <0.149){
		 
			debounces[6] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[6] > debounceDelay){
	    	buttonInputs[3] = 1;
	    		digitalWrite(context, n, P8_16, 1);
	    }
			
		}else if(buttonPressOneToFour >0.093&& buttonPressOneToFour <0.1){
		 
			debounces[7] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[7] > debounceDelay){
	    	buttonInputs[4] = 1;
	    	buttonInputs[5] = 1;
	    	buttonInputs[6] = 1;
	    }
	    }else if(buttonPressOneToFour >0.087&& buttonPressOneToFour <0.91){
		 
			debounces[8] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[8] > debounceDelay){
	    	buttonInputs[4] = 1;
	    	buttonInputs[5] = 1;
	    
	    }
	    }else if(buttonPressOneToFour >0.08&& buttonPressOneToFour <0.088){
		 
			debounces[9] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[9] > debounceDelay){
	    	buttonInputs[4] = 1;
	    	buttonInputs[6] = 1;
	    }
	    }else if (buttonPressOneToFour >0.072&& buttonPressOneToFour < 0.075){
			debounces[10] = context->audioFramesElapsed;
					 if(context->audioFramesElapsed + debounces[10] > debounceDelay){
	    		buttonInputs[4] = 1;
	    }
	    	//debounces[1] = context->audioFramesElapsed;
		//	buttonInputs[5] = 1;
			
		}else if(buttonPressOneToFour >0.023&& buttonPressOneToFour <0.3){
		 
			debounces[11] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[11] > debounceDelay){
	    
	    	buttonInputs[5] = 1;
	    	buttonInputs[6] = 1;
	    }
	    }else if (buttonPressOneToFour >0.015 && buttonPressOneToFour < 0.02){
				debounces[12] = context->audioFramesElapsed;
				 if(context->audioFramesElapsed + debounces[12] > debounceDelay){
	    	buttonInputs[5] = 1;
	    }
		//	buttonInputs[6] = 1;
			
		}else if (buttonPressOneToFour >0.006&& buttonPressOneToFour < 0.01){
			debounces[13] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[13] > debounceDelay){
	    	buttonInputs[6] = 1;
	    }
		//	buttonInputs[4] = 1;
			
		}else {
				buttonInputs[3] = 0;
				buttonInputs[4] = 0;
				buttonInputs[5] = 0;
				buttonInputs[6] = 0;
					digitalWrite(context, n, P8_16, 0);
			
		}
		buttonPressFiveToEight = analogRead(context, n/gAudioFramesPerAnalogFrame, buttonsPinFiveToEight);
	
		 if (buttonPressFiveToEight >0.209){
			debounces[14] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[14] > debounceDelay){
	    	buttonInputs[7] = 1;
	    	buttonInputs[8] = 1;
	    	buttonInputs[9] = 1;
	    	buttonInputs[10] = 1;
	    }
		//	buttonInputs[4] = 1;
			
		}else if(buttonPressFiveToEight >0.204&& buttonPressFiveToEight <0.209){
		 
			debounces[15] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[15] > debounceDelay){
	    	buttonInputs[7] = 1;
	    	buttonInputs[8] = 1;
	    	buttonInputs[9] = 1;
	    }
	    }else if(buttonPressFiveToEight >0.195&& buttonPressFiveToEight <0.2){
		 
			debounces[16] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[16] > debounceDelay){
	    	buttonInputs[7] = 1;
	    	buttonInputs[8] = 1;
	    	
	    }
	    }else if(buttonPressFiveToEight >0.15&& buttonPressFiveToEight <0.18){
		 
			debounces[17] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[17] > debounceDelay){
	    	buttonInputs[7] = 1;
	    	buttonInputs[8] = 1;
	    	buttonInputs[10] = 1;
	    }
	    }else if(buttonPressFiveToEight >0.154&& buttonPressFiveToEight < 0.16){
		 
			debounces[18] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[18] > debounceDelay){
	    	
	    	buttonInputs[7] = 1;
	    	buttonInputs[9] = 1;
	    }
	    }else if(buttonPressFiveToEight >0.148&& buttonPressFiveToEight <0.152){
		 
			debounces[19] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[19] > debounceDelay){
	    	buttonInputs[7] = 1;
	    	
	    	buttonInputs[10] = 1;
	    }
	    }else if(buttonPressFiveToEight >0.14&& buttonPressFiveToEight <0.149){
		 
			debounces[20] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[20] > debounceDelay){
	    	buttonInputs[7] = 1;
	    }
			
		}else if(buttonPressFiveToEight >0.093&& buttonPressFiveToEight <0.1){
		 
			debounces[21] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[22] > debounceDelay){
	    	buttonInputs[7] = 1;
	    	buttonInputs[9] = 1;
	    	buttonInputs[10] = 1;
	    }
	    }else if(buttonPressFiveToEight >0.087&& buttonPressFiveToEight <0.91){
		 
			debounces[22] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[22] > debounceDelay){
	    	buttonInputs[8] = 1;
	    	buttonInputs[9] = 1;
	    
	    }
	    }else if(buttonPressFiveToEight >0.08&& buttonPressFiveToEight <0.088){
		 
			debounces[23] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[23] > debounceDelay){
	    	buttonInputs[8] = 1;
	    	buttonInputs[10] = 1;
	    }
	    }else if (buttonPressFiveToEight >0.072&& buttonPressFiveToEight < 0.075){
			debounces[24] = context->audioFramesElapsed;
					 if(context->audioFramesElapsed + debounces[24] > debounceDelay){
	    		buttonInputs[8] = 1;
	    }
	    	//debounces[1] = context->audioFramesElapsed;
		//	buttonInputs[5] = 1;
			
		}else if(buttonPressFiveToEight >0.023&& buttonPressFiveToEight <0.3){
		 
			debounces[25] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[25] > debounceDelay){
	    
	    	buttonInputs[9] = 1;
	    	buttonInputs[10] = 1;
	    }
	    }else if (buttonPressFiveToEight >0.015 && buttonPressFiveToEight < 0.02){
				debounces[26] = context->audioFramesElapsed;
				 if(context->audioFramesElapsed + debounces[26] > debounceDelay){
	    	buttonInputs[9] = 1;
	    }
		//	buttonInputs[6] = 1;
			
		}else if (buttonPressFiveToEight >0.006&& buttonPressFiveToEight < 0.01){
			debounces[27] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[27] > debounceDelay){
	    	buttonInputs[10] = 1;
	    }
		//	buttonInputs[4] = 1;
			
		}else {
				buttonInputs[7] = 0;
				buttonInputs[8] = 0;
				buttonInputs[9] = 0;
				buttonInputs[10] = 0;
			
		}
		//  if(buttonPressFiveToEight >0.824){
		 
	    		
	    
		// 	debounces[4] = context->audioFramesElapsed;
	 //   if(context->audioFramesElapsed + debounces[4] > debounceDelay){
	 //   	buttonInputs[7] = 1;
	 //   }
			
		// }else if (buttonPressFiveToEight >0.812&& buttonPressFiveToEight < 0.820){
		// 		debounces[5] = context->audioFramesElapsed;
		// 			 if(context->audioFramesElapsed + debounces[5] > debounceDelay){
	 //   		buttonInputs[8] = 1;
	 //   }
		// //	buttonInputs[5] = 1;
			
		// }else if (buttonPressFiveToEight >0.73 && buttonPressFiveToEight < 0.8){
		// 		debounces[6] = context->audioFramesElapsed;
		// 		 if(context->audioFramesElapsed + debounces[6] > debounceDelay){
	 //   	buttonInputs[9] = 1;
	 //   }
		// //	buttonInputs[6] = 1;
			
		// }else if (buttonPressFiveToEight >0.41 && buttonPressFiveToEight < 0.6){
		// 	debounces[7] = context->audioFramesElapsed;
	 //   if(context->audioFramesElapsed + debounces[7] > debounceDelay){
	 //   	buttonInputs[10] = 1;
	 //   }
		// //	buttonInputs[4] = 1;
			
		// }else {
		// 		buttonInputs[7] = 0;
		// 		buttonInputs[8] = 0;
		// 		buttonInputs[9] = 0;
		// 		buttonInputs[10] = 0;
			
		//  }
		 
		 ptone =tone;
		 pfrequency = frequency;
		//  if(buttonPressFiveToEight >0.824){
		// 	buttonInputs[7] = 1;
			
		// }else if (buttonPressFiveToEight >0.819){
		// 	buttonInputs[9] = 1;
			
		// }else if (buttonPressFiveToEight >0.812){
		// 	buttonInputs[10] = 1;
			
		// }else if (buttonPressFiveToEight >0.74){
		// 	buttonInputs[8] = 1;
			
		// }else {
		// 		buttonInputs[7] = 0;
		// 		buttonInputs[8] = 0;
		// 		buttonInputs[9] = 0;
		// 		buttonInputs[10] = 0;
			
		// }
		// 	buttonPressOneToFour = analogRead(context, n/gAudioFramesPerAnalogFrame, buttonsPinOneToFour);
	
		//  if(buttonPressOneToFour >0.824){
		 	
	 //   		buttonInputs[3] = 1;
	    
		
			
		// }else if (buttonPressOneToFour >0.819){
		
		 
	 //   		buttonInputs[5] = 1;
	    
			
		// }else if (buttonPressOneToFour >0.812){
	
		 	
	 //   		buttonInputs[6] = 1;
	    
			
		// }else if (buttonPressOneToFour >0.74){
		
		 	
	 //   		buttonInputs[4] = 1;
	    
			
		// }else {
		// 		buttonInputs[3] = 0;
		// 		buttonInputs[4] = 0;
		// 		buttonInputs[5] = 0;
		// 		buttonInputs[6] = 0;
			
		// }
		
		// buttonPressFiveToEight = analogRead(context, n/gAudioFramesPerAnalogFrame, buttonsPinFiveToEight);
		//  if(buttonPressFiveToEight >0.824){
		// 	buttonInputs[7] = 1;
			
		// }else if (buttonPressFiveToEight >0.819){
		// 	buttonInputs[9] = 1;
			
		// }else if (buttonPressFiveToEight >0.812){
		// 	buttonInputs[10] = 1;
			
		// }else if (buttonPressFiveToEight >0.74){
		// 	buttonInputs[8] = 1;
			
		// }else {
		// 		buttonInputs[7] = 0;
		// 		buttonInputs[8] = 0;
		// 		buttonInputs[9] = 0;
		// 		buttonInputs[10] = 0;
			
		// }
		// 	buttonPressFiveToEight = analogRead(context, n/gAudioFramesPerAnalogFrame, buttonsPinFiveToEight);
		//  if(buttonPressFiveToEight >0.824){
		// 	buttonInputs[3] = 1;
			
		// }else if (buttonPressFiveToEight >0.819){
		// 	buttonInputs[5] = 1;
			
		// }else if (buttonPressFiveToEight >0.812){
		// 	buttonInputs[6] = 1;
			
		// }else if (buttonPressFiveToEight >0.74){
		// 	buttonInputs[4] = 1;
			
		// }else {
		// 		buttonInputs[3] = 0;
		// 		buttonInputs[4] = 0;
		// 		buttonInputs[5] = 0;
		// 		buttonInputs[6] = 0;
			
		// }
		 //if((context->audioFramesElapsed % 10) == 0)
		 //	Bela_scheduleAuxiliaryTask(gPrintData);
		
		 //}
		}
	 
	 
	//around five millisecond samplerate for inpiuts
	if(!(n % 220)){
		if(digitalRead(context, 0, P8_07) ==1)
			debouncePLAY = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debouncePLAY > debounceDelay){
	    		buttonInputs[0]=digitalRead(context, 0, P8_07); //read the value of the button
	    }
		
	//buttonInputs[1]=digitalRead(context, 0, P8_08); //read the value of the button
		if(digitalRead(context, 0, P8_08) ==1)
			debounceSHIFT = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounceSHIFT > debounceDelay){
	    		buttonInputs[1]=digitalRead(context, 0, P8_08); //read the value of the button
	    }
		
//	buttonInputs[2]=digitalRead(context, 0, P8_09); //read the value of the button
//	buttonInputs[3]=digitalRead(context, 0, P8_10); //read the value of the button
//	buttonInputs[4]=digitalRead(context, 0, P8_11); //read the value of the button
//	buttonInputs[5]=digitalRead(context, 0, P8_12); //read the value of the button
	
	//	buttonInputs[6]=digitalRead(context, 0, P8_15); //read the value of the button
		if(digitalRead(context, 0, P8_15) ==1)
			debounceSELECT = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounceSHIFT > debounceDelay){
	    		buttonInputs[2]=digitalRead(context, 0, P8_15); //read the value of the button
	    }
	Bela_scheduleAuxiliaryTask(gCheckButtons);
		
	if(buttonStates[0] == 1){
	//	debounce = milis();
	
	// 		//	Bela_scheduleAuxiliaryTask(gGenerateEuclideanSequence);
	 			if(play == 1){
	 				play = 0;
	 				playHead = 0;
	 		}	else{
	 				play = 1;
	 				playHead = 0;
	 				
	 			}
	 	 }
	 	 	if(buttonStates[1] == 1 || buttonStates[1] == 2){
			testBrain.triggerRecordMode();
	 	 }else{
	 	 		testBrain.endRecordMode();
	 	 }
	 	 
	 	
	 	 if(buttonStates[2] == 1 || buttonStates[2] == 2){
			testBrain.triggerSelectMode();
	 	 }else{
	 	 	testBrain.endSelectMode();
	 	 }
	 	 
	 	  if((buttonStates[2] == 1 || buttonStates[2] == 2)&&(buttonStates[1] == 1 || buttonStates[1] == 2))
			testBrain.clear();
		//	if(!shift)
		testBrain.isIdle(buttonStates);
			
		
					
		
	}
	// if(buttonStates[2] == 1){
	// 		//	Bela_scheduleAuxiliaryTask(gGenerateEuclideanSequence);
	// 			testBrain.voices[1].trigger();
//		 }
		//digitalWriteOnce(context, n, P8_07, 0);
//	if(status2 ==0 || status1 ==0){
		
//	digitalWriteOnce(context, n, P8_07, 1); //write the status to the LED
//	}
		
		  benKlock.ticker();
  if(play == 1){
  	
  	

	//	if( shift ==true){
		
	//	}
        if (benKlock.tick) {
           
          
            testBrain.step(playHead);
             playHead++;
            playHead%=16;
		//	if(test.sequence[playHead] == 1)		
        //	  test.trigger();
        }
      
}

   float out = testBrain.play(n) * analogRead(context, n/gAudioFramesPerAnalogFrame, volPot); ;
	
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

for( int i = 0; i < 11; i++){

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

void runRegression (void*){
		
		 	std::vector<double> regressionOutput;
		 regressionOutput= testRapid.addRunExample(frequency, tone);
		 testBrain.updateAll(regressionOutput);
			//  testBrain.voices[testBrain.selectedVoice].frequency =regressionOutput[0];
		 //testBrain.voices[testBrain.selectedVoice].oscMix =regressionOutput[1];
		 //testBrain.voices[testBrain.selectedVoice].pitchModAmount = regressionOutput[2];
		 //testBrain.voices[testBrain.selectedVoice].setOscEnv( regressionOutput[3], 1, 1, regressionOutput[4]);
		 
		//  	   testBrain.voices[0].frequency =regressionOutput[0];
		//  for(int i = 0; i < regressionOutput.size(); i++){
		//  //	if (!regressionOutput[0] )
		//  //		std::cout << i << std::endl;
		//  //}
		//  	   printf("%f", regressionOutput[i]);
		//  	     printf("%s", "\n");
		//  	     //printf("%f", regressionOutput[1]); 
		//  	     //printf("%s", "\n");
		//  	     //  printf("%f", regressionOutput[2]);
		//  	     //    printf("%s", "\n");
		//  	     //    printf("%f", regressionOutput[3]);
		//  	     //      printf("%s", "\n");
		//  	     //      printf("%f", regressionOutput[4]);
		//  	     //        printf("%s", "\n");
		//  	     //        printf("%f", regressionOutput[5]);
		//  	               printf("%f", 9999999.);
		// }
		//  testBrain.voices[0].oscMix =regressionOutput[1];
		// // testBrain.voices[0].toneMix =regressionOutput[2];
		//  testBrain.voices[0].noiseMix =regressionOutput[3];
		//  testBrain.voices[0].pitchModAmount = regressionOutput[4];
		//  //testBrain.voices[0].setOscEnv( regressionOutput[5], 1, 1, regressionOutput[6]);
		//  //testBrain.voices[0].setNoiseEnv( regressionOutput[7], 1, 1, regressionOutput[8]);
		// // for(int i = 0; i < 4; i++){
		// //  testBrain.voices[i].frequency =regressionOutput[0+i];
		// //  testBrain.voices[i].oscMix =regressionOutput[2+i];
		// //  testBrain.voices[i].toneMix =regressionOutput[3+9];
		// //  testBrain.voices[i].noiseMix =regressionOutput[4+i];
		// //  testBrain.voices[i].pitchModAmount = regressionOutput[11];
		// // }	 
		//  	 testBrain.voices[1].frequency =regressionOutput[9];
		//  testBrain.voices[1].oscMix =regressionOutput[10];
		//  testBrain.voices[1].toneMix =regressionOutput[11];
		//  testBrain.voices[1].noiseMix =regressionOutput[12];
		//  testBrain.voices[1].pitchModAmount = regressionOutput[13];
		//  //testBrain.voices[1].setOscEnv( regressionOutput[14], 1, 1, regressionOutput[15]);
		//  //testBrain.voices[1].setNoiseEnv( regressionOutput[16], 1, 1, regressionOutput[17]);
		
		//  testBrain.voices[2].frequency =regressionOutput[18];
		//   testBrain.voices[2].oscMix =regressionOutput[19];
		//   testBrain.voices[2].toneMix =regressionOutput[20];
		//   testBrain.voices[2].noiseMix =regressionOutput[21];
		//   testBrain.voices[2].pitchModAmount = regressionOutput[22];
		// //  testBrain.voices[2].setOscEnv( regressionOutput[20], 1, 1, regressionOutput[21]);
		// //  testBrain.voices[2].setNoiseEnv( regressionOutput[22], 1, 1, regressionOutput[23]);
		// // // // 	//testBrain.updateVoice(0, regressionOutput[0],regressionOutput[1],regressionOutput[2],regressionOutput[3],regressionOutput[4],regressionOutput[5],regressionOutput[6],regressionOutput[7],regressionOutput[8],regressionOutput[9],regressionOutput[10],regressionOutput[11],regressionOutput[12]);

	 //testBrain.voices[3].frequency =regressionOutput[27];
		//   testBrain.voices[3].oscMix =regressionOutput[28];
		//   testBrain.voices[3].toneMix =regressionOutput[29];
		//   testBrain.voices[3].noiseMix =regressionOutput[30];
		//  testBrain.voices[3].pitchModAmount = regressionOutput[31];
		//  //testBrain.voices[3].setOscEnv( regressionOutput[29], 1, 1, regressionOutput[30]);
		//  //testBrain.voices[3].setNoiseEnv( regressionOutput[31], 1, 1, regressionOutput[32]);
	
}
void printData (void*){
		cout<< frequency << " "  << tone << endl;
	
}

void cleanup(BelaContext *context, void *userData)
{

}
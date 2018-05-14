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
bool trained;
//bool startStop;
uint8_t pulse;
uint8_t length;
float frequency;
float tone;
float pfrequency;
float ptone;
float pfrequency2;
float pFreqMult;
float ptone2;
float vol;
float pot0;
float pot1;
float pot2;
float pot3;

int mode;
 
 float	buttonPressOneToFour = 0.0;
 float	buttonPressFiveToEight = 0.0;
 float prev1t4 = 0.;
 float prev5t8 = 0.;
 
// float filterWeight = 0.0;
int buttonStates[12];
int buttonInputs[12];

int leds[8];

float gPhase;
float gInverseSampleRate;
int gAudioFramesPerAnalogFrame;
//---------------

AuxiliaryTask gGenerateEuclideanSequence;
void generateEuclideanSequence(void*);

AuxiliaryTask gNewTrainingData;
void newTrainingData(void*);

AuxiliaryTask gTrain;
void train(void*);

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
	pinMode(context, 0, P8_18, OUTPUT);
	pinMode(context, 0, P8_27, OUTPUT);
	pinMode(context, 0, P8_28, OUTPUT);
	pinMode(context, 0, P8_29, OUTPUT);
	pinMode(context, 0, P8_30, OUTPUT);
	pinMode(context, 0, P9_12, OUTPUT);
	pinMode(context, 0, P9_14, OUTPUT);
	
	trained = false;
	playHead = 0;
	play = 0;
	shift = false;

//	selectMode = false
	pot3 = 1.0;
	pot2 = 0.5;
	ptone = 1.0;
	pfrequency = 0.5;
	pFreqMult = 1.;
    benKlock.setTempo(128      );
    benKlock.setTicksPerBeat(4);
	
	mode = 0;
	
	if((gGenerateEuclideanSequence= Bela_createAuxiliaryTask(&generateEuclideanSequence, 99, "generating-sequences")) == 0)
		return false;
		
	if((gCheckButtons= Bela_createAuxiliaryTask(&checkButtons, 85, "checking-buttons")) == 0)
		return false;
	
		if((gPrintData= Bela_createAuxiliaryTask(&printData, 86, "printing")) == 0)
		return false;
		
		if((gRunRegression= Bela_createAuxiliaryTask(&runRegression, 50, "regressing")) == 0)
		return false;
		
		if((gNewTrainingData= Bela_createAuxiliaryTask(&newTrainingData, 30, "new_training_data")) == 0)
		return false;

		if((gTrain= Bela_createAuxiliaryTask(&train, 40, "training")) == 0)
		return false;

	for (uint8_t i = 0; i < 4; i++){
		
		buttonInputs[i] = 0;
		buttonStates[i] = 0;
	}
		for (uint8_t i = 0; i < 8; i++){
		
		leds[i] = 0;
	
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
vector<double> drumKitLong = { 32.7, 4.184,  1.,  0.692,       1, 5000., 30,  200.,
						  174.615,    5, 0.9,       0.5,        0.5,    200., 0.1,  100.,
					 	     98.,     2,  0.,       0.2,      0.0, 504., 0.,  84.5,
						   130.81, 4.184, 1.,    0.692,        0.1, 50., 0.,  10 
				  
};
vector<double> drumKit = {   32.7,  4.184, 1.,    0.2,      0.1, 100., 0.,  84.,
						   174.61,    3.6, 1.,   0.14,     0.1,  428., 0., 100.,
					 	     98.6,      0, 1.,     1.,      0.0,  504., 0.,  84.,
						   130.81,  4.184, 1.,    0.2,       0.1,  504., 0.,  84.,  
				  
};
	//freq, pma, gain_, om, tm,oa,  oR, pa, pr, 
vector<double> major = { 130,   0.1, 0.9, 0,  50, 50, 50, 50, 
						 175,   0.1, 0.9, 0,  50, 50, 50, 50,
				     	  	196,   0.1, 0.9, 0,  50,  500, 50, 50,
						261,    0.1, 0.9, 0,   50,  50, 50, 50, 
				  
};



	for(int i = 0; i < 1; i++){
	float r =0;
	
	testRapid.addTrainingExampleVector(0.81-r, 0.81-r, drumKitLong);
	testRapid.addTrainingExampleVector(0.41-r, 0.81-r, drumKit);
	// //f1, t1, nm1, pm1, a1, r1, na1, nr1
	testRapid.addTrainingExampleVector(0.0+r, 0.0+r, major);


trained = 	testRapid.myRegression.train(testRapid.trainingSet);
	
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
			pot0 = analogRead(context, n/gAudioFramesPerAnalogFrame, 0);
			pot0 =	floorf_neon(pot0 * 100.) / 100;
			pot1 = analogRead(context, n/gAudioFramesPerAnalogFrame, 1);
			pot1 =	floorf_neon(pot1 * 100.) / 100;
			pot2 = analogRead(context, n/gAudioFramesPerAnalogFrame, 2);
			pot2 =	floorf_neon(pot2 * 100.) / 100;
			pot3 = analogRead(context, n/gAudioFramesPerAnalogFrame, 3);
			pot3 =	floorf_neon(pot3 * 100.) / 100;
		
			vol = pot1;
	 	 	benKlock.setTempo( ::map(pot0, 0, 1, 30, 250));
		
		//	}
		
		if(mode==0){
		
	if((pot3 != ptone) || (pot2 != pfrequency)  ){
		//		std::vector<double> regressionOutput;
				
	
		Bela_scheduleAuxiliaryTask(gRunRegression);
	}
	}	
	

		buttonPressOneToFour = analogRead(context, n/gAudioFramesPerAnalogFrame, buttonsPinOneToFour);

	 	 if (buttonPressOneToFour >0.209){
			debounces[0] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[0] > debounceDelay){
	    	buttonInputs[4] = 1;
	    	buttonInputs[5] = 1;
	    	buttonInputs[6] = 1;
	    	buttonInputs[7] = 1;
	    }
			buttonInputs[4] = 1;
			
		}else if(buttonPressOneToFour >0.204&& buttonPressOneToFour <0.209){
		 
			debounces[1] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[1] > debounceDelay){
	    	buttonInputs[4] = 1;
	    	buttonInputs[5] = 1;
	    	buttonInputs[6] = 1;
	    }
	    }else if(buttonPressOneToFour >0.195&& buttonPressOneToFour <0.2){
		 
			debounces[2] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[2] > debounceDelay){
	    	buttonInputs[4] = 1;
	    	buttonInputs[5] = 1;
	    	
	    }
	    }else if(buttonPressOneToFour >0.15&& buttonPressOneToFour <0.18){
		 
			debounces[3] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[3] > debounceDelay){
	    	buttonInputs[4] = 1;
	    	buttonInputs[6] = 1;
	    	buttonInputs[7] = 1;
	    }
	    }else if(buttonPressOneToFour >0.154&& buttonPressOneToFour < 0.16){
		 
			debounces[4] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[4] > debounceDelay){
	    	
	    	buttonInputs[4] = 1;
	    	buttonInputs[6] = 1;
	    }
	    }else if(buttonPressOneToFour >0.148&& buttonPressOneToFour <0.152){
		 
			debounces[5] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[5] > debounceDelay){
	    	buttonInputs[4] = 1;
	    	
	    	buttonInputs[7] = 1;
	    }
	    }else
	    if(buttonPressOneToFour >0.14&& buttonPressOneToFour <0.149){
		 
			debounces[6] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[6] > debounceDelay){
	    	buttonInputs[4] = 1;
	    	//	digitalWrite(context, n, P8_16, 1);
	    }
			
		}else if(buttonPressOneToFour >0.093&& buttonPressOneToFour <0.1){
		 
			debounces[7] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[7] > debounceDelay){
	    	buttonInputs[5] = 1;
	    	buttonInputs[6] = 1;
	    	buttonInputs[7] = 1;
	    }
	    }else if(buttonPressOneToFour >0.087&& buttonPressOneToFour <0.091){
		 
			debounces[8] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[8] > debounceDelay){
	    	buttonInputs[5] = 1;
	    	buttonInputs[6] = 1;
	    
	    }
	    }else if(buttonPressOneToFour >0.08&& buttonPressOneToFour <0.088){
		 
			debounces[9] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[9] > debounceDelay){
	    	buttonInputs[5] = 1;
	    	buttonInputs[7] = 1;
	    }
	    }else if (buttonPressOneToFour >0.070&& buttonPressOneToFour < 0.08){
			debounces[10] = context->audioFramesElapsed;
					 if(context->audioFramesElapsed + debounces[10] > debounceDelay){
	    		buttonInputs[5] = 1;
	    }
	    	debounces[1] = context->audioFramesElapsed;
			buttonInputs[5] = 1;
			
		}else if(buttonPressOneToFour >0.023&& buttonPressOneToFour <0.3){
		 
			debounces[11] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[11] > debounceDelay){
	    
	    	buttonInputs[6] = 1;
	    	buttonInputs[7] = 1;
	    }
	    }else if (buttonPressOneToFour >0.015 && buttonPressOneToFour < 0.02){
				debounces[12] = context->audioFramesElapsed;
				 if(context->audioFramesElapsed + debounces[12] > debounceDelay){
	    	buttonInputs[6] = 1;
	    }
		//	buttonInputs[6] = 1;
			
		}else if (buttonPressOneToFour >0.007&& buttonPressOneToFour < 0.08){
			debounces[13] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[13] > debounceDelay){
	    	buttonInputs[7] = 1;
	    }
		//	buttonInputs[4] = 1;
			
		}else {
				buttonInputs[4] = 0;
				buttonInputs[5] = 0;
				buttonInputs[6] = 0;
				buttonInputs[7] = 0;
				
		}
		buttonPressFiveToEight = analogRead(context, n/gAudioFramesPerAnalogFrame, buttonsPinFiveToEight);
	
		 if (buttonPressFiveToEight >0.209){
			debounces[14] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[14] > debounceDelay){
	    	buttonInputs[8] = 1;
	    	buttonInputs[9] = 1;
	    	buttonInputs[10] = 1;
	    	buttonInputs[11] = 1;
	    }
		//	buttonInputs[4] = 1;
			
		}else if(buttonPressFiveToEight >0.204&& buttonPressFiveToEight <0.209){
		 
			debounces[15] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[15] > debounceDelay){
	    	buttonInputs[10] = 1;
	    	buttonInputs[8] = 1;
	    	buttonInputs[9] = 1;
	    }
	    }else if(buttonPressFiveToEight >0.195&& buttonPressFiveToEight <0.2){
		 
			debounces[16] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[16] > debounceDelay){
	    	buttonInputs[9] = 1;
	    	buttonInputs[8] = 1;
	    	
	    }
	    }else if(buttonPressFiveToEight >0.15&& buttonPressFiveToEight <0.18){
		 
			debounces[17] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[17] > debounceDelay){
	    	buttonInputs[7] = 1;
	    	buttonInputs[9] = 1;
	    	buttonInputs[11] = 1;
	    }
	    }else if(buttonPressFiveToEight >0.154&& buttonPressFiveToEight < 0.16){
		 
			debounces[18] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[18] > debounceDelay){
	    	
	    	buttonInputs[8] = 1;
	    	buttonInputs[10] = 1;
	    }
	    }else if(buttonPressFiveToEight >0.148&& buttonPressFiveToEight <0.152){
		 
			debounces[19] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[19] > debounceDelay){
	    	buttonInputs[8] = 1;
	    	
	    	buttonInputs[11] = 1;
	    }
	    }else
	 if(buttonPressFiveToEight >0.14&& buttonPressFiveToEight <0.149){
		 
			debounces[20] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[20] > debounceDelay){
	    	buttonInputs[8] = 1;
	    }
			
		}else if(buttonPressFiveToEight >0.093&& buttonPressFiveToEight <0.1){
		 
			debounces[21] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[22] > debounceDelay){
	    	buttonInputs[8] = 1;
	    	buttonInputs[11] = 1;
	    	buttonInputs[10] = 1;
	    }
	    }else if(buttonPressFiveToEight >0.087&& buttonPressFiveToEight <0.91){
		 
			debounces[22] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[22] > debounceDelay){
	    	buttonInputs[10] = 1;
	    	buttonInputs[9] = 1;
	    
	    }
	    }else if(buttonPressFiveToEight >0.08&& buttonPressFiveToEight <0.088){
		 
			debounces[23] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[23] > debounceDelay){
	    	buttonInputs[9] = 1;
	    	buttonInputs[11] = 1;
	    }
	    }else if (buttonPressFiveToEight >0.072&& buttonPressFiveToEight < 0.075){
			debounces[24] = context->audioFramesElapsed;
					 if(context->audioFramesElapsed + debounces[24] > debounceDelay){
	    		buttonInputs[9] = 1;
	    }
	    	//debounces[1] = context->audioFramesElapsed;
		//	buttonInputs[5] = 1;
			
		}else if(buttonPressFiveToEight >0.023&& buttonPressFiveToEight <0.3){
		 
			debounces[25] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[25] > debounceDelay){
	    
	    	buttonInputs[11] = 1;
	    	buttonInputs[10] = 1;
	    }
	    }else if (buttonPressFiveToEight >0.015 && buttonPressFiveToEight < 0.02){
				debounces[26] = context->audioFramesElapsed;
				 if(context->audioFramesElapsed + debounces[26] > debounceDelay){
	    	buttonInputs[10] = 1;
	    }
		//	buttonInputs[6] = 1;
			
		}else if (buttonPressFiveToEight >0.006&& buttonPressFiveToEight < 0.01){
			debounces[27] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[27] > debounceDelay){
	    	buttonInputs[11] = 1;
	    }
		//	buttonInputs[4] = 1;
			
		}else {
				buttonInputs[11] = 0;
				buttonInputs[8]  = 0;
				buttonInputs[9]  = 0;
				buttonInputs[10] = 0;
			
		}
		
		 
		 ptone =pot3;
		 pfrequency = pot2;
		  ptone2 =pot0;
		 pfrequency2 = pot1;
	
			
	
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
		

	//	buttonInputs[6]=digitalRead(context, 0, P8_15); //read the value of the button
		if(digitalRead(context, 0, P8_09) ==1)
			debounceSELECT = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounceSHIFT > debounceDelay){
	    		buttonInputs[2]=digitalRead(context, 0, P8_09); //read the value of the button
	    }
	    	if(digitalRead(context, 0, P8_10) ==1)
			debounceSELECT = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounceSHIFT > debounceDelay){
	    		buttonInputs[3]=digitalRead(context, 0, P8_10); //read the value of the button
	    }
	Bela_scheduleAuxiliaryTask(gCheckButtons);
	if(buttonStates[3] == 1 && mode < 4){
	mode ++;
	mode = mode%4;
		
	}
		if(mode == 0){
	if(buttonStates[0] == 1){
	
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
	 	 	  if((buttonStates[2] == 1 || buttonStates[2] == 2)&&(buttonStates[1] == 1 || buttonStates[1] == 2))
			testBrain.clear();
		}else if(mode ==1){
			play = 0;
		
			for(uint8_t i = 0 ; i < 8; i++){
		
            		leds[i] = 1;
				}
		
			float om = ::map(pot0, 0, 0.82, 0, 1);
			float pma = ::map(pot1, 0, 0.82, 1, 10);
			float freq = ::map(pot2, 0, 0.82, 20, 300);
	
		//	float pma = ::map(pot3, 0, 0.82, 1, 10);
		
	
				  
	
			testBrain.voices[testBrain.selectedVoice].oscMix = om;
			testBrain.voices[testBrain.selectedVoice].pitchModAmount = pma;
			testBrain.voices[testBrain.selectedVoice].frequency = freq;
		
	

						  	

		}else if (mode ==2){
				for(uint8_t i = 0 ; i < 8; i+=2){
		
            		leds[i] = 0;
				}
				float oscAtk = ::map(pot0, 0, 0.82, 0, 500);
				float oscRls = ::map(pot1,0,0.82, 20, 1000);
				float pmAtk = ::map(pot2, 0, 0.82, 0, 500);
				float pmRls = ::map(pot3,0,0.82, 20, 1000);
			
			testBrain.voices[testBrain.selectedVoice].pm.setAttack( pmAtk);
			testBrain.voices[testBrain.selectedVoice].pm.setRelease( pmRls);
			testBrain.voices[testBrain.selectedVoice].oscAmpEnv.setAttack( oscAtk);
			testBrain.voices[testBrain.selectedVoice].oscAmpEnv.setRelease( oscRls);
			testBrain.voices[testBrain.selectedVoice].pmAtk = pmAtk;
			testBrain.voices[testBrain.selectedVoice].pmRls = pmRls;
			testBrain.voices[testBrain.selectedVoice].oscAtk = oscAtk;
			testBrain.voices[testBrain.selectedVoice].oscRls = oscRls;
			}else if (mode ==3){
			//	trained = false;
			if (buttonStates[1] == 0 )
			Bela_scheduleAuxiliaryTask(gNewTrainingData);
					cout << mode <<endl;
				mode = 4;
			
		}else if(mode ==4){
				cout << mode <<endl;

			//	trained = false;
				Bela_scheduleAuxiliaryTask(gTrain);
			//	testRapid.myRegression.train(testRapid.trainingSet);
				mode = 5;
		}
		else if(mode ==5){
				cout << mode <<endl;
			// for(uint8_t i = 0 ; i < 8; i+=2){
		
   //         		leds[i] = 0;
			// 	}
			if(trained){
				mode = 0;
			cout << "trained" << endl;
				
			}
		// 		// for(uint8_t i = 0 ; i < 8; i++){
		
  //  //         		leds[i] = 0;
		// 		// }
		// 	//}
		 }
		
	 	 if(buttonStates[2] == 1 || buttonStates[2] == 2){
			testBrain.triggerSelectMode();
	 	 }else{
	 	 	testBrain.endSelectMode();
	 	 }
	 	 
	 
		//	if(!shift)
		testBrain.isIdle(buttonStates);
			
		// if (n% 10 == 0)
		// Bela_scheduleAuxiliaryTask(gPrintData);
		
	}

	
		  benKlock.ticker();
  if(play == 1){
  	
  	

	//	if( shift ==true){
		
	//	}
	
        if (benKlock.tick) {
           
          
            testBrain.step(playHead);
             playHead++;
            playHead%=8;
            // for(uint8_t i = 0 ; i < 8; i++){
            // 	if(i == playHead)
            // 		leds[i] = 1;
            // 	else
            // 		leds[i]=0;
            	
            // }
		//	if(test.sequence[playHead] == 1)		
        //	  test.trigger();
        }
      
      
}
if(mode == 0){
	int *  curSeq = testBrain.returnCurSeq();
	for(uint8_t i = 0 ; i < 8; i++){
			if(i == playHead && play ==1)
            		leds[i] = 1;
            else if(curSeq[i] == 1)	
            		leds[i]=1;
            else
            	leds[i] = 0;
	
            	
            }
}

   float out = testBrain.play(n) * vol ;
	
		for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
			// Two equivalent ways to write this code

			// The long way, using the buffers directly:
			// context->audioOut[n * context->audioOutChannels + channel] = out;

			// Or using the macros:
			audioWrite(context, n, channel, out);
		}
		digitalWrite(context, n, P8_16, leds[0]);
        digitalWrite(context, n, P8_18, leds[1]);
        digitalWrite(context, n, P8_27, leds[2]);
        digitalWrite(context, n, P8_28, leds[3]);
        digitalWrite(context, n, P8_29, leds[4]);
        digitalWrite(context, n, P8_30, leds[5]);
        digitalWrite(context, n, P9_12, leds[6]);
        digitalWrite(context, n, P9_14, leds[7]);
	}

	
}

void checkButtons(void*){

	for( int i = 0; i < 11; i++){

		if(buttonInputs[i] == 1 && buttonStates[i] ==0)
				buttonStates[i] = 1;
	else if (buttonInputs[i] == 1 && buttonStates[i] ==1)
		buttonStates[i] = 2;
	else if( buttonInputs[i] == 0)
		buttonStates[i] = 0;
	}

}

void generateEuclideanSequence (void*){
	
		uint8_t l = uint8_t(length);
		uint8_t p = uint8_t(pulse);
		
		testBrain.euclidSeq(l, p);
	
}

void runRegression (void*){
	
//	cout <<"running" <<endl;
		std::vector<double> regressionOutput;
		 regressionOutput= testRapid.addRunExample(pot2, pot3);
		 for(uint8_t i = 0; i < 32; i++){
	if(regressionOutput[i] < 0)
		regressionOutput[i] =	0;//regressionOutput[i] -(regressionOutput[i]*2) ;
		
//	cout<< regressionOutput[i] <<endl;
		 }
		 testBrain.updateAll(regressionOutput);
		 
	//	testBrain.updateSome(regressionOutput);
		
}

void newTrainingData (void*){
	
		//freq, pma, gain_, om, oa,  oR, pa, pr, 
			vector<double> newKit = {  testBrain.voices[0].frequency, testBrain.voices[0].pitchModAmount, 1.0,  testBrain.voices[0].oscMix, testBrain.voices[0].oscAtk,  testBrain.voices[0].oscRls,   testBrain.voices[0].pmAtk,  testBrain.voices[0].pmRls, 
								       testBrain.voices[1].frequency, testBrain.voices[1].pitchModAmount, 1.0,  testBrain.voices[1].oscMix, testBrain.voices[1].oscAtk,  testBrain.voices[1].oscRls,   testBrain.voices[1].pmAtk,  testBrain.voices[1].pmRls,
					 			       testBrain.voices[2].frequency, testBrain.voices[2].pitchModAmount, 1.0,  testBrain.voices[2].oscMix, testBrain.voices[2].oscAtk,  testBrain.voices[2].oscRls, 	testBrain.voices[2].pmAtk,  testBrain.voices[2].pmRls,
									   testBrain.voices[3].frequency, testBrain.voices[3].pitchModAmount, 1.0,  testBrain.voices[3].oscMix, testBrain.voices[3].oscAtk,  testBrain.voices[3].oscRls,   testBrain.voices[3].pmAtk,  testBrain.voices[3].pmRls
						  	};
	//					  		for(int i = 0; i < 4; i++){
		
//	float r =0;//	(rand()*(1.0 / (RAND_MAX + 1.0))*0.1);
				testRapid.addTrainingExampleVector( pot2, pot3, newKit);
		//				  		}
							  	//	for(int i = 0; i < 32; i++){
							  		cout <<newKit[3] << " " << newKit[1]<<" " << newKit[5] <<" " << newKit[6] << " "<<newKit[7] <<endl;
							  		
							  			
		//					  		}
		 
}

void train (void*){
			//	for(int i = 0; i < 1; i++){
		//	testRapid.myRegression.reset();
			trained = 	testRapid.myRegression.train(testRapid.trainingSet);
			//	}
}
void printData (void*){
		cout<< " "  << mode<< endl;
	//testBrain.voices[2].oscAmpEnv.attack
}

void cleanup(BelaContext *context, void *userData)
{

}
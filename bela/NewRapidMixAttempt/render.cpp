#include <Bela.h>
#include "maximilian.h"

#define NUM_OF_BUTTONS 2;
//#include "rapidLib/regression.h"
#include "drumSynth.h"
#include "rapidBrain.h"
#include "drumBrain.h"


maxiFilter inputfilter;

//maximilian stuff
maxiClock benKlock;
int playHead;

drumSynth test;
drumBrain testBrain;
rapidBrain testRapid;


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
	
    benKlock.setTempo(128      );
    benKlock.setTicksPerBeat(4);
	
	if((gGenerateEuclideanSequence= Bela_createAuxiliaryTask(&generateEuclideanSequence, 99, "generating-sequences")) == 0)
		return false;
		
	if((gCheckButtons= Bela_createAuxiliaryTask(&checkButtons, 85, "checking-buttons")) == 0)
		return false;
	
		if((gPrintData= Bela_createAuxiliaryTask(&printData, 86, "printing")) == 0)
		return false;
		
		if((gRunRegression= Bela_createAuxiliaryTask(&runRegression, 99, "regressing")) == 0)
		return false;	

	for (uint8_t i = 0; i < 4; i++){
		
		buttonInputs[i] = 0;
		buttonStates[i] = 0;
	}
	testRapid.addTrainingExample(0, 1.0, 1.0);
	testRapid.addTrainingExample(0, 0.0, 0.0);
	testRapid.addTrainingExample(0, 1.0, 0.0);
	testRapid.addTrainingExample(0, 0.0, 1.0);
	testRapid.myRegression.train(testRapid.trainingSet);

	return true;
}

void render(BelaContext *context, void *userData)
{
 int debouncePLAY =  0;
 int debounceSHIFT = 0;
 int debounceSELECT = 0;
 int debounces[] = {0,0,0,0,0,0,0,0};
 int debounceDelay = 1;
for(unsigned int n = 0; n < context->audioFrames; n++) {
	
		if(!(n % gAudioFramesPerAnalogFrame)) {
			// On even audio samples: read analog inputs and update frequency and amplitude
		//	if(analogRead(context, n/gAudioFramesPerAnalogFrame, pitchPot) != frequency){
			frequency = analogRead(context, n/gAudioFramesPerAnalogFrame, 1);
			//testBrain.voices[0].frequency 
			benKlock.setTempo( ::map(analogRead(context, n/gAudioFramesPerAnalogFrame, 0), 0, 1, 30, 400));
		//	}
			//tone = map(analogRead(context, n/gAudioFramesPerAnalogFrame, pitchPot), 0, 1, , 100);
		//	testBrain.voices[0].frequency = ::map(frequency = analogRead(context, n/gAudioFramesPerAnalogFrame, 1), 0, 1, 30, 100);
			tone = analogRead(context, n/gAudioFramesPerAnalogFrame, 3);
	
		//		std::vector<double> regressionOutput;
		//regressionOutput=	testRapid.addRunExample(frequency, tone);
	
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
	
		 if(buttonPressOneToFour >0.142){
		 
	    		
	    
			debounces[0] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[0] > debounceDelay){
	    	buttonInputs[3] = 1;
	    }
			
		}else if (buttonPressOneToFour >0.074&& buttonPressOneToFour < 0.076){
			
					 if(context->audioFramesElapsed + debounces[1] > debounceDelay){
	    		buttonInputs[4] = 1;
	    }
	    	debounces[1] = context->audioFramesElapsed;
		//	buttonInputs[5] = 1;
			
		}else if (buttonPressOneToFour >0.016 && buttonPressOneToFour < 0.02){
				debounces[2] = context->audioFramesElapsed;
				 if(context->audioFramesElapsed + debounces[2] > debounceDelay){
	    	buttonInputs[6] = 1;
	    }
		//	buttonInputs[6] = 1;
			
		}else if (buttonPressOneToFour >0.007&& buttonPressOneToFour < 0.01){
			debounces[3] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[3] > debounceDelay){
	    	buttonInputs[5] = 1;
	    }
		//	buttonInputs[4] = 1;
			
		}else {
				buttonInputs[3] = 0;
				buttonInputs[4] = 0;
				buttonInputs[5] = 0;
				buttonInputs[6] = 0;
			
		}
		buttonPressFiveToEight = analogRead(context, n/gAudioFramesPerAnalogFrame, buttonsPinFiveToEight);
	
		
		 if(buttonPressFiveToEight >0.824){
		 
	    		
	    
			debounces[4] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[4] > debounceDelay){
	    	buttonInputs[7] = 1;
	    }
			
		}else if (buttonPressFiveToEight >0.812&& buttonPressFiveToEight < 0.820){
				debounces[5] = context->audioFramesElapsed;
					 if(context->audioFramesElapsed + debounces[5] > debounceDelay){
	    		buttonInputs[8] = 1;
	    }
		//	buttonInputs[5] = 1;
			
		}else if (buttonPressFiveToEight >0.74 && buttonPressFiveToEight < 0.8){
				debounces[6] = context->audioFramesElapsed;
				 if(context->audioFramesElapsed + debounces[6] > debounceDelay){
	    	buttonInputs[9] = 1;
	    }
		//	buttonInputs[6] = 1;
			
		}else if (buttonPressFiveToEight >0.41 && buttonPressFiveToEight < 0.6){
			debounces[7] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[7] > debounceDelay){
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
		 //if((context->audioFramesElapsed % 10) == 0){
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
		
		// 	std::vector<double> regressionOutput;
		// regressionOutput=	testRapid.addRunExample(frequency, tone);
		//	testBrain.updateVoice(0, regressionOutput[0],regressionOutput[1],regressionOutput[2],regressionOutput[3],regressionOutput[4],regressionOutput[5],regressionOutput[6],regressionOutput[7],regressionOutput[8],regressionOutput[9],regressionOutput[10],regressionOutput[11],regressionOutput[12]);
}
void printData (void*){
		cout<< buttonStates[0] << ",  " << buttonStates[1] <<",  " << buttonStates[2] << endl;
	
}

void cleanup(BelaContext *context, void *userData)
{

}
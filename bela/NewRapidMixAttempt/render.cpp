#include <Bela.h>
#include "maximilian.h"

#define NUM_OF_BUTTONS 2;
//#include "rapidLib/regression.h"
#include "drumSynth.h"
#include "rapidBrain.h"
#include "drumBrain.h"
#include <math_neon.h>



//maximilian stuff
maxiClock benKlock;
int playHead;


drumBrain testBrain;
rapidBrain testRapid;


//utilities
int buttonsPinOneToFour = 4;
int buttonsPinFiveToEight = 5;
int play;
bool trained;
float pfrequency;
float ptone;
int mode;

float vol;
float pot0;
float pot1;
float pot2;
float pot3;



float	buttonPressOneToFour = 0.0;
float	buttonPressFiveToEight = 0.0;
 

int buttonStates[12];
int buttonInputs[12];

int leds[8];

int gAudioFramesPerAnalogFrame;
//---------------



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
	
		// Useful calculations -taken from bela
	gAudioFramesPerAnalogFrame = context->audioFrames / context->analogFrames;

//LED pins
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


	pot3 = 1.0;
	pot2 = 0.5;
	ptone = 1.0;
	pfrequency = 0.5;

    benKlock.setTempo(128      );
    benKlock.setTicksPerBeat(4);
	
	mode = 0;
	

//setup out auxiliart functions, with their priority level
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


//turn everything off
	for (uint8_t i = 0; i < 4; i++){
		
		buttonInputs[i] = 0;
		buttonStates[i] = 0;
	}
		for (uint8_t i = 0; i < 8; i++){
		
		leds[i] = 0;
	
	}
	
//freq, pma, gain_, om, tm,oa,  oR, pa, pr,
vector<double> drumKitLong = { 32.7, 4.184,  1.,  0.692,    1, 5000.,  30,  200.,
						    174.615,     5, 0.9,    0.5,  0.5,  200., 0.1,  100.,
					 	        98.,     2,  0.,    0.2,  0.0,  504.,  0.,  84.5,
					 	         98,   0.1, 1.0,      1,    0,    51,   0,    50,
						       32.7, 4.184,  1.,  0.692,     1,5000.,  30,  200.,
						    174.615,     5, 0.9,    0.5,   0.5, 200.,  0.1, 100.,
					 	     98.,     2,  0.,       0.2,      0.0, 504., 0.,  84.5,
					 	    
				  
};
vector<double> drumKit = {   32.7,  4.184, 1.,    0.2,      0.1, 100., 0.,  84.,
						   174.61,    3.6, 1.,   0.14,     0.1,  428., 0., 100.,
					 	     98.6,      0, 1.,     1.,      0.0,  504., 0.,  84.,
					 	      130.81,  4.184, 1.,    0.2,       0.1,  504., 0.,  84.,
						 32.7,  4.184, 1.,    0.2,      0.1, 100., 0.,  84.,
						   174.61,    3.6, 1.,   0.14,     0.1,  428., 0., 100.,
					 	     98.6,      0, 1.,     1.,      0.0,  504., 0.,  84.
				  
};
	 
vector<double> major = { 130,   0.1, 0.9, 0,  50, 50, 50, 50, 
						 175,   0.1, 0.9, 0,  50, 50, 50, 50,
				     	  	196,   0.1, 0.9, 0,  50,  500, 50, 50,
				     	  	267,    0.1, 0.9, 0,   50,  500, 50, 500, 
					 130,   0.1, 0.9, 0,  50, 50, 50, 50, 
						 175,   0.1, 0.9, 0,  50, 50, 50, 50,
				     	  	196,   0.1, 0.9, 0,  50,  500, 50, 50,
				  
};




	//add ur presets to the model
	testRapid.addTrainingExampleVector(0.81, 0.81, drumKitLong);
	testRapid.addTrainingExampleVector(0.41, 0.81, drumKit);

	testRapid.addTrainingExampleVector(0.0, 0.0, major);


trained = 	testRapid.myRegression.train(testRapid.trainingSet);
	
	
		std::cout << "training complete, device running" <<std::endl; 
	return true;
}

void render(BelaContext *context, void *userData)
{
	
	//reset all our debounce stuff
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
	
				
	
		Bela_scheduleAuxiliaryTask(gRunRegression);
	}
	}	
	

	//read the analog vlaue from our chaiend swithces, big messy if statements compare against values found in testing and updates switches accordingly
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
	
		}else if (buttonPressFiveToEight >0.006&& buttonPressFiveToEight < 0.01){
			debounces[27] = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounces[27] > debounceDelay){
	    	buttonInputs[11] = 1;
	   
	    }
	
			
		}else {
				buttonInputs[11] = 0;
				buttonInputs[8]  = 0;
				buttonInputs[9]  = 0;
				buttonInputs[10] = 0;
			
		}
		
		 //updare privious pot values
		 ptone =pot3;
		 pfrequency = pot2;
		
	
		}
	 
	 
	//around five millisecond samplerate for inpiuts
	if(!(n % 220)){
			
	//check the function switches

		if(digitalRead(context, 0, P8_07) ==1)
			debouncePLAY = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debouncePLAY > debounceDelay){
	    		buttonInputs[0]=digitalRead(context, 0, P8_07); //read the value of the button
	    }
		

		if(digitalRead(context, 0, P8_08) ==1)
			debounceSHIFT = context->audioFramesElapsed;
	    if(context->audioFramesElapsed + debounceSHIFT > debounceDelay){
	    		buttonInputs[1]=digitalRead(context, 0, P8_08); //read the value of the button
	    }
		


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

	
	//udate the buttons
	Bela_scheduleAuxiliaryTask(gCheckButtons);
	
	//cycle through the modes
	if(buttonStates[3] == 1 && mode < 4){
	mode ++;
	mode = mode%4;
		
	}
	
	//if in PLAY mode
		if(mode == 0){
	if(buttonStates[0] == 1){
	
			//press play, reset playhead when stopping and starting
	 			if(play == 1){
	 				play = 0;
	 				playHead = 0;
	 		}	else{
	 				play = 1;
	 				playHead = 0;
	 				
	 			}
	 	 }
	 	 
	 	 //is dumbraind looking for new voice
	 	 	if(buttonStates[1] == 1 || buttonStates[1] == 2){
	 	 			
			testBrain.triggerRecordMode();
	 	 }else{
	 	 		testBrain.endRecordMode();
	 	 }
	 	 
	 	 //clear sequences when both swiches are pressed
	 	 	  if((buttonStates[2] == 1 || buttonStates[2] == 2)&&(buttonStates[1] == 1 || buttonStates[1] == 2))
			testBrain.clear();
			
			//enter edit mode
		}else if(mode ==1){
			
			//stope the sequenceer
			play = 0;
			
			//check if we want to go back
			if(buttonStates[0] == 1){
				
				//reset mode and reassign parmaeters from network
				mode =0;
				Bela_scheduleAuxiliaryTask(gRunRegression);
			}
			
			//light up all the LEDs
			for(uint8_t i = 0 ; i < 8; i++){
		
            		leds[i] = 1;
				}
		
			//get some more aprpropriate values
			float om = ::map(pot0, 0, 0.82, 0, 1);
			float pma = ::map(pot1, 0, 0.82, 1, 10);
			float freq = ::map(pot2, 0, 0.82, 20, 300);
	
		//will find a use for this yet!
		//	float pma = ::map(pot3, 0, 0.82, 1, 10);
		
	
				  
		//update curret voice, should probaly be a drumbrain function...
			testBrain.voices[testBrain.selectedVoice].oscMix = om;
			testBrain.voices[testBrain.selectedVoice].pitchModAmount = pma;
			testBrain.voices[testBrain.selectedVoice].frequency = freq;
		
	

						  	
		//onto enveloes
		}else if (mode ==2){
			
			//are we going back
			if(buttonStates[0] == 1){
			
				mode =1;
				
			}
			//make every other LED off
				for(uint8_t i = 0 ; i < 8; i+=2){
		
            		leds[i] = 0;
				}
				
				
				float oscAtk = ::map(pot0, 0, 0.82, 0, 500);
				float oscRls = ::map(pot1,0,0.82, 20, 1000);
				float pmAtk = ::map(pot2, 0, 0.82, 0, 500);
				float pmRls = ::map(pot3,0,0.82, 20, 1000);
				
			//set thos eenvelopes
			testBrain.voices[testBrain.selectedVoice].pm.setAttack( pmAtk);
			testBrain.voices[testBrain.selectedVoice].pm.setRelease( pmRls);
			testBrain.voices[testBrain.selectedVoice].oscAmpEnv.setAttack( oscAtk);
			testBrain.voices[testBrain.selectedVoice].oscAmpEnv.setRelease( oscRls);
			testBrain.voices[testBrain.selectedVoice].pmAtk = pmAtk;
			testBrain.voices[testBrain.selectedVoice].pmRls = pmRls;
			testBrain.voices[testBrain.selectedVoice].oscAtk = oscAtk;
			testBrain.voices[testBrain.selectedVoice].oscRls = oscRls;
			}else if (mode ==3){
		
			if (buttonStates[1] == 0 )
			
			//add new trainig data
			Bela_scheduleAuxiliaryTask(gNewTrainingData);
				//onto the next mode
				mode = 4;
			
		}else if(mode ==4){
			

			//	train on new data
				Bela_scheduleAuxiliaryTask(gTrain);
				
				//moving on
				mode = 5;
		}
		else if(mode ==5){
			
			// 	wait for training to finish before moving on
			if(trained){
				mode = 0;
		
				
			}
	
		 }
		
		//if second button held tell testBrain to wait for voice sellection
	 	 if(buttonStates[2] == 1 || buttonStates[2] == 2){
			testBrain.triggerSelectMode();
	 	 }else{
	 	 	testBrain.endSelectMode();
	 	 }
	 	 
	 
		//	let drumbrain know whats going on
		testBrain.isIdle(buttonStates);
			
			//in case we want to pring
		// if (n% 10 == 0)
		// Bela_scheduleAuxiliaryTask(gPrintData);
		
	}

	//tick tock
		  benKlock.ticker();
		  
		  //if we are playing
  	if(play == 1){

		//on the beat
        if (benKlock.tick) {
        	//advance the playhead
           testBrain.step(playHead);
            playHead++;
            playHead%=8;
       
        }
      
      
}
//if in the right mode light up the leds
if(mode == 0){
	int *  curSeq = testBrain.returnCurSeq();
	for(uint8_t i = 0 ; i < 8; i++){
		//show playhead positions
			if(i == playHead && play ==1)
            		leds[i] = 1;
            else if(curSeq[i] == 1)	
            		leds[i]=1;
            else
            	leds[i] = 0;
	
            	
            }
}
	
	//assignin the output
   float out = testBrain.play(n) * vol ;
	
	//bela stuff
		for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
		
			audioWrite(context, n, channel, out);
		}
		
		//write LEDs
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

//compare current state with previous to see f switch has just been pressed, being helpd or is off
	for( int i = 0; i <= 11; i++){

		if(buttonInputs[i] == 1 && buttonStates[i] ==0)
				buttonStates[i] = 1;
	else if (buttonInputs[i] == 1 && buttonStates[i] ==1)
		buttonStates[i] = 2;
	else if( buttonInputs[i] == 0)
		buttonStates[i] = 0;
	}

}

void runRegression (void*){
	

	//assign regression output to vector and use it to update synth
		std::vector<double> regressionOutput;
		 regressionOutput= testRapid.addRunExample(pot2, pot3);
	
		 testBrain.updateAll(regressionOutput);
		 

		
}

void newTrainingData (void*){
	
		//lets get the input
		//freq, pma, gain_, om, oa,  oR, pa, pr, 
			vector<double> newKit = {  testBrain.voices[0].frequency, testBrain.voices[0].pitchModAmount, 1.0,  testBrain.voices[0].oscMix, testBrain.voices[0].oscAtk,  testBrain.voices[0].oscRls,   testBrain.voices[0].pmAtk,  testBrain.voices[0].pmRls, 
								       testBrain.voices[1].frequency, testBrain.voices[1].pitchModAmount, 1.0,  testBrain.voices[1].oscMix, testBrain.voices[1].oscAtk,  testBrain.voices[1].oscRls,   testBrain.voices[1].pmAtk,  testBrain.voices[1].pmRls,
					 			       testBrain.voices[2].frequency, testBrain.voices[2].pitchModAmount, 1.0,  testBrain.voices[2].oscMix, testBrain.voices[2].oscAtk,  testBrain.voices[2].oscRls, 	testBrain.voices[2].pmAtk,  testBrain.voices[2].pmRls,
									   testBrain.voices[3].frequency, testBrain.voices[3].pitchModAmount, 1.0,  testBrain.voices[3].oscMix, testBrain.voices[3].oscAtk,  testBrain.voices[3].oscRls,   testBrain.voices[3].pmAtk,  testBrain.voices[3].pmRls,
									   testBrain.voices[4].frequency, testBrain.voices[4].pitchModAmount, 1.0,  testBrain.voices[4].oscMix, testBrain.voices[4].oscAtk,  testBrain.voices[4].oscRls,   testBrain.voices[4].pmAtk,  testBrain.voices[4].pmRls, 
								       testBrain.voices[5].frequency, testBrain.voices[5].pitchModAmount, 1.0,  testBrain.voices[5].oscMix, testBrain.voices[5].oscAtk,  testBrain.voices[5].oscRls,   testBrain.voices[5].pmAtk,  testBrain.voices[5].pmRls,
			};
						  	//and chuck it in the model
				testRapid.addTrainingExampleVector( pot2, pot3, newKit);
			
		 
}

void train (void*){
		//self explanatory
			trained = 	testRapid.myRegression.train(testRapid.trainingSet);
			
}
void printData (void*){
	//can use this to print whatver we want
		cout<< buttonInputs[0] << " " << mode << endl;

}

void cleanup(BelaContext *context, void *userData)
{

}
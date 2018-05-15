/***** drumBrain.cpp *****/
#include "drumBrain.h"

drumBrain::drumBrain(){
	
	
	for (uint8_t i = 0; i < NUM_OF_VOICES; i++){
		
		seqLengths[i] = 8;
	}
	
	selectedVoice = 0;
	
	recordMode = false;
	selectMode = false;

}

void drumBrain::step(int & playHead){
	
	for(uint8_t i = 0; i < NUM_OF_VOICES; i++){
		//see if voice should be plying, if it should, trigger it
			int curPlayHead = playHead % seqLengths[i];
           
            if(sequences[i][curPlayHead]  ==1){
            voices[i].trigger();
             }
    }

}

 


double drumBrain::play(int n){
	
	double output = 0;
    
  //add up thue juicy voices
		for (uint8_t i = 0; i < NUM_OF_VOICES; i++){
		
		output += voices[i].play(false, n);
	}
   
   
    return output;

}



	void drumBrain::updateVoice(uint8_t voice, double  freq, double  pma, float  gain_, float  om, float  tm,  float  oa, float  oh, float  oR, float  pa, float  pr){
	
		voices[voice].setAll(freq, pma, gain_, om, tm, oa, oh, oR, pa, pr);
	}

void drumBrain::isIdle(int (&buttonStates)[12]){

		//will select/edit sequence, trgger whe and where its appropriate
		for(uint8_t i = 4; i < 12; i++){
			
				if(buttonStates[i] == 1){
					if(selectMode && i < 12)
						selectedVoice = i-4;
					else if (recordMode)
						sequences[selectedVoice][i-4] = 1-sequences[selectedVoice][i-4]; 
					else if(i < 12)
						voices[i-4].trigger();
		
				}
			}
	
}

void drumBrain::triggerSelectMode(){
	
	selectMode = true;
	
}
void drumBrain::endSelectMode(){
	
	selectMode = false;
	
}

void drumBrain::triggerRecordMode(){
	
	recordMode = true;
	
}
void drumBrain::endRecordMode(){
	
	recordMode = false;
	
}

void drumBrain::clear(){
	//empty all the sequences
		for (uint8_t i = 0; i < NUM_OF_VOICES; i++){
			for(uint8_t j = 0; j < 16; ++j){
		sequences[i][j] = 0;
			}
	}
	
}

void drumBrain::updateAll(vector<double> vals){
	int inc = 0;
		for (uint8_t i = 0; i < 6; i++){
			
			
			//bit ugly but stops envelopes blowing up
			 if ( vals[5 + inc] < 0)
			 	 vals[5 + inc] = 1;
			if ( vals[7 + inc] < 0)
				 vals[7 + inc] = 1;
			if ( vals[6 + inc] < 0)
			 	 vals[6 + inc] = 1;
			if ( vals[ + inc] < 0)
				 vals[7 + inc] = 1;
				
				
			updateVoice(i, vals[0+ inc], vals[1+inc], 1., vals[3 + inc], 1.0, 0., 1., vals[5 + inc], vals[6 + inc], vals[7+inc]);
				inc+=8;
			}
	
	
}


	
	



int * drumBrain::returnSeq(int voice){
	
	return sequences[voice];
	
}

int * drumBrain::returnCurSeq(){
	
	return sequences[selectedVoice];
	
}

 

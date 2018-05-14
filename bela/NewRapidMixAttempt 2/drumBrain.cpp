/***** drumBrain.cpp *****/
#include "drumBrain.h"

drumBrain::drumBrain(){
	
				//freq, pma, gain_, om, tm, nm, oa, oh, oR, pa, pr, na, nr
	// voices[0].setAll(40., 4.184, 1., 0.692, 1., 0., 0., 1., 504., 0., 84., 0., 0. );
	updateVoice(0, 40., 4.184, 1., 0.692, 1., 0., 1., 504., 0., 84. );
	updateVoice(1, 200., 3.6, 1., 0.14, 0.14,  0., 1., 428., 0., 100. );
	updateVoice(2, 0., 0, 1., 0., 0., 1., 0.,  504., 0., 84. );
	updateVoice(3, 80., 4.184, 1., 0.692, 1.,  0., 1., 504., 0., 84.);
		
	// voices[0] = drumSynth();
    // voices[1].frequency = 200 ;
    // voices[2].frequency =  120;
    // voices[3].frequency= 400 ;
   /* voices[4] = drumSynth(128,  1.5, 0.5, 0.75, 0.075,     0, 0, 750, 1, 0, 16,  0, 44.5) ;
    voices[5] = drumSynth(260, 3.66, 0.5, 0.75, 0.075,     0, 0, 750, 1, 0, 16,  0, 44.5) ;
    voices[6] = drumSynth(260, 3.66, 0.5, 0.75, 0.075,     0, 0, 750, 1, 0, 16,  0, 44.5) ;
    voices[7] = drumSynth(128,  1.5, 0.5, 0.75, 0.075,     0, 0, 750, 1, 0, 16,  0, 44.5) ;
*/
	 //int chan1Seq[]= { 1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0};
  //  memcpy(sequences[0], chan1Seq, sizeof(chan1Seq));
  //  int chan2Seq[]= { 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0};
  //  memcpy(sequences[1], chan2Seq, sizeof(chan2Seq));
  //  int chan3Seq[]= { 0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0};
  //  memcpy(sequences[2], chan3Seq, sizeof(chan3Seq));
  //  int chan4Seq[]= { 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
  //  memcpy(sequences[3], chan4Seq, sizeof(chan4Seq));
   // int chan5Seq[]= { 1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0};
    // memcpy(sequences[4], chan5Seq, sizeof(chan5Seq));
    // int chan6Seq[]= { 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0};
    // memcpy(sequences[5], chan6Seq, sizeof(chan6Seq));
    // int chan7Seq[]= { 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0};
    // memcpy(sequences[6], chan7Seq, sizeof(chan7Seq));
    // int chan8Seq[]= { 0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0};
    // memcpy(sequences[7], chan8Seq, sizeof(chan8Seq));

	for (uint8_t i = 0; i < NUM_OF_VOICES; i++){
		
		seqLengths[i] = 8;
	}
	
	selectedVoice = 0;
	
	recordMode = false;
	selectMode = false;
//	seqLengths[1] = 7;
//	seqLengths[2] = 3;
	// uint8_t p = 3;
	// uint8_t l = 7;
	// euclidSeq(l, p);
}

void drumBrain::step(int & playHead){
	
	for(uint8_t i = 0; i < NUM_OF_VOICES; i++){
		
			int curPlayHead = playHead % seqLengths[i];
           
            if(sequences[i][curPlayHead]  ==1){
            voices[i].trigger();
             }
    }

}

 void drumBrain::euclidSeq( uint8_t & length, uint8_t pulses){
 	 
 	 for(uint8_t i = 0; i < pulses; ++i){
 	 	
 	 	sequences[0][i]= 1;
 	 }
 	  for(uint8_t i = pulses; i < 8; ++i){
 	 	
 	 	sequences[0][i]= 0;
 	 }
 	 
 	 seqLengths[0] = length;
 	  
 	 //euclid E;
 	 //E.euclidAlg(length, pulses);
 	 //vector<uint8_t> euclidOutput = E.returnSequence();
 	 //for(uint8_t i = 0; i < length; ++i){
 	 	
 	 //	sequences[0][i]= euclidOutput[0];
 	 //}
 
 	
 }

// void drumBrain::euclidSeq(int voice, int length, int pulses){

// vector<uint8_t> remaindr;	
// vector<uint8_t> count;	
// int  divisor;
// int level;
// divisor = length - pulses;
//  remaindr.push_back(pulses);
//  level = 0;
//  do {
//  count.push_back( divisor / remaindr[level]);
//  remaindr.push_back( divisor% remaindr[level]);
//  divisor = remaindr[level];
//  level = level + 1;
//  }
//  while (remaindr[level] > 1);
 
//  count[level] = divisor; 
// }

// void drumBrain::euclid(int8_t level){
// if (level == -1)
//  //append a “0” to the end of the bitmap;
//  else if (level == -2)
// // append a “1” to the end of the bitmap;
//  else {
//  for (i=0; i < count[level]; i++)
//  build_string (level-1);
//  if (remainder[level] != 0)
//  build_string (level-2); 
//  }
// }
double drumBrain::play(int n){
	
	double output = 0;
    
  
		for (uint8_t i = 0; i < NUM_OF_VOICES; i++){
		
		output += voices[i].play(false, n);
	}
   // output = channels[5].play(mutes[5]);
   
    return output;

}


// void drumBrain::updateVoice(uint8_t voice, double & freq, double & pma, float & gain_, float & om, float & tm, float & nm, float & oa, float & oh, float & oR, float & pa, float & pr, float & na, float & nr){
		
// 		voices[voice].setAll(freq, pma, gain_, om, tm, nm, oa, oh, oR, pa, pr, na, nr);
// 	}
	void drumBrain::updateVoice(uint8_t voice, double  freq, double  pma, float  gain_, float  om, float  tm,  float  oa, float  oh, float  oR, float  pa, float  pr){
	
		voices[voice].setAll(freq, pma, gain_, om, tm, oa, oh, oR, pa, pr);
	}

void drumBrain::isIdle(int (&buttonStates)[12]){

		
		for(uint8_t i = 4; i < 12; i++){
			
				if(buttonStates[i] == 1){
					//	Bela_scheduleAuxiliaryTask(gGenerateEuclideanSequence);
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
	
		for (uint8_t i = 0; i < NUM_OF_VOICES; i++){
			for(uint8_t j = 0; j < 16; ++j){
		sequences[i][j] = 0;
			}
	}
	
}

void drumBrain::updateAll(vector<double> vals){
	int inc = 0;
			//freq, pma, gain_, om, tm,  oa, oh, oR, pa, pr, 
		//	float f1, float t1,float osc1, float nm1, float pm1, float a1, float r1, float na1, float nr1,
			for (uint8_t i = 0; i < 4; i++){
			//	updateVoice(i, vals[0+ inc], vals[1+ inc], 1., vals[3 + inc], vals[4 + inc], vals[5 + inc], 0., 1., 504., 0., 84., 0., 0. );
			
			//updateVoice(0, 40., 4.184, 1., 0.692, 1., 0., 0., 1., 504., 0., 84., 0., 0. );
			 if ( vals[5 + inc] < 0)
			 	 vals[5 + inc] = 1;
			if ( vals[7 + inc] < 0)
				 vals[7 + inc] = 1;
				
				updateVoice(i, vals[0+ inc], vals[1+inc], 1., vals[3 + inc], 1.0, 0., 1., vals[5 + inc], vals[6 + inc], vals[7+inc]);
				inc+=8;
			}
	
	
}

void drumBrain::updateSome(vector<double> vals){
	int inc = 0;
			// pma,  om,   pa, pr, 
		//	float f1, float t1,float osc1, float nm1, float pm1, float a1, float r1, float na1, float nr1,
			for (uint8_t i = 0; i < 4; i++){
			
				voices[i].pitchModAmount = vals[i+inc];
				voices[i].oscMix = vals[i+1+inc];
				voices[i].pm.setAttack(vals[i+2+inc]);
				voices[i].pm.setRelease(vals[i+3+inc]);
				inc+=4;
			}
	
	
}


int * drumBrain::returnSeq(int voice){
	
	return sequences[voice];
	
}

int * drumBrain::returnCurSeq(){
	
	return sequences[selectedVoice];
	
}

  void drumBrain::releaseAll(){
  	
  		for (uint8_t i = 0; i < NUM_OF_VOICES; i++){
		
		voices[i].releaseAll();
	}
  	
  }

/***** drumBrain.cpp *****/
#include "drumBrain.h"

drumBrain::drumBrain(){
	
				//freq, pma, gain_, om, tm, nm, oa, oh, oR, pa, pr, na, nr
	// voices[0].setAll(40., 4.184, 1., 0.692, 1., 0., 0., 1., 504., 0., 84., 0., 0. );
	updateVoice(0, 40., 4.184, 1., 0.692, 1., 0., 0., 1., 504., 0., 84., 0., 0. );
	updateVoice(1, 200., 3.6, 1., 0.14, 0.14, 0.85, 0., 1., 428., 0., 100., 0., 228. );
	updateVoice(2, 0., 0, 1., 0., 0., 1., 0., 1., 504., 0., 84., 0., 29. );
	updateVoice(3, 80., 4.184, 1., 0.692, 1., 0., 0., 1., 504., 0., 84., 0., 0. );
		
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
double drumBrain::play(){
	
	double output = 0;
    
   // output = voices[2].play(false);
   // output = channels[0].play(mutes[0]) +channels[1].play(mutes[1]) + channels[2].play(mutes[2]) + channels[3].play(mutes[3]) + channels[4].play(mutes[4]) /*+ channels[5].play(mutes[5]) */+ channels[6].play(mutes[6]) + channels[7].play(mutes[7]);
  //  output = channels[0].play(mutes[0]) +channels[1].play(mutes[1]) + channels[2].play(mutes[2]) + channels[3].play(mutes[3]) + channels[4].play(mutes[4]) + channels[5].play(mutes[5]) + channels[6].play(mutes[6]) + channels[7].play(mutes[7]);
		for (uint8_t i = 0; i < NUM_OF_VOICES; i++){
		
		output += voices[i].play(false);
	}
   // output = channels[5].play(mutes[5]);
   
    return output;

}


// void drumBrain::updateVoice(uint8_t voice, double & freq, double & pma, float & gain_, float & om, float & tm, float & nm, float & oa, float & oh, float & oR, float & pa, float & pr, float & na, float & nr){
		
// 		voices[voice].setAll(freq, pma, gain_, om, tm, nm, oa, oh, oR, pa, pr, na, nr);
// 	}
	void drumBrain::updateVoice(uint8_t voice, double  freq, double  pma, float  gain_, float  om, float  tm, float  nm, float  oa, float  oh, float  oR, float  pa, float  pr, float  na, float  nr){
		
		voices[voice].setAll(freq, pma, gain_, om, tm, nm, oa, oh, oR, pa, pr, na, nr);
	}

void drumBrain::isIdle(int (&buttonStates)[11]){

		
		for(uint8_t i = 3; i < 11; i++){
			
				if(buttonStates[i] == 1){
					//	Bela_scheduleAuxiliaryTask(gGenerateEuclideanSequence);
					if(selectMode && i < 7)
						selectedVoice = i-3;
					else if (recordMode)
						sequences[selectedVoice][i-3] = 1-sequences[selectedVoice][i-3]; 
					else if(i < 7)
						voices[i-3].trigger();
		
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


/***** drumBrain.cpp *****/
#include "drumBrain.h"

drumBrain::drumBrain(){
	
	for (uint8_t i = 0; i < NUM_OF_VOICES; i++){
		
		seqLengths[i] = 16;
	}
	
}

void drumBrain::step(int & playHead){
	
	for(int i = 0; i < NUM_OF_VOICES; i++){
		
			int curPlayHead = playHead % seqLengths[i];
           
            if(voices[i].sequence[curPlayHead]  ==1){
            voices[i].trigger();
             }
    }

}


double drumBrain::play(){
	
	double output = 0;
    
    
   // output = channels[0].play(mutes[0]) +channels[1].play(mutes[1]) + channels[2].play(mutes[2]) + channels[3].play(mutes[3]) + channels[4].play(mutes[4]) /*+ channels[5].play(mutes[5]) */+ channels[6].play(mutes[6]) + channels[7].play(mutes[7]);
  //  output = channels[0].play(mutes[0]) +channels[1].play(mutes[1]) + channels[2].play(mutes[2]) + channels[3].play(mutes[3]) + channels[4].play(mutes[4]) + channels[5].play(mutes[5]) + channels[6].play(mutes[6]) + channels[7].play(mutes[7]);
		for (uint8_t i = 0; i < NUM_OF_VOICES; i++){
		
		output += voices[i].play(false);
	}
   // output = channels[5].play(mutes[5]);
    if (output > 1) output = 0.7;
    return output;

}
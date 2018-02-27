/***** drumBrain.cpp *****/
#include "drumBrain.h"

drumBrain::drumBrain(){
	
	// voices[0] = drumSynth();
    voices[1].frequency = 200 ;
    voices[2].frequency =  120;
 //   voices[3] = drumSynth() ;
   /* voices[4] = drumSynth(128,  1.5, 0.5, 0.75, 0.075,     0, 0, 750, 1, 0, 16,  0, 44.5) ;
    voices[5] = drumSynth(260, 3.66, 0.5, 0.75, 0.075,     0, 0, 750, 1, 0, 16,  0, 44.5) ;
    voices[6] = drumSynth(260, 3.66, 0.5, 0.75, 0.075,     0, 0, 750, 1, 0, 16,  0, 44.5) ;
    voices[7] = drumSynth(128,  1.5, 0.5, 0.75, 0.075,     0, 0, 750, 1, 0, 16,  0, 44.5) ;
*/
	 int chan1Seq[]= { 1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0};
    memcpy(sequences[0], chan1Seq, sizeof(chan1Seq));
    int chan2Seq[]= { 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0};
    memcpy(sequences[1], chan2Seq, sizeof(chan2Seq));
    int chan3Seq[]= { 0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0};
    memcpy(sequences[2], chan3Seq, sizeof(chan3Seq));
  /*  int chan4Seq[]= { 0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0};
    memcpy(sequences[3], chan4Seq, sizeof(chan4Seq));
    int chan5Seq[]= { 1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0};
    memcpy(sequences[4], chan5Seq, sizeof(chan5Seq));
    int chan6Seq[]= { 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0};
    memcpy(sequences[5], chan6Seq, sizeof(chan6Seq));
    int chan7Seq[]= { 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0};
    memcpy(sequences[6], chan7Seq, sizeof(chan7Seq));
    int chan8Seq[]= { 0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0};
    memcpy(sequences[7], chan8Seq, sizeof(chan8Seq));
*/
	for (uint8_t i = 0; i < NUM_OF_VOICES; i++){
		
		seqLengths[i] = 16;
	}
	
	seqLengths[1] = 7;
	seqLengths[2] = 3;
}

void drumBrain::step(int & playHead){
	
	for(uint8_t i = 0; i < NUM_OF_VOICES; i++){
		
			int curPlayHead = playHead % seqLengths[i];
           
            if(sequences[i][curPlayHead]  ==1){
            voices[i].trigger();
             }
    }

}


double drumBrain::play(){
	
	double output = 0;
    
   // output = voices[2].play(false);
   // output = channels[0].play(mutes[0]) +channels[1].play(mutes[1]) + channels[2].play(mutes[2]) + channels[3].play(mutes[3]) + channels[4].play(mutes[4]) /*+ channels[5].play(mutes[5]) */+ channels[6].play(mutes[6]) + channels[7].play(mutes[7]);
  //  output = channels[0].play(mutes[0]) +channels[1].play(mutes[1]) + channels[2].play(mutes[2]) + channels[3].play(mutes[3]) + channels[4].play(mutes[4]) + channels[5].play(mutes[5]) + channels[6].play(mutes[6]) + channels[7].play(mutes[7]);
		for (uint8_t i = 0; i < NUM_OF_VOICES; i++){
		
		output += voices[i].play(false);
	}
   // output = channels[5].play(mutes[5]);
    if (output > 1) output = 0.7;
    return output;

}
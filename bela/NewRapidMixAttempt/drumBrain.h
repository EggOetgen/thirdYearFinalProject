/***** drumBrain.h *****/
//
//  drumMachineBrain.hpp
//  kickMachine
//
//  Created by Edmund Oetgen on 21/11/2017.
//
//

#pragma once

#include "stdint.h"
#include "maximilian.h"
#include "drumSynth.h"
#include "euclid.h"

#define NUM_OF_VOICES 8


class drumBrain{

public:

    drumBrain();

    double play(int n);
    void step(int &playHead);
    int * returnSeq(int voice);
    int * returnCurSeq();
  
     void triggerSelectMode();
    void endSelectMode();
   
    void triggerRecordMode();
    void endRecordMode();
      void clear();
   
  
     void updateVoice(uint8_t voice,  double  freq, double  pma, float  gain_, float  om, float  tm, float  oa, float  oh, float  oR, float  pa, float  pr);
	void updateAll(vector<double> vals);

	void isIdle(int (&buttonStates)[12]);
    void euclidSeq( uint8_t & length, uint8_t pulses);
     void sequenceVoice( int voice);
    
	drumSynth voices[NUM_OF_VOICES];
	

	
	int seqLengths[NUM_OF_VOICES];
	int sequences[NUM_OF_VOICES][8];
   
    	int8_t selectedVoice;
    private:
    	bool selectMode;
    	bool recordMode;
    
   

};

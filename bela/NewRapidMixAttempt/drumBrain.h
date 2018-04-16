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

#define NUM_OF_VOICES 6


class drumBrain{

public:

    drumBrain();

    double play(int n);
    void step(int &playHead);
   
     void triggerSelectMode();
    void endSelectMode();
    void triggerRecordMode();
    void endRecordMode();
      void clear();
   
   	// void updateVoice(uint8_t voice, double & freq, double & pma, float & gain_, float & om, float & tm, float & nm, float & oa, float & oh, float & oR, float & pa, float & pr, float & na, float & nr);
     void updateVoice(uint8_t voice,  double  freq, double  pma, float  gain_, float  om, float  tm, float  nm, float  oa, float  oh, float  oR, float  pa, float  pr, float  na, float  nr);
	void updateAll(vector<double> vals);
	void isIdle(int (&buttonStates)[11]);
    void euclidSeq( uint8_t & length, uint8_t pulses);
     void sequenceVoice( int voice);
    // void euclidSeq(int voice, int length, int pulses);
	// vector<uint8_t> euclid(int8_t level);
//	int numOfVoices = 1;
   // drumSynth test, test2, test3, test4;
	drumSynth voices[NUM_OF_VOICES];
	

	
	int seqLengths[NUM_OF_VOICES];
	int sequences[NUM_OF_VOICES][8];
    float freqs[16]  = {400,160,0,80,0,160,120,200,180,80,40,80,40,160,40,80};
    	int8_t selectedVoice;
    private:
    	bool selectMode;
    	bool recordMode;
    
    // void update();
    // void setFrequency(double newFreq);
    // void setGain(float newGain);
    // void setModAmount(double modAmount);
    // void setOscEnv(double attack, double decay, double sustain, double release);
    // void setModEnv(double attack, double decay, double sustain, double release);
    // void setNoiseEnv(double attack, double decay, double sustain, double release);
    // void setOscMix(float mix);
    // void setNoiseMix(float mix);
    // void setToneMix(float mix);
    // void randomiseSeq();
    //
    // maxiOsc sine;
    // maxiOsc saw;
    // maxiOsc noise;
    // maxiOsc FM;
    //
    // maxiEnv oscAmpEnv;
    // maxiEnv test;
    //
    // maxiEnv noiseAmpEnv;
    //
    // maxiEnv pitchMod;
    // maxiEnv pm;
    //
    //
    // double frequency = 40;
    //
    // double oscEnvOut = 0;
    // double pitchModOut = 1.0;
    // double pitchModAmount = 0;
    // double noiseEnvOut = 0;
    //
    // double output = 0;
    //
    // float gain = 0;
    //
    // float oscMix = 1.0;
    // float noiseMix = 0.5;
    // float toneMix = 0.25;
    //
    // int sequence [16] = {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0};

};

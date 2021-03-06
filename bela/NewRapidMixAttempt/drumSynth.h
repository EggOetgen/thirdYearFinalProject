//
//  drumSynth.hpp
//  kickMachine
//
//  Created by Edmund Oetgen on 21/11/2017.
//
//

#pragma once


#include "maximilian.h"
#include <OscillatorBank.h>

class drumSynth{

public:

    drumSynth(double freq = 60, double pma = 2.5, float gain_ =0.5, float om = 1.0, float tm = 1.0, float nm = 0.1, float oa = 0,float oh = 1.0, float oR = 1000, float pa = 0, float pr = 100, float na =0, float nr = 5, int sampleRate_ = 1024);

    double play(bool isMute, int n);
    void trigger();
    void update();
    void setFrequency(double newFreq);
    void setGain(float newGain);
    void setModAmount(double modAmount);
    void setOscEnv(double attack, double decay, double sustain, double release);
    void setModEnv(double attack, double decay, double sustain, double release);
    void setNoiseEnv(double attack, double decay, double sustain, double release);
    void setOscMix(float mix);
    void setNoiseMix(float mix);
    void setToneMix(float mix);
    void setAll(double & freq, double & pma, float & gain_, float & om, float & tm,  float & oa, float & oh, float & oR, float & pa, float & pr );
    
    maxiOsc sine;
    maxiOsc saw;
    maxiOsc noise;
   // maxiOsc FM;
	
	OscillatorBank sineBank;
	
    maxiEnv oscAmpEnv;
  
    maxiEnv noiseAmpEnv;

   ;
    maxiEnv pm;


    double frequency = 40;

    double oscEnvOut = 0;
    double pitchModOut = 1.0;
    double pitchModAmount = 0;
   // double noiseEnvOut = 0;

    double output = 0;

    float gain = 0;

    float oscMix = 1.0;
    float noiseMix = 0.5;
    float toneMix = 0.25;

    
	int sampleRate;
	
	///writ about this
	float pmAtk, pmRls;
	float oscAtk, oscRls;
};

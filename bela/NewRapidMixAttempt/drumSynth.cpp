#include "drumSynth.h"

//
//  drumSynth.cpp
//  kickMachine
//
//  Created by Edmund Oetgen on 21/11/2017.
//


drumSynth::drumSynth(double freq, double pma, float gain_, float om, float tm, float nm, float oa, float oh, float oR, float pa, float pr, float na, float nr, int sampleRate_){

  
    frequency = freq;
    pitchModAmount = pma;

    gain = gain_;

    oscMix = om;
    toneMix = tm;
    noiseMix = nm;

    oscAmpEnv.setAttack(oa);
    oscAmpEnv.setDecay(1);
    oscAmpEnv.setSustain(1.);
    oscAmpEnv.setRelease(oR);
    oscAmpEnv.holdtime = 0;
    oscAmpEnv.trigger = 0;



    pm.setAttack(pa);
    pm.setDecay(1);
    pm.setSustain(1.);
    pm.setRelease(pr);
    pm.holdtime = 0;
    pm.trigger = 0;



    // noiseAmpEnv.setAttack(na);
    // noiseAmpEnv.setDecay(1);
    // noiseAmpEnv.setSustain(1.);
    // noiseAmpEnv.setRelease(nr);
    // noiseAmpEnv.holdtime = 0;
    // noiseAmpEnv.trigger = 0;



}

double drumSynth::play(bool isMute, int n){


output = 0;
    if(!isMute){
    
     //get envelope oututs
        oscEnvOut = oscAmpEnv.adsr(1., oscAmpEnv.trigger);
        pitchModOut = pm.adsr(1., pm.trigger);

       //keep the noise stuff jsut in case
        pitchModOut = (pitchModOut * (pitchModAmount));
     // noiseEnvOut = noiseAmpEnv.adsr(1., noiseAmpEnv.trigger);
     //   noiseEnvOut = test.adsr(1., test.trigger);
      
// calculate oscillaor levels
        float sineMix = oscMix;
        float sawMix = (1.-oscMix);
	
	//get osccialotos outous
        double sineOutput =   sine.sinebuf(frequency*(1+pitchModOut))*oscEnvOut;
        double sawOutput = saw.triangle(frequency*(1+pitchModOut))*oscEnvOut;
       // double noiseOutput;
      
    	  // noiseOutput = noise.noise()*noiseEnvOut;
//         else
//         	noiseOutput = 0;

        output = (((sineMix*sineOutput) + (sawMix*sawOutput)) * toneMix);// + (noiseOutput * noiseMix);
    

        if(oscAmpEnv.trigger == 1){
            oscAmpEnv.trigger =0;

        }

      
        if(pm.trigger == 1){
            pm.trigger =0;

        }

        // if(noiseAmpEnv.trigger == 1){
        //     noiseAmpEnv.trigger =0;

        // }

      


        return output * gain;
    }else{
        return 0;
    }
}

void drumSynth::trigger(){
	
	//set al our triggers to 0 and reset oscillato phase, pahse bt might not be needed
    oscAmpEnv.trigger = 1;
  //  noiseAmpEnv.trigger = 1;
    pm.trigger = 1;
    sine.phaseReset(0);
    saw.phaseReset(0);


}

void drumSynth::setFrequency(double newFreq){

    frequency = newFreq;

}

void drumSynth::setModAmount(double modAmount){

    pitchModAmount = modAmount;

}

void drumSynth::setOscEnv(double attack, double decay, double sustain, double release){

    oscAmpEnv.setAttack(attack);
    oscAmpEnv.setDecay(decay);
    oscAmpEnv.setSustain(sustain);
    oscAmpEnv.setRelease(release);

}

void drumSynth::setModEnv(double attack, double decay, double sustain, double release){

  
    pm.setAttack(attack);
    pm.setDecay(decay);
    pm.setSustain(sustain);
    pm.setRelease(release);

}

void drumSynth::setNoiseEnv(double attack, double decay, double sustain, double release){

    noiseAmpEnv.setAttack(attack);
    noiseAmpEnv.setDecay(decay);
    noiseAmpEnv.setSustain(sustain);
    noiseAmpEnv.setRelease(release);

  

}

void drumSynth::setOscMix(float mix){

    oscMix = mix;
}

void drumSynth::setNoiseMix(float mix){

    noiseMix = mix;
}

void drumSynth::setToneMix(float mix){

    toneMix = mix;
}

void drumSynth::setGain(float newGain){

    gain = newGain;
}




 void drumSynth::setAll(double & freq, double & pma, float & gain_, float & om, float & tm, float & oa, float & oh, float & oR, float & pa, float & pr){
	frequency = freq;
    pitchModAmount = pma;

    gain = gain_;

    oscMix = om;
    toneMix = tm;
  //  noiseMix = nm;

    oscAmpEnv.setAttack(oa);
    oscAmpEnv.setDecay(1);
    oscAmpEnv.setSustain(0.5);
    oscAmpEnv.setRelease(oR);
    oscAmpEnv.holdtime = 0;
    oscAmpEnv.trigger = 0;


  

    pm.setAttack(pa);
    pm.setDecay(1);
    pm.setSustain(1.);
    pm.setRelease(pr);
    pm.holdtime = 0;
    pm.trigger = 0;
    
 }
 


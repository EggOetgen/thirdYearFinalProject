#include "drumSynth.h"

//
//  drumSynth.cpp
//  kickMachine
//
//  Created by Edmund Oetgen on 21/11/2017.
//


drumSynth::drumSynth(double freq, double pma, float gain_, float om, float tm, float nm, float oa, float oh, float oR, float pa, float pr, float na, float nr){


    frequency = freq;
    pitchModAmount = pma;

    gain = gain_;

    oscMix = om;
    toneMix = tm;
    noiseMix = nm;

    oscAmpEnv.setAttack(oa);
    oscAmpEnv.setDecay(1);
    oscAmpEnv.setSustain(oh);
    oscAmpEnv.setRelease(oR);
    oscAmpEnv.holdtime = 0;
    oscAmpEnv.trigger = 0;


    pitchMod.setAttack(pa);
    pitchMod.setDecay(1);
    pitchMod.setSustain(1);
    pitchMod.setRelease(pr);
    pitchMod.holdtime = 0;
    pitchMod.trigger = 0;

    pm.setAttack(pa);
    pm.setDecay(1);
    pm.setSustain(1);
    pm.setRelease(pr);
    pm.holdtime = 0;
    pm.trigger = 0;



    noiseAmpEnv.setAttack(na);
    noiseAmpEnv.setDecay(1);
    noiseAmpEnv.setSustain(1);
    noiseAmpEnv.setRelease(nr);
    noiseAmpEnv.holdtime = 0;
    noiseAmpEnv.trigger = 0;

    test.setAttack(na);
    test.setDecay(1);
     test.setSustain(1);
    test.setRelease(nr);
   test.holdtime = 0;
    test.trigger = 0;



}

double drumSynth::play(bool isMute){

    output = 0;
    if(!isMute){
        oscEnvOut = oscAmpEnv.adsr(1., oscAmpEnv.trigger);
        pitchModOut = pitchMod.adsr(pitchModAmount, pitchMod.trigger);
        pitchModOut = pm.adsr(1., pitchMod.trigger);

        //https://stackoverflow.com/questions/3451553/value-remapping
      //  pitchModOut = (pitchModOut * (pitchModAmount));
      // noiseEnvOut = noiseAmpEnv.adsr(1., noiseAmpEnv.trigger);
        noiseEnvOut = test.adsr(1., test.trigger);
       // cout <<noiseEnvOut <<endl;

        float sineMix = oscMix;
        float sawMix = (1.-oscMix);
//
        double sineOutput = sine.sinewave(frequency*(1+pitchModOut))*oscEnvOut;
        double sawOutput = saw.triangle(frequency*(1+pitchModOut))*oscEnvOut;
        double noiseOutput = noise.noise()*noiseEnvOut;
//                double sineOutput = sine.sinewave(frequency*(1+oscEnvOut))*oscEnvOut;
//                double sawOutput = saw.triangle(frequency*(1+oscEnvOut))*oscEnvOut;
//                double noiseOutput = noise.noise()*noiseEnvOut;
        output = (((sineMix*sineOutput) + (sawMix*sawOutput)) * toneMix) + (noiseOutput * noiseMix);
     //   output = sineOutput*sineMix ;

        if(oscAmpEnv.trigger == 1){
            oscAmpEnv.trigger =0;

        }

        if(pitchMod.trigger == 1){
            pitchMod.trigger =0;

        }
        if(pm.trigger == 1){
            pm.trigger =0;

        }

        if(noiseAmpEnv.trigger == 1){
            noiseAmpEnv.trigger =0;

        }

        if(test.trigger == 1){
            test.trigger =0;

        }



        return output * gain;
    }else{
        return 0;
    }
}

void drumSynth::trigger(){
    oscAmpEnv.trigger = 1;
    pitchMod.trigger = 1;
    noiseAmpEnv.trigger = 1;
    test.trigger = 1;
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

    pitchMod.setAttack(attack);
    pitchMod.setDecay(decay);
    pitchMod.setSustain(sustain);
    pitchMod.setRelease(release);

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

    test.setAttack(attack);
    test.setDecay(decay);
    test.setSustain(sustain);
    test.setRelease(release);


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

void drumSynth::randomiseSeq(){
    //
    // for(int i = 0; i < 16; i++){
    //     sequence[i] = (int)ofRandom(2);
    // }
}

void drumSynth::mutate(){

  for (int i = 0; i < 16; i++){
  int rando = rand() % 10;
//  cout<<rando <<endl;
  if(rando == 1){

  if(sequence[i] ==0) sequence[i]=1;
 else if(sequence[i] ==1) sequence[i]=0;
  }
    }


}

#include "drumMachineBrain.h"

//
//  drumSynth.cpp
//  kickMachine
//
//  Created by Edmund Oetgen on 21/11/2017.
//


drumMachineBrain::drumMachineBrain(){

  test2 = drumSynth(260, 3.66, 0.5, 0.75, 0.075, 0.815, 0, 750, 1, 0, 16,  0, 100);
  int chan2Seq[]= { 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0};
  test2.frequency = 160;
  memcpy(test2.sequence, chan2Seq, sizeof(chan2Seq));

  test3 = drumSynth(260, 3.66, 0.5, 0.75, 0., 0.815, 0, 750, 1, 0, 16,  0, 30);
  int chan3Seq[]= { 0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1};
  test3.frequency = 0;
  memcpy(test3.sequence, chan3Seq, sizeof(chan3Seq));

test4 = drumSynth(40,  2.5, 0.5, 0.5, 1.0, 0.0, 0,1.0,  400,  0,   100,  0, 5);
//test4.oscMix = 0.1;
  int chan4Seq[]= { 0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1};
  memcpy(test4.sequence, chan4Seq, sizeof(chan4Seq));

}

double drumMachineBrain::play(){

 double output = (test.play(false)+test2.play(false)+test3.play(false)+test4.play(false))* 0.3;

 return output;
}

void drumMachineBrain::trigger(int playHead){
  test4.frequency = freqs[playHead];

  if(test.sequence[playHead]  ==1){
 test.trigger();

        }
        if(test2.sequence[playHead]  ==1){
       test2.trigger();

              }
              if(test3.sequence[playHead]  ==1){
             test3.trigger();

                    }
                    if(test4.sequence[playHead]  ==1){
                   test4.trigger();

                          }
if (playHead ==0){
  test.mutate();
  //test2.mutate();
  test3.mutate();
  test4.mutate();


}

}

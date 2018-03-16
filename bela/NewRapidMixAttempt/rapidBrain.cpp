// /***** rapidBrain.cpp *****/
 #include "rapidBrain.h"
 

//  rapidBrain::rapidBrain(){
	
// // //myNN = 	new neuralNetwork<regression>(1, {0,1},1,2 );
//  }

void rapidBrain::addTrainingExample(float contKnob, float PitchKnob, float timbreKnob){
	
 trainingExample tempExample;
      // tempExample.input = {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14};
        // tempExample.input = {contKnob};
        // tempExample.output = {PitchKnob, timbreKnob};
        // trainingSet.push_back(tempExample);
        
         tempExample.input = {PitchKnob, timbreKnob};
        tempExample.output = {(rand()*(1.0 / (RAND_MAX + 1.0)*400)),(rand()*(1.0 / (RAND_MAX + 1.0)*4))  };
      //  double freq, double pma, float gain_, float om, float tm, float nm, float oa, float oh, float oR, float pa, float pr, float na, float nr
        std::cout<<tempExample.output[0] <<std::endl; 
        trainingSet.push_back(tempExample);


}

  std::vector<double> rapidBrain::addRunExample(float PitchKnob, float timbreKnob){
	
  		std::vector<double> input = {PitchKnob, timbreKnob};
        std::vector<double> output = myRegression.process( input);
        
       	// std::vector<double> input = {knobPos};
        // std::vector<double> output = myRegression.process( input);
        // modulationFrequency = output[0];
        // modulationDepth = output[1];
        // centerFrequency = output[2];
        // resonance = output[3];
	return output;
	
}

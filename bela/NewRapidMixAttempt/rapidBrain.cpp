// /***** rapidBrain.cpp *****/
 #include "rapidBrain.h"
 

//  rapidBrain::rapidBrain(){
	
// // //myNN = 	new neuralNetwork<regression>(1, {0,1},1,2 );
//  }

void rapidBrain::addTrainingExample(float contKnob, float PitchKnob, float timbreKnob, float f, float t, float pm, float a,float r){
	
 trainingExample tempExample;
      // tempExample.input = {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14};
        // tempExample.input = {contKnob};
        // tempExample.output = {PitchKnob, timbreKnob};
        // trainingSet.push_back(tempExample);
        
         tempExample.input = {PitchKnob, timbreKnob};
       // tempExample.output = {(rand()*(1.0 / (RAND_MAX + 1.0)*400)),(rand()*(1.0 / (RAND_MAX + 1.0)*4)),(rand()*(1.0 / (RAND_MAX + 1.0)*4000))  };
       tempExample.output = {f, t, pm, a, r ,f, t, pm, a, r ,f, t, pm, a, r ,f, t, pm, a, r  };
      //  double freq, double pma, float gain_, float om, float tm, float nm, float oa, float oh, float oR, float pa, float pr, float na, float nr
       // std::cout<<tempExample.output[0] <<std::endl; 
        trainingSet.push_back(tempExample);


}

void rapidBrain::addTrainingExampleWholeKit(float contKnob, float PitchKnob, float timbreKnob, float f1, float t1, float nm1, float pm1, float a1, float r1, float na1, float nr1,float f2, float t2, float nm2, float pm2, float a2, float r2, float na2, float nr2,float f3, float t3, float nm3, float pm3, float a3, float r3, float na3, float nr3,float f4, float t4, float nm4, float pm4, float a4, float r4, float na4, float nr4){
	
 trainingExample tempExample;
      // tempExample.input = {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14};
        // tempExample.input = {contKnob};
        // tempExample.output = {PitchKnob, timbreKnob};
        // trainingSet.push_back(tempExample);
        
         tempExample.input = {PitchKnob, timbreKnob};
       // tempExample.output = {(rand()*(1.0 / (RAND_MAX + 1.0)*400)),(rand()*(1.0 / (RAND_MAX + 1.0)*4)),(rand()*(1.0 / (RAND_MAX + 1.0)*4000))  };
       tempExample.output = {f1, t1, nm1, pm1, a1, r1, na1, nr1,f2, t2, nm2, pm2, a2, r2, na2, nr2,f3, t3, nm3, pm3, a3, r3, na3, nr3,f4, t4, nm4, pm4, a4, r4, na4, nr4 };
      //  double freq, double pma, float gain_, float om, float tm, float nm, float oa, float oh, float oR, float pa, float pr, float na, float nr
       // std::cout<<tempExample.output[0] <<std::endl; 
        trainingSet.push_back(tempExample);


}

void rapidBrain::addTrainingExampleVector(float PitchKnob, float timbreKnob, std::vector<double>trainingVals){
	
	trainingExample tempExample;
      // tempExample.input = {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14};
        // tempExample.input = {contKnob};
        // tempExample.output = {PitchKnob, timbreKnob};
        // trainingSet.push_back(tempExample);
        
         tempExample.input = {PitchKnob, timbreKnob};
       // tempExample.output = {(rand()*(1.0 / (RAND_MAX + 1.0)*400)),(rand()*(1.0 / (RAND_MAX + 1.0)*4)),(rand()*(1.0 / (RAND_MAX + 1.0)*4000))  };
       tempExample.output = {trainingVals };
      //  double freq, double pma, float gain_, float om, float tm, float nm, float oa, float oh, float oR, float pa, float pr, float na, float nr
       // std::cout<<tempExample.output[0] <<std::endl; 
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
std::vector<double> rapidBrain::addRunExampleVec(std::vector<double> inputs){
	
  		std::vector<double> input = inputs;
        std::vector<double> output = myRegression.process( input);
        
       	// std::vector<double> input = {knobPos};
        // std::vector<double> output = myRegression.process( input);
        // modulationFrequency = output[0];
        // modulationDepth = output[1];
        // centerFrequency = output[2];
        // resonance = output[3];
	return output;
	
}

// /***** rapidBrain *****/

#pragma once
#include <iostream>  
#include <stdlib.h>
#include "regression.h"
#include "neuralNetwork.h"


class rapidBrain{
	public:

     regression  myRegression;//(2, {0,1},1,2 );
    std::vector<trainingExample> trainingSet;
    //neuralNetwork<regression> * myNN;
    bool recording;
    bool trained;
    // void addTrainingExample(float contKnob, float PitchKnob, float timbreKnob, float f, float t, float pm, float a, float r);
    // void addTrainingExampleWholeKit(float contKnob, float PitchKnob, float timbreKnob, float f1, float t1, float nm1, float pm1, float a1, float r1, float na1, float nr1,float f2, float t2, float nm2, float pm2, float a2, float r2, float na2, float nr2,float f3, float t3, float nm3, float pm3, float a3, float r3, float na3, float nr3,float f4, float t4, float nm4, float pm4, float a4, float r4, float na4, float nr4);
	void addTrainingExampleVector(float pot0, float pot1, std::vector<double>trainingVals);
    std::vector<double> addRunExample(float pot0, float pot1);
     std::vector<double> addRunExampleVec(std::vector<double> inputs);

	
 };


// /***** rapidBrain *****/

#pragma once
#include <iostream>  
#include <stdlib.h>
#include "regression.h"
#include "neuralNetwork.h"


class rapidBrain{
	public:
// //	std::vector<double> getInputs(ofxFaceTracker& tracker);
// 	rapidBrain();
// 	bool isTraining;
// /** This constructor creates a neural network that needs to be trained.
//      *
//      * @param num_inputs is the number of inputs the network will process
//      * @param which_inputs is an vector of which values in the input vector are being fed to the network. ex: {0,2,4}
//      * @param num_hidden_layer is the number of hidden layers in the network. Must be at least 1.
//      * @param num_hidden_nodes is the number of hidden nodes in each hidden layer. Often, this is the same as num_inputs
//      *
//      * @return A neuralNetwork instance with randomized weights and no normalization values. These will be set or adjusted during training.
//      */
//     // neuralNetwork<regression> * myNN;//(2, {0,1},1,2 );
    
     regression  myRegression;//(2, {0,1},1,2 );
    std::vector<trainingExample> trainingSet;
    //neuralNetwork<regression> * myNN;
    bool recording;
    bool trained;
    void addTrainingExample(float contKnob, float PitchKnob, float timbreKnob, float f, float t, float pm, float a, float r);
    void addTrainingExampleWholeKit(float contKnob, float PitchKnob, float timbreKnob, float f1, float t1, float nm1, float pm1, float a1, float r1, float na1, float nr1,float f2, float t2, float nm2, float pm2, float a2, float r2, float na2, float nr2,float f3, float t3, float nm3, float pm3, float a3, float r3, float na3, float nr3,float f4, float t4, float nm4, float pm4, float a4, float r4, float na4, float nr4);
	void addTrainingExampleVector(float pot0, float pot1, std::vector<double>trainingVals);
    std::vector<double> addRunExample(float pot0, float pot1);
     std::vector<double> addRunExampleVec(std::vector<double> inputs);

	
 };


// /***** rapidBrain *****/

#pragma once
#include <iostream>  
#include <stdlib.h>
#include "regression.h"


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
    bool recording;
    bool trained;
    void addTrainingExample(float contKnob, float PitchKnob, float timbreKnob);
    std::vector<double> addRunExample(float PitchKnob, float timbreKnob);

	
 };


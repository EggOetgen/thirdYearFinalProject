// /***** rapidBrain.cpp *****/
 #include "rapidBrain.h"
 

//all quite self explantaroty, just wrapping up the rapidlib functions
void rapidBrain::addTrainingExampleVector(float pot0, float pot1, std::vector<double>trainingVals){
	
	//add inoputs to trainign data
	trainingExample tempExample;
  
        
        tempExample.input = {pot0, pot1};
      
       tempExample.output = {trainingVals };
      
        trainingSet.push_back(tempExample);

}
  
  
  std::vector<double> rapidBrain::addRunExample(float pot0, float pot1){
	
  		std::vector<double> input = {pot0, pot1};
        std::vector<double> output = myRegression.process( input);
        
       
	return output;
	
}
std::vector<double> rapidBrain::addRunExampleVec(std::vector<double> inputs){
	
  		std::vector<double> input = inputs;
        std::vector<double> output = myRegression.process( input);
        
       
	return output;
	
}

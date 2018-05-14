// /***** euclid.cpp *****/
#include "euclid.h"

void euclid::euclidAlg(uint8_t & length, uint8_t  &pulses){
	
 uint8_t divisor = length - pulses;
 remainders.push_back(pulses);
 int8_t level = 0;
 do {
 counts.push_back( divisor / remainders[level]); //counts[level]
 remainders.push_back( divisor % remainders[level]); //remainders[level+1]
 divisor = remainders[level];
 level = level + 1;
 }
 while (remainders[level] > 1);
 counts[level] = divisor;
 /*---------------------
 * Now build the bitmap string
 */
build(level);
	
}

void euclid::build(int8_t  level){


 if (level == -1)
sequence.push_back(0);// append a “0” to the end of the bitmap;
 else if (level == -2)
 sequence.push_back(1);//append a “1” to the end of the bitmap;
 else {
 for (uint8_t i = 0; 0 < counts[level]; ++i){
 build (level-1);
 }
 if (remainders[level] != 0)
 build (level-2);
 }
}

	std::vector<uint8_t> euclid::returnSequence(){
		
		return sequence;
	}
// void function compute_bitmap (int num_slots, int num_pulses)
// {
//  /*---------------------
//  * First, compute the count and remainder arrays
//  */
//  divisor = num_slots – num_pulses;
//  remainder[0] = num_pulses;
//  level = 0;
//  do {
//  count[level] = divisor / remainder[level];
//  remainder[level+1] = mod(divisor, remainder[level]);
//  divisor = remainder[level];
//  level = level + 1;
//  while (remainder[level] > 1);
//  count[level] = divisor;
//  /*---------------------
//  * Now build the bitmap string
//  */
//  build_string (level);
// }
// F
// def bjorklund(steps, pulses):
//     steps = int(steps)
//     pulses = int(pulses)
//     if pulses > steps:
//         raise ValueError    
//     pattern = []    
//     counts = []
//     remainders = []
//     divisor = steps - pulses
//     remainders.append(pulses)
//     level = 0
//     while True:
//         counts.append(divisor // remainders[level])
//         remainders.append(divisor % remainders[level])
//         divisor = remainders[level]
//         level = level + 1
//         if remainders[level] <= 1:
//             break
//     counts.append(divisor)
    
//     def build(level):
//         if level == -1:
//             pattern.append(0)
//         elif level == -2:
//             pattern.append(1)         
//         else:
//             for i in range(0, counts[level]):
//                 build(level - 1)
//             if remainders[level] != 0:
//                 build(level - 2)
    
//     build(level)
//     i = pattern.index(1)
//     pattern = pattern[i:] + pattern[0:i]
//     return pattern
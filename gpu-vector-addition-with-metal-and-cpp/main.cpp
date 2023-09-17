//
//  main.cpp
//  gpu-vector-addition-with-metal-and-cpp
//
//  Created by Julian Lork on 03.09.23.
//

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <iostream>
#include "src/AdditionWrapper.hpp"

bool verifyResults(float* summandA, float* summandB, float* result, size_t arrayLength) {
    for(size_t i = 0; i < arrayLength; i++) {
        if(summandA[i] + summandB[i] != result[i]) {
            return false;
        }
    }
    return true;
}

int main(int argc, const char * argv[]) {
    
    NS::AutoreleasePool *pReleasePool = NS::AutoreleasePool::alloc()->init();
    
    float arrayA[] = {1.0, 2.0, 3.0};
    float arrayB[] = {1.0, 2.0, 3.0};
    float *pResult;
    
    int arrayLength = sizeof(arrayA) / sizeof(float);
    int arraySize = sizeof(arrayA);
    
    GPUAddition *gpuAddition = new GPUAddition(arrayA, arrayB, arraySize);
    gpuAddition->compute();
    pResult = gpuAddition->getResult();
        
    if(verifyResults(arrayA, arrayB, pResult, arrayLength)) {
        std::cout << "\nThe GPU computation has been verified by the CPU.\n\n";
    }else {
        std::cout << "\nThe GPU computation could not be verified by the CPU.\n\n";
    }
    
    pReleasePool->release();
    
    return 0;
}

//
//  AdditionWrapper.hpp
//  gpu-vector-addition-with-metal-and-cpp
//
//  Created by Julian Lork on 03.09.23.
//

#ifndef AdditionWrapper_hpp
#define AdditionWrapper_hpp

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <Metal/Metal.hpp>

class GPUAddition {
    
public:    
    GPUAddition(float *summandA, float *summandB, int bufferSize);
    void compute();
    float *getResult();
    
private:
    MTL::Device *mDevice;
    MTL::CommandQueue *mCommandQueue;
    MTL::Library *mLibrary;
    MTL::Function *mComputeFcn;
    MTL::ComputePipelineState *mPipelineState;
    
    MTL::Buffer *mBufferSummandA;
    MTL::Buffer *mBufferSummandB;
    MTL::Buffer *mBufferResult;
    size_t mArrayLength;
    
    MTL::Size getGridSize();
    MTL::Size getNumThreadsPerThreadgroup();
};

#endif /* AdditionWrapper_hpp */

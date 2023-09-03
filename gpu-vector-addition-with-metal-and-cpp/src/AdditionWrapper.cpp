//
//  AdditionWrapper.cpp
//  gpu-vector-addition-with-metal-and-cpp
//
//  Created by Julian Lork on 03.09.23.
//

#include "AdditionWrapper.hpp"

static NS::String *SHADER_FCN_NAME = NS::String::string("sum_of_arrays", NS::ASCIIStringEncoding);
static MTL::ResourceOptions STORAGE_MODE = MTL::ResourceStorageModeShared;

GPUAddition::GPUAddition(float *summandA, float *summandB, int arraySize) {
    
    NS::Error *error;
    
    mDevice = MTL::CreateSystemDefaultDevice();
    
    if(!mDevice) {
        std::cerr << "No metal compatibe device could be found.";
        exit(-1);
    }
    
    mCommandQueue = mDevice->newCommandQueue();
    
    if(!mCommandQueue) {
        std::cerr << "Device failed to create a new command queue.";
        exit(-1);
    }
    
    mLibrary = mDevice->newDefaultLibrary();
    
    if(!mLibrary) {
        std::cerr << "Device failed to find the default library.";
        exit(-1);
    }
    
    mComputeFcn = mLibrary->newFunction(SHADER_FCN_NAME);
    
    if(!mComputeFcn){
        std::cerr << "Library failed to find the shader function.";
        exit(-1);
    }
    
    mPipelineState = mDevice->newComputePipelineState(mComputeFcn, &error);
    
    if(!mPipelineState) {
        std::cerr << "Device failed to create the pipeline state.";
        exit(-1);
    }
    
    mBufferSummandA = mDevice->newBuffer(summandA, arraySize, STORAGE_MODE);
    
    if(!mBufferSummandA) {
        std::cerr << "Device failed to create buffer for summand a.";
        exit(-1);
    }
    
    mBufferSummandB = mDevice->newBuffer(summandB, arraySize, STORAGE_MODE);
    
    if(!mBufferSummandB) {
        std::cerr << "Device failed to create buffer for summand b.";
        exit(-1);
    }
    
    mBufferResult = mDevice->newBuffer(arraySize, STORAGE_MODE);
    
    if(!mBufferResult) {
        std::cerr << "Device failed to create buffer for result.";
        exit(-1);
    }
    
    mArrayLength = arraySize / sizeof(float);
}

void GPUAddition::compute() {
    MTL::CommandBuffer *pCommandBuffer = mCommandQueue->commandBuffer();
    MTL::ComputeCommandEncoder *pCommandEncoder = pCommandBuffer->computeCommandEncoder();
    
    pCommandEncoder->setComputePipelineState(mPipelineState);
    pCommandEncoder->setBuffer(mBufferSummandA, 0, 0);
    pCommandEncoder->setBuffer(mBufferSummandB, 0, 1);
    pCommandEncoder->setBuffer(mBufferResult, 0, 2);
    pCommandEncoder->dispatchThreads(getGridSize(), getNumThreadsPerThreadgroup());
    pCommandEncoder->endEncoding();
    
    pCommandBuffer->commit();
    pCommandBuffer->waitUntilCompleted();
}

MTL::Size GPUAddition::getGridSize(){
    return MTL::Size(mArrayLength, 1, 1);
}

MTL::Size GPUAddition::getNumThreadsPerThreadgroup(){
    size_t maxThreadsPerGroup = (size_t) mPipelineState->maxTotalThreadsPerThreadgroup();
    size_t threadsPerGroup = std::min(maxThreadsPerGroup, mArrayLength);
    return MTL::Size(threadsPerGroup, 1, 1);
}

float* GPUAddition::getResult() {
    return (float*) mBufferResult->contents();
}

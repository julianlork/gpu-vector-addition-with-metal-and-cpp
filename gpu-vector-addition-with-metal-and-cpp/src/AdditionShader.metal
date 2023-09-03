//
//  AdditionShader.metal
//  gpu-vector-addition-with-metal-and-cpp
//
//  Created by Julian Lork on 03.09.23.
//

#include <metal_stdlib>
using namespace metal;

kernel void sum_of_arrays(device const float *summandA [[buffer(0)]],
                          device const float *summandB [[buffer(1)]],
                          device float *result [[buffer(2)]],
                          uint index [[thread_position_in_grid]]) {
    result[index] = summandA[index] + summandB[index];
}

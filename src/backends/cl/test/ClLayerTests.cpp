﻿//
// Copyright © 2017 Arm Ltd. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "ClContextControlFixture.hpp"
#include "ClWorkloadFactoryHelper.hpp"

#include "test/TensorHelpers.hpp"
#include "test/UnitTests.hpp"

#include <backendsCommon/CpuTensorHandle.hpp>
#include <cl/ClLayerSupport.hpp>
#include <cl/ClWorkloadFactory.hpp>
#include <cl/workloads/ClWorkloadUtils.hpp>
#include <reference/RefWorkloadFactory.hpp>
#include <backendsCommon/test/ActivationFixture.hpp>
#include <backendsCommon/test/LayerTests.hpp>
#include <backendsCommon/test/PermuteTestImpl.hpp>

#include <arm_compute/core/CL/CLKernelLibrary.h>
#include <arm_compute/runtime/CL/CLScheduler.h>

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>

BOOST_FIXTURE_TEST_SUITE(Compute_ArmComputeCl, ClContextControlFixture)
using FactoryType = armnn::ClWorkloadFactory;

// ============================================================================
// UNIT tests

// Activation
ARMNN_AUTO_TEST_CASE(ConstantLinearActivation, ConstantLinearActivationTest)

ARMNN_AUTO_TEST_CASE(ReLu1Uint8, BoundedReLuUint8UpperAndLowerBoundTest)
ARMNN_AUTO_TEST_CASE(ReLu6Uint8, BoundedReLuUint8UpperBoundOnlyTest)

ARMNN_AUTO_TEST_CASE(Logistic, SimpleSigmoidTest)
ARMNN_AUTO_TEST_CASE(LogisticUint8, SimpleSigmoidUint8Test)

// Batch To Space
ARMNN_AUTO_TEST_CASE(BatchToSpaceNdNhwcFloat321, BatchToSpaceNdNhwcTest1<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(BatchToSpaceNdNhwcFloat322, BatchToSpaceNdNhwcTest2<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(BatchToSpaceNdNhwcFloat323, BatchToSpaceNdNhwcTest3<armnn::DataType::Float32>)

ARMNN_AUTO_TEST_CASE(BatchToSpaceNdNchwFloat321, BatchToSpaceNdNchwTest1<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(BatchToSpaceNdNchwFloat322, BatchToSpaceNdNchwTest2<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(BatchToSpaceNdNchwFloat323, BatchToSpaceNdNchwTest3<armnn::DataType::Float32>)

ARMNN_AUTO_TEST_CASE(BatchToSpaceNdNhwcUint1, BatchToSpaceNdNhwcTest1<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(BatchToSpaceNdNhwcUint2, BatchToSpaceNdNhwcTest2<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(BatchToSpaceNdNhwcUint3, BatchToSpaceNdNhwcTest3<armnn::DataType::QuantisedAsymm8>)

ARMNN_AUTO_TEST_CASE(BatchToSpaceNdNchwUint1, BatchToSpaceNdNchwTest1<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(BatchToSpaceNdNchwUint2, BatchToSpaceNdNchwTest2<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(BatchToSpaceNdNchwUint3, BatchToSpaceNdNchwTest3<armnn::DataType::QuantisedAsymm8>)

// Fully Connected
ARMNN_AUTO_TEST_CASE(SimpleFullyConnected, FullyConnectedFloat32Test, false, false)
ARMNN_AUTO_TEST_CASE(SimpleFullyConnectedWithBias, FullyConnectedFloat32Test, true, false)
ARMNN_AUTO_TEST_CASE(SimpleFullyConnectedWithTranspose, FullyConnectedFloat32Test, false, true)
ARMNN_AUTO_TEST_CASE(FullyConnectedUint8, FullyConnectedTest<armnn::DataType::QuantisedAsymm8>, false)
ARMNN_AUTO_TEST_CASE(FullyConnectedBiasedUint8, FullyConnectedTest<armnn::DataType::QuantisedAsymm8>, true)

ARMNN_AUTO_TEST_CASE(FullyConnectedLarge, FullyConnectedLargeTest, false)
ARMNN_AUTO_TEST_CASE(FullyConnectedLargeTransposed, FullyConnectedLargeTest, true)

// Convolution
ARMNN_AUTO_TEST_CASE(SimpleConvolution1d, Convolution1dTest, true)

ARMNN_AUTO_TEST_CASE(SimpleConvolution2d, SimpleConvolution2d3x5Test, true, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(SimpleConvolution2dNhwc, SimpleConvolution2d3x5Test, true, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(SimpleConvolution2d3x3Uint8, SimpleConvolution2d3x3Uint8Test, true, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(SimpleConvolution2d3x3Uint8Nhwc, SimpleConvolution2d3x3Uint8Test, true, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(UnbiasedConvolution2d, SimpleConvolution2d3x5Test, false, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(UnbiasedConvolution2dNhwc, SimpleConvolution2d3x5Test, false, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(UnbiasedConvolution2dStride2x2Nhwc,
                     SimpleConvolution2d3x3Stride2x2Test, false, armnn::DataLayout::NHWC)

ARMNN_AUTO_TEST_CASE(UnbiasedConvolution2dSquare, SimpleConvolution2d3x3Test, false, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(SimpleConvolution2dAsymmetricPadding, Convolution2dAsymmetricPaddingTest, armnn::DataLayout::NCHW)

ARMNN_AUTO_TEST_CASE(UnbiasedConvolution2dSquareNhwc, SimpleConvolution2d3x3Test, false, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(SimpleConvolution2dAsymmetricPaddingNhwc,
                     Convolution2dAsymmetricPaddingTest,
                     armnn::DataLayout::NHWC)

ARMNN_AUTO_TEST_CASE(SimpleConvolution2dSquareNhwc, SimpleConvolution2d3x3NhwcTest, false)

ARMNN_AUTO_TEST_CASE(Convolution2d3x3Dilation3x3,
                     Convolution2d3x3Dilation3x3Test<armnn::DataType::Float32, armnn::DataType::Float32>,
                     false,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(Convolution2d3x3Dilation3x3Nhwc,
                     Convolution2d3x3Dilation3x3Test<armnn::DataType::Float32, armnn::DataType::Float32>,
                     false,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(Convolution2d3x3Dilation3x3Uint8,
                     Convolution2d3x3Dilation3x3Test<armnn::DataType::QuantisedAsymm8, armnn::DataType::Signed32>,
                     false,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(Convolution2d3x3Dilation3x3NhwcUint8,
                     Convolution2d3x3Dilation3x3Test<armnn::DataType::QuantisedAsymm8, armnn::DataType::Signed32>,
                     false,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(Convolution2d2x3x3Dilation3x3,
                     Convolution2d2x3x3Dilation3x3Test<armnn::DataType::Float32, armnn::DataType::Float32>,
                     false,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(Convolution2d2x3x3Dilation3x3Nhwc,
                     Convolution2d2x3x3Dilation3x3Test<armnn::DataType::Float32, armnn::DataType::Float32>,
                     false,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(Convolution2d2x3x3Dilation3x3Uint8,
                     Convolution2d2x3x3Dilation3x3Test<armnn::DataType::QuantisedAsymm8, armnn::DataType::Signed32>,
                     false,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(Convolution2d2x3x3Dilation3x3NhwcUint8,
                     Convolution2d2x3x3Dilation3x3Test<armnn::DataType::QuantisedAsymm8, armnn::DataType::Signed32>,
                     false,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(Convolution2d2x2Dilation2x2Padding2x2Stride3x3,
                     Convolution2d2x2Dilation2x2Padding2x2Stride3x3Test
                             <armnn::DataType::Float32, armnn::DataType::Float32>,
                     false,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(Convolution2d2x2Dilation2x2Padding2x2Stride3x3Nhwc,
                     Convolution2d2x2Dilation2x2Padding2x2Stride3x3Test
                             <armnn::DataType::Float32, armnn::DataType::Float32>,
                     false,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(Convolution2d2x2Dilation2x2Padding2x2Stride3x3Uint8,
                     Convolution2d2x2Dilation2x2Padding2x2Stride3x3Test
                             <armnn::DataType::QuantisedAsymm8, armnn::DataType::Signed32>,
                     false,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(Convolution2d2x2Dilation2x2Padding2x2Stride3x3NhwcUint8,
                     Convolution2d2x2Dilation2x2Padding2x2Stride3x3Test
                             <armnn::DataType::QuantisedAsymm8, armnn::DataType::Signed32>,
                     false,
                     armnn::DataLayout::NHWC)
                     
// Depthwise Convolution
ARMNN_AUTO_TEST_CASE(DepthwiseConvolution2dDepthMul1,
                     DepthwiseConvolution2dDepthMul1Test, true, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(UnbiasedDepthwiseConvolution2dDepthMul1,
                     DepthwiseConvolution2dDepthMul1Test, false, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(DepthwiseConvolution2dDepthMul1Uint8,
                     DepthwiseConvolution2dDepthMul1Uint8Test, true, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(UnbiasedDepthwiseConvolution2dDepthMul1Uint8,
                     DepthwiseConvolution2dDepthMul1Uint8Test, false, armnn::DataLayout::NCHW)

// NHWC Depthwise Convolution
ARMNN_AUTO_TEST_CASE(DepthwiseConvolution2dDepthMul1Nhwc,
                     DepthwiseConvolution2dDepthMul1Test, true, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(UnbiasedDepthwiseConvolution2dDepthMul1Nhwc,
                     DepthwiseConvolution2dDepthMul1Test, false, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(DepthwiseConvolution2dDepthMul1Uint8Nhwc,
                     DepthwiseConvolution2dDepthMul1Uint8Test, true, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(UnbiasedDepthwiseConvolution2dDepthMul1Uint8Nhwc,
                     DepthwiseConvolution2dDepthMul1Uint8Test, false, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(SimpleDepthwiseConvolution2d3x3Dilation3x3Nhwc,
                     SimpleDepthwiseConvolution2d3x3Dilation3x3NhwcTest)


ARMNN_AUTO_TEST_CASE(DepthwiseConvolution2dDepthNhwc, DepthwiseConvolution2dDepthNhwcTest, false)

ARMNN_AUTO_TEST_CASE(DepthwiseConvolution2dAsymmetric,
                     DepthwiseConvolution2dAsymmetricTest, true, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(UnbiasedDepthwiseConvolution2dAsymmetric,
                     DepthwiseConvolution2dAsymmetricTest, false, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(DepthwiseConvolution2dAsymmetricNhwc,
                     DepthwiseConvolution2dAsymmetricTest, true, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(UnbiasedDepthwiseConvolution2dAsymmetricNhwc,
                     DepthwiseConvolution2dAsymmetricTest, false, armnn::DataLayout::NHWC)

ARMNN_AUTO_TEST_CASE(DepthwiseConvolution2dDepthMul64, DepthwiseConvolution2dDepthMul64Test);

// Splitter
ARMNN_AUTO_TEST_CASE(SimpleSplitter, SplitterTest)
ARMNN_AUTO_TEST_CASE(SimpleSplitterUint8, SplitterUint8Test)

ARMNN_AUTO_TEST_CASE(CopyViaSplitter, CopyViaSplitterTest)
ARMNN_AUTO_TEST_CASE(CopyViaSplitterUint8, CopyViaSplitterUint8Test)

// Concat
ARMNN_AUTO_TEST_CASE(SimpleConcat, ConcatTest)
ARMNN_AUTO_TEST_CASE(ConcatUint8, ConcatUint8Test)
ARMNN_AUTO_TEST_CASE(ConcatUint8DifferentInputOutputQParam,
                     ConcatDifferentInputOutputQParamTest<armnn::DataType::QuantisedAsymm8>, false)

// Normalization
ARMNN_AUTO_TEST_CASE(SimpleNormalizationAcross, SimpleNormalizationAcrossTest)
ARMNN_AUTO_TEST_CASE(SimpleNormalizationWithin, SimpleNormalizationWithinTest)
ARMNN_AUTO_TEST_CASE(SimpleNormalizationAcrossNhwc, SimpleNormalizationAcrossNhwcTest)

// Pooling
ARMNN_AUTO_TEST_CASE(SimpleMaxPooling2dSize3x3Stride2x4, SimpleMaxPooling2dSize3x3Stride2x4Test, true)
ARMNN_AUTO_TEST_CASE(SimpleMaxPooling2dSize3x3Stride2x4Uint8, SimpleMaxPooling2dSize3x3Stride2x4Uint8Test, true)

ARMNN_AUTO_TEST_CASE(IgnorePaddingSimpleMaxPooling2d, IgnorePaddingSimpleMaxPooling2dTest)
ARMNN_AUTO_TEST_CASE(IgnorePaddingSimpleMaxPooling2dUint8, IgnorePaddingSimpleMaxPooling2dUint8Test)
ARMNN_AUTO_TEST_CASE(IgnorePaddingMaxPooling2dSize3, IgnorePaddingMaxPooling2dSize3Test)
ARMNN_AUTO_TEST_CASE(IgnorePaddingMaxPooling2dSize3Uint8, IgnorePaddingMaxPooling2dSize3Uint8Test)

ARMNN_AUTO_TEST_CASE(IgnorePaddingSimpleAveragePooling2d, IgnorePaddingSimpleAveragePooling2dTest)
ARMNN_AUTO_TEST_CASE(IgnorePaddingSimpleAveragePooling2dUint8, IgnorePaddingSimpleAveragePooling2dUint8Test)
ARMNN_AUTO_TEST_CASE(IgnorePaddingSimpleAveragePooling2dNoPadding, IgnorePaddingSimpleAveragePooling2dNoPaddingTest)
ARMNN_AUTO_TEST_CASE(IgnorePaddingSimpleAveragePooling2dNoPaddingUint8,
                     IgnorePaddingSimpleAveragePooling2dNoPaddingUint8Test)
ARMNN_AUTO_TEST_CASE(IgnorePaddingAveragePooling2dSize3, IgnorePaddingAveragePooling2dSize3Test)
ARMNN_AUTO_TEST_CASE(IgnorePaddingAveragePooling2dSize3Uint8, IgnorePaddingAveragePooling2dSize3Uint8Test)

ARMNN_AUTO_TEST_CASE(IgnorePaddingSimpleL2Pooling2d, IgnorePaddingSimpleL2Pooling2dTest)
ARMNN_AUTO_TEST_CASE(UNSUPPORTED_IgnorePaddingSimpleL2Pooling2dUint8, IgnorePaddingSimpleL2Pooling2dUint8Test)
ARMNN_AUTO_TEST_CASE(IgnorePaddingL2Pooling2dSize3, IgnorePaddingL2Pooling2dSize3Test)
ARMNN_AUTO_TEST_CASE(UNSUPPORTED_IgnorePaddingL2Pooling2dSize3Uint8, IgnorePaddingL2Pooling2dSize3Uint8Test)

ARMNN_AUTO_TEST_CASE(SimpleMaxPooling2d, SimpleMaxPooling2dTest, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(SimpleMaxPooling2dNhwc, SimpleMaxPooling2dTest, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(SimpleMaxPooling2dUint8, SimpleMaxPooling2dUint8Test, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(SimpleMaxPooling2dUint8Nhwc, SimpleMaxPooling2dUint8Test, armnn::DataLayout::NHWC)

ARMNN_AUTO_TEST_CASE(SimpleAveragePooling2d, SimpleAveragePooling2dTest, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(SimpleAveragePooling2dNhwc, SimpleAveragePooling2dTest, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(SimpleAveragePooling2dUint8, SimpleAveragePooling2dUint8Test, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(SimpleAveragePooling2dUint8Nhwc, SimpleAveragePooling2dUint8Test, armnn::DataLayout::NHWC)

ARMNN_AUTO_TEST_CASE(IgnorePaddingAveragePooling2dSize3x2Stride2x2,
                     IgnorePaddingAveragePooling2dSize3x2Stride2x2Test,
                     false)
ARMNN_AUTO_TEST_CASE(IgnorePaddingAveragePooling2dSize3x2Stride2x2NoPadding,
                     IgnorePaddingAveragePooling2dSize3x2Stride2x2Test,
                     true)
ARMNN_AUTO_TEST_CASE(LargeTensorsAveragePooling2d, LargeTensorsAveragePooling2dTest)
ARMNN_AUTO_TEST_CASE(LargeTensorsAveragePooling2dUint8, LargeTensorsAveragePooling2dUint8Test)

ARMNN_AUTO_TEST_CASE(SimpleL2Pooling2d, SimpleL2Pooling2dTest, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(SimpleL2Pooling2dNhwc, SimpleL2Pooling2dTest, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(UNSUPPORTED_SimpleL2Pooling2dUint8, SimpleL2Pooling2dUint8Test, armnn::DataLayout::NCHW)

ARMNN_AUTO_TEST_CASE(L2Pooling2dSize3Stride1, L2Pooling2dSize3Stride1Test)
ARMNN_AUTO_TEST_CASE(UNSUPPORTED_L2Pooling2dSize3Stride1Uint8, L2Pooling2dSize3Stride1Uint8Test)
ARMNN_AUTO_TEST_CASE(L2Pooling2dSize3Stride3, L2Pooling2dSize3Stride3Test)
ARMNN_AUTO_TEST_CASE(UNSUPPORTED_L2Pooling2dSize3Stride3Uint8, L2Pooling2dSize3Stride3Uint8Test)
ARMNN_AUTO_TEST_CASE(L2Pooling2dSize3Stride4, L2Pooling2dSize3Stride4Test)
ARMNN_AUTO_TEST_CASE(UNSUPPORTED_L2Pooling2dSize3Stride4Uint8, L2Pooling2dSize3Stride4Uint8Test)
ARMNN_AUTO_TEST_CASE(L2Pooling2dSize7, L2Pooling2dSize7Test)
ARMNN_AUTO_TEST_CASE(UNSUPPORTED_L2Pooling2dSize7Uint8, L2Pooling2dSize7Uint8Test)
ARMNN_AUTO_TEST_CASE(L2Pooling2dSize9, L2Pooling2dSize9Test)
ARMNN_AUTO_TEST_CASE(UNSUPPORTED_L2Pooling2dSize9Uint8, L2Pooling2dSize9Uint8Test)

// Add
ARMNN_AUTO_TEST_CASE(SimpleAdd, AdditionTest)
ARMNN_AUTO_TEST_CASE(AddBroadcast1Element, AdditionBroadcast1ElementTest)
ARMNN_AUTO_TEST_CASE(AddBroadcast, AdditionBroadcastTest)

ARMNN_AUTO_TEST_CASE(AdditionUint8, AdditionUint8Test)
ARMNN_AUTO_TEST_CASE(AddBroadcastUint8, AdditionBroadcastUint8Test)
ARMNN_AUTO_TEST_CASE(AddBroadcast1ElementUint8, AdditionBroadcast1ElementUint8Test)

// Sub
ARMNN_AUTO_TEST_CASE(SimpleSub, SubtractionTest)
ARMNN_AUTO_TEST_CASE(SubBroadcast1Element, SubtractionBroadcast1ElementTest)
ARMNN_AUTO_TEST_CASE(SubBroadcast, SubtractionBroadcastTest)

ARMNN_AUTO_TEST_CASE(SubtractionUint8, SubtractionUint8Test)
ARMNN_AUTO_TEST_CASE(SubBroadcastUint8, SubtractionBroadcastUint8Test)
ARMNN_AUTO_TEST_CASE(SubBroadcast1ElementUint8, SubtractionBroadcast1ElementUint8Test)

// Div
ARMNN_AUTO_TEST_CASE(SimpleDivision, DivisionTest)
ARMNN_AUTO_TEST_CASE(DivisionByZero, DivisionByZeroTest)
ARMNN_AUTO_TEST_CASE(DivisionBroadcast1Element, DivisionBroadcast1ElementTest)
ARMNN_AUTO_TEST_CASE(DivisionBroadcast1DVector, DivisionBroadcast1DVectorTest)
// NOTE: quantized division is not supported by CL and not required by the
//       android NN api

// Mul
ARMNN_AUTO_TEST_CASE(SimpleMultiplication, MultiplicationTest)
ARMNN_AUTO_TEST_CASE(MultiplicationBroadcast1Element, MultiplicationBroadcast1ElementTest)
ARMNN_AUTO_TEST_CASE(MultiplicationBroadcast1DVector, MultiplicationBroadcast1DVectorTest)
ARMNN_AUTO_TEST_CASE(MultiplicationUint8, MultiplicationUint8Test)
ARMNN_AUTO_TEST_CASE(MultiplicationBroadcast1ElementUint8, MultiplicationBroadcast1ElementUint8Test)
ARMNN_AUTO_TEST_CASE(MultiplicationBroadcast1DVectorUint8, MultiplicationBroadcast1DVectorUint8Test)

// Batch Norm
ARMNN_AUTO_TEST_CASE(BatchNorm, BatchNormTest)
ARMNN_AUTO_TEST_CASE(BatchNormNhwc, BatchNormNhwcTest)

// L2 Normalization
ARMNN_AUTO_TEST_CASE(L2Normalization1d, L2Normalization1dTest, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(L2Normalization2d, L2Normalization2dTest, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(L2Normalization3d, L2Normalization3dTest, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(L2Normalization4d, L2Normalization4dTest, armnn::DataLayout::NCHW)

ARMNN_AUTO_TEST_CASE(L2Normalization1dNhwc, L2Normalization1dTest, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(L2Normalization2dNhwc, L2Normalization2dTest, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(L2Normalization3dNhwc, L2Normalization3dTest, armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(L2Normalization4dNhwc, L2Normalization4dTest, armnn::DataLayout::NHWC)

ARMNN_AUTO_TEST_CASE(L2Normalization2dShape, L2Normalization2dShapeTest);

ARMNN_AUTO_TEST_CASE(L2NormalizationDefaultEpsilon, L2NormalizationDefaultEpsilonTest, armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(L2NormalizationNonDefaultEpsilon, L2NormalizationNonDefaultEpsilonTest, armnn::DataLayout::NCHW)

// Constant
ARMNN_AUTO_TEST_CASE(Constant, ConstantTest)
ARMNN_AUTO_TEST_CASE(ConstantUint8, ConstantUint8SimpleQuantizationScaleNoOffsetTest)

// Concat
ARMNN_AUTO_TEST_CASE(Concatenation1d, Concatenation1dTest)
ARMNN_AUTO_TEST_CASE(Concatenation1dUint8, Concatenation1dUint8Test)

ARMNN_AUTO_TEST_CASE(Concatenation2dDim0, Concatenation2dDim0Test)
ARMNN_AUTO_TEST_CASE(Concatenation2dDim0Uint8, Concatenation2dDim0Uint8Test)
ARMNN_AUTO_TEST_CASE(Concatenation2dDim1, Concatenation2dDim1Test)
ARMNN_AUTO_TEST_CASE(Concatenation2dDim1Uint8, Concatenation2dDim1Uint8Test)

ARMNN_AUTO_TEST_CASE(Concatenation2dDim0DiffInputDims, Concatenation2dDim0DiffInputDimsTest)
ARMNN_AUTO_TEST_CASE(Concatenation2dDim0DiffInputDimsUint8, Concatenation2dDim0DiffInputDimsUint8Test)
ARMNN_AUTO_TEST_CASE(Concatenation2dDim1DiffInputDims, Concatenation2dDim1DiffInputDimsTest)
ARMNN_AUTO_TEST_CASE(Concatenation2dDim1DiffInputDimsUint8, Concatenation2dDim1DiffInputDimsUint8Test)

ARMNN_AUTO_TEST_CASE(Concatenation3dDim0, Concatenation3dDim0Test)
ARMNN_AUTO_TEST_CASE(Concatenation3dDim0Uint8, Concatenation3dDim0Uint8Test)
ARMNN_AUTO_TEST_CASE(Concatenation3dDim1, Concatenation3dDim1Test)
ARMNN_AUTO_TEST_CASE(Concatenation3dDim1Uint8, Concatenation3dDim1Uint8Test)
ARMNN_AUTO_TEST_CASE(Concatenation3dDim2, Concatenation3dDim2Test, false)
ARMNN_AUTO_TEST_CASE(Concatenation3dDim2Uint8, Concatenation3dDim2Uint8Test, false)

ARMNN_AUTO_TEST_CASE(Concatenation3dDim0DiffInputDims, Concatenation3dDim0DiffInputDimsTest)
ARMNN_AUTO_TEST_CASE(Concatenation3dDim0DiffInputDimsUint8, Concatenation3dDim0DiffInputDimsUint8Test)
ARMNN_AUTO_TEST_CASE(Concatenation3dDim1DiffInputDims, Concatenation3dDim1DiffInputDimsTest)
ARMNN_AUTO_TEST_CASE(Concatenation3dDim1DiffInputDimsUint8, Concatenation3dDim1DiffInputDimsUint8Test)
ARMNN_AUTO_TEST_CASE(Concatenation3dDim2DiffInputDims, Concatenation3dDim2DiffInputDimsTest, false)
ARMNN_AUTO_TEST_CASE(Concatenation3dDim2DiffInputDimsUint8, Concatenation3dDim2DiffInputDimsUint8Test, false)

ARMNN_AUTO_TEST_CASE(Concatenation4dDim0, Concatenation4dDim0Test)
ARMNN_AUTO_TEST_CASE(Concatenation4dDim1, Concatenation4dDim1Test)
ARMNN_AUTO_TEST_CASE(Concatenation4dDim3, Concatenation4dDim3Test, false)
ARMNN_AUTO_TEST_CASE(Concatenation4dDim0Uint8, Concatenation4dDim0Uint8Test)
ARMNN_AUTO_TEST_CASE(Concatenation4dDim1Uint8, Concatenation4dDim1Uint8Test)
ARMNN_AUTO_TEST_CASE(Concatenation4dDim3Uint8, Concatenation4dDim3Uint8Test, false)

ARMNN_AUTO_TEST_CASE(Concatenation4dDiffShapeDim0, Concatenation4dDiffShapeDim0Test)
ARMNN_AUTO_TEST_CASE(Concatenation4dDiffShapeDim1, Concatenation4dDiffShapeDim1Test)
ARMNN_AUTO_TEST_CASE(Concatenation4dDiffShapeDim3, Concatenation4dDiffShapeDim3Test, false)
ARMNN_AUTO_TEST_CASE(Concatenation4dDiffShapeDim0Uint8, Concatenation4dDiffShapeDim0Uint8Test)
ARMNN_AUTO_TEST_CASE(Concatenation4dDiffShapeDim1Uint8, Concatenation4dDiffShapeDim1Uint8Test)
ARMNN_AUTO_TEST_CASE(Concatenation4dDiffShapeDim3Uint8, Concatenation4dDiffShapeDim3Uint8Test, false)

// Floor
ARMNN_AUTO_TEST_CASE(SimpleFloor, SimpleFloorTest<armnn::DataType::Float32>)

// Reshape
ARMNN_AUTO_TEST_CASE(SimpleReshapeFloat32, SimpleReshapeTest<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(SimpleReshapeUint8, SimpleReshapeTest<armnn::DataType::QuantisedAsymm8>)

// Pad
ARMNN_AUTO_TEST_CASE(PadFloat322d, PadFloat322dTest)
ARMNN_AUTO_TEST_CASE(PadFloat322dCustomPadding, PadFloat322dCustomPaddingTest)
ARMNN_AUTO_TEST_CASE(PadFloat323d, PadFloat323dTest)
ARMNN_AUTO_TEST_CASE(PadFloat324d, PadFloat324dTest)

ARMNN_AUTO_TEST_CASE(PadUint82d, PadUint82dTest)
ARMNN_AUTO_TEST_CASE(PadUint82dCustomPadding, PadUint82dCustomPaddingTest)
ARMNN_AUTO_TEST_CASE(PadUint83d, PadUint83dTest)
ARMNN_AUTO_TEST_CASE(PadUint84d, PadUint84dTest)

ARMNN_AUTO_TEST_CASE(Pad2dQSymm16, Pad2dTestCommon<armnn::DataType::QuantisedSymm16>, 2.0f, 0, 0.0f)
ARMNN_AUTO_TEST_CASE(Pad2dQSymm16CustomPadding, Pad2dTestCommon<armnn::DataType::QuantisedSymm16>, 2.0f, 0, 1.0f)
ARMNN_AUTO_TEST_CASE(Pad3dQSymm16, Pad3dTestCommon<armnn::DataType::QuantisedSymm16>, 2.0f, 0)
ARMNN_AUTO_TEST_CASE(Pad4dQSymm16, Pad4dTestCommon<armnn::DataType::QuantisedSymm16>, 2.0f, 0)

// PReLU
ARMNN_AUTO_TEST_CASE(PreluFloat32, PreluTest<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(PreluUint8,   PreluTest<armnn::DataType::QuantisedAsymm8>)

// Permute
ARMNN_AUTO_TEST_CASE(SimplePermuteFloat32, SimplePermuteTest<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(PermuteFloat32ValueSet1Test, PermuteValueSet1Test<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(PermuteFloat32ValueSet2Test, PermuteValueSet2Test<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(PermuteFloat32ValueSet3Test, PermuteValueSet3Test<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(SimplePermuteQASymm8, SimplePermuteTest<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(PermuteQASymm8ValueSet1Test, PermuteValueSet1Test<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(PermuteQASymm8ValueSet2Test, PermuteValueSet2Test<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(PermuteQASymm8ValueSet3Test, PermuteValueSet3Test<armnn::DataType::QuantisedAsymm8>)

// Lstm
ARMNN_AUTO_TEST_CASE(LstmLayerFloat32WithCifgWithPeepholeNoProjection,
                     LstmLayerFloat32WithCifgWithPeepholeNoProjectionTest)
ARMNN_AUTO_TEST_CASE(LstmLayerFloat32NoCifgNoPeepholeNoProjection,
                     LstmLayerFloat32NoCifgNoPeepholeNoProjectionTest)
ARMNN_AUTO_TEST_CASE(LstmLayerFloat32NoCifgWithPeepholeWithProjection,
                     LstmLayerFloat32NoCifgWithPeepholeWithProjectionTest)

ARMNN_AUTO_TEST_CASE(LstmLayerFloat32NoCifgWithPeepholeWithProjectionWithLayerNorm,
                     LstmLayerFloat32NoCifgWithPeepholeWithProjectionWithLayerNormTest)

// Convert from Float16 to Float32
ARMNN_AUTO_TEST_CASE(SimpleConvertFp16ToFp32, SimpleConvertFp16ToFp32Test)
// Convert from Float32 to Float16
ARMNN_AUTO_TEST_CASE(SimpleConvertFp32ToFp16, SimpleConvertFp32ToFp16Test)

ARMNN_AUTO_TEST_CASE(AdditionAfterMaxPool, AdditionAfterMaxPoolTest)

//Max
ARMNN_AUTO_TEST_CASE(MaximumSimple, MaximumSimpleTest)
ARMNN_AUTO_TEST_CASE(MaximumBroadcast1Element, MaximumBroadcast1ElementTest)
ARMNN_AUTO_TEST_CASE(MaximumBroadcast1DVector, MaximumBroadcast1DVectorTest)
ARMNN_AUTO_TEST_CASE(MaximumUint8, MaximumUint8Test)
ARMNN_AUTO_TEST_CASE(MaximumBroadcast1ElementUint8, MaximumBroadcast1ElementUint8Test)
ARMNN_AUTO_TEST_CASE(MaximumBroadcast1DVectorUint8, MaximumBroadcast1DVectorUint8Test)

// Mean
ARMNN_AUTO_TEST_CASE(MeanSimpleFloat32, MeanSimpleTest<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(MeanSimpleAxisFloat32, MeanSimpleAxisTest<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(MeanKeepDimsFloat32, MeanKeepDimsTest<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(MeanMultipleDimsFloat32, MeanMultipleDimsTest<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(MeanVts1Float32, MeanVts1Test<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(MeanVts2Float32, MeanVts2Test<armnn::DataType::Float32>)
ARMNN_AUTO_TEST_CASE(MeanVts3Float32, MeanVts3Test<armnn::DataType::Float32>)

ARMNN_AUTO_TEST_CASE(MeanSimpleQuantisedAsymm8, MeanSimpleTest<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(MeanSimpleAxisQuantisedAsymm8, MeanSimpleAxisTest<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(MeanKeepDimsQuantisedAsymm8, MeanKeepDimsTest<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(MeanMultipleDimsQuantisedAsymm8, MeanMultipleDimsTest<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(MeanVts1QuantisedAsymm8, MeanVts1Test<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(MeanVts2QuantisedAsymm8, MeanVts2Test<armnn::DataType::QuantisedAsymm8>)
ARMNN_AUTO_TEST_CASE(MeanVts3QuantisedAsymm8, MeanVts3Test<armnn::DataType::QuantisedAsymm8>)

// Minimum
ARMNN_AUTO_TEST_CASE(MinimumBroadcast1Element1, MinimumBroadcast1ElementTest1)
ARMNN_AUTO_TEST_CASE(MinimumBroadcast1Element2, MinimumBroadcast1ElementTest2)
ARMNN_AUTO_TEST_CASE(MinimumBroadcast1DVectorUint8, MinimumBroadcast1DVectorUint8Test)

// Greater
ARMNN_AUTO_TEST_CASE(SimpleGreater, GreaterSimpleTest)
ARMNN_AUTO_TEST_CASE(GreaterBroadcast1Element, GreaterBroadcast1ElementTest)
ARMNN_AUTO_TEST_CASE(GreaterBroadcast1DVector, GreaterBroadcast1DVectorTest)
ARMNN_AUTO_TEST_CASE(GreaterUint8, GreaterUint8Test)
ARMNN_AUTO_TEST_CASE(GreaterBroadcast1ElementUint8, GreaterBroadcast1ElementUint8Test)
ARMNN_AUTO_TEST_CASE(GreaterBroadcast1DVectorUint8, GreaterBroadcast1DVectorUint8Test)

// Softmax
ARMNN_AUTO_TEST_CASE(SimpleSoftmaxBeta1, SimpleSoftmaxTest, 1.0f)
ARMNN_AUTO_TEST_CASE(SimpleSoftmaxBeta2, SimpleSoftmaxTest, 2.0f)
ARMNN_AUTO_TEST_CASE(SimpleSoftmaxBeta1Uint8, SimpleSoftmaxUint8Test, 1.0f)
ARMNN_AUTO_TEST_CASE(SimpleSoftmaxBeta2Uint8, SimpleSoftmaxUint8Test, 2.0f)

ARMNN_AUTO_TEST_CASE(Simple3dSoftmax, Simple3dSoftmaxTest, 1.0f)
ARMNN_AUTO_TEST_CASE(Simple3dSoftmaxUint8, Simple3dSoftmaxUint8Test, 1.0f)

ARMNN_AUTO_TEST_CASE(Simple4dSoftmax, Simple4dSoftmaxTest, 1.0f)
ARMNN_AUTO_TEST_CASE(Simple4dSoftmaxUint8, Simple4dSoftmaxUint8Test, 1.0f)

ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareSoftmaxBeta1WithReference, CompareSoftmaxTest, 1.0f)
ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareSoftmaxBeta2WithReference, CompareSoftmaxTest, 2.0f)
ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareSoftmaxUint8, CompareSoftmaxUint8Test, 1.0f)

// Space To Batch Nd
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdSimpleFloat32, SpaceToBatchNdSimpleFloat32Test)
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdMultiChannelsFloat32, SpaceToBatchNdMultiChannelsFloat32Test)
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdMultiBlockFloat32, SpaceToBatchNdMultiBlockFloat32Test)
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdPaddingFloat32, SpaceToBatchNdPaddingFloat32Test)

ARMNN_AUTO_TEST_CASE(SpaceToBatchNdSimpleUint8, SpaceToBatchNdSimpleUint8Test)
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdMultiChannelsUint8, SpaceToBatchNdMultiChannelsUint8Test)
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdMultiBlockUint8, SpaceToBatchNdMultiBlockUint8Test)
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdPaddingUint8, SpaceToBatchNdPaddingUint8Test)

ARMNN_AUTO_TEST_CASE(SpaceToBatchNdSimpleNHWCFloat32, SpaceToBatchNdSimpleNHWCFloat32Test)
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdMultiChannelsNHWCFloat32, SpaceToBatchNdMultiChannelsNHWCFloat32Test)
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdMultiBlockNHWCFloat32, SpaceToBatchNdMultiBlockNHWCFloat32Test)
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdPaddingNHWCFloat32, SpaceToBatchNdPaddingNHWCFloat32Test)

ARMNN_AUTO_TEST_CASE(SpaceToBatchNdSimpleNHWCUint8, SpaceToBatchNdSimpleNHWCUint8Test)
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdMultiChannelsNHWCUint8, SpaceToBatchNdMultiChannelsNHWCUint8Test)
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdMultiBlockNHWCUint8, SpaceToBatchNdMultiBlockNHWCUint8Test)
ARMNN_AUTO_TEST_CASE(SpaceToBatchNdPaddingNHWCUint8, SpaceToBatchNdPaddingNHWCUint8Test)

// Space To Depth
ARMNN_AUTO_TEST_CASE(SpaceToDepthNCHWAsymmQ8, SpaceToDepthNCHWAsymmQ8Test)
ARMNN_AUTO_TEST_CASE(SpaceToDepthNHWCAsymmQ8, SpaceToDepthNHWCAsymmQ8Test)

ARMNN_AUTO_TEST_CASE(SpaceToDepthNHWC1Float32, SpaceToDepthNHWCFloat32Test1)
ARMNN_AUTO_TEST_CASE(SpaceToDepthNCHW1Float32, SpaceToDepthNCHWFloat32Test1)

ARMNN_AUTO_TEST_CASE(SpaceToDepthNHWC2Float32, SpaceToDepthNHWCFloat32Test2)
ARMNN_AUTO_TEST_CASE(SpaceToDepthNCHW2Float32, SpaceToDepthNCHWFloat32Test2)

ARMNN_AUTO_TEST_CASE(SpaceToDepthNHWCQSymm16, SpaceToDepthNHWCQSymm16Test)
ARMNN_AUTO_TEST_CASE(SpaceToDepthNCHWQSymm16, SpaceToDepthNCHWQSymm16Test)

// Strided Slice
ARMNN_AUTO_TEST_CASE(StridedSlice4DFloat32, StridedSlice4DFloat32Test)
ARMNN_AUTO_TEST_CASE(StridedSlice4DReverseFloat32, StridedSlice4DReverseFloat32Test)
ARMNN_AUTO_TEST_CASE(StridedSliceSimpleStrideFloat32, StridedSliceSimpleStrideFloat32Test)
ARMNN_AUTO_TEST_CASE(StridedSliceSimpleRangeMaskFloat32, StridedSliceSimpleRangeMaskFloat32Test)
ARMNN_AUTO_TEST_CASE(StridedSliceShrinkAxisMaskFloat32, StridedSliceShrinkAxisMaskFloat32Test)
ARMNN_AUTO_TEST_CASE(StridedSlice3DFloat32, StridedSlice3DFloat32Test)
ARMNN_AUTO_TEST_CASE(StridedSlice3DReverseFloat32, StridedSlice3DReverseFloat32Test)
ARMNN_AUTO_TEST_CASE(StridedSlice2DFloat32, StridedSlice2DFloat32Test)
ARMNN_AUTO_TEST_CASE(StridedSlice2DReverseFloat32, StridedSlice2DReverseFloat32Test)

ARMNN_AUTO_TEST_CASE(StridedSlice4DUint8, StridedSlice4DUint8Test)
ARMNN_AUTO_TEST_CASE(StridedSlice4DReverseUint8, StridedSlice4DReverseUint8Test)
ARMNN_AUTO_TEST_CASE(StridedSliceSimpleStrideUint8, StridedSliceSimpleStrideUint8Test)
ARMNN_AUTO_TEST_CASE(StridedSliceSimpleRangeMaskUint8, StridedSliceSimpleRangeMaskUint8Test)
ARMNN_AUTO_TEST_CASE(StridedSliceShrinkAxisMaskUint8, StridedSliceShrinkAxisMaskUint8Test)
ARMNN_AUTO_TEST_CASE(StridedSlice3DUint8, StridedSlice3DUint8Test)
ARMNN_AUTO_TEST_CASE(StridedSlice3DReverseUint8, StridedSlice3DReverseUint8Test)
ARMNN_AUTO_TEST_CASE(StridedSlice2DUint8, StridedSlice2DUint8Test)
ARMNN_AUTO_TEST_CASE(StridedSlice2DReverseUint8, StridedSlice2DReverseUint8Test)

// Resize Bilinear - NCHW
ARMNN_AUTO_TEST_CASE(SimpleResizeBilinear,
                     SimpleResizeBilinearTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(SimpleResizeBilinearUint8,
                     SimpleResizeBilinearTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeBilinearNop,
                     ResizeBilinearNopTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeBilinearNopUint8,
                     ResizeBilinearNopTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeBilinearSqMin,
                     ResizeBilinearSqMinTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeBilinearSqMinUint8,
                     ResizeBilinearSqMinTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeBilinearMin,
                     ResizeBilinearMinTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeBilinearMinUint8,
                     ResizeBilinearMinTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NCHW)

// Resize Bilinear - NHWC
ARMNN_AUTO_TEST_CASE(ResizeBilinearNopNhwc,
                     ResizeBilinearNopTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(ResizeBilinearNopUint8Nhwc,
                     ResizeBilinearNopTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(SimpleResizeBilinearNhwc,
                     SimpleResizeBilinearTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(SimpleResizeBilinearUint8Nhwc,
                     SimpleResizeBilinearTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(ResizeBilinearSqMinNhwc,
                     ResizeBilinearSqMinTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(ResizeBilinearSqMinUint8Nhwc,
                     ResizeBilinearSqMinTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(ResizeBilinearMinNhwc,
                     ResizeBilinearMinTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(ResizeBilinearMinUint8Nhwc,
                     ResizeBilinearMinTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NHWC)

// Resize NearestNeighbor - NCHW
ARMNN_AUTO_TEST_CASE(SimpleResizeNearestNeighbor,
                     SimpleResizeNearestNeighborTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(SimpleResizeNearestNeighborUint8,
                     SimpleResizeNearestNeighborTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborNop,
                     ResizeNearestNeighborNopTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborNopUint8,
                     ResizeNearestNeighborNopTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborSqMin,
                     ResizeNearestNeighborSqMinTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborSqMinUint8,
                     ResizeNearestNeighborSqMinTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborMin,
                     ResizeNearestNeighborMinTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborMinUint8,
                     ResizeNearestNeighborMinTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NCHW)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborMag,
                     ResizeNearestNeighborMagTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NCHW, 0.1f, 50, 0.1f, 50)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborMagUint8,
                     ResizeNearestNeighborMagTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NCHW, 0.1f, 50, 0.1f, 50)

// Resize NearestNeighbor - NHWC
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborNopNhwc,
                     ResizeNearestNeighborNopTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborNopUint8Nhwc,
                     ResizeNearestNeighborNopTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(SimpleResizeNearestNeighborNhwc,
                     SimpleResizeNearestNeighborTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(SimpleResizeNearestNeighborUint8Nhwc,
                     SimpleResizeNearestNeighborTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborSqMinNhwc,
                     ResizeNearestNeighborSqMinTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborSqMinUint8Nhwc,
                     ResizeNearestNeighborSqMinTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborMinNhwc,
                     ResizeNearestNeighborMinTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborMinUint8Nhwc,
                     ResizeNearestNeighborMinTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NHWC)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborMagNhwc,
                     ResizeNearestNeighborMagTest<armnn::DataType::Float32>,
                     armnn::DataLayout::NHWC, 0.1f, 50, 0.1f, 50)
ARMNN_AUTO_TEST_CASE(ResizeNearestNeighborMagUint8Nhwc,
                     ResizeNearestNeighborMagTest<armnn::DataType::QuantisedAsymm8>,
                     armnn::DataLayout::NHWC, 0.1f, 50, 0.1f, 50)

// Quantize
ARMNN_AUTO_TEST_CASE(QuantizeSimpleUint8, QuantizeSimpleUint8Test)
ARMNN_AUTO_TEST_CASE(QuantizeClampUint8, QuantizeClampUint8Test)

// Dequantize
// NOTE: current clframework (46a49a0a8206f0efa7afd514940e180a88ffd732)
//       CLDequantizationLayerKernel accepts DataType::QASYMM8 input
//       and can output DataType::F16 or DataType::F32
ARMNN_AUTO_TEST_CASE(DequantizeSimpleUint8, DequantizeSimpleUint8Test)
ARMNN_AUTO_TEST_CASE(DequantizeOffsetUint8, DequantizeOffsetUint8Test)

// TransposeConvolution2d
ARMNN_AUTO_TEST_CASE(SimpleTransposeConvolution2dFloatNchw, SimpleTransposeConvolution2dFloatNchwTest)
ARMNN_AUTO_TEST_CASE(SimpleTransposeConvolution2dFloatNhwc, SimpleTransposeConvolution2dFloatNhwcTest)
ARMNN_AUTO_TEST_CASE(SimpleTransposeConvolution2dUint8Nchw, SimpleTransposeConvolution2dUint8NchwTest)
ARMNN_AUTO_TEST_CASE(SimpleTransposeConvolution2dUint8Nhwc, SimpleTransposeConvolution2dUint8NhwcTest)

ARMNN_AUTO_TEST_CASE(UnbiasedSimpleTransposeConvolution2dFloatNchw, UnbiasedSimpleTransposeConvolution2dFloatNchwTest)
ARMNN_AUTO_TEST_CASE(UnbiasedSimpleTransposeConvolution2dFloatNhwc, UnbiasedSimpleTransposeConvolution2dFloatNhwcTest)
ARMNN_AUTO_TEST_CASE(UnbiasedSimpleTransposeConvolution2dUint8Nchw, UnbiasedSimpleTransposeConvolution2dUint8NchwTest)
ARMNN_AUTO_TEST_CASE(UnbiasedSimpleTransposeConvolution2dUint8Nhwc, UnbiasedSimpleTransposeConvolution2dUint8NhwcTest)

ARMNN_AUTO_TEST_CASE(PaddedTransposeConvolution2dFloatNchw, PaddedTransposeConvolution2dFloatNchwTest)
ARMNN_AUTO_TEST_CASE(PaddedTransposeConvolution2dFloatNhwc, PaddedTransposeConvolution2dFloatNhwcTest)
ARMNN_AUTO_TEST_CASE(PaddedTransposeConvolution2dUint8Nchw, PaddedTransposeConvolution2dUint8NchwTest)
ARMNN_AUTO_TEST_CASE(PaddedTransposeConvolution2dUint8Nhwc, PaddedTransposeConvolution2dUint8NhwcTest)

ARMNN_AUTO_TEST_CASE(UnbiasedPaddedTransposeConvolution2dFloatNchw, UnbiasedPaddedTransposeConvolution2dFloatNchwTest)
ARMNN_AUTO_TEST_CASE(UnbiasedPaddedTransposeConvolution2dFloatNhwc, UnbiasedPaddedTransposeConvolution2dFloatNhwcTest)
ARMNN_AUTO_TEST_CASE(UnbiasedPaddedTransposeConvolution2dUint8Nchw, UnbiasedPaddedTransposeConvolution2dUint8NchwTest)
ARMNN_AUTO_TEST_CASE(UnbiasedPaddedTransposeConvolution2dUint8Nhwc, UnbiasedPaddedTransposeConvolution2dUint8NhwcTest)

ARMNN_AUTO_TEST_CASE(StridedTransposeConvolution2dFloatNhwc, StridedTransposeConvolution2dFloatNhwcTest)
ARMNN_AUTO_TEST_CASE(StridedTransposeConvolution2dFloatNchw, StridedTransposeConvolution2dFloatNchwTest)
ARMNN_AUTO_TEST_CASE(StridedTransposeConvolution2dUint8Nhwc, StridedTransposeConvolution2dUint8NhwcTest)
ARMNN_AUTO_TEST_CASE(StridedTransposeConvolution2dUint8Nchw, StridedTransposeConvolution2dUint8NchwTest)

ARMNN_AUTO_TEST_CASE(UnbiasedStridedTransposeConvolution2dFloatNhwc, UnbiasedStridedTransposeConvolution2dFloatNhwcTest)
ARMNN_AUTO_TEST_CASE(UnbiasedStridedTransposeConvolution2dFloatNchw, UnbiasedStridedTransposeConvolution2dFloatNchwTest)
ARMNN_AUTO_TEST_CASE(UnbiasedStridedTransposeConvolution2dUint8Nhwc, UnbiasedStridedTransposeConvolution2dUint8NhwcTest)
ARMNN_AUTO_TEST_CASE(UnbiasedStridedTransposeConvolution2dUint8Nchw, UnbiasedStridedTransposeConvolution2dUint8NchwTest)

// ============================================================================
// COMPARE tests

ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareConv2dWithReference, CompareConvolution2dTest)

ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareDepthwiseConv2dWithReferenceFloat32,
                                 CompareDepthwiseConvolution2dFloatTest,
                                 armnn::DataLayout::NCHW)
ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareDepthwiseConv2dWithReferenceUint8,
                                 CompareDepthwiseConvolution2dUint8Test,
                                 armnn::DataLayout::NCHW)

ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareDepthwiseConv2dWithReferenceFloat32Nhwc,
                                 CompareDepthwiseConvolution2dFloatTest,
                                 armnn::DataLayout::NHWC)
ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareDepthwiseConv2dWithReferenceUint8Nhwc,
                                 CompareDepthwiseConvolution2dUint8Test,
                                 armnn::DataLayout::NHWC)

ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareNormalizationWithinWithReference, CompareNormalizationTest,
                                 armnn::NormalizationAlgorithmChannel::Within,
                                 armnn::NormalizationAlgorithmMethod::LocalBrightness)
ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareNormalizationAcrossWithReference, CompareNormalizationTest,
                                 armnn::NormalizationAlgorithmChannel::Across,
                                 armnn::NormalizationAlgorithmMethod::LocalBrightness)

ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareMaxPooling2dWithRef, ComparePooling2dTest, armnn::PoolingAlgorithm::Max)

ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareAveragePooling2dWithRef, ComparePooling2dTest, armnn::PoolingAlgorithm::Average)
ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareAveragePooling2dWithRefUint8, ComparePooling2dUint8Test,
                                 armnn::PoolingAlgorithm::Average)

ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareL2Pooling2dWithRef, ComparePooling2dTest, armnn::PoolingAlgorithm::L2)

ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareAddition, CompareAdditionTest)

ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareMultiplicationWithRef, CompareMultiplicationTest)

ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareBatchNorm, CompareBatchNormTest)

ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareReLu1, CompareBoundedReLuTest, 1.0f, -1.0f)
ARMNN_COMPARE_REF_AUTO_TEST_CASE(CompareReLu6, CompareBoundedReLuTest, 6.0f, 0.0f)

// ============================================================================
// FIXTURE tests

ARMNN_COMPARE_REF_FIXTURE_TEST_CASE(CompareSigmoidActivationWithReference, ActivationFixture,
                                    CompareActivationTest, armnn::ActivationFunction::Sigmoid, 5u)

ARMNN_COMPARE_REF_FIXTURE_TEST_CASE(CompareTanhActivationWithReference, ActivationFixture,
                                    CompareActivationTest, armnn::ActivationFunction::TanH, 5u)

ARMNN_COMPARE_REF_FIXTURE_TEST_CASE(CompareLinearActivationWithReference, ActivationFixture,
                                    CompareActivationTest, armnn::ActivationFunction::Linear, 5u)

ARMNN_COMPARE_REF_FIXTURE_TEST_CASE(CompareReLuActivationWithReference, ActivationFixture,
                                    CompareActivationTest, armnn::ActivationFunction::ReLu, 5u)

ARMNN_COMPARE_REF_FIXTURE_TEST_CASE(CompareBoundedReLuActivationWithReference, ActivationFixture,
                                    CompareActivationTest, armnn::ActivationFunction::BoundedReLu, 5u)
ARMNN_COMPARE_REF_FIXTURE_TEST_CASE(CompareBoundedReLuActivationWithReferenceUint8, ActivationFixture,
                                    CompareActivationUint8Test, armnn::ActivationFunction::BoundedReLu)

ARMNN_COMPARE_REF_FIXTURE_TEST_CASE(CompareSoftReLuActivationWithReference, ActivationFixture,
                                    CompareActivationTest, armnn::ActivationFunction::SoftReLu, 5u)

ARMNN_COMPARE_REF_FIXTURE_TEST_CASE(CompareLeakyReLuActivationWithReference, ActivationFixture,
                                    CompareActivationTest, armnn::ActivationFunction::LeakyReLu, 5u)

ARMNN_COMPARE_REF_FIXTURE_TEST_CASE(CompareAbsActivationWithReference, ActivationFixture,
                                    CompareActivationTest, armnn::ActivationFunction::Abs, 5u)

ARMNN_COMPARE_REF_FIXTURE_TEST_CASE(CompareSqrtActivationWithReference, PositiveActivationFixture,
                                    CompareActivationTest, armnn::ActivationFunction::Sqrt, 5u)

ARMNN_COMPARE_REF_FIXTURE_TEST_CASE(CompareSquareActivationWithReference, ActivationFixture,
                                    CompareActivationTest, armnn::ActivationFunction::Square, 5u)

BOOST_AUTO_TEST_SUITE_END()

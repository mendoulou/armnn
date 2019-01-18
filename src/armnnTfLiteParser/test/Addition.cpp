//
// Copyright © 2017 Arm Ltd. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <boost/test/unit_test.hpp>
#include "ParserFlatbuffersFixture.hpp"
#include "../TfLiteParser.hpp"

#include <string>
#include <iostream>

BOOST_AUTO_TEST_SUITE(TensorflowLiteParser)

struct AddFixture : public ParserFlatbuffersFixture
{
    explicit AddFixture(const std::string & inputShape1,
                        const std::string & inputShape2,
                        const std::string & outputShape,
                        const std::string & activation="NONE")
    {
        m_JsonString = R"(
            {
                "version": 3,
                "operator_codes": [ { "builtin_code": "ADD" } ],
                "subgraphs": [ {
                    "tensors": [
                        {
                            "shape": )" + inputShape1 + R"(,
                            "type": "UINT8",
                            "buffer": 0,
                            "name": "inputTensor1",
                            "quantization": {
                                "min": [ 0.0 ],
                                "max": [ 255.0 ],
                                "scale": [ 1.0 ],
                                "zero_point": [ 0 ],
                            }
                        },
                        {
                            "shape": )" + inputShape2 + R"(,
                            "type": "UINT8",
                            "buffer": 1,
                            "name": "inputTensor2",
                            "quantization": {
                                "min": [ 0.0 ],
                                "max": [ 255.0 ],
                                "scale": [ 1.0 ],
                                "zero_point": [ 0 ],
                            }
                        },
                        {
                            "shape": )" + outputShape + R"( ,
                            "type": "UINT8",
                            "buffer": 2,
                            "name": "outputTensor",
                            "quantization": {
                                "min": [ 0.0 ],
                                "max": [ 255.0 ],
                                "scale": [ 1.0 ],
                                "zero_point": [ 0 ],
                            }
                        }
                    ],
                    "inputs": [ 0, 1 ],
                    "outputs": [ 2 ],
                    "operators": [
                        {
                            "opcode_index": 0,
                            "inputs": [ 0, 1 ],
                            "outputs": [ 2 ],
                            "builtin_options_type": "AddOptions",
                            "builtin_options": {
                                "fused_activation_function": )" + activation + R"(
                            },
                            "custom_options_format": "FLEXBUFFERS"
                        }
                    ],
                } ],
                "buffers" : [
                    { },
                    { }
                ]
            }
        )";
        Setup();
    }
};


struct SimpleAddFixture : AddFixture
{
    SimpleAddFixture() : AddFixture("[ 2, 2 ]",
                                    "[ 2, 2 ]",
                                    "[ 2, 2 ]") {}
};

BOOST_FIXTURE_TEST_CASE(SimpleAdd, SimpleAddFixture)
{
  RunTest<2, armnn::DataType::QuantisedAsymm8>(
      0,
      {{"inputTensor1", { 0, 1, 2, 3 }},
      {"inputTensor2", { 4, 5, 6, 7 }}},
      {{"outputTensor", { 4, 6, 8, 10 }}});
}

BOOST_AUTO_TEST_SUITE_END()
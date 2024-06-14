#ifndef __ALU_H__
#define __ALU_H__

#include "DigitalCircuit.h"

#include <cassert>

class ALU : public DigitalCircuit {

public:

    ALU(const Wire<4> *iALUControl,
        const Wire<32> *iInput0,
        const Wire<32> *iInput1,
        Wire<32> *oOutput,
        Wire<1> *oZero) : DigitalCircuit("ALU") {
        _iALUControl = iALUControl;
        _iInput0 = iInput0;
        _iInput1 = iInput1;
        _oOutput = oOutput;
        _oZero = oZero;
    }

    virtual void advanceCycle() {
        // INPUT
        // input0: ALU의 첫 번째 32비트 입력값
        // input1: ALU의 두 번째 32비트 입력값

        // OUTPUT
        // output: ALU의 32비트 출력값
        // zero: ALU의 zero flag -> output이 0이면 1로 설정됨

        // 파라미터를 받아오고 초기값 설정
        uint32_t aluControl = static_cast<uint32_t>(_iALUControl->to_ulong());
        uint32_t input0 = static_cast<uint32_t>(_iInput0->to_ulong());
        uint32_t input1 = static_cast<uint32_t>(_iInput1->to_ulong());
        uint32_t output = 0;

        // ALU Control에 따라 연산의 종류 결정
        switch (aluControl) {
            case 0b0000: // AND
                output = input0 & input1;
                break;
            case 0b0001: // OR
                output = input0 | input1;
                break;
            case 0b0010: // add
                output = input0 + input1;
                break;
            case 0b0110: // subtract
                output = input0 - input1;
                break;
            case 0b0111: // set on less than
                output = (input0 < input1) ? 1 : 0;
                break;
            case 0b1100: // NOR
                output = ~(input0 | input1);
                break;
            default:
                assert(false && "지원하지 않는 ALU Control입니다.");
        }

        // Set the output value
        *_oOutput = std::bitset<32>(output);

        // Set the zero flag
        *_oZero = std::bitset<1>(output == 0);
    }

private:

    const Wire<4> *_iALUControl;
    const Wire<32> *_iInput0;
    const Wire<32> *_iInput1;
    Wire<32> *_oOutput;
    Wire<1> *_oZero;

};

#endif

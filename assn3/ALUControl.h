#ifndef __ALU_CONTROL_H__
#define __ALU_CONTROL_H__

#include "DigitalCircuit.h"

#include <cassert>

class ALUControl : public DigitalCircuit {

  public:

    ALUControl(const Wire<2> *iALUOp,
               const Wire<6> *iFunct,
               Wire<4> *oOperation) : DigitalCircuit("ALUControl") {
      _iALUOp = iALUOp;
      _iFunct = iFunct;
      _oOperation = oOperation;
    }

    virtual void advanceCycle() {
    uint32_t aluOp = static_cast<uint32_t>(_iALUOp->to_ulong());
    uint32_t funct = static_cast<uint32_t>(_iFunct->to_ulong());
    uint32_t operation = 0;

    if (aluOp == 0b00) {
        operation = 0b0010;
    } else if (aluOp == 0b01 || aluOp == 0b11) {
        operation = 0b0110;
    } else if (aluOp == 0b10 || aluOp == 0b11) {
        uint32_t func_low_4 = funct & 0b1111;
        switch (func_low_4) {
            case 0b0000:
                operation = 0b0010;
                break;
            case 0b0010:
                operation = 0b0110;
                break;
            case 0b0100:
                operation = 0b0000;
                break;
            case 0b0101:
                operation = 0b0001;
                break;
            case 0b1010:
                operation = 0b0111;
                break;
        }
    } else {
        assert(false && "지원하지 않는 ALUOp 또는 funct 필드입니다.");
    }

    *_oOperation = std::bitset<4>(operation);
}

  private:

    const Wire<2> *_iALUOp;
    const Wire<6> *_iFunct;
    Wire<4> *_oOperation;

};

#endif


#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "DigitalCircuit.h"

class Control : public DigitalCircuit {

public:

    Control(const Wire<6> *iOpcode,
            Wire<1> *oRegDst,
            Wire<1> *oALUSrc,
            Wire<1> *oMemToReg,
            Wire<1> *oRegWrite,
            Wire<1> *oMemRead,
            Wire<1> *oMemWrite,
            Wire<1> *oBranch,
            Wire<2> *oALUOp) : DigitalCircuit("Control") {
        _iOpcode = iOpcode;
        _oRegDst = oRegDst;
        _oALUSrc = oALUSrc;
        _oMemToReg = oMemToReg;
        _oRegWrite = oRegWrite;
        _oMemRead = oMemRead;
        _oMemWrite = oMemWrite;
        _oBranch = oBranch;
        _oALUOp = oALUOp;
    }

    virtual void advanceCycle() {
        // INPUT
        // opcode: 6비트 연산 코드
        uint32_t opcode = static_cast<uint32_t>(_iOpcode->to_ulong());

        // OUTPUT
        // RegDst: Destination Register가 $rt field에서 오는지 -> I-Type이면 0, R-Type이면 1
        // ALUSrc: ALU의 두 번째 연산자가 immediate이면 1
        // MemToReg: destination register에 쓰일 값이 ALU 결과면 0, 메모리로부터 읽어오면 1
        // MemRead: 데이터 메모리부터 읽어오는 명령어(eg. lw)의 경우 1
        // MemWrite: 데이터 메모리에 값을 쓰는 명령어(eg. sw)의 경우 1
        // Branch: 분기가 있는 명령어의 경우 1
        // ALUOp: ALU가 수행할 연산의 종류 지정 -> 00 (add), 01 (sub), 10 (R-Type 연산으로 funct 코드를 함께 보게 됨)

        // 초기값 설정 (모두 0으로 설정)
        _oRegDst->reset();
        _oALUSrc->reset();
        _oMemToReg->reset();
        _oRegWrite->reset();
        _oMemRead->reset();
        _oMemWrite->reset();
        _oBranch->reset();
        _oALUOp->reset();

        /*
            [R-Type]
            add: opcode: 000000, funct: 100000
            sub: opcode: 000000, funct: 100010
            and: opcode: 000000, funct: 100100
            or: opcode: 000000, funct: 100101
            nor: opcode: 000000, funct: 100111
            slt: opcode: 000000, funct: 101010

            [I-Type]
            addi: opcode: 001000
            lw: opcode: 100011
            sw: opcode: 101011
            beq: opcode: 000100
        */

        switch (opcode) {
            case 0b000000: // R-Type (add, sub, and, or, nor, slt)
                *_oRegDst = 1;
                *_oALUSrc = 0;
                *_oMemToReg = 0;
                *_oRegWrite = 1;
                *_oMemRead = 0;
                *_oMemWrite = 0;
                *_oBranch = 0;
                *_oALUOp = 0b10;
                break;
            case 0b100011: // lw
                *_oRegDst = 0;
                *_oALUSrc = 1;
                *_oMemToReg = 1;
                *_oRegWrite = 1;
                *_oMemRead = 1;
                *_oMemWrite = 0;
                *_oBranch = 0;
                *_oALUOp = 0b00;
                break;
            case 0b101011: // sw
                *_oRegDst = 0; // 상관없음
                *_oALUSrc = 1;
                *_oMemToReg = 0; // 상관없음
                *_oRegWrite = 0;
                *_oMemRead = 0;
                *_oMemWrite = 1;
                *_oBranch = 0;
                *_oALUOp = 0b00;
                break;
            case 0b000100: // beq
                *_oRegDst = 0; // 상관없음
                *_oALUSrc = 0;
                *_oMemToReg = 0; // 상관없음
                *_oRegWrite = 0;
                *_oMemRead = 0;
                *_oMemWrite = 0;
                *_oBranch = 1;
                *_oALUOp = 0b01;
                break;
            case 0b001000: // addi
                *_oRegDst = 0;
                *_oALUSrc = 1;
                *_oMemToReg = 0;
                *_oRegWrite = 1;
                *_oMemRead = 0;
                *_oMemWrite = 0;
                *_oBranch = 0;
                *_oALUOp = 0b00;
                break;
        }
    }

    bool getRegDst() const {
        return _oRegDst->test(0);
    }

    bool getALUSrc() const {
        return _oALUSrc->test(0);
    }

    uint8_t getALUOp() const {
        return static_cast<uint8_t>(_oALUOp->to_ulong());
    }

    bool getMemToReg() const {
        return _oMemToReg->test(0);
    }

    bool getRegWrite() const {
        return _oRegWrite->test(0);
    }

    bool getMemRead() const {
        return _oMemRead->test(0);
    }

    bool getMemWrite() const {
        return _oMemWrite->test(0);
    }

    bool getBranch() const {
        return _oBranch->test(0);
    }

private:

    const Wire<6> *_iOpcode;
    Wire<1> *_oRegDst;
    Wire<1> *_oALUSrc;
    Wire<1> *_oMemToReg;
    Wire<1> *_oRegWrite;
    Wire<1> *_oMemRead;
    Wire<1> *_oMemWrite;
    Wire<1> *_oBranch;
    Wire<2> *_oALUOp;
};

#endif

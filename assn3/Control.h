#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "DigitalCircuit.h"

class Control : public DigitalCircuit
{

public:
  Control(const Wire<6> *iOpcode,
          Wire<1> *oRegDst,
          Wire<1> *oALUSrc,
          Wire<1> *oMemToReg,
          Wire<1> *oRegWrite,
          Wire<1> *oMemRead,
          Wire<1> *oMemWrite,
          Wire<1> *oBranch,
          Wire<2> *oALUOp) : DigitalCircuit("Control")
  {
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

  virtual void advanceCycle()
  {
    _oRegDst->reset();
    _oALUSrc->reset();
    _oMemToReg->reset();
    _oRegWrite->reset();
    _oMemRead->reset();
    _oMemWrite->reset();
    _oBranch->reset();
    _oALUOp->reset();

    // std::cout << "iOpcode: " << _iOpcode->to_ulong() << std::endl;

    // R-type
    // add, sub, and, or, slt, nor
    // iOpcode = 000000 (0x00)
    if (_iOpcode->test(5) == false &&
        _iOpcode->test(4) == false &&
        _iOpcode->test(3) == false &&
        _iOpcode->test(2) == false &&
        _iOpcode->test(1) == false &&
        _iOpcode->test(0) == false)
    {
      // std::cout << "R-type" << std::endl;
      _oRegDst->set(0, true);
      _oALUSrc->set(0, false);
      _oMemToReg->set(0, false);
      _oRegWrite->set(0, true);
      _oMemRead->set(0, false);
      _oMemWrite->set(0, false);
      _oBranch->set(0, false);
      _oALUOp->set(1, true);
      _oALUOp->set(0, false);
    }
    // lw
    // iOpcode = 100011 (0x23)
    else if (_iOpcode->test(5) == true &&
             _iOpcode->test(4) == false &&
             _iOpcode->test(3) == false &&
             _iOpcode->test(2) == false &&
             _iOpcode->test(1) == true &&
             _iOpcode->test(0) == true)
    {
      // std::cout << "lw" << std::endl;
      _oRegDst->set(0, false);
      _oALUSrc->set(0, true);
      _oMemToReg->set(0, true);
      _oRegWrite->set(0, true);
      _oMemRead->set(0, true);
      _oMemWrite->set(0, false);
      _oBranch->set(0, false);
      _oALUOp->set(0, false);
      _oALUOp->set(1, false);
    }
    // sw
    // iOpcode = 101011 (0x2B)
    else if (_iOpcode->test(5) == true &&
             _iOpcode->test(4) == false &&
             _iOpcode->test(3) == true &&
             _iOpcode->test(2) == false &&
             _iOpcode->test(1) == true &&
             _iOpcode->test(0) == true)
    {
      // std::cout << "sw" << std::endl;
      _oRegDst->set(0, false);
      _oALUSrc->set(0, true);
      _oMemToReg->set(0, false);
      _oRegWrite->set(0, false);
      _oMemRead->set(0, false);
      _oMemWrite->set(0, true);
      _oBranch->set(0, false);
      _oALUOp->set(0, false);
      _oALUOp->set(1, false);
    }
    // beq
    // iOpcode = 000100 (0x04)
    else if (_iOpcode->test(5) == false &&
             _iOpcode->test(4) == false &&
             _iOpcode->test(3) == false &&
             _iOpcode->test(2) == true &&
             _iOpcode->test(1) == false &&
             _iOpcode->test(0) == false)
    {
      // std::cout << "beq" << std::endl;
      _oRegDst->set(0, false);
      _oALUSrc->set(0, false);
      _oMemToReg->set(0, false);
      _oRegWrite->set(0, false);
      _oMemRead->set(0, false);
      _oMemWrite->set(0, false);
      _oBranch->set(0, true);
      _oALUOp->set(1, false);
      _oALUOp->set(0, true);
    }
    // I-type
    // addi, slti, andi, ori
    // addi
    // iOpcode = 001000 (0x08)
    else if (_iOpcode->test(5) == false &&
             _iOpcode->test(4) == false &&
             _iOpcode->test(3) == true &&
             _iOpcode->test(2) == false &&
             _iOpcode->test(1) == false &&
             _iOpcode->test(0) == false)
    {
      // std::cout << "addi" << std::endl;
      _oRegDst->set(0, false);
      _oALUSrc->set(0, true);
      _oMemToReg->set(0, false);
      _oRegWrite->set(0, true);
      _oMemRead->set(0, false);
      _oMemWrite->set(0, false);
      _oBranch->set(0, false);
      _oALUOp->set(0, false);
      _oALUOp->set(1, false);
    }
    // slti
    // Opcode = 001010 (0x0A)
    else if (_iOpcode->test(5) == false &&
             _iOpcode->test(4) == false &&
             _iOpcode->test(3) == true &&
             _iOpcode->test(2) == false &&
             _iOpcode->test(1) == true &&
             _iOpcode->test(0) == false)
    {
      // std::cout << "slti" << std::endl;
      _oRegDst->set(0, false);
      _oALUSrc->set(0, false);
      _oMemToReg->set(0, false);
      _oRegWrite->set(0, true);
      _oMemRead->set(0, false);
      _oMemWrite->set(0, false);
      _oBranch->set(0, false);
      _oALUOp->set(0, false);
      _oALUOp->set(1, true);
    }
    // andi
    // Opcode = 001100 (0x0C)
    else if (_iOpcode->test(5) == false &&
             _iOpcode->test(4) == false &&
             _iOpcode->test(3) == true &&
             _iOpcode->test(2) == true &&
             _iOpcode->test(1) == false &&
             _iOpcode->test(0) == false)
    {
      // std::cout << "andi" << std::endl;
      _oRegDst->set(0, false);
      _oALUSrc->set(0, true);
      _oMemToReg->set(0, false);
      _oRegWrite->set(0, true);
      _oMemRead->set(0, false);
      _oMemWrite->set(0, false);
      _oBranch->set(0, false);
      _oALUOp->set(0, false);
      _oALUOp->set(1, false);
    }
    // ori
    // Opcode = 001101 (0x0D)
    else if (_iOpcode->test(5) == false &&
             _iOpcode->test(4) == false &&
             _iOpcode->test(3) == true &&
             _iOpcode->test(2) == true &&
             _iOpcode->test(1) == false &&
             _iOpcode->test(0) == true)
    {
      // std::cout << "ori" << std::endl;
      _oRegDst->set(0, false);
      _oALUSrc->set(0, true);
      _oMemToReg->set(0, false);
      _oRegWrite->set(0, true);
      _oMemRead->set(0, false);
      _oMemWrite->set(0, false);
      _oBranch->set(0, false);
      _oALUOp->set(0, false);
      _oALUOp->set(1, false);
    }
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

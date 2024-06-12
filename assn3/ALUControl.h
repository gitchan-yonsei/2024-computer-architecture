#ifndef __ALU_CONTROL_H__
#define __ALU_CONTROL_H__

#include "DigitalCircuit.h"

#include <cassert>
#include <iostream>

class ALUControl : public DigitalCircuit
{

public:
  ALUControl(const Wire<2> *iALUOp,
             const Wire<6> *iFunct,
             Wire<4> *oOperation) : DigitalCircuit("ALUControl")
  {
    _iALUOp = iALUOp;
    _iFunct = iFunct;
    _oOperation = oOperation;
  }

  virtual void advanceCycle()
  {
    _oOperation->reset();
    // Two bit iALUOp signal
    // Six bit Funct field
    // Output: four bit operation signal oOperation
    if (_iALUOp->test(1) == false && _iALUOp->test(0) == false)
    {
      // 0010: ADD
      _oOperation->set(3, false);
      _oOperation->set(2, false);
      _oOperation->set(1, true);
      _oOperation->set(0, false);
      return;
    }
    else if (_iALUOp->test(1) == true)
    {
      // funct field 3, 2, 1, 0 are zero --> 0010: ADD
      if (_iFunct->test(3) == false &&
          _iFunct->test(2) == false &&
          _iFunct->test(1) == false &&
          _iFunct->test(0) == false)
      {
        _oOperation->set(3, false);
        _oOperation->set(2, false);
        _oOperation->set(1, true);
        _oOperation->set(0, false);
        return;
      }
      // funct field XX0010 --> 0110: SUB
      else if (_iFunct->test(3) == false &&
               _iFunct->test(2) == false &&
               _iFunct->test(1) == true &&
               _iFunct->test(0) == false)
      {
        _oOperation->set(3, false);
        _oOperation->set(2, true);
        _oOperation->set(1, true);
        _oOperation->set(0, false);
        return;
      }
      // funct field XX0100 --> 0000: AND
      else if (_iFunct->test(3) == false &&
               _iFunct->test(2) == true &&
               _iFunct->test(1) == false &&
               _iFunct->test(0) == false)
      {
        _oOperation->set(3, false);
        _oOperation->set(2, false);
        _oOperation->set(1, false);
        _oOperation->set(0, false);
        return;
      }
      // funct field XX0101 --> 0001: OR
      else if (_iFunct->test(3) == false &&
               _iFunct->test(2) == true &&
               _iFunct->test(1) == false &&
               _iFunct->test(0) == true)
      {
        _oOperation->set(3, false);
        _oOperation->set(2, false);
        _oOperation->set(1, false);
        _oOperation->set(0, true);
        return;
      }
      // funct field XX1010 --> 0111: SLT
      else if (_iFunct->test(3) == true &&
               _iFunct->test(2) == false &&
               _iFunct->test(1) == true &&
               _iFunct->test(0) == false)
      {
        _oOperation->set(3, false);
        _oOperation->set(2, true);
        _oOperation->set(1, true);
        _oOperation->set(0, true);
        return;
      }
    }
    else if (_iALUOp->test(0) == true)
    {
      // 0110: SUB
      _oOperation->set(3, false);
      _oOperation->set(2, true);
      _oOperation->set(1, true);
      _oOperation->set(0, false);
    }
  }

private:
  const Wire<2> *_iALUOp;
  const Wire<6> *_iFunct;
  Wire<4> *_oOperation;
};

#endif

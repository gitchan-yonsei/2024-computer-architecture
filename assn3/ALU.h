#ifndef __ALU_H__
#define __ALU_H__

#include "DigitalCircuit.h"

#include <iostream>
#include <cassert>

class ALU : public DigitalCircuit
{

public:
  ALU(const Wire<4> *iALUControl,
      const Wire<32> *iInput0,
      const Wire<32> *iInput1,
      Wire<32> *oOutput,
      Wire<1> *oZero) : DigitalCircuit("ALU")
  {
    _iALUControl = iALUControl;
    _iInput0 = iInput0;
    _iInput1 = iInput1;
    _oOutput = oOutput;
    _oZero = oZero;
  }

  virtual void advanceCycle()
  {
    _oOutput->reset();
    _oZero->reset();

    // ALU that supports 32bit signed integer AND, OR, ADD, SUB, SLT, NOR
    // 0000: AND, 0001: OR, 0010: ADD, 0110: SUB, 0111: SLT, 1100: NOR

    // print 0, 1, 2, 3 bits of alucontrol in order
    // std::cout << "ALU Control: " << _iALUControl->test(3) << _iALUControl->test(2) << _iALUControl->test(1) << _iALUControl->test(0) << std::endl;

    // 0000 AND
    if (_iALUControl->test(3) == false &&
        _iALUControl->test(2) == false &&
        _iALUControl->test(1) == false &&
        _iALUControl->test(0) == false)
    {
      // std::cout << "AND" << std::endl;
      for (size_t i = 0; i < 32; i++)
      {
        _oOutput->set(i, _iInput0->test(i) && _iInput1->test(i));
      }
    }
    // 0001 OR
    else if (_iALUControl->test(3) == false &&
             _iALUControl->test(2) == false &&
             _iALUControl->test(1) == false &&
             _iALUControl->test(0) == true)
    {
      // std::cout << "OR" << std::endl;
      for (size_t i = 0; i < 32; i++)
      {
        _oOutput->set(i, _iInput0->test(i) || _iInput1->test(i));
      }
    }
    // 0010 ADD
    else if (_iALUControl->test(3) == false &&
             _iALUControl->test(2) == false &&
             _iALUControl->test(1) == true &&
             _iALUControl->test(0) == false)
    {
      unsigned tmp = 0;
      for (size_t i = 0; i < 32; i++)
      {
        if (_iInput0->test(i))
        {
          tmp++;
        }
        if (_iInput1->test(i))
        {
          tmp++;
        }
        _oOutput->set(i, (tmp % 2) == 1 ? true : false);
        tmp /= 2;
      }
      if (tmp > 0)
      {
        _oZero->set(0, true);
      }
      // print add input0 + input1 = output  in one line
      // std::cout << "Add Input0: " << _iInput0->to_ulong() << " + Input1: " << _iInput1->to_ulong() << " = Output: " << _oOutput->to_ulong() << std::endl;
    }
    // 0110 SUB
    else if (_iALUControl->test(3) == false &&
             _iALUControl->test(2) == true &&
             _iALUControl->test(1) == true &&
             _iALUControl->test(0) == false)
    {
      unsigned carry = 1;
      for (size_t i = 0; i < 32; i++)
      {
        if (_iInput0->test(i))
        {
          carry++;
        }
        if (!(_iInput1->test(i)))
        {
          carry++;
        }
        _oOutput->set(i, (carry % 2) == 1 ? true : false);
        carry /= 2;
      }
      if (carry > 0)
      {
        _oZero->set(0, true);
      }
      // print sub input0 - input1 = output  in one line
      // std::cout << "Sub Input0: " << _iInput0->to_ulong() << " - Input1: " << _iInput1->to_ulong() << " = Output: " << _oOutput->to_ulong() << std::endl;
    }
    // 0111 SLT
    // set 1 if iInput0 is less than iInput1, otherwise set 0
    else if (_iALUControl->test(3) == false &&
             _iALUControl->test(2) == true &&
             _iALUControl->test(1) == true &&
             _iALUControl->test(0) == true)
    {
      bool isLess = false;
      for (size_t i = 31; i >= 0; i--)
      {
        if (_iInput0->test(i) && !(_iInput1->test(i)))
        {
          isLess = true;
          break;
        }
        else if (!(_iInput0->test(i)) && _iInput1->test(i))
        {
          isLess = false;
          break;
        }
      }
      _oOutput->set(0, !isLess);
      // print slt input0 < input1 = output  in one line
      // std::cout << "SLT Input0: " << _iInput0->to_ulong() << " < Input1: " << _iInput1->to_ulong() << " = Output: " << _oOutput->to_ulong() << std::endl;
    }
    // 1100 NOR
    else if (_iALUControl->test(3) == true &&
             _iALUControl->test(2) == true &&
             _iALUControl->test(1) == false &&
             _iALUControl->test(0) == false)
    {
      // std::cout << "NOR" << std::endl;
      for (size_t i = 0; i < 32; i++)
      {
        _oOutput->set(i, !(_iInput0->test(i) || _iInput1->test(i)));
      }
    }
    else
    {
      assert(false && "Invalid ALU control signal");
    }
    // set the zero signal to 1 if the output value is zero
    if (_oOutput->to_ulong() == 0)
    {
      _oZero->set(0, true);
    }
  }

private:
  const Wire<4> *_iALUControl;
  const Wire<32> *_iInput0;
  const Wire<32> *_iInput1;
  Wire<32> *_oOutput;
  Wire<1> *_oZero;
};

#endif

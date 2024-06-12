#ifndef __REGISTER_FILE_HPP__
#define __REGISTER_FILE_HPP__

#include "DigitalCircuit.h"

#include <cassert>
#include <cstdio>

class RegisterFile : public DigitalCircuit
{

public:
  RegisterFile(const Wire<5> *iReadRegister1,
               const Wire<5> *iReadRegister2,
               const Wire<5> *iWriteRegister,
               const Wire<32> *iWriteData,
               const Wire<1> *iRegWrite,
               Wire<32> *oReadData1,
               Wire<32> *oReadData2,
               const char *initFileName = nullptr)
      : DigitalCircuit("RegisterFile")
  {
    _iReadRegister1 = iReadRegister1;
    _iReadRegister2 = iReadRegister2;
    _iWriteRegister = iWriteRegister;
    _iWriteData = iWriteData;
    _iRegWrite = iRegWrite;
    _oReadData1 = oReadData1;
    _oReadData2 = oReadData2;

    for (size_t i = 0; i < 32; i++)
    {
      _registers[i].reset();
    }

    if (initFileName != nullptr)
    {
      // Each line of the memory initialization file consists of:
      //   1) the target register index in decimal value
      //   2) the eight-digit hexadecimal value of the data
      // For example, "10 ABCD1234" stores 0xABCD1234 to register #10.
      FILE *initFile = fopen(initFileName, "r");
      assert(initFile != NULL);
      std::uint32_t reg, val;
      while (fscanf(initFile, " %u %x", &reg, &val) == 2)
      {
        if (reg != 0)
        {
          printf("INFO: $%02u <-- 0x%08lx\n", reg, (unsigned long)val);
          _registers[reg] = val;
        }
      }
    }
  }

  void printRegisters()
  {
    for (size_t i = 0; i < 32; i++)
    {
      if (_registers[i].any())
      {
        printf("  $%02lu = 0x%08lx\n", i, _registers[i].to_ulong());
      }
    }
  }

  virtual void advanceCycle()
  {
    uint32_t readReg1 = 0;
    uint32_t readReg2 = 0;
    uint32_t writeReg = 0;
    uint32_t writeData = 0;
    bool regWrite = _iRegWrite->test(0);

    // For reading, RegWrite is 0.
    // iReadRegister1 = $rs
    // iReadRegister2 = $rt
    // $rs and $rt are returned to oReadData1 and oReadData2, respectively.
    _oReadData1->reset();
    _oReadData2->reset();

    for (size_t i = 0; i < 5; ++i)
    {
      if (_iReadRegister1->test(i))
      {
        readReg1 |= (1 << i);
      }
      if (_iReadRegister2->test(i))
      {
        readReg2 |= (1 << i);
      }
      if (_iWriteRegister->test(i))
      {
        writeReg |= (1 << i);
      }
    }

    uint32_t readData1 = _registers[readReg1].to_ulong();
    uint32_t readData2 = _registers[readReg2].to_ulong();

    for (size_t i = 0; i < 32; ++i)
    {
      _oReadData1->set(i, (readData1 & (1 << i)) != 0);
      _oReadData2->set(i, (readData2 & (1 << i)) != 0);
    }
    // printf("oReadData1 = 0x%08lx\n", (unsigned long)readData1);
    // printf("oReadData2 = 0x%08lx\n", (unsigned long)readData2);

    // For writing to a destination register, RegWrite is 1.
    // Index of the destination register is iWriteRegister.
    // Value to be written is iWriteData.
    // No return value exists for writes.
    // Writing to zero register is ignored.
    if (regWrite && writeReg != 0)
    {
      for (size_t i = 0; i < 32; ++i)
      {
        if (_iWriteData->test(i))
        {
          writeData |= (1 << i);
        }
      }
      _registers[writeReg] = writeData;
    }
  }

private:
  const Wire<5> *_iReadRegister1;
  const Wire<5> *_iReadRegister2;
  const Wire<5> *_iWriteRegister;
  const Wire<32> *_iWriteData;
  const Wire<1> *_iRegWrite;
  Wire<32> *_oReadData1;
  Wire<32> *_oReadData2;

  std::bitset<32> _registers[32];
};

#endif

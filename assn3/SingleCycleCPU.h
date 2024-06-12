#ifndef __SINGLE_CYCLE_CPU_H__
#define __SINGLE_CYCLE_CPU_H__

#include "DigitalCircuit.h"

#include "ALU.h"
#include "ALUControl.h"
#include "Control.h"
#include "RegisterFile.h"
#include "Memory.h"

#include <bitset>
#include <cstdint>
#include <cstdio>
#include <string>

template <size_t N>
class MUX : public DigitalCircuit
{

public:
  MUX(const std::string &name,
      const Wire<N> *iInput0,
      const Wire<N> *iInput1,
      const Wire<1> *iSelect,
      Wire<N> *oOutput) : DigitalCircuit(name)
  {
    _iInput0 = iInput0;
    _iInput1 = iInput1;
    _iSelect = iSelect;
    _oOutput = oOutput;
  }

  // MUX that selects between two N-bit inputs based on a 1-bit select signal
  // If the select signal is 0, the output is the first input
  // If the select signal is 1, the output is the second input
  virtual void advanceCycle()
  {
    _oOutput->reset();

    if (_iSelect->test(0) == false)
    {
      for (size_t i = 0; i < N; i++)
      {
        _oOutput->set(i, _iInput0->test(i));
      }
    }
    else
    {
      for (size_t i = 0; i < N; i++)
      {
        _oOutput->set(i, _iInput1->test(i));
      }
    }
  }

private:
  const Wire<N> *_iInput0;
  const Wire<N> *_iInput1;
  const Wire<1> *_iSelect;
  Wire<N> *_oOutput;
};

class SingleCycleCPU : public DigitalCircuit
{

public:
  SingleCycleCPU(const std::string name,
                 const std::uint32_t initialPC,
                 const char *regName,
                 const char *instMemFileName,
                 const char *dataMemFileName) : DigitalCircuit(name)
  {
    _currCycle = 0;

    _alwaysHi = 1;
    _alwaysLo = 0;
    _alwaysLo32.reset();

    _PC = initialPC;

    /* FIXME: setup various sequential/combinational circuits and wires as needed */
    _registerFile =
        new RegisterFile(&_regReadRegister1,       // $rs
                         &_regReadRegister2,       // $rt
                         &_muxRegWriteRegisterOut, // if Write=1, write $rd
                         &_regWriteData,           // if Write=1, write ALU result or data memory read data
                         &_ctrlRegWrite,           // RegWrite
                         &_regReadData1,           // if Write=0, read $rs
                         &_regReadData2,           // if Write=0, read $rt
                         regName);

    // Instruction Memory read from PC and write to instruction register
    _instMemory = new Memory("InstructionMemory",
                             &_PC,                 // Read address
                             &_alwaysLo32,         // Write data  (no writing to instruction memory)
                             &_alwaysHi,           // ctrlMemRead (always 1)
                             &_alwaysLo,           // ctrlMemWrite (unused)
                             &_instMemInstruction, // Output read data
                             Memory::LittleEndian, // Endianness
                             instMemFileName);     // File name

    _dataMemory = new Memory("DataMemory",
                             &_aluResult,          // address
                             &_regReadData2,       // write data
                             &_ctrlMemRead,        // MemRead
                             &_ctrlMemWrite,       // MemWrite
                             &_dataMemReadData,    // Output read data
                             Memory::LittleEndian, // Endianness
                             dataMemFileName);     // File name

    _control = new Control(&_ctrlOpcode, // Input opcode
                           &_ctrlRegDst,
                           &_ctrlALUSrc,
                           &_ctrlMemToReg,
                           &_ctrlRegWrite,
                           &_ctrlMemRead,
                           &_ctrlMemWrite,
                           &_ctrlBranch,
                           &_ctrlALUOp);

    _aluControl = new ALUControl(&_ctrlALUOp,    // ALU operation 2 bits
                                 &_aluCtrlFunct, // ALU function 6 bits
                                 &_aluCtrlOp);   // Result operation in 4 bits

    _alu = new ALU(&_aluCtrlOp,          // ALU operation (e.g., ADD, SUB, AND, OR, SLT, NOR)
                   &_regReadData1,       // $rs
                   &_muxALUInput1Output, // ALU second input (either $rt or imm)
                   &_aluResult,          // ALU result
                   &_aluZero);           // ALU zero flag

    _muxWriteRegister = new MUX<5>("MUX_RegFileWriteRegister",
                                   &_muxWriteRegInput0,
                                   &_muxWriteRegInput1,
                                   &_ctrlRegDst,
                                   &_muxRegWriteRegisterOut);

    _muxALUInput1 = new MUX<32>("MUX_ALUInput1",
                                &_regReadData2,        // $rt
                                &_signExtendOutput,    // sign-extended imm
                                &_ctrlALUSrc,          // I-type or R-type
                                &_muxALUInput1Output); // ALU second input

    // MUX for selecting between ALU result and data memory read data
    _muxRegWriteData = new MUX<32>("MUX_RegFileWriteData",
                                   &_aluResult,
                                   &_dataMemReadData,
                                   &_ctrlMemToReg,
                                   &_regWriteData);

    _muxPC = new MUX<32>("MUX_PC",
                         &_muxPCInput0,
                         &_muxPCInput1,
                         &_muxPCSelect,
                         &_PC);
  }

  void printPVS()
  {
    printf("==================== Cycle %lu ====================\n", _currCycle);
    printf("PC = 0x%08lx\n", _PC.to_ulong());
    printf("Registers:\n");
    _registerFile->printRegisters();
    printf("Data Memory:\n");
    _dataMemory->printMemory();
    printf("Instruction Memory:\n");
    _instMemory->printMemory();
  }

  virtual void advanceCycle()
  {
    _currCycle += 1;

    /* FIXME: implement the single-cycle behavior of the single-cycle MIPS CPU */

    // Fetch
    _instMemory->advanceCycle(); // output: _instMemInstruction

    // Decode
    _ctrlOpcode = (_instMemInstruction >> 26).to_ulong();
    // printf("_ctrlOpcode = %lu\n", _ctrlOpcode.to_ulong());
    // reset all the control signals
    _ctrlRegDst.reset();
    _ctrlALUSrc.reset();
    _ctrlMemToReg.reset();
    _ctrlRegWrite.reset();
    _ctrlMemRead.reset();
    _ctrlMemWrite.reset();
    _ctrlBranch.reset();
    _ctrlALUOp.reset();

    _control->advanceCycle(); // set control signals

    _muxWriteRegInput0.reset();
    _muxWriteRegInput1.reset();
    for (size_t i = 0; i < 5; ++i)
    {
      if (_instMemInstruction.test(i + 16))
      {
        _muxWriteRegInput0.set(i); // $rt
      }
      if (_instMemInstruction.test(i + 11))
      {
        _muxWriteRegInput1.set(i); // $rd
      }
    }
    _muxWriteRegister->advanceCycle();

    _regReadRegister1.reset();
    _regReadRegister2.reset();
    for (size_t i = 0; i < 5; ++i)
    {
      if (_instMemInstruction.test(i + 21))
      {
        _regReadRegister1.set(i); // $rs
      }
      if (_instMemInstruction.test(i + 16))
      {
        _regReadRegister2.set(i); // $rt
      }
    }
    // printf("_regReadRegister1 = %lu\n", _regReadRegister1.to_ulong());
    // printf("_regReadRegister2 = %lu\n", _regReadRegister2.to_ulong());
    _registerFile->advanceCycle();
    // printf("_regReadData1 = %lu\n", _regReadData1.to_ulong());

    _signExtendInput.reset();
    for (size_t i = 0; i < 16; ++i)
    {
      if (_instMemInstruction.test(i))
      {
        _signExtendInput.set(i);
      }
    }

    if (_signExtendInput.test(15))
    {
      for (size_t i = 16; i < 32; ++i)
      {
        _signExtendInput.set(i);
      }
    }

    _signExtendOutput.reset();
    for (size_t i = 0; i < 32; ++i)
    {
      if (i < 16 && _signExtendInput.test(i))
      {
        _signExtendOutput.set(i);
      }
      else if (i >= 16 && _signExtendInput.test(15))
      {
        _signExtendOutput.set(i);
      }
    }

    _muxALUInput1->advanceCycle();
    // printf("_muxALUInput1Output = %lu\n", _muxALUInput1Output.to_ulong());

    _aluCtrlFunct.reset();
    for (size_t i = 0; i < 6; ++i)
    {
      if (_instMemInstruction.test(i))
      {
        _aluCtrlFunct.set(i);
      }
    }
    _aluControl->advanceCycle();

    // Execute
    _alu->advanceCycle();
    // printf("ALU result = %s\n", _aluResult.to_string().c_str());

    // Memory Access
    _dataMemory->advanceCycle();
    // if (_ctrlMemRead.test(0))
    // {
    //   // print in hex format
    //   printf("dataMemReadData = 0x%08lx\n", _dataMemReadData.to_ulong());
    // }
    // if (_ctrlMemWrite.test(0))
    // {
    //   // print in hex format
    //   printf("dataMemWriteData = 0x%08lx to address 0x%08lx\n", _regReadData2.to_ulong(), _aluResult.to_ulong());
    // }

    // Write Back
    _muxRegWriteData->advanceCycle();
    _registerFile->advanceCycle();
    // if (_ctrlRegWrite.test(0))
    // {
    //   // print in hex format and register number in decimal
    //   printf("write 0x%08lx to register %lu\n", _regWriteData.to_ulong(), _muxRegWriteRegisterOut.to_ulong());
    // }

    _muxPCInput0 = _PC.to_ulong() + 4; // PC + 4
    // Compute Branch Target
    _muxPCInput1 = _PC.to_ulong() + 4 + (_signExtendOutput.to_ulong() << 2);

    // Update PC
    _muxPCSelect = (_ctrlBranch.test(0) & _aluZero.test(0));
    _muxPC->advanceCycle();
  }

  ~SingleCycleCPU()
  {
    delete _instMemory;
    delete _registerFile;
    delete _dataMemory;

    delete _control;
    delete _aluControl;
    delete _alu;
    delete _muxWriteRegister;
    delete _muxALUInput1;
    delete _muxRegWriteData;
    delete _muxPC;
  }

private:
  // Cycle tracker
  std::uint64_t _currCycle;

  // Always-1/0 wires
  Wire<1> _alwaysHi;
  Wire<1> _alwaysLo;
  Wire<32> _alwaysLo32;

  // Program Counter (PC) register
  Register<32> _PC;

  // Sequential circuits
  Memory *_instMemory;
  RegisterFile *_registerFile;
  Memory *_dataMemory;

  // Combinational circuits
  Control *_control;
  ALUControl *_aluControl;
  ALU *_alu;
  MUX<5> *_muxWriteRegister;
  MUX<32> *_muxALUInput1;
  MUX<32> *_muxRegWriteData;
  MUX<32> *_muxPC;

  // instruction[31-0] from the instruction memory
  Wire<32> _instMemInstruction;
  // the control signals to/from the Control unit
  Wire<6> _ctrlOpcode;
  Wire<1> _ctrlRegDst, _ctrlALUSrc, _ctrlMemToReg, _ctrlRegWrite, _ctrlMemRead, _ctrlMemWrite, _ctrlBranch;
  Wire<2> _ctrlALUOp;
  // the wires to/from the MUX in front of the Register File's Write Register
  Wire<5> _muxWriteRegInput0, _muxWriteRegInput1, _muxRegWriteRegisterOut;
  // the wires to/from the Register File
  Wire<5> _regReadRegister1, _regReadRegister2;
  Wire<32> _regWriteData, _regReadData1, _regReadData2;
  // the wires to/from the Sign-extend unit
  Wire<16> _signExtendInput;
  Wire<32> _signExtendOutput;
  // the wires from the MUX in front of the ALU's second input
  Wire<32> _muxALUInput1Output;
  // the wires to/from the ALU control unit
  Wire<6> _aluCtrlFunct;
  Wire<4> _aluCtrlOp;
  // the wires from the ALU
  Wire<32> _aluResult;
  Wire<1> _aluZero;
  // the wire from the data memory
  Wire<32> _dataMemReadData;
  // the wires to/from the MUX in front of the PC register
  Wire<32> _muxPCInput0, _muxPCInput1;
  Wire<1> _muxPCSelect; // a.k.a. PCSrc
};

#endif

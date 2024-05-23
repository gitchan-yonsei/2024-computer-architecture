#ifndef __SINGLE_CYCLE_CPU_H__
#define __SINGLE_CYCLE_CPU_H__

#include "DigitalCircuit.h"

#include "ALU.h"
#include "ALUControl.h"
#include "Control.h"
#include "RegisterFile.h"
#include "Memory.h"

template<size_t N>
class MUX : public DigitalCircuit {

  public:

    MUX(const std::string &name,
        const Wire<N> *iInput0,
        const Wire<N> *iInput1,
        const Wire<1> *iSelect,
        Wire<N> *oOutput) : DigitalCircuit(name) {
      _iInput0 = iInput0;
      _iInput1 = iInput1;
      _iSelect = iSelect;
      _oOutput = oOutput;
    }

    virtual void advanceCycle() {
          if(_iSelect->test(0)) {
            *_oOutput = *_iInput1;
        } else {
            *_oOutput = *_iInput0;
        }
    }

  private:

    const Wire<N> *_iInput0;
    const Wire<N> *_iInput1;
    const Wire<1> *_iSelect;
    Wire<N> *_oOutput;

};

class SingleCycleCPU : public DigitalCircuit {

  public:

    SingleCycleCPU(const std::string name,
                   const std::uint32_t initialPC,
                   const char *regFileName,
                   const char *instMemFileName,
                   const char *dataMemFileName) : DigitalCircuit(name) {
      _currCycle = 0;

      _alwaysHi = 1;
      _alwaysLo = 0;

      _PC = initialPC;

      // Initialize Instruction Memory
      _instMemory = new Memory("InstructionMemory", &_PC, nullptr, &_alwaysHi, &_alwaysLo, &_instMemInstruction, Memory::LittleEndian);

      // Initialize Register File
      _registerFile = new RegisterFile(&_regFileReadRegister1, &_regFileReadRegister2, &_muxRegFileWriteRegisterOutput, &_regFileWriteData, &_ctrlRegWrite, &_regFileReadData1, &_regFileReadData2, regFileName);

      // Initialize Data Memory
      _dataMemory = new Memory("DataMemory", &_aluResult, &_regFileReadData2, &_ctrlMemRead, &_ctrlMemWrite, &_dataMemReadData, Memory::LittleEndian);

      // Initialize Control Unit
      _control = new Control(&_ctrlOpcode, &_ctrlRegDst, &_ctrlALUSrc, &_ctrlMemToReg, &_ctrlRegWrite, &_ctrlMemRead, &_ctrlMemWrite, &_ctrlBranch, &_ctrlALUOp);

      // Initialize ALU Control Unit
      _aluControl = new ALUControl(&_ctrlALUOp, &_aluCtrlFunct, &_aluCtrlOp);

      // Initialize ALU
      _alu = new ALU(&_aluCtrlOp, &_regFileReadData1, &_muxALUInput1Output, &_aluResult, &_aluZero);

      // Initialize MUXes
      _muxRegFileWriteRegister = new MUX<5>("MUX_RegFileWriteRegister", &_muxRegFileWriteRegisterInput0, &_muxRegFileWriteRegisterInput1, &_ctrlRegDst, &_muxRegFileWriteRegisterOutput);
      _muxALUInput1 = new MUX<32>("MUX_ALUInput1", &_regFileReadData2, &_signExtendOutput, &_ctrlALUSrc, &_muxALUInput1Output);
      _muxRegFileWriteData = new MUX<32>("MUX_RegFileWriteData", &_aluResult, &_dataMemReadData, &_ctrlMemToReg, &_regFileWriteData);
      _muxPC = new MUX<32>("MUX_PC", &_muxPCInput0, &_muxPCInput1, &_muxPCSelect, &_PC);
    }

    void printPVS() {
      printf("==================== Cycle %lu ====================\n", _currCycle);
      printf("PC = 0x%08lx\n", _PC.to_ulong());
      printf("Registers:\n");
      _registerFile->printRegisters();
      printf("Data Memory:\n");
      _dataMemory->printMemory();
      printf("Instruction Memory:\n");
      _instMemory->printMemory();
    }

    virtual void advanceCycle() {
      _currCycle += 1;

      // IF (Instruction Fetch)
      _instMemory->advanceCycle();
      _ctrlOpcode = _instMemInstruction.to_ulong() >> 26; // 명령어 상위 6비트 추출
      _control->advanceCycle();

      // ID (Instruction Decode)
      _regFileReadRegister1 = (_instMemInstruction.to_ulong() >> 21) & 0x1F;
      _regFileReadRegister2 = (_instMemInstruction.to_ulong() >> 16) & 0x1F;
      _muxRegFileWriteRegisterInput0 = _regFileReadRegister2;
      _muxRegFileWriteRegisterInput1 = (_instMemInstruction.to_ulong() >> 11) & 0x1F;
      _registerFile->advanceCycle();

      // EXE (Execute)
      _aluCtrlFunct = _instMemInstruction.to_ulong() & 0x3F;
      _aluControl->advanceCycle();
      _signExtendInput = _instMemInstruction.to_ulong() & 0xFFFF;
      _signExtendOutput = (_signExtendInput.test(15) ? 0xFFFF0000 : 0x00000000) | _signExtendInput.to_ulong();
      _muxALUInput1->advanceCycle();
      _alu->advanceCycle();

      // MEM (Memory Access)
      if (_ctrlMemRead.test(0) || _ctrlMemWrite.test(0)) {
          _dataMemory->advanceCycle();
      }

      // WB (Write Back)
      _muxRegFileWriteData->advanceCycle();
      if (_ctrlRegWrite.test(0)) {
          _registerFile->advanceCycle();
      }

      // Update PC (Program Counter)
      _muxPCInput0 = _PC.to_ulong() + 4;
      _muxPCInput1 = (_signExtendOutput.to_ulong() << 2) + _muxPCInput0.to_ulong();
      _muxPCSelect = _ctrlBranch & _aluZero;
      _muxPC->advanceCycle();

      // reset all wires
      _instMemInstruction.reset();
      _ctrlOpcode.reset();
      _ctrlRegDst.reset();
      _ctrlALUSrc.reset();
      _ctrlMemToReg.reset();
      _ctrlRegWrite.reset();
      _ctrlMemRead.reset();
      _ctrlMemWrite.reset();
      _ctrlBranch.reset();
      _ctrlALUOp.reset();
      _muxRegFileWriteRegisterInput0.reset();
      _muxRegFileWriteRegisterInput1.reset();
      _muxRegFileWriteRegisterOutput.reset();
      _regFileReadRegister1.reset();
      _regFileReadRegister2.reset();
      _regFileWriteData.reset();
      _regFileReadData1.reset();
      _regFileReadData2.reset();
      _signExtendInput.reset();
      _signExtendOutput.reset();
      _muxALUInput1Output.reset();
      _aluCtrlFunct.reset();
      _aluCtrlOp.reset();
      _aluResult.reset();
      _aluZero.reset();
      _dataMemReadData.reset();
      _muxPCInput0.reset();
      _muxPCInput1.reset();
      _muxPCSelect.reset();
    }

    ~SingleCycleCPU() {
      delete _instMemory;
      delete _registerFile;
      delete _dataMemory;

      delete _control;
      delete _aluControl;
      delete _alu;
      delete _muxRegFileWriteRegister;
      delete _muxALUInput1;
      delete _muxRegFileWriteData;
      delete _muxPC;
    }

  private:

    // Cycle tracker
    std::uint64_t _currCycle;

    // Always-1/0 wires
    Wire<1> _alwaysHi;
    Wire<1> _alwaysLo;

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
    MUX<5> *_muxRegFileWriteRegister;
    MUX<32> *_muxALUInput1;
    MUX<32> *_muxRegFileWriteData;
    MUX<32> *_muxPC;

    // instruction[31-0] from the instruction memory
    Wire<32> _instMemInstruction;
    // the control signals to/from the Control unit
    Wire<6> _ctrlOpcode;
    Wire<1> _ctrlRegDst, _ctrlALUSrc, _ctrlMemToReg, _ctrlRegWrite, _ctrlMemRead, _ctrlMemWrite, _ctrlBranch;
    Wire<2> _ctrlALUOp;
    // the wires to/from the MUX in front of the Register File's Write Register
    Wire<5> _muxRegFileWriteRegisterInput0, _muxRegFileWriteRegisterInput1, _muxRegFileWriteRegisterOutput;
    // the wires to/from the Register File
    Wire<5> _regFileReadRegister1, _regFileReadRegister2;
    Wire<32> _regFileWriteData, _regFileReadData1, _regFileReadData2;
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


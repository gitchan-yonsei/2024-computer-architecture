#ifndef __PIPELINED_CPU_H__
#define __PIPELINED_CPU_H__

#include "DigitalCircuit.h"

#include "Memory.h"
#include "Control.h"
#include "RegisterFile.h"
#include "ALU.h"
#include "ALUControl.h"

#include "Miscellaneous.h"

#ifdef ENABLE_DATA_FORWARDING
class ForwardingUnit : public DigitalCircuit {
  public:
    ForwardingUnit(
      const std::string &name,
      const Wire<5> *iIDEXRs,
      const Wire<5> *iIDEXRt,
      const Wire<1> *iEXMEMRegWrite,
      const Wire<5> *iEXMEMRegDstIdx,
      const Wire<1> *iMEMWBRegWrite,
      const Wire<5> *iMEMWBRegDstIdx,
      Wire<2> *oForwardA,
      Wire<2> *oForwardB
    ) : DigitalCircuit(name) {
      _iIDEXRs = iIDEXRs;
      _iIDEXRt = iIDEXRt;
      _iEXMEMRegWrite = iEXMEMRegWrite;
      _iEXMEMRegDstIdx = iEXMEMRegDstIdx;
      _iMEMWBRegWrite = iMEMWBRegWrite;
      _iMEMWBRegDstIdx = iMEMWBRegDstIdx;
      _oForwardA = oForwardA;
      _oForwardB = oForwardB;
    }
    virtual void advanceCycle() {
      /* FIXME */
    }
  private:
    const Wire<5> *_iIDEXRs;
    const Wire<5> *_iIDEXRt;
    const Wire<1> *_iEXMEMRegWrite;
    const Wire<5> *_iEXMEMRegDstIdx;
    const Wire<1> *_iMEMWBRegWrite;
    const Wire<5> *_iMEMWBRegDstIdx;
    Wire<2> *_oForwardA;
    Wire<2> *_oForwardB;
};

#ifdef ENABLE_HAZARD_DETECTION
class HazardDetectionUnit : public DigitalCircuit {
  public:
    HazardDetectionUnit(
      const std::string &name,
      const Wire<5> *iIFIDRs,
      const Wire<5> *iIFIDRt,
      const Wire<1> *iIDEXMemRead,
      const Wire<5> *iIDEXRt,
      Wire<1> *oPCWrite,
      Wire<1> *oIFIDWrite,
      Wire<1> *oIDEXCtrlWrite
    ) : DigitalCircuit(name) {
      _iIFIDRs = iIFIDRs;
      _iIFIDRt = iIFIDRt;
      _iIDEXMemRead = iIDEXMemRead;
      _iIDEXRt = iIDEXRt;
      _oPCWrite = oPCWrite;
      _oIFIDWrite = oIFIDWrite;
      _oIDEXCtrlWrite = oIDEXCtrlWrite;
    }
    virtual void advanceCycle() {
      /* FIXME */
    }
  private:
    const Wire<5> *_iIFIDRs;
    const Wire<5> *_iIFIDRt;
    const Wire<1> *_iIDEXMemRead;
    const Wire<5> *_iIDEXRt;
    Wire<1> *_oPCWrite;
    Wire<1> *_oIFIDWrite;
    Wire<1> *_oIDEXCtrlWrite;
};
#endif // ENABLE_HAZARD_DETECTION
#endif // ENABLE_DATA_FORWARDING

class PipelinedCPU : public DigitalCircuit {

  public:

    PipelinedCPU(
      const std::string &name,
      const std::uint32_t &initialPC,
      const Memory::Endianness &memoryEndianness,
      const char *regFileName,
      const char *instMemFileName,
      const char *dataMemFileName
    ) : DigitalCircuit(name) {
    _currCycle = 0;

    _PC = initialPC;

       _registerFile =
            new RegisterFile(
                &_regFileReadRegister1,       // $rs
                &_regFileReadRegister2,       // $rt
                &_latchMEMWB.regDstIdx,       // if Write=1, write $rd
                &_muxMemToRegOutput,          // if Write=1, write ALU result or data memory read data
                &_latchMEMWB.ctrlWB.regWrite, // RegWrite
                &_latchIDEX.regFileReadData1, // if Write=0, read $rs
                &_latchIDEX.regFileReadData2, // if Write=0, read $rt
                regFileName                   // 파일 이름
            );

      // Instruction Memory read from PC and write to instruction register
    _instMemory = new Memory(
            "InstructionMemory",
            &_PC,                    // Read address
            nullptr,            // Write data  (no writing to instruction memory)
            &_alwaysHi,              // ctrlMemRead (always 1)
            &_alwaysLo,              // ctrlMemWrite (unused)
            &_latchIFID.instruction, // Output read data
            memoryEndianness,        // Endianness
            instMemFileName          // File name
        );

    _dataMemory = new Memory(
            "DataMemory",
            &_latchEXMEM.aluResult,     // address
            &_latchEXMEM.regFileReadData2, // write data
            &_latchEXMEM.ctrlMEM.memRead,  // MemRead
            &_latchEXMEM.ctrlMEM.memWrite, // MemWrite
            &_latchMEMWB.dataMemReadData,  // Output read data
            memoryEndianness,              // Endianness
            dataMemFileName                // File name
        );

        _control = new Control(
            &_opcode,                       // Input opcode
            &_latchIDEX.ctrlEX.regDst
            &_latchIDEX.ctrlEX.aluOp,
            &_latchIDEX.ctrlEX.aluSrc,
            &_latchIDEX.ctrlMEM.branch,
            &_latchIDEX.ctrlMEM.memRead,
            &_latchIDEX.ctrlMEM.memWrite,
            &_latchIDEX.ctrlWB.memToReg,
            &_latchIDEX.ctrlWB.regWrite
        );

    _aluControl = new ALUControl(
            &_latchIDEX.ctrlEX.aluOp,   // ALU operation 2 bits
            &_aluControlInput,          // ALU function 6 bits
            &_aluControlOutput          // Result operation in 4 bits
        );

     _alu = new ALU(
            &_aluControlOutput,             // ALU operation (e.g., ADD, SUB, AND, OR, SLT, NOR)
            &_latchIDEX.regFileReadData1,   // $rs
            &_muxALUSrcOutput,              // ALU second input (either $rt or imm)
            &_latchEXMEM.aluResult,         // ALU result
            &_latchEXMEM.aluZero            // ALU zero flag
        );

     _muxRegDst = new MUX2<5>(
            "MUX_RegFileWriteRegister",
            &_latchIDEX.rt,
            &_latchIDEX.rd,
            &_latchIDEX.ctrlEX.regDst,
            &_latchEXMEM.regDstIdx
        );

      _muxALUSrc = new MUX2<32>(
            "MUX_ALUInput1",
            &_latchIDEX.regFileReadData2,  // $rt
            &_latchIDEX.signExtImmediate,  // sign-extended imm
            &_latchIDEX.ctrlEX.aluSrc,     // I-type or R-type
            &_muxALUSrcOutput              // ALU second input
        );

        // MUX for selecting between ALU result and data memory read data
        _muxMemToReg = new MUX2<32>(
            "MUX_RegFileWriteData",
            &_latchMEMWB.aluResult,
            &_latchMEMWB.dataMemReadData,
            &_latchMEMWB.ctrlWB.memToReg,
            &_muxMemToRegOutput
        );

         _muxPCSrc = new MUX2<32>(
            "MUX_PC",
            &_pcPlus4,
            &_latchEXMEM.branchTargetAddr,
            &_muxPCSrcSelect,
            &_PC
        );
    }

    virtual void advanceCycle() {
      _currCycle += 1;

    // [IF] stage
    _PC = _muxPCSrc->advanceCycle();

    // save to IF/ID Latch
    _latchIFID.pcPlus4 = _pcPlus4 + 4;
    _latchIFID.instruction = _instMemory->advanceCycle();

    // [ID] stage

    // fetch from IF/ID Latch
    unsigned long _instruction = _latchIFID.instruction.to_ulong();
    _pcPlus4 = _latchIFID.pcPlus4;

    // 32 bit instruction parsing
    _opcode = (_instruction >> 26) & 0x3F;       // 상위 6비트 (opcode)
    unsigned int _rs = (_instruction >> 21) & 0x1F;           // 다음 5비트 (rs)
    unsigned int _rt = (_instruction >> 16) & 0x1F;           // 다음 5비트 (rt)
    unsigned int _rd = (_instruction >> 11) & 0x1F;           // 다음 5비트 (rd)
    unsigned int _shamt = (_instruction >> 6) & 0x1F;         // 다음 5비트 (shamt)
    unsigned int _funct = _instruction & 0x3F;                // 하위 6비트 (funct)
    unsigned int _immediate = _instruction & 0xFFFF;          // 하위 16비트 (immediate)
    unsigned int _address = _instruction & 0x3FFFFFF;         // 하위 26비트 (address)

    _control->advanceCycle();

    // Read Register Data
    _latchIDEX.regFileReadData1 = _registerFile->readData(rs);
    _latchIDEX.regFileReadData2 = _registerFile->readData(rt);
    _registerFile->advanceCycle();
    _latchIDEX.regFileReadData1 = *_registerFile->readData1()

    // save to ID/EX latch
    _latchIDEX.pcPlus4 = _pcPlus4;
    _latchIDEX.rs = rs;
    _latchIDEX.rt = rt;
    _latchIDEX.rd = rd;
    _latchIDEX.ctrlEX.regDst = *_control->_oRegDst;
    _latchIDEX.ctrlEX.aluOp = *_control->_oALUOp;
    _latchIDEX.ctrlEX.aluSrc = *_control->_oALUSrc;
    _latchIDEX.ctrlMEM.branch = *_control->_oBranch;
    _latchIDEX.ctrlMEM.memRead = *_control->_oMemRead;
    _latchIDEX.ctrlMEM.memWrite = *_control->_oMemWrite;
    _latchIDEX.ctrlWB.memToReg = *_control->_oMemToReg;
    _latchIDEX.ctrlWB.regWrite = *_control->_oRegWrite;
    // [EXE] stage

    // fetch from ID/EX Latch
    unsigned long reg1Data = _latchIDEX.reg1Data;
    unsigned long reg2Data = _latchIDEX.reg2Data;
    unsigned int rs = _latchIDEX.rs;
    unsigned int rt = _latchIDEX.rt;
    unsigned int rd = _latchIDEX.rd;
    unsigned int shamt = _latchIDEX.shamt;
    unsigned int funct = _latchIDEX.funct;
    unsigned int immediate = _latchIDEX.immediate;
    unsigned long pcPlus4 = _latchIDEX.pcPlus4;
    unsigned int controlSignals = _latchIDEX.controlSignals;

    // ALU






    // EXE
    _alu->advanceCycle();

    // Update EX/MEM latch
    _latchEXMEM.ctrlWB = _latchIDEX.ctrlWB;
    _latchEXMEM.ctrlMEM = _latchIDEX.ctrlMEM;
    _latchEXMEM.branchTargetAddr = _branchTargetAddr;
    _latchEXMEM.aluZero = _aluZero;
    _latchEXMEM.aluResult = _aluResult;
    _latchEXMEM.regFileReadData2 = _latchIDEX.regFileReadData2;
    _latchEXMEM.regDstIdx = _regDstIdx;

    // MEM
    _dataMemory->advanceCycle();

    // Update MEM/WB latch
    _latchMEMWB.ctrlWB = _latchEXMEM.ctrlWB;
    _latchMEMWB.dataMemReadData = _dataMemory->readData();
    _latchMEMWB.aluResult = _latchEXMEM.aluResult;
    _latchMEMWB.regDstIdx = _latchEXMEM.regDstIdx;

    // WB Stage
    _muxRegWriteData->advanceCycle();
    if (_latchMEMWB.ctrlWB.regWrite.test(0)) {
        _registerFile->write(_latchMEMWB.regDstIdx, _muxMemToRegOutput);
    }

    // Update PC
    _muxPCInput0 = _PC.to_ulong() + 4; // PC + 4
    _muxPCInput1 = _PC.to_ulong() + 4 + (_signExtendOutput.to_ulong() << 2);
    _muxPCSelect = (_ctrlBranch.test(0) & _aluZero.test(0));
    _muxPC->advanceCycle();
    }

    ~PipelinedCPU() {
      delete _adderPCPlus4;
      delete _instMemory;
      delete _control;
      delete _registerFile;
      delete _signExtend;
      delete _adderBranchTargetAddr;
      delete _muxALUSrc;
      delete _aluControl;
      delete _alu;
      delete _muxRegDst;
      delete _muxPCSrc;
      delete _dataMemory;
      delete _muxMemToReg;
#ifdef ENABLE_DATA_FORWARDING
      delete _forwardingUnit;
      delete _muxForwardA;
      delete _muxForwardB;
#ifdef ENABLE_HAZARD_DETECTION
      delete _hazDetUnit;
#endif
#endif
    }

  private:

    // Cycle tracker
    std::uint64_t _currCycle = 0;

    // Always-1/0 wires
    const Wire<1> _alwaysHi = 1;
    const Wire<1> _alwaysLo = 0;

    // Components for the IF stage
    Register<32> _PC; // the Program Counter (PC) register
    Adder<32> *_adderPCPlus4; // the 32-bit adder in the IF stage
    Memory *_instMemory; // the instruction memory
    // Components for the ID stage
    Control *_control; // the Control unit
    RegisterFile *_registerFile; // the Register File
    SignExtend<16, 32> *_signExtend; // the sign-extend unit
    // Components for the EX stage
    Adder<32> *_adderBranchTargetAddr; // the 32-bit adder in the EX stage
    MUX2<32> *_muxALUSrc; // the MUX whose control signal is 'ALUSrc'
    ALUControl *_aluControl; // the ALU Control unit
    ALU *_alu; // the ALU
    MUX2<5> *_muxRegDst; // the MUX whose control signal is 'RegDst'
    // Components for the MEM stage
    MUX2<32> *_muxPCSrc; // the MUX whose control signal is 'PCSrc'
    Memory *_dataMemory; // the data memory
    // Components for the WB stage
    MUX2<32> *_muxMemToReg; // the MUX whose control signal is 'MemToReg'
#ifdef ENABLE_DATA_FORWARDING
    ForwardingUnit *_forwardingUnit; // the forwarding unit
    MUX3<32> *_muxForwardA; // the 3-to-1 MUX whose control signal is 'forwardA'
    MUX3<32> *_muxForwardB; // the 3-to-1 MUX whose control signal is 'forwardB'
#ifdef ENABLE_HAZARD_DETECTION
    HazardDetectionUnit *_hazDetUnit; // the Hazard Detection unit
#endif
#endif

    // Latches
    typedef struct {
      Register<1> regDst;
      Register<2> aluOp;
      Register<1> aluSrc;
    } ControlEX_t; // the control signals for the EX stage
    typedef struct {
      Register<1> branch;
      Register<1> memRead;
      Register<1> memWrite;
    } ControlMEM_t; // the control signals for the MEM stage
    typedef struct {
      Register<1> memToReg;
      Register<1> regWrite;
    } ControlWB_t; // the control signals for the WB stage
    struct {
      Register<32> pcPlus4; // PC+4
      Register<32> instruction; // 32-bit instruction
    } _latchIFID = {}; // the IF-ID latch
    struct {
      ControlWB_t ctrlWB; // the control signals for the WB stage
      ControlMEM_t ctrlMEM; // the control signals for the MEM stage
      ControlEX_t ctrlEX; // the control signals for the EX stage
      Register<32> pcPlus4; // PC+4
      Register<32> regFileReadData1; // 'ReadData1' from the register file
      Register<32> regFileReadData2; // 'ReadData2' from the register file
      Register<32> signExtImmediate; // the 32-bit sign-extended immediate value
#ifdef ENABLE_DATA_FORWARDING
      Register<5> rs; // the 5-bit 'rs' field
#endif
      Register<5> rt; // the 5-bit 'rt' field
      Register<5> rd; // the 5-bit 'rd' field
    } _latchIDEX = {}; // the ID-EX latch
    struct {
      ControlWB_t ctrlWB; // the control signals for the WB stage
      ControlMEM_t ctrlMEM; // the control signals for the MEM stage
      Register<32> branchTargetAddr; // the 32-bit branch target address
      Register<1> aluZero; // 'Zero' from the ALU
      Register<32> aluResult; // the 32-bit ALU output
      Register<32> regFileReadData2; // 'ReadData2' from the register file
      Register<5> regDstIdx; // the index of the destination register
    } _latchEXMEM = {}; // the EX-MEM latch
    struct {
      ControlWB_t ctrlWB; // the control signals for the WB stage
      Register<32> dataMemReadData; // the 32-bit data read from the data memory
      Register<32> aluResult; // the 32-bit ALU output
      Register<5> regDstIdx; // the index of the destination register
    } _latchMEMWB = {}; // the MEM-WB latch

    // Wires
    Wire<32> _adderPCPlus4Input1; // the second input to the adder in the IF stage (i.e., 4)
    Wire<32> _pcPlus4; // the output of the adder in the IF stage
    Wire<6> _opcode; // the input to the Control unit
    Wire<5> _regFileReadRegister1; // 'ReadRegister1' for the Register File
    Wire<5> _regFileReadRegister2; // 'ReadRegister2' for the Register File
    Wire<32> _muxMemToRegOutput; // the output of the MUX whose control signal is 'MemToReg'
    Wire<16> _signExtendInput; // the input to the sign-extend unit
    Wire<32> _adderBranchTargetAddrInput1; // the second input to the adder in the EX stage
    Wire<32> _muxALUSrcOutput; // the output of the MUX whose control signal is 'ALUSrc'
    Wire<6> _aluControlInput; // the input to the ALU Control unit (i.e., the 'funct' field)
    Wire<4> _aluControlOutput; // the output of the ALU Control unit
    Wire<1> _muxPCSrcSelect; // the control signal (a.k.a. selector) for the MUX whose control signal is 'PCSrc'
#ifdef ENABLE_DATA_FORWARDING
    Wire<2> _forwardA, _forwardB; // the outputs from the Forwarding unit
    Wire<32> _muxForwardAOutput; // the output of the 3-to-1 MUX whose control signal is 'forwardA'
    Wire<32> _muxForwardBOutput; // the output of the 3-to-1 MUX whose control signal is 'forwardB'
#ifdef ENABLE_HAZARD_DETECTION
    Wire<5> _hazDetIFIDRs, _hazDetIFIDRt; // the inputs to the Hazard Detection unit
    Wire<1> _hazDetPCWrite, _hazDetIFIDWrite, _hazDetIDEXCtrlWrite; // the outputs of the Hazard Detection unit
#endif
#endif

  public:

    void printPVS() {
      printf("==================== Cycle %lu ====================\n", _currCycle);
      printf("PC = 0x%08lx\n", _PC.to_ulong());
      printf("Registers:\n");
      _registerFile->printRegisters();
      printf("Data Memory:\n");
      _dataMemory->printMemory();
      printf("Instruction Memory:\n");
      _instMemory->printMemory();
      printf("Latches:\n");
      printf("  IF-ID Latch:\n");
      printf("    pcPlus4          = 0x%08lx\n", _latchIFID.pcPlus4.to_ulong());
      printf("    instruction      = 0x%08lx\n", _latchIFID.instruction.to_ulong());
      printf("  ID-EX Latch:\n");
      printf("    ctrlWBMemToReg   = 0b%s\n", _latchIDEX.ctrlWB.memToReg.to_string().c_str());
      printf("    ctrlWBRegWrite   = 0b%s\n", _latchIDEX.ctrlWB.regWrite.to_string().c_str());
      printf("    ctrlMEMBranch    = 0b%s\n", _latchIDEX.ctrlMEM.branch.to_string().c_str());
      printf("    ctrlMEMMemRead   = 0b%s\n", _latchIDEX.ctrlMEM.memRead.to_string().c_str());
      printf("    ctrlMEMMemWrite  = 0b%s\n", _latchIDEX.ctrlMEM.memWrite.to_string().c_str());
      printf("    ctrlEXRegDst     = 0b%s\n", _latchIDEX.ctrlEX.regDst.to_string().c_str());
      printf("    ctrlEXALUOp      = 0b%s\n", _latchIDEX.ctrlEX.aluOp.to_string().c_str());
      printf("    ctrlEXALUSrc     = 0b%s\n", _latchIDEX.ctrlEX.aluSrc.to_string().c_str());
      printf("    pcPlus4          = 0x%08lx\n", _latchIDEX.pcPlus4.to_ulong());
      printf("    regFileReadData1 = 0x%08lx\n", _latchIDEX.regFileReadData1.to_ulong());
      printf("    regFileReadData2 = 0x%08lx\n", _latchIDEX.regFileReadData2.to_ulong());
      printf("    signExtImmediate = 0x%08lx\n", _latchIDEX.signExtImmediate.to_ulong());
#ifdef ENABLE_DATA_FORWARDING
      printf("    rs               = 0b%s\n", _latchIDEX.rs.to_string().c_str());
#endif
      printf("    rt               = 0b%s\n", _latchIDEX.rt.to_string().c_str());
      printf("    rd               = 0b%s\n", _latchIDEX.rd.to_string().c_str());
      printf("  EX-MEM Latch:\n"); 
      printf("    ctrlWBMemToReg   = 0b%s\n", _latchEXMEM.ctrlWB.memToReg.to_string().c_str());
      printf("    ctrlWBRegWrite   = 0b%s\n", _latchEXMEM.ctrlWB.regWrite.to_string().c_str());
      printf("    ctrlMEMBranch    = 0b%s\n", _latchEXMEM.ctrlMEM.branch.to_string().c_str());
      printf("    ctrlMEMMemRead   = 0b%s\n", _latchEXMEM.ctrlMEM.memRead.to_string().c_str());
      printf("    ctrlMEMMemWrite  = 0b%s\n", _latchEXMEM.ctrlMEM.memWrite.to_string().c_str());
      printf("    branchTargetAddr = 0x%08lx\n", _latchEXMEM.branchTargetAddr.to_ulong());
      printf("    aluZero          = 0b%s\n", _latchEXMEM.aluZero.to_string().c_str());
      printf("    aluResult        = 0x%08lx\n", _latchEXMEM.aluResult.to_ulong());
      printf("    regFileReadData2 = 0x%08lx\n", _latchEXMEM.regFileReadData2.to_ulong());
      printf("    regDstIdx        = 0b%s\n", _latchEXMEM.regDstIdx.to_string().c_str());
      printf("  MEM-WB Latch:\n"); 
      printf("    ctrlWBMemToReg   = 0b%s\n", _latchMEMWB.ctrlWB.memToReg.to_string().c_str());
      printf("    ctrlWBRegWrite   = 0b%s\n", _latchMEMWB.ctrlWB.regWrite.to_string().c_str());
      printf("    dataMemReadData  = 0x%08lx\n", _latchMEMWB.dataMemReadData.to_ulong());
      printf("    aluResult        = 0x%08lx\n", _latchMEMWB.aluResult.to_ulong());
      printf("    regDstIdx        = 0b%s\n", _latchMEMWB.regDstIdx.to_string().c_str());
    }

};

#endif


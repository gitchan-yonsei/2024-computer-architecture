#include "ALU.h"
#include "ALUControl.h"
#include "Control.h"
#include "RegisterFile.h"
#include "Memory.h"
#include "SingleCycleCPU.h"

void testALU() {
  Wire<4> aluControl;
  Wire<32> input0, input1, output;
  Wire<1> zero;
  ALU *alu = new ALU(&aluControl, &input0, &input1, &output, &zero);
  fprintf(stderr, "INFO: testing ALU...\n");

  fprintf(stderr, "INFO: 0x03478bcf AND 0x00000000 --> output = 0x00000000, zero = 0x1\n");
  input0 = 0x03478bcf;
  input1 = 0x00000000;
  aluControl = 0x0;
  printWire("input0", input0); printWire("input1", input1); printWire("aluControl", aluControl);
  alu->advanceCycle();
  printWire("output", output); printWire("zero", zero);

  fprintf(stderr, "INFO: 0x03478bcf AND 0x12569ade --> output = 0x02468ace, zero = 0x0\n");
  input1 = 0x12569ade;
  aluControl = 0x0;
  printWire("input0", input0); printWire("input1", input1); printWire("aluControl", aluControl);
  alu->advanceCycle();
  printWire("output", output); printWire("zero", zero);

  fprintf(stderr, "INFO: 0x03478bcf OR  0x12569ade --> output = 0x13579bdf, zero = 0x0\n");
  aluControl = 0x1;
  printWire("input0", input0); printWire("input1", input1); printWire("aluControl", aluControl);
  alu->advanceCycle();
  printWire("output", output); printWire("zero", zero);

  fprintf(stderr, "INFO: 0x03478bcf add 0x12569ade --> output = 0x159e26ad, zero = 0x0\n");
  aluControl = 0x2;
  printWire("input0", input0); printWire("input1", input1); printWire("aluControl", aluControl);
  alu->advanceCycle();
  printWire("output", output); printWire("zero", zero);

  fprintf(stderr, "INFO: 0x03478bcf sub 0x12569ade --> output = 0xf0f0f0f1, zero = 0x0\n");
  aluControl = 0x6;
  printWire("input0", input0); printWire("input1", input1); printWire("aluControl", aluControl);
  alu->advanceCycle();
  printWire("output", output); printWire("zero", zero);

  fprintf(stderr, "INFO: 0x03478bcf slt 0x12569ade --> output = 0x00000001, zero = 0x0\n");
  aluControl = 0x7;
  printWire("input0", input0); printWire("input1", input1); printWire("aluControl", aluControl);
  alu->advanceCycle();
  printWire("output", output); printWire("zero", zero);

  fprintf(stderr, "INFO: 0x03478bcf NOR 0x12569ade --> output = 0xeca86420, zero = 0x0\n");
  aluControl = 0xc;
  printWire("input0", input0); printWire("input1", input1); printWire("aluControl", aluControl);
  alu->advanceCycle();
  printWire("output", output); printWire("zero", zero);

  delete alu;
}

void testALUControl() {
  Wire<2> aluOp;
  Wire<6> funct;
  Wire<4> operation;
  ALUControl *aluControl = new ALUControl(&aluOp, &funct, &operation);
  fprintf(stderr, "INFO: testing ALUControl...\n");

  fprintf(stderr, "INFO: aluOp = 0x0 --> operation = 0x2\n");
  aluOp = 0;
  printWire("aluOp", aluOp); printWire("funct", funct);
  aluControl->advanceCycle();
  printWire("operation", operation);

  fprintf(stderr, "INFO: aluOp = 0x1 --> operation = 0x6\n");
  aluOp = 1;
  printWire("aluOp", aluOp); printWire("funct", funct);
  aluControl->advanceCycle();
  printWire("operation", operation);

  fprintf(stderr, "INFO: aluOp = 0x2, funct = 0x30 --> operation = 0x2\n");
  aluOp = 2;
  funct = 0x30;
  printWire("aluOp", aluOp); printWire("funct", funct);
  aluControl->advanceCycle();
  printWire("operation", operation);

  fprintf(stderr, "INFO: aluOp = 0x2, funct = 0x22 --> operation = 0x6\n");
  aluOp = 2;
  funct = 0x22;
  printWire("aluOp", aluOp); printWire("funct", funct);
  aluControl->advanceCycle();
  printWire("operation", operation);

  fprintf(stderr, "INFO: aluOp = 0x2, funct = 0x14 --> operation = 0x0\n");
  aluOp = 2;
  funct = 0x14;
  printWire("aluOp", aluOp); printWire("funct", funct);
  aluControl->advanceCycle();
  printWire("operation", operation);

  fprintf(stderr, "INFO: aluOp = 0x2, funct = 0x05 --> operation = 0x1\n");
  aluOp = 2;
  funct = 0x05;
  printWire("aluOp", aluOp); printWire("funct", funct);
  aluControl->advanceCycle();
  printWire("operation", operation);

  fprintf(stderr, "INFO: aluOp = 0x2, funct = 0x1a --> operation = 0x7\n");
  aluOp = 2;
  funct = 0x1a;
  printWire("aluOp", aluOp); printWire("funct", funct);
  aluControl->advanceCycle();
  printWire("operation", operation);

  delete aluControl;
}

void testControl() {
  Wire<6> opcode;
  Wire<1> regDst, aluSrc, memToReg, regWrite, memRead, memWrite, branch;
  Wire<2> aluOp;
  Control *control = new Control(&opcode, &regDst, &aluSrc, &memToReg,
                                 &regWrite, &memRead, &memWrite, &branch,
                                 &aluOp);

  fprintf(stderr, "INFO: testing Control...\n");

  fprintf(stderr, "INFO: opcode = 0x00 --> regDst = 1, aluSrc = 0, memToReg = 0,"
                  " regWrite = 1, memRead = 0, memWrite = 0, branch = 0, aluOp = 0x2\n");
  opcode = 0x0;
  printWire("opcode", opcode);
  control->advanceCycle();
  printWire("regDst", regDst); printWire("aluSrc", aluSrc); printWire("memToReg", memToReg);
  printWire("regWrite", regWrite); printWire("memRead", memRead); printWire("memWrite", memWrite);
  printWire("branch", branch); printWire("aluOp", aluOp);

  fprintf(stderr, "INFO: opcode = 0x23 --> regDst = 0, aluSrc = 1, memToReg = 1,"
                  " regWrite = 1, memRead = 1, memWrite = 0, branch = 0, aluOp = 0x0\n");
  opcode = 0x23;
  printWire("opcode", opcode);
  control->advanceCycle();
  printWire("regDst", regDst); printWire("aluSrc", aluSrc); printWire("memToReg", memToReg);
  printWire("regWrite", regWrite); printWire("memRead", memRead); printWire("memWrite", memWrite);
  printWire("branch", branch); printWire("aluOp", aluOp);

  fprintf(stderr, "INFO: opcode = 0x2b --> regDst = X, aluSrc = 1, memToReg = X,"
                  " regWrite = 0, memRead = 0, memWrite = 1, branch = 0, aluOp = 0x0\n");
  opcode = 0x2b;
  printWire("opcode", opcode);
  control->advanceCycle();
  printWire("regDst", regDst); printWire("aluSrc", aluSrc); printWire("memToReg", memToReg);
  printWire("regWrite", regWrite); printWire("memRead", memRead); printWire("memWrite", memWrite);
  printWire("branch", branch); printWire("aluOp", aluOp);

  fprintf(stderr, "INFO: opcode = 0x04 --> regDst = X, aluSrc = 0, memToReg = X,"
                  " regWrite = 0, memRead = 0, memWrite = 0, branch = 1, aluOp = 0x1\n");
  opcode = 0x04;
  printWire("opcode", opcode);
  control->advanceCycle();
  printWire("regDst", regDst); printWire("aluSrc", aluSrc); printWire("memToReg", memToReg);
  printWire("regWrite", regWrite); printWire("memRead", memRead); printWire("memWrite", memWrite);
  printWire("branch", branch); printWire("aluOp", aluOp);

  delete control;
}

void testRegisterFile() {
  Wire<5> readRegister1 = 0, readRegister2 = 0, writeRegister = 0;
  Wire<32> writeData = 0, readData1 = 0, readData2 = 0;
  Wire<1> regWrite = 0;
  RegisterFile *regFile = new RegisterFile(
    &readRegister1, &readRegister2, &writeRegister, &writeData,
    &regWrite,
    &readData1, &readData2);

  fprintf(stderr, "INFO: testing RegisterFile...\n");

  fprintf(stderr, "INFO: regWrite = 1, writeRegister = 0x0, writeData = 0x01234567\n");
  regWrite = 1;
  writeRegister = 0;
  writeData = 0x01234567;
  printWire("regWrite", regWrite); printWire("writeRegister", writeRegister); printWire("writeData", writeData);
  regFile->advanceCycle();
  fprintf(stderr, "INFO: current register values:\n");
  regFile->printRegisters();
  fprintf(stderr, "INFO: regWrite = 1, writeRegister = 0x1, writeData = 0x89abcdef\n");
  regWrite = 1;
  writeRegister = 1;
  writeData = 0x89abcdef;
  printWire("regWrite", regWrite); printWire("writeRegister", writeRegister); printWire("writeData", writeData);
  regFile->advanceCycle();
  fprintf(stderr, "INFO: current register values:\n");
  regFile->printRegisters();
  fprintf(stderr, "INFO: regWrite = 1, writeRegister = 0x2, writeData = 0x02468ace\n");
  regWrite = 1;
  writeRegister = 2;
  writeData = 0x02468ace;
  printWire("regWrite", regWrite); printWire("writeRegister", writeRegister); printWire("writeData", writeData);
  regFile->advanceCycle();
  fprintf(stderr, "INFO: current register values:\n");
  regFile->printRegisters();
  fprintf(stderr, "INFO: regWrite = 1, writeRegister = 0x3, writeData = 0x13579bdf\n");
  regWrite = 1;
  writeRegister = 3;
  writeData = 0x13579bdf;
  printWire("regWrite", regWrite); printWire("writeRegister", writeRegister); printWire("writeData", writeData);
  regFile->advanceCycle();
  fprintf(stderr, "INFO: current register values:\n");
  regFile->printRegisters();
  fprintf(stderr, "INFO: regWrite = 0, readRegister1 = 0x0, readRegister2 = 0x1 --> readData1 = 0x00000000, readData2 = 0x89abcdef\n");
  regWrite = 0;
  readRegister1 = 0; readRegister2 = 1;
  printWire("regWrite", regWrite); printWire("readRegister1", readRegister1); printWire("readRegister2", readRegister2);
  regFile->advanceCycle();
  printWire("readData1", readData1); printWire("readData2", readData2);
  fprintf(stderr, "INFO: regWrite = 0, readRegister1 = 0x2, readRegister2 = 0x3 --> readData1 = 0x02468ace, readData2 = 0x13579bdf\n");
  regWrite = 0;
  readRegister1 = 2; readRegister2 = 3;
  printWire("regWrite", regWrite); printWire("readRegister1", readRegister1); printWire("readRegister2", readRegister2);
  regFile->advanceCycle();
  printWire("readData1", readData1); printWire("readData2", readData2);
  fprintf(stderr, "INFO: regWrite = 0, readRegister1 = 0x4, readRegister2 = 0x2 --> readData1 = 0x00000000, readData2 = 0x02468ace\n");
  regWrite = 0;
  readRegister1 = 4; readRegister2 = 2;
  printWire("regWrite", regWrite); printWire("readRegister1", readRegister1); printWire("readRegister2", readRegister2);
  regFile->advanceCycle();
  printWire("readData1", readData1); printWire("readData2", readData2);

  delete regFile;
}

void testMemory() {
  Wire<32> address, writeData, readData;
  Wire<1> memRead, memWrite;
  Memory *memoryLE, *memoryBE;

  memoryLE = new Memory("MemoryLE",
    &address, &writeData, &memRead, &memWrite, &readData,
    Memory::LittleEndian);
  memoryBE = new Memory("MemoryBE",
    &address, &writeData, &memRead, &memWrite, &readData,
    Memory::BigEndian);

  fprintf(stderr, "INFO: testing Memory...\n");

  fprintf(stderr, "INFO: memRead = 0, memWrite = 1, address = 0x1000, writeData = 0x01234567\n");
  memRead = 0; memWrite = 1; address = 0x1000;
  writeData = 0x01234567;
  printWire("memRead", memRead); printWire("memWrite", memWrite); printWire("address", address);
  printWire("writeData", writeData);
  memoryLE->advanceCycle();
  fprintf(stderr, "INFO: current data stored in Memory (Little Endian):\n");
  memoryLE->printMemory();
  memoryBE->advanceCycle();
  fprintf(stderr, "INFO: current data stored in Memory (Big Endian):\n");
  memoryBE->printMemory();

  fprintf(stderr, "INFO: memRead = 0, memWrite = 1, address = 0x1234, writeData = 0xDEADBEEF\n");
  memRead = 0; memWrite = 1; address = 0x1234;
  writeData = 0xDEADBEEF;
  printWire("memRead", memRead); printWire("memWrite", memWrite); printWire("address", address);
  printWire("writeData", writeData);
  memoryLE->advanceCycle();
  fprintf(stderr, "INFO: current data stored in Memory (Little Endian):\n");
  memoryLE->printMemory();
  memoryBE->advanceCycle();
  fprintf(stderr, "INFO: current data stored in Memory (Big Endian):\n");
  memoryBE->printMemory();

  fprintf(stderr, "INFO: memRead = 0, memWrite = 1, address = 0x1500, writeData = 0x89abcdef\n");
  memRead = 0; memWrite = 1; address = 0x1500;
  writeData = 0x89ABCDEF;
  printWire("memRead", memRead); printWire("memWrite", memWrite); printWire("address", address);
  printWire("writeData", writeData);
  memoryLE->advanceCycle();
  fprintf(stderr, "INFO: current data stored in Memory (Little Endian):\n");
  memoryLE->printMemory();
  memoryBE->advanceCycle();
  fprintf(stderr, "INFO: current data stored in Memory (Big Endian):\n");
  memoryBE->printMemory();

  fprintf(stderr, "INFO: memRead = 1, memWrite = 0, address = 0x1234 --> readData = 0xDEADBEEF\n");
  memRead = 1; memWrite = 0; address = 0x1234;
  printWire("memRead", memRead); printWire("memWrite", memWrite); printWire("address", address);
  readData.reset();
  memoryLE->advanceCycle();
  printWire("readData (from MemoryLE)", readData);
  readData.reset();
  memoryBE->advanceCycle();
  printWire("readData (from MemoryBE)", readData);

  fprintf(stderr, "INFO: memRead = 1, memWrite = 0, address = 0x1000 --> readData = 0x01234567\n");
  memRead = 1; memWrite = 0; address = 0x1000;
  printWire("memRead", memRead); printWire("memWrite", memWrite); printWire("address", address);
  readData.reset();
  memoryLE->advanceCycle();
  printWire("readData (from MemoryLE)", readData);
  readData.reset();
  memoryBE->advanceCycle();
  printWire("readData (from MemoryBE)", readData);

  fprintf(stderr, "INFO: memRead = 1, memWrite = 0, address = 0x1500 --> readData = 0x89abcdef\n");
  memRead = 1; memWrite = 0; address = 0x1500;
  printWire("memRead", memRead); printWire("memWrite", memWrite); printWire("address", address);
  readData.reset();
  memoryLE->advanceCycle();
  printWire("readData (from MemoryLE)", readData);
  readData.reset();
  memoryBE->advanceCycle();
  printWire("readData (from MemoryBE)", readData);

  delete memoryLE;
  delete memoryBE;
}

void testSingleCycleCPU(int argc, char **argv) {
  if (argc != 6) {
    fprintf(stderr, "[%s] Usage: %s initialPC regFileName instMemFileName dataMemFileName numCycles\n", __func__, argv[0]);
    fflush(stdout);
  } else {
    const std::uint32_t initialPC = (std::uint32_t)atoll(argv[1]);
    const char *regFileName = argv[2];
    const char *instMemFileName = argv[3];
    const char *dataMemFileName = argv[4];
    const std::uint64_t numCycles = (std::uint64_t)atoll(argv[5]);

    SingleCycleCPU *cpu = new SingleCycleCPU("SingleCycleCPU", initialPC, regFileName, instMemFileName, dataMemFileName);

    cpu->printPVS();
    for (size_t i = 0; i < numCycles; i++) {
      cpu->advanceCycle();
      cpu->printPVS();
    }

    delete cpu;
  }
}

int main(int argc, char **argv) {
  if (argc == 1) {
    testALU();
    testALUControl();
    testControl();
    testRegisterFile();
    testMemory();
  } else {
    testSingleCycleCPU(argc, argv);
  }

  return 0;
}


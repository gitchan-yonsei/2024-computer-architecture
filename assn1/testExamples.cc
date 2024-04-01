#include "UIntAdder.h"
#include "Latch.h"

void testUIntAdder() {
  fprintf(stderr, "\n\n<<<<<<<<<<<<<<<<<<<< testing %s >>>>>>>>>>>>>>>>>>>>\n",
          "UIntAdder");

  Wire<32> adderInput0, adderInput1;
  Wire<33> adderOutput;
  UIntAdder<32> *adder = new UIntAdder<32>("UInt32Adder", &adderInput0, &adderInput1, &adderOutput);
  adder->printName();

  fprintf(stderr, "##### Cycle 1: 0x01010101 + 0x10101010 #####\n");
  adderInput0 = (unsigned long long)0x01010101;
  adderInput1 = (unsigned long long)0x10101010;
  printWire("adderInput0", adderInput0);
  printWire("adderInput1", adderInput1);
  adder->advanceCycle();
  printWire("adderOutput", adderOutput);

  fprintf(stderr, "##### Cycle 2: 0x01010101 + 0x10101010 #####\n");
  adderInput0 = (unsigned long long)0x01010101;
  adderInput1 = (unsigned long long)0x10101010;
  printWire("adderInput0", adderInput0);
  printWire("adderInput1", adderInput1);
  adder->advanceCycle();
  printWire("adderOutput", adderOutput);

  fprintf(stderr, "##### Cycle 3: 0xFFFFFFFF + 0x00000001 #####\n");
  adderInput0 = (unsigned long long)0xFFFFFFFF;
  adderInput1 = (unsigned long long)0x00000001;
  printWire("adderInput0", adderInput0);
  printWire("adderInput1", adderInput1);
  adder->advanceCycle();
  printWire("adderOutput", adderOutput);

  fprintf(stderr, "##### Cycle 4: 0xFFFF0000 + 0x0000FFFF #####\n");
  adderInput0 = (unsigned long long)0xFFFF0000;
  adderInput1 = (unsigned long long)0x0000FFFF;
  printWire("adderInput0", adderInput0);
  printWire("adderInput1", adderInput1);
  adder->advanceCycle();
  printWire("adderOutput", adderOutput);

  delete adder;
}

void testLatch() {
  fprintf(stderr, "\n\n<<<<<<<<<<<<<<<<<<<< testing %s >>>>>>>>>>>>>>>>>>>>\n",
          "Latch");

  Wire<1> writeEnable;
  Wire<32> input, output;
  Latch<32> *latch = new Latch<32>("Latch32", &writeEnable, &input, &output);

  fprintf(stderr, "##### Cycle 1: writeEnable = 1, input = 0x12341234 #####\n");
  writeEnable = (unsigned long long)1;
  input = (unsigned long long)0x12341234;
  printWire("input", input);
  latch->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 2: writeEnable = 0, input = 0xABCDFFFF #####\n");
  writeEnable = (unsigned long long)0;
  input = (unsigned long long)0xABCDFFFF;
  printWire("input", input);
  latch->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 3: writeEnable = 1, input = 0xABCDFFFF #####\n");
  writeEnable = (unsigned long long)1;
  input = (unsigned long long)0xABCDFFFF;
  printWire("input", input);
  latch->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 4: writeEnable = 1, input = 0xDEADBEEF #####\n");
  writeEnable = (unsigned long long)1;
  input = (unsigned long long)0xDEADBEEF;
  printWire("input", input);
  latch->advanceCycle();
  printWire("output", output);

  delete latch;

}

int main(int argc, char **argv) {
  testUIntAdder();
  testLatch();

  return 0;
}


#include "assn1-stu.h"

void testNANDGate() {
  fprintf(stderr, "\n\n<<<<<<<<<<<<<<<<<<<< testing %s >>>>>>>>>>>>>>>>>>>>\n",
          "NANDGate");

  Wire<8> input0, input1, output;
  NANDGate<8> *gate = new NANDGate<8>("NANDGate8", &input0, &input1, &output);

  fprintf(stderr, "##### Cycle 1: input0 = 0xF0, input1 = 0x0F --> output = 0xFF\n");
  input0 = (unsigned long long)0xF0;
  input1 = (unsigned long long)0x0F;
  printWire("input0", input0);
  printWire("input1", input1);
  gate->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 2: input0 = 0xFF, input1 = 0x00 --> output = 0xFF\n");
  input0 = (unsigned long long)0xFF;
  input1 = (unsigned long long)0x00;
  printWire("input0", input0);
  printWire("input1", input1);
  gate->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 3: input0 = 0xFF, input1 = 0xFF --> output = 0x00\n");
  input0 = (unsigned long long)0xFF;
  input1 = (unsigned long long)0xFF;
  printWire("input0", input0);
  printWire("input1", input1);
  gate->advanceCycle();
  printWire("output", output);

  delete gate;
}

void testSignedAdder() {
  fprintf(stderr, "\n\n<<<<<<<<<<<<<<<<<<<< testing %s >>>>>>>>>>>>>>>>>>>>\n",
          "SignedAdder");

  Wire<8> input0, input1, output;
  SignedAdder<8> *adder = new SignedAdder<8>("Int8Adder", &input0, &input1, &output);

  fprintf(stderr, "##### Cycle 1: input0 = 0b00001111, input1 = 0b00000001 --> output = 0b00010000\n");
  input0 = (unsigned long long)0x0F; printWire("input0", input0);
  input1 = (unsigned long long)0x01; printWire("input1", input1);
  adder->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 2: input0 = 0b00001111, input1 = 0b11111000 --> output = 0b00000111\n");
  input0 = (unsigned long long)0x0F; printWire("input0", input0);
  input1 = (unsigned long long)0xF8; printWire("input1", input1);
  adder->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 3: input0 = 0b00001000, input1 = 0b11110111 --> output = 0b11111111\n");
  input0 = (unsigned long long)0x08; printWire("input0", input0);
  input1 = (unsigned long long)0xF7; printWire("input1", input1);
  adder->advanceCycle();
  printWire("output", output);

  delete adder;
}

void testSignedSubtractor() {
  fprintf(stderr, "\n\n<<<<<<<<<<<<<<<<<<<< testing %s >>>>>>>>>>>>>>>>>>>>\n",
          "SignedSubtractor");

  Wire<8> input0, input1, output;
  SignedSubtractor<8> *subtractor = new SignedSubtractor<8>("Int8Subtractor", &input0, &input1, &output);

  fprintf(stderr, "##### Cycle 1: input0 = 0b00001111, input1 = 0b11111111 --> output = 0b00010000\n");
  input0 = (unsigned long long)0x0F; printWire("input0", input0);
  input1 = (unsigned long long)0xFF; printWire("input1", input1);
  subtractor->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 2: input0 = 0b00001111, input1 = 0b00001000 --> output = 0b00000111\n");
  input0 = (unsigned long long)0x0F; printWire("input0", input0);
  input1 = (unsigned long long)0x08; printWire("input1", input1);
  subtractor->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 3: input0 = 0b00001000, input1 = 0b00001001 --> output = 0b11111111\n");
  input0 = (unsigned long long)0x08; printWire("input0", input0);
  input1 = (unsigned long long)0x09; printWire("input1", input1);
  subtractor->advanceCycle();
  printWire("output", output);

  delete subtractor;
}

void testLogicalUnit() {
  fprintf(stderr, "\n\n<<<<<<<<<<<<<<<<<<<< testing %s >>>>>>>>>>>>>>>>>>>>\n",
          "LogicalUnit");

  Wire<3> operation;
  Wire<32> input0, input1, output;
  LogicalUnit<32> *lu = new LogicalUnit<32>("LU", &operation, &input0, &input1, &output);

  fprintf(stderr, "##### Cycle 1: operation = 0b000, input0 = 0xFF00FF00, input1 = 0xF0F0F0F0"
                  " --> output = 0x00000000\n");
  operation = (unsigned long long)0; printWire("operation", operation);
  input0 = (unsigned long long)0xFF00FF00; printWire("input0", input0);
  input1 = (unsigned long long)0xF0F0F0F0; printWire("input1", input1);
  lu->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 2: operation = 0b001, input0 = 0xFF00FF00, input1 = 0xF0F0F0F0"
                  " --> output = 0xFFF0FFF0\n");
  operation = (unsigned long long)1; printWire("operation", operation);
  input0 = (unsigned long long)0xFF00FF00; printWire("input0", input0);
  input1 = (unsigned long long)0xF0F0F0F0; printWire("input1", input1);
  lu->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 3: operation = 0b010, input0 = 0xFF00FF00, input1 = 0xF0F0F0F0"
                  " --> output = 0x0FF00FF0\n");
  operation = (unsigned long long)2; printWire("operation", operation);
  input0 = (unsigned long long)0xFF00FF00; printWire("input0", input0);
  input1 = (unsigned long long)0xF0F0F0F0; printWire("input1", input1);
  lu->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 4: operation = 0b011, input0 = 0xFF00FF00, input1 = 0xF0F0F0F0"
                  " --> output = 0x00FF00FF\n");
  operation = (unsigned long long)3; printWire("operation", operation);
  input0 = (unsigned long long)0xFF00FF00; printWire("input0", input0);
  input1 = (unsigned long long)0xF0F0F0F0; printWire("input1", input1);
  lu->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 5: operation = 0b100, input0 = 0xFF00FF00, input1 = 0xF0F0F0F0"
                  " --> output = 0x0FFF0FFF\n");
  operation = (unsigned long long)4; printWire("operation", operation);
  input0 = (unsigned long long)0xFF00FF00; printWire("input0", input0);
  input1 = (unsigned long long)0xF0F0F0F0; printWire("input1", input1);
  lu->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 6: operation = 0b101, input0 = 0xFF00FF00, input1 = 0xF0F0F0F0"
                  " --> output = 0x000F000F\n");
  operation = (unsigned long long)5; printWire("operation", operation);
  input0 = (unsigned long long)0xFF00FF00; printWire("input0", input0);
  input1 = (unsigned long long)0xF0F0F0F0; printWire("input1", input1);
  lu->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 7: operation = 0b110, input0 = 0xFF00FF00, input1 = 0xF0F0F0F0"
                  " --> output = 0xF00FF00F\n");
  operation = (unsigned long long)6; printWire("operation", operation);
  input0 = (unsigned long long)0xFF00FF00; printWire("input0", input0);
  input1 = (unsigned long long)0xF0F0F0F0; printWire("input1", input1);
  lu->advanceCycle();
  printWire("output", output);

  delete lu;
}

void testHashTable() {
  fprintf(stderr, "\n\n<<<<<<<<<<<<<<<<<<<< testing %s >>>>>>>>>>>>>>>>>>>>\n",
          "HashTable");

  Wire<2> index;
  Wire<1> isWrite;
  Wire<32> input, output;
  HashTable<32, 2> *hashTable = new HashTable<32, 2>("HashTable4x32", &index, &isWrite, &input, &output);

  fprintf(stderr, "##### Cycle 1: index = 0b00, isWrite = 1, input = 0x01234567 --> output = 0x00000000\n");
  index = (unsigned long long)0; printWire("index", index);
  isWrite = (unsigned long long)1; printWire("isWrite", isWrite);
  input = (unsigned long long)0x01234567; printWire("input", input);
  hashTable->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 2: index = 0b10, isWrite = 1, input = 0x89ABCDEF --> output = 0x00000000\n");
  index = (unsigned long long)2; printWire("index", index);
  isWrite = (unsigned long long)1; printWire("isWrite", isWrite);
  input = (unsigned long long)0x89ABCDEF; printWire("input", input);
  hashTable->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 3: index = 0b01, isWrite = 1, input = 0x0123CDEF --> output = 0x00000000\n");
  index = (unsigned long long)1; printWire("index", index);
  isWrite = (unsigned long long)1; printWire("isWrite", isWrite);
  input = (unsigned long long)0x0123CDEF; printWire("input", input);
  hashTable->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 4: index = 0b11, isWrite = 1, input = 0x89AB4567 --> output = 0x00000000\n");
  index = (unsigned long long)3; printWire("index", index);
  isWrite = (unsigned long long)1; printWire("isWrite", isWrite);
  input = (unsigned long long)0x89AB4567; printWire("input", input);
  hashTable->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 5: index = 0b10, isWrite = 0, input = 0xDEADBEEF --> output = 0x89ABCDEF\n");
  index = (unsigned long long)2; printWire("index", index);
  isWrite = (unsigned long long)0; printWire("isWrite", isWrite);
  input = (unsigned long long)0xDEADBEEF; printWire("input", input);
  hashTable->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 6: index = 0b11, isWrite = 0, input = 0xDEADBEEF --> output = 0x89AB4567\n");
  index = (unsigned long long)3; printWire("index", index);
  isWrite = (unsigned long long)0; printWire("isWrite", isWrite);
  input = (unsigned long long)0xDEADBEEF; printWire("input", input);
  hashTable->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 7: index = 0b01, isWrite = 0, input = 0xDEADBEEF --> output = 0x0123CDEF\n");
  index = (unsigned long long)1; printWire("index", index);
  isWrite = (unsigned long long)0; printWire("isWrite", isWrite);
  input = (unsigned long long)0xDEADBEEF; printWire("input", input);
  hashTable->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 8: index = 0b00, isWrite = 0, input = 0xDEADBEEF --> output = 0x01234567\n");
  index = (unsigned long long)0; printWire("index", index);
  isWrite = (unsigned long long)0; printWire("isWrite", isWrite);
  input = (unsigned long long)0xDEADBEEF; printWire("input", input);
  hashTable->advanceCycle();
  printWire("output", output);


  delete hashTable;
}

void testDelayQueue() {
  fprintf(stderr, "\n\n<<<<<<<<<<<<<<<<<<<< testing %s >>>>>>>>>>>>>>>>>>>>\n",
          "DelayQueue");

  Wire<1> isPush;
  Wire<32> input, output;
  DelayQueue<32, 4> *delayQueue = new DelayQueue<32, 4>("DelayQueue4x32", &isPush, &input, &output);

  fprintf(stderr, "##### Cycle 1: isPush = 1, input = 0x01234567 --> output = 0x00000000\n");
  isPush = (unsigned long long)1; printWire("isPush", isPush);
  input = (unsigned long long)0x01234567; printWire("input", input);
  delayQueue->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 2: isPush = 1, input = 0x89ABCDEF --> output = 0x00000000\n");
  isPush = (unsigned long long)1; printWire("isPush", isPush);
  input = (unsigned long long)0x89ABCDEF; printWire("input", input);
  delayQueue->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 3: isPush = 1, input = 0x0123CDEF --> output = 0x00000000\n");
  isPush = (unsigned long long)1; printWire("isPush", isPush);
  input = (unsigned long long)0x0123CDEF; printWire("input", input);
  delayQueue->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 4: isPush = 0, input = 0x89AB0123 --> output = 0x00000000\n");
  isPush = (unsigned long long)0; printWire("isPush", isPush);
  input = (unsigned long long)0x89AB0123; printWire("input", input);
  delayQueue->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 5: isPush = 1, input = 0x89AB0123 --> output = 0x01234567\n");
  isPush = (unsigned long long)1; printWire("isPush", isPush);
  input = (unsigned long long)0x89AB0123; printWire("input", input);
  delayQueue->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 6: isPush = 0, input = 0xDEADBEEF --> output = 0x89ABCDEF\n");
  isPush = (unsigned long long)0; printWire("isPush", isPush);
  input = (unsigned long long)0xDEADBEEF; printWire("input", input);
  delayQueue->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 7: isPush = 0, input = 0xDEADBEEF --> output = 0x0123CDEF\n");
  isPush = (unsigned long long)0; printWire("isPush", isPush);
  input = (unsigned long long)0xDEADBEEF; printWire("input", input);
  delayQueue->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 8: isPush = 0, input = 0xDEADBEEF --> output = 0x00000000\n");
  isPush = (unsigned long long)0; printWire("isPush", isPush);
  input = (unsigned long long)0xDEADBEEF; printWire("input", input);
  delayQueue->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 9: isPush = 0, input = 0xDEADBEEF --> output = 0x89AB0123\n");
  isPush = (unsigned long long)0; printWire("isPush", isPush);
  input = (unsigned long long)0xDEADBEEF; printWire("input", input);
  delayQueue->advanceCycle();
  printWire("output", output);

  fprintf(stderr, "##### Cycle 10: isPush = 0, input = 0xDEADBEEF --> output = 0x00000000\n");
  isPush = (unsigned long long)0; printWire("isPush", isPush);
  input = (unsigned long long)0xDEADBEEF; printWire("input", input);
  delayQueue->advanceCycle();
  printWire("output", output);

  delete delayQueue; 
}

int main(int argc, char **argv) {
  testNANDGate();
  testSignedAdder();
  testSignedSubtractor();
  testLogicalUnit();
  testHashTable();
  testDelayQueue();

  return 0;
}


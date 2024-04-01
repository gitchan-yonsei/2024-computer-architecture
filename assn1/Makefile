
default: testExamples testAssn1

testExamples: DigitalCircuit.h UIntAdder.h Latch.h testExamples.cc
	g++ -o testExamples -std=c++11 -I. testExamples.cc

testAssn1: DigitalCircuit.h assn1.h assn1-stu.h testAssn1.cc
	g++ -o testAssn1 -std=c++11 -I. testAssn1.cc

.PHONY: clean
clean:
	rm -f testExamples testAssn1


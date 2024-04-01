#ifndef __ASSN1_H__
#define __ASSN1_H__

#include "DigitalCircuit.h"

#include <cassert>

template<size_t N>
class NANDGate : public DigitalCircuit {
  public:
    NANDGate(
      const std::string &name,
      const Wire<N> *input0,
      const Wire<N> *input1,
      Wire<N> *output
    ) : DigitalCircuit(name) {
      assert(input0 != nullptr && input1 != nullptr && output != nullptr);
      _inputs[0] = input0;
      _inputs[1] = input1;
      _output = output;
    }
    virtual void advanceCycle();
  private:
    const Wire<N> *_inputs[2];
    Wire<N> *_output;
};

template<size_t N>
class SignedAdder : public DigitalCircuit {
  public:
    SignedAdder(
      const std::string &name,
      const Wire<N> *input0,
      const Wire<N> *input1,
      Wire<N> *output
    ) : DigitalCircuit(name) {
      assert(input0 != nullptr && input1 != nullptr && output != nullptr);
      _inputs[0] = input0;
      _inputs[1] = input1;
      _output = output;
    }
    virtual void advanceCycle();
  private:
    const Wire<N> *_inputs[2];
    Wire<N> *_output;
};

template<size_t N>
class SignedSubtractor : public DigitalCircuit {
  public:
    SignedSubtractor(
      const std::string &name,
      const Wire<N> *input0,
      const Wire<N> *input1,
      Wire<N> *output
    ) : DigitalCircuit(name) {
      assert(input0 != nullptr && input1 != nullptr && output != nullptr);
      _inputs[0] = input0;
      _inputs[1] = input1;
      _output = output;
    }
    virtual void advanceCycle();
  private:
    const Wire<N> *_inputs[2];
    Wire<N> *_output;
};

template<size_t N>
class LogicalUnit : public DigitalCircuit {
  public:
    LogicalUnit(
      const std::string &name,
      const Wire<3> *operation,
      const Wire<N> *input0,
      const Wire<N> *input1,
      Wire<N> *output
    ) : DigitalCircuit(name) {
      assert(operation != nullptr);
      assert(input0 != nullptr && input1 != nullptr && output != nullptr);
      _operation = operation;
      _inputs[0] = input0;
      _inputs[1] = input1;
      _output = output;
    }
    virtual void advanceCycle();
  private:
    const Wire<3> *_operation;
    const Wire<N> *_inputs[2];
    Wire<N> *_output;
};

#define POW2(x) (1 << (x))

template<size_t N, size_t M>
class HashTable : public DigitalCircuit {
  public:
    HashTable(
      const std::string &name,
      const Wire<M> *index,
      const Wire<1> *isWrite,
      const Wire<N> *input,
      Wire<N> *output
    ) : DigitalCircuit(name) {
      assert(index != nullptr && isWrite != nullptr && input != nullptr
             && output != nullptr);
      _index = index;
      _isWrite = isWrite;
      _input = input;
      _output = output;
      for (size_t i = 0; i < POW2(M); i++) {
        _entries[i] = (unsigned long long)0xDEADBEEF;
      }
    }
    virtual void advanceCycle();
  private:
    const Wire<M> *_index;
    const Wire<1> *_isWrite;
    const Wire<N> *_input;
    Wire<N> *_output;
    Register<N> _entries[POW2(M)];
};

template<size_t N, size_t Delay = 1>
class DelayQueue : public DigitalCircuit {
  public:
    DelayQueue(
      const std::string &name,
      const Wire<1> *isPush,
      const Wire<N> *input,
      Wire<N> *output
    ) : DigitalCircuit(name) {
      assert(isPush != nullptr && input != nullptr && output != nullptr);
      _isPush = isPush;
      _input = input;
      _output = output;
      for (size_t i = 0; i < Delay; i++) {
        _entries[i].reset();
      }
    }
    virtual void advanceCycle();
  private:
    const Wire<1> *_isPush;
    const Wire<N> *_input;
    Wire<N> *_output;
    Register<N> _entries[Delay];
};

#endif


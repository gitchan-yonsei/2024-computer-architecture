#ifndef __ADDER_H__
#define __ADDER_H__

#include "DigitalCircuit.h"

#include <bitset>
#include <cassert>

template<size_t N>
class UIntAdder : public DigitalCircuit {

  public:

    UIntAdder(
      const std::string &name,
      const Wire<N> *input0,
      const Wire<N> *input1,
      Wire<N + 1> *output
    ) : DigitalCircuit(name) {
      assert(input0 != nullptr && input1 != nullptr && output != nullptr);
      _inputs[0] = input0;
      _inputs[1] = input1;
      _output = output;
    }

    virtual void advanceCycle() {
      _output->reset();

      unsigned tmp = 0;
      for(size_t i = 0; i < N; i++) {
        if (_inputs[0]->test(i)) { tmp++; }
        if (_inputs[1]->test(i)) { tmp++; }

        _output->set(i, (tmp % 2) == 1 ? true : false);

        tmp /= 2;
      }
      if (tmp > 0) { _output->set(N, true); }
    }

  private:

    const Wire<N> *_inputs[2];

    Wire<N + 1> *_output;

};

#endif


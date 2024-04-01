#ifndef __LATCH_H__
#define __LATCH_H__

#include "DigitalCircuit.h"

#include <cassert>

template<size_t N>
class Latch : public DigitalCircuit {

  public:

    Latch(
      const std::string &name,
      const Wire<1> *writeEnable,
      const Wire<N> *input,
      Wire<N> *output
    ) : DigitalCircuit(name) {
      assert(writeEnable != nullptr && input != nullptr && output != nullptr);
      _writeEnable = writeEnable;
      _input = input;
      _output = output;

      _data.reset();
    }

    virtual void advanceCycle() {
      if (_writeEnable->any()) {
        _data = (*_input);
      }

      (*_output) = _data;
    }

  private:

    const Wire<1> *_writeEnable;
    const Wire<N> *_input;

    Wire<N> *_output;

    Register<N> _data;

};

#endif


#ifndef __ASSN1_STU_H__
#define __ASSN1_STU_H__

#include "assn1.h"

template<size_t N>
void NANDGate<N>::advanceCycle() {
    _output->reset();

    for (size_t i = 0; i < N; i++) {
        unsigned tmp1 = _inputs[0]->test(i);
        unsigned tmp2 = _inputs[1]->test(i);

        bool result = !(tmp1 && tmp2);

        _output->set(i, result);
    }
}

template<size_t N>
void SignedAdder<N>::advanceCycle() {
    _output->reset();

    unsigned tmp = 0;
    for (size_t i = 0; i < N; i++) {
        if (_inputs[0]->test(i)) { tmp++; }
        if (_inputs[1]->test(i)) { tmp++; }

        _output->set(i, (tmp % 2) == 1 ? true : false);

        tmp /= 2;
    }
}

template<size_t N>
void SignedSubtractor<N>::advanceCycle() {
    _output->reset();

    Wire<N> complement = ~(*_inputs[1]);

    unsigned addValue = 1;
    for (size_t i = 0; i < N; i++) {
        unsigned tmp = complement.test(i) + addValue;
        complement.set(i, (tmp % 2) == 1 ? true : false);
        addValue = tmp / 2;
    }

    unsigned tmp = 0;
    for (size_t i = 0; i < N; i++) {
        if (_inputs[0]->test(i)) { tmp++; }
        if (complement.test(i)) { tmp++; }

        _output->set(i, (tmp % 2) == 1 ? true : false);

        tmp /= 2;
    }
}

template<size_t N>
void LogicalUnit<N>::advanceCycle() {
    _output->reset();

    bool op_0 = _operation->test(2);
    bool op_1 = _operation->test(1);
    bool op_2 = _operation->test(0);

    if (op_0 == 0 && op_1 == 0 && op_2 == 0) {
        // AND operation
        for (size_t i = 0; i < N; i++) {
            bool input0_bit = _inputs[0]->test(i);
            bool input1_bit = _inputs[1]->test(i);
            _output->set(i, input0_bit && input1_bit);
        }
    }
    if (op_0 == 0 && op_1 == 0 && op_2 == 1) {
        // OR operation
        for (size_t i = 0; i < N; i++) {
            bool input0_bit = _inputs[0]->test(i);
            bool input1_bit = _inputs[1]->test(i);
            _output->set(i, input0_bit || input1_bit);
        }
    }
    if (op_0 == 0 && op_1 == 1 && op_2 == 0) {
        // XOR operation
        for (size_t i = 0; i < N; i++) {
            bool input0_bit = _inputs[0]->test(i);
            bool input1_bit = _inputs[1]->test(i);
            _output->set(i, input0_bit != input1_bit);
        }
    }
    if (op_0 == 0 && op_1 == 1 && op_2 == 1) {
        // NOT operation
        for (size_t i = 0; i < N; i++) {
            bool input0_bit = _inputs[0]->test(i);
            _output->set(i, !input0_bit);
        }
    }
    if (op_0 == 1 && op_1 == 0 && op_2 == 0) {
        // NAND operation
        for (size_t i = 0; i < N; i++) {
            bool input0_bit = _inputs[0]->test(i);
            bool input1_bit = _inputs[1]->test(i);
            _output->set(i, !(input0_bit && input1_bit));
        }
    }
    if (op_0 == 1 && op_1 == 0 && op_2 == 1) {
        // NOR operation
        for (size_t i = 0; i < N; i++) {
            bool input0_bit = _inputs[0]->test(i);
            bool input1_bit = _inputs[1]->test(i);
            _output->set(i, !(input0_bit || input1_bit));
        }
    }
    if (op_0 == 1 && op_1 == 1 && op_2 == 0) {
        // XNOR operation
        for (size_t i = 0; i < N; i++) {
            bool input0_bit = _inputs[0]->test(i);
            bool input1_bit = _inputs[1]->test(i);
            _output->set(i, input0_bit == input1_bit);
        }
    }
}

template<size_t N, size_t M>
void HashTable<N, M>::advanceCycle() {
    _output->reset();

    size_t index = _index->to_ulong();

    if (_isWrite->test(0)) {
        _entries[index] = _input->to_ullong();
        *_output = 0;
    } else {
        *_output = _entries[index];
    }
}

template<size_t N, size_t D>
void DelayQueue<N, D>::advanceCycle() {
    *_output = _entries[0].to_ulong();

    for (size_t i = 1; i <= D - 1; i++) {
        _entries[i - 1] = _entries[i];
    }

    if (_isPush->test(0) == 1) {
        _entries[D - 1] = _input->to_ulong();
    } else {
        _entries[D - 1] = 0;
    }
}

#endif


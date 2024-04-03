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
    for(size_t i = 0; i < N; i++) {
        if (_inputs[0]->test(i)) { tmp++; }
        if (_inputs[1]->test(i)) { tmp++; }

        _output->set(i, (tmp % 2) == 1 ? true : false);

        tmp /= 2;
    }
}

template<size_t N>
void SignedSubtractor<N>::advanceCycle() {
  /* FIXME */
}

template<size_t N>
void LogicalUnit<N>::advanceCycle() {
  /* FIXME */
}

template<size_t N, size_t M>
void HashTable<N, M>::advanceCycle() {
  /* FIXME */
}

template<size_t N, size_t D>
void DelayQueue<N, D>::advanceCycle() {
  /* FIXME */
}

#endif


#include "Bitset.h"

#include <bit>

Bitset::Bitset() :
    size(0) {

}

Bitset::Bitset(size_t size, bool value) :
    size(size) {
    blocks.resize(NumBlocks(), value ? (Block)(-1) : (Block)(0));
}

size_t Bitset::FindFirst(bool value) {
    for (size_t i = 0; i < NumBlocks(); ++i) {
        if (blocks[i] != (value ? 0 : ~0)) {
            size_t bitIndex = static_cast<size_t>(value ? std::countr_zero(blocks[i]) : std::countr_one(blocks[i]));
            if (bitIndex < kBlockSize) {
                return i * kBlockSize + bitIndex;
            }
        }
    }
    return NumBlocks() * kBlockSize;
}

Bitset& Bitset::FlipAll() {
    for (auto& word : blocks) {
        word = ~word;
    }
    return *this;
}
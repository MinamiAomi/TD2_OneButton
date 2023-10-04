#pragma once

#include <limits>
#include <bit>
#include <vector>


class Bitset {
public:
    Bitset();
    Bitset(size_t size, bool value = false);
    // 先頭から探す
    size_t FindFirst(bool value = false);
    // すべて反転
    Bitset& FlipAll();
    // 指定したビットを反転
    Bitset& Flip(size_t bitIndex);
    // すべてのビットを変更
    Bitset& SetAll(bool value = true);
    // 指定したビットを変更
    Bitset& Set(size_t bitIndex, bool value = true);
    // すべてのビットを0にする
    Bitset& ResetAll();
    // 指定したビットを0にする
    Bitset& Reset(size_t bitIndex);
    // 指定したビットを判定
    bool Test(size_t bitIndex) const;
    // すべてのビットを判定
    bool All(bool value = true) const;

    // ビット数
    constexpr size_t Size() const { return N; }

private:
    using Block = unsigned long long;

    static constexpr size_t kBlockSize = std::numeric_limits<Block>::digits;
    static constexpr size_t kBlockMask = kBlockSize - 1;
    static constexpr size_t kBitIndexToBlockIndex = 6; // log2(kWordSize)

    Block& GetWord(size_t bitIndex);
    const Block& GetWord(size_t bitIndex) const;

    size_t NumBlocks() { return (size - 1) / kBlockSize + 1; }

    std::vector<Block> blocks;
    size_t size;
};

template<size_t N>
inline Bitset<N>& Bitset<N>::FlipAll() {
    for (auto& word : blocks_) {
        word = ~word;
    }
    return *this;
}

template<size_t N>
inline Bitset<N>& Bitset<N>::Flip(size_t bitIndex) {
    auto& word = GetWord(bitIndex);
    word = word ^ static_cast<uint64_t>(1) << (bitIndex & kBlockMask);
    return *this;
}

template<size_t N>
inline Bitset<N>& Bitset<N>::SetAll(bool value) {
    memset(blocks_, value ? ~0 : 0, sizeof(blocks_));
    return *this;
}

template<size_t N>
inline Bitset<N>& Bitset<N>::Set(size_t bitIndex, bool value) {
    uint64_t& word = GetWord(bitIndex);
    if (value) {
        word |= static_cast<uint64_t>(1) << (bitIndex & kBlockMask);
    }
    else {
        word &= ~(static_cast<uint64_t>(1) << (bitIndex & kBlockMask));
    }
    return *this;
}

template<size_t N>
inline Bitset<N>& Bitset<N>::ResetAll() {
    SetAll(false);
    return *this;
}

template<size_t N>
inline Bitset<N>& Bitset<N>::Reset(size_t bitIndex) {
    Set(bitIndex, false);
    return *this;
}

template<size_t N>
inline bool Bitset<N>::Test(size_t bitIndex) const {
    return(GetWord(bitIndex) & (static_cast<uint64_t>(1) << (bitIndex & kBlockMask))) != 0;
}
template<size_t N>
inline bool Bitset<N>::All(bool value) const {
    for (auto& word : blocks_) {
        if (word != (value ? ~0 : 0)) { return false; }
    }
    return true;
}

template<size_t N>
inline Bitset<N>::Block& Bitset<N>::GetWord(size_t bitIndex) {
    return blocks_[bitIndex >> kBitIndexToBlockIndex];
}
template<size_t N>
inline const Bitset<N>::Block& Bitset<N>::GetWord(size_t bitIndex) const {
    return blocks_[bitIndex >> kBitIndexToBlockIndex];
}
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>

using namespace std;

// === lab 3 ===

class BitWriter {
public:
    BitWriter(const string& filename);
    ~BitWriter();
    void writeBits(const uint8_t* buffer, uint32_t n);
    void close();
private:
    ofstream file;
    uint8_t buf;
    int bitPos;
};

class BitReader {
public:
    BitReader(const string& filename);
    ~BitReader();
    void readBits(uint8_t* buffer, uint32_t n);
    bool isEOF() const;
    void close();
private:
    ifstream file;
    uint8_t buf;
    int bitPos;
};

// === lab 4 ===

struct HuffNode {
    uint8_t   symbol;
    uint32_t  freq;
    HuffNode* left;
    HuffNode* right;
};

void encodeHuffman(const string& inputName, const string& outputName, bool useBWT = false, bool useMTF = false);
void decodeHuffman(const string& inputName, const string& outputName);

// === lab 5 ===

void encodeLZW(const string& inputName, const string& outputName, int maxBits, bool reset, bool useBWT = false, bool useMTF = false);
void decodeLZW(const string& inputName, const string& outputName);

// === lab 6 ===

vector<uint8_t> bwtForward(const vector<uint8_t>& data, uint32_t& index);
vector<uint8_t> bwtInverse(const vector<uint8_t>& data, uint32_t index);
vector<uint8_t> mtfForward(const vector<uint8_t>& data);
vector<uint8_t> mtfInverse(const vector<uint8_t>& data);
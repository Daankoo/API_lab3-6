#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <queue>
#include <algorithm>

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

void encodeHuffman(const string& inputName, const string& outputName);
void decodeHuffman(const string& inputName, const string& outputName);

// === lab 5 ===

void encodeLZW(const string& inputName, const string& outputName, int maxBits, bool reset);
void decodeLZW(const string& inputName, const string& outputName);
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

using namespace std;

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
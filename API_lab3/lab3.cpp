#include "header.h"

BitWriter::BitWriter(const string& filename) : buf(0), bitPos(0) {
    file.open(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error: cannot open file: " << filename << "\n";
        exit(1);
    }
}

BitWriter::~BitWriter() {
    close();
}

void BitWriter::writeBits(const uint8_t* buffer, uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
        int bit = (buffer[i / 8] >> (i % 8)) & 1;
        buf |= (bit << bitPos);
        bitPos++;
        if (bitPos == 8) {
            file.put(static_cast<char>(buf));
            buf = 0;
            bitPos = 0;
        }
    }
}

void BitWriter::close() {
    if (!file.is_open()) return;
    if (bitPos > 0) file.put(static_cast<char>(buf));
    file.close();
    buf = 0; bitPos = 0;
}

BitReader::BitReader(const string& filename) : buf(0), bitPos(8) {
    file.open(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error: cannot open file: " << filename << "\n";
        exit(1);
    }
}

BitReader::~BitReader() {
    close();
}

void BitReader::readBits(uint8_t* buffer, uint32_t n) {
    for (uint32_t j = 0; j < (n + 7) / 8; j++) buffer[j] = 0;

    for (uint32_t i = 0; i < n; i++) {
        if (bitPos == 8) {
            buf = file.get();
            bitPos = 0;
        }
        int bit = (buf >> bitPos) & 1;
        bitPos++;
        buffer[i / 8] |= (bit << (i % 8));
    }
}

bool BitReader::isEOF() const {
    return file.eof();
}

void BitReader::close() {
    if (file.is_open()) file.close();
}
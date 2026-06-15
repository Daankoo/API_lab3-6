#include "header.h"

BitWriter::BitWriter(const string& filename) : buf_(0), bitPos_(0) {
    file_.open(filename, ios::binary);
    if (!file_.is_open()) {
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
        buf_ |= (bit << bitPos_);
        bitPos_++;
        if (bitPos_ == 8) {
            file_.put(static_cast<char>(buf_));
            buf_ = 0;
            bitPos_ = 0;
        }
    }
}

void BitWriter::close() {
    if (!file_.is_open()) return;
    if (bitPos_ > 0) file_.put(static_cast<char>(buf_));
    file_.close();
    buf_ = 0; bitPos_ = 0;
}

BitReader::BitReader(const string& filename) : buf_(0), bitPos_(0) {
    file_.open(filename, ios::binary);
    if (!file_.is_open()) {
        cerr << "Error: cannot open file: " << filename << "\n";
        exit(1);
    }
}

BitReader::~BitReader() { 
    close(); 
}

void BitReader::readBits(uint8_t* buffer, uint32_t n) {

}

bool BitReader::isEOF() const { 
    return file_.eof(); 
}

void BitReader::close() { 
    if (file_.is_open()) file_.close(); 
}
#include "header.h"

void initDict(vector<vector<uint8_t>>& dict) {
    dict.clear();
    for (int i = 0; i < 256; i++)
        dict.push_back({ (uint8_t)i });
}

void encodeLZW(const string& inputName, const string& outputName, int maxBits, bool reset) {

}

void decodeLZW(const string& inputName, const string& outputName) {

}
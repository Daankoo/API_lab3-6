#include "header.h"

void encodeHuffman(const string& inputName, const string& outputName) {
    
    ifstream input(inputName, ios::binary);
    if (!input) { cerr << "Error: cannot open file: " << inputName << "\n"; exit(1); }

    
    uint32_t freq[256] = {};
    uint8_t byte;
    while (input.read(reinterpret_cast<char*>(&byte), 1))
        freq[byte]++;

    
    for (int i = 0; i < 256; i++)
        if (freq[i] > 0)
            cout << "byte " << i << ": " << freq[i] << "\n";
}

void decodeHuffman(const string& inputName, const string& outputName) {

}
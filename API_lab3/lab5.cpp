#include "header.h"

void initDict(vector<vector<uint8_t>>& dict) {
    dict.clear();
    for (int i = 0; i < 256; i++)
        dict.push_back({ (uint8_t)i });
}

// Лінійний пошук фрази у словнику
int findInDict(const vector<vector<uint8_t>>& dict, const vector<uint8_t>& phrase) {
    for (int i = 0; i < (int)dict.size(); i++)
        if (dict[i] == phrase) return i;
    return -1;
}

void encodeLZW(const string& inputName, const string& outputName, int maxBits, bool reset) {
    ifstream input(inputName, ios::binary);
    if (!input) { cerr << "Error: cannot open file: " << inputName << "\n"; exit(1); }

    vector<vector<uint8_t>> dict;
    initDict(dict);
    int maxSize = 1 << maxBits;

    BitWriter writer(outputName);

    uint8_t mb = maxBits;
    uint8_t rm = reset ? 1 : 0;
    writer.writeBits(&mb, 8);
    writer.writeBits(&rm, 8);

    vector<uint8_t> current;
    uint8_t byte;

    while (input.read(reinterpret_cast<char*>(&byte), 1)) {
        vector<uint8_t> next = current;
        next.push_back(byte);

        if (findInDict(dict, next) != -1) {
            current = next;
        }
        else {
            uint32_t code = findInDict(dict, current);
            writer.writeBits(reinterpret_cast<uint8_t*>(&code), maxBits);

            if ((int)dict.size() < maxSize)
                dict.push_back(next);
            else if (reset)
                initDict(dict);

            current = { byte };
        }
    }

    if (!current.empty()) {
        uint32_t code = findInDict(dict, current);
        writer.writeBits(reinterpret_cast<uint8_t*>(&code), maxBits);
    }

    writer.close();
    cout << "Encoding complete. Result saved to " << outputName << "\n";
}

void decodeLZW(const string& inputName, const string& outputName) {

}
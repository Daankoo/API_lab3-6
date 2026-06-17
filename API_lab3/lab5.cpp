#include "header.h"

void encodeLZW(const string& inputName, const string& outputName, int maxBits, bool reset) {
    ifstream input(inputName, ios::binary);
    if (!input) { cerr << "Error: cannot open file: " << inputName << "\n"; exit(1); }

    int maxSize = 1 << maxBits;

    vector<vector<int>> lookup(maxSize, vector<int>(256, -1));

    BitWriter writer(outputName);

    uint8_t mb = maxBits;
    uint8_t rm = reset ? 1 : 0;
    writer.writeBits(&mb, 8);
    writer.writeBits(&rm, 8);

    int dictSize = 256;
    int current = -1;
    uint8_t byte;

    while (input.read(reinterpret_cast<char*>(&byte), 1)) {
        if (current == -1) {
            current = byte;
            continue;
        }

        if (lookup[current][byte] != -1) {
            current = lookup[current][byte];
        }
        else {
            uint32_t code = current;
            writer.writeBits(reinterpret_cast<uint8_t*>(&code), maxBits);

            if (dictSize < maxSize) {
                lookup[current][byte] = dictSize++;
            }
            else if (reset) {
                for (auto& row : lookup) fill(row.begin(), row.end(), -1);
                dictSize = 256;
            }

            current = byte;
        }
    }

    if (current != -1) {
        uint32_t code = current;
        writer.writeBits(reinterpret_cast<uint8_t*>(&code), maxBits);
    }

    writer.close();
    cout << "Encoding complete. Result saved to " << outputName << "\n";
}

void decodeLZW(const string& inputName, const string& outputName) {
    BitReader reader(inputName);

    uint8_t mb = 0, rm = 0;
    reader.readBits(&mb, 8);
    reader.readBits(&rm, 8);
    int maxBits = mb;
    bool reset = rm == 1;
    int maxSize = 1 << maxBits;

    vector<vector<uint8_t>> dict;
    for (int i = 0; i < 256; i++)
        dict.push_back({ (uint8_t)i });

    ofstream output(outputName, ios::binary);
    if (!output) { cerr << "Error: cannot open file: " << outputName << "\n"; exit(1); }

    uint32_t code = 0;
    reader.readBits(reinterpret_cast<uint8_t*>(&code), maxBits);
    if (reader.isEOF()) return;

    vector<uint8_t> prev = dict[code];
    for (uint8_t b : prev) output.put(b);

    while (true) {
        code = 0;
        reader.readBits(reinterpret_cast<uint8_t*>(&code), maxBits);
        if (reader.isEOF()) break;

        vector<uint8_t> entry;

        if (code < (uint32_t)dict.size()) {
            entry = dict[code];
        }
        else if (code == (uint32_t)dict.size()) {
            entry = prev;
            entry.push_back(prev[0]);
        }
        else {
            cerr << "Error: invalid code " << code << "\n";
            return;
        }

        for (uint8_t b : entry) output.put(b);

        vector<uint8_t> newEntry = prev;
        newEntry.push_back(entry[0]);

        if ((int)dict.size() < maxSize)
            dict.push_back(newEntry);
        else if (reset) {
            dict.clear();
            for (int i = 0; i < 256; i++)
                dict.push_back({ (uint8_t)i });
        }

        prev = entry;
    }

    output.close();
    cout << "Decoding complete. Result saved to " << outputName << "\n";
}
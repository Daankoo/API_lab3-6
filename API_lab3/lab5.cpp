#include "header.h"

void encodeLZW(const string& inputName, const string& outputName, int maxBits, bool reset, bool useBWT, bool useMTF) {
    ifstream input(inputName, ios::binary);
    if (!input) { cerr << "Error: cannot open file: " << inputName << "\n"; exit(1); }

    vector<uint8_t> data((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());

    if (useBWT) {
        uint32_t index;
        data = bwtForward(data, index);
        vector<uint8_t> withIndex(4);
        withIndex[0] = index & 0xFF;
        withIndex[1] = (index >> 8) & 0xFF;
        withIndex[2] = (index >> 16) & 0xFF;
        withIndex[3] = (index >> 24) & 0xFF;
        withIndex.insert(withIndex.end(), data.begin(), data.end());
        data = withIndex;
    }
    if (useMTF) data = mtfForward(data);

    int maxSize = 1 << maxBits;
    vector<vector<int>> lookup(maxSize, vector<int>(256, -1));

    BitWriter writer(outputName);

    uint8_t mb = maxBits;
    uint8_t rm = reset ? 1 : 0;
    uint8_t bf = useBWT ? 1 : 0;
    uint8_t mf = useMTF ? 1 : 0;
    writer.writeBits(&mb, 8);
    writer.writeBits(&rm, 8);
    writer.writeBits(&bf, 8);
    writer.writeBits(&mf, 8);

    int dictSize = 256;
    int current = -1;

    for (uint8_t byte : data) {
        if (current == -1) { current = byte; continue; }
        if (lookup[current][byte] != -1) {
            current = lookup[current][byte];
        }
        else {
            uint32_t code = current;
            writer.writeBits(reinterpret_cast<uint8_t*>(&code), maxBits);
            if (dictSize < maxSize)
                lookup[current][byte] = dictSize++;
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

    uint8_t mb = 0, rm = 0, bf = 0, mf = 0;
    reader.readBits(&mb, 8);
    reader.readBits(&rm, 8);
    reader.readBits(&bf, 8);
    reader.readBits(&mf, 8);

    int maxBits = mb;
    bool reset = rm == 1;
    bool useBWT = bf == 1;
    bool useMTF = mf == 1;
    int maxSize = 1 << maxBits;

    vector<vector<uint8_t>> dict;
    for (int i = 0; i < 256; i++) dict.push_back({ (uint8_t)i });

    vector<uint8_t> data;

    uint32_t code = 0;
    reader.readBits(reinterpret_cast<uint8_t*>(&code), maxBits);
    if (reader.isEOF()) return;

    vector<uint8_t> prev = dict[code];
    data.insert(data.end(), prev.begin(), prev.end());

    while (true) {
        code = 0;
        reader.readBits(reinterpret_cast<uint8_t*>(&code), maxBits);
        if (reader.isEOF()) break;

        vector<uint8_t> entry;
        if (code < (uint32_t)dict.size())
            entry = dict[code];
        else if (code == (uint32_t)dict.size()) {
            entry = prev;
            entry.push_back(prev[0]);
        }
        else {
            cerr << "Error: invalid code " << code << "\n";
            return;
        }

        data.insert(data.end(), entry.begin(), entry.end());

        vector<uint8_t> newEntry = prev;
        newEntry.push_back(entry[0]);
        if ((int)dict.size() < maxSize)
            dict.push_back(newEntry);
        else if (reset) {
            dict.clear();
            for (int i = 0; i < 256; i++) dict.push_back({ (uint8_t)i });
        }

        prev = entry;
    }

    if (useMTF) data = mtfInverse(data);
    if (useBWT) {
        uint32_t index = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
        data = bwtInverse(vector<uint8_t>(data.begin() + 4, data.end()), index);
    }

    ofstream output(outputName, ios::binary);
    if (!output) { cerr << "Error: cannot open file: " << outputName << "\n"; exit(1); }
    for (uint8_t b : data) output.put(b);
    output.close();

    cout << "Decoding complete. Result saved to " << outputName << "\n";
}
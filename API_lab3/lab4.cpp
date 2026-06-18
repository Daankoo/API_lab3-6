#include "header.h"

struct Compare {
    bool operator()(HuffNode* a, HuffNode* b) {
        return a->freq > b->freq;
    }
};

HuffNode* buildTree(uint32_t freq[256]) {
    priority_queue<HuffNode*, vector<HuffNode*>, Compare> pq;
    for (int i = 0; i < 256; i++) {
        if (freq[i] == 0) continue;
        HuffNode* node = new HuffNode();
        node->symbol = i;
        node->freq = freq[i];
        node->left = nullptr;
        node->right = nullptr;
        pq.push(node);
    }
    while (pq.size() > 1) {
        HuffNode* left = pq.top(); pq.pop();
        HuffNode* right = pq.top(); pq.pop();
        HuffNode* parent = new HuffNode();
        parent->symbol = 0;
        parent->freq = left->freq + right->freq;
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    return pq.top();
}

void buildCodes(HuffNode* node, uint32_t code, int len, uint32_t codes[256], int lens[256]) {
    if (node->left == nullptr && node->right == nullptr) {
        codes[node->symbol] = code;
        lens[node->symbol] = len;
        return;
    }
    if (node->left)  buildCodes(node->left, (code << 1) | 0, len + 1, codes, lens);
    if (node->right) buildCodes(node->right, (code << 1) | 1, len + 1, codes, lens);
}

void encodeHuffman(const string& inputName, const string& outputName, bool useBWT, bool useMTF) {
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

    uint32_t freq[256] = {};
    for (uint8_t b : data) freq[b]++;

    HuffNode* root = buildTree(freq);
    uint32_t codes[256] = {};
    int      lens[256] = {};
    buildCodes(root, 0, 0, codes, lens);

    BitWriter writer(outputName);

    // Заголовок: прапорці BWT і MTF
    uint8_t flags[2] = { (uint8_t)useBWT, (uint8_t)useMTF };
    writer.writeBits(&flags[0], 8);
    writer.writeBits(&flags[1], 8);

    // Таблиця частот
    for (int i = 0; i < 256; i++)
        writer.writeBits(reinterpret_cast<uint8_t*>(&freq[i]), 32);

    // Стиснені дані
    for (uint8_t byte : data) {
        uint32_t code = codes[byte];
        int      len = lens[byte];
        for (int b = len - 1; b >= 0; b--) {
            uint8_t bit = (code >> b) & 1;
            writer.writeBits(&bit, 1);
        }
    }

    writer.close();
    cout << "Encoding complete. Result saved to " << outputName << "\n";
}

void decodeHuffman(const string& inputName, const string& outputName) {
    BitReader reader(inputName);

    uint8_t bwtFlag = 0, mtfFlag = 0;
    reader.readBits(&bwtFlag, 8);
    reader.readBits(&mtfFlag, 8);

    uint32_t freq[256] = {};
    for (int i = 0; i < 256; i++)
        reader.readBits(reinterpret_cast<uint8_t*>(&freq[i]), 32);

    HuffNode* root = buildTree(freq);
    uint32_t total = 0;
    for (int i = 0; i < 256; i++) total += freq[i];

    vector<uint8_t> data;
    HuffNode* node = root;
    uint32_t decoded = 0;
    while (decoded < total) {
        uint8_t bit = 0;
        reader.readBits(&bit, 1);
        node = (bit == 0) ? node->left : node->right;
        if (node->left == nullptr && node->right == nullptr) {
            data.push_back(node->symbol);
            decoded++;
            node = root;
        }
    }

    if (mtfFlag) data = mtfInverse(data);
    if (bwtFlag) {
        uint32_t index = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
        data = bwtInverse(vector<uint8_t>(data.begin() + 4, data.end()), index);
    }

    ofstream output(outputName, ios::binary);
    if (!output) { cerr << "Error: cannot open file: " << outputName << "\n"; exit(1); }
    for (uint8_t b : data) output.put(b);
    output.close();

    cout << "Decoding complete. Result saved to " << outputName << "\n";
}
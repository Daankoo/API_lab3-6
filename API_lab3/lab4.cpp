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

void encodeHuffman(const string& inputName, const string& outputName) {
    ifstream input(inputName, ios::binary);
    if (!input) { cerr << "Error: cannot open file: " << inputName << "\n"; exit(1); }

    uint32_t freq[256] = {};
    uint8_t byte;
    while (input.read(reinterpret_cast<char*>(&byte), 1))
        freq[byte]++;

    HuffNode* root = buildTree(freq);

    uint32_t codes[256] = {};
    int      lens[256] = {};
    buildCodes(root, 0, 0, codes, lens);

    for (int i = 0; i < 256; i++) {
        if (lens[i] == 0) continue;
        cout << "byte " << i << " ('" << (char)i << "'): ";
        for (int b = lens[i] - 1; b >= 0; b--)
            cout << ((codes[i] >> b) & 1);
        cout << " (len=" << lens[i] << ")\n";
    }
}

void decodeHuffman(const string& inputName, const string& outputName) {

}
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

void encodeHuffman(const string& inputName, const string& outputName) {
    ifstream input(inputName, ios::binary);
    if (!input) { cerr << "Error: cannot open file: " << inputName << "\n"; exit(1); }

    uint32_t freq[256] = {};
    uint8_t byte;
    while (input.read(reinterpret_cast<char*>(&byte), 1))
        freq[byte]++;

    HuffNode* root = buildTree(freq);
    cout << "Tree root freq: " << root->freq << "\n";
}

void decodeHuffman(const string& inputName, const string& outputName) {

}
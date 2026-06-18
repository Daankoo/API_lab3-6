#include "header.h"

vector<uint8_t> bwtForward(const vector<uint8_t>& data, uint32_t& index) {
    int N = data.size();

    vector<int> idx(N);
    iota(idx.begin(), idx.end(), 0);

    sort(idx.begin(), idx.end(), [&](int a, int b) {
        for (int i = 0; i < N; i++) {
            uint8_t ca = data[(a + i) % N];
            uint8_t cb = data[(b + i) % N];
            if (ca != cb) return ca < cb;
        }
        return false;
        });

    vector<uint8_t> result(N);
    for (int i = 0; i < N; i++) {
        result[i] = data[(idx[i] + N - 1) % N];
        if (idx[i] == 0) index = i;
    }
    return result;
}

vector<uint8_t> bwtInverse(const vector<uint8_t>& L, uint32_t index) {
    int N = L.size();

    int count[256] = {};
    for (uint8_t b : L) count[b]++;

    int start[256] = {};
    for (int i = 1; i < 256; i++)
        start[i] = start[i - 1] + count[i - 1];

    vector<int> lf(N);
    int rank[256] = {};
    for (int i = 0; i < N; i++)
        lf[i] = start[L[i]] + rank[L[i]]++;

    vector<uint8_t> result(N);
    int cur = index;
    for (int i = N - 1; i >= 0; i--) {
        result[i] = L[cur];
        cur = lf[cur];
    }
    return result;
}

vector<uint8_t> mtfForward(const vector<uint8_t>& data) {
    vector<uint8_t> alphabet(256);
    iota(alphabet.begin(), alphabet.end(), 0);

    vector<uint8_t> result(data.size());
    for (int i = 0; i < (int)data.size(); i++) {
        int pos = find(alphabet.begin(), alphabet.end(), data[i]) - alphabet.begin();
        result[i] = pos;
        alphabet.erase(alphabet.begin() + pos);
        alphabet.insert(alphabet.begin(), data[i]);
    }
    return result;
}

vector<uint8_t> mtfInverse(const vector<uint8_t>& data) {
    vector<uint8_t> alphabet(256);
    iota(alphabet.begin(), alphabet.end(), 0);

    vector<uint8_t> result(data.size());
    for (int i = 0; i < (int)data.size(); i++) {
        uint8_t pos = data[i];
        result[i] = alphabet[pos];
        uint8_t byte = alphabet[pos];
        alphabet.erase(alphabet.begin() + pos);
        alphabet.insert(alphabet.begin(), byte);
    }
    return result;
}
#include "header.h"

int main() {
    encodeLZW("test.txt", "test.lzw", 12, false);
    decodeLZW("test.lzw", "test_decoded.txt");

    ifstream a("test.txt"), b("test_decoded.txt");
    string sa((istreambuf_iterator<char>(a)), istreambuf_iterator<char>());
    string sb((istreambuf_iterator<char>(b)), istreambuf_iterator<char>());
    cout << "Match: " << (sa == sb ? "YES" : "NO") << "\n";

    return 0;
}
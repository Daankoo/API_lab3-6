#include "header.h"

int main() {
    encodeHuffman("test.txt", "test.huf");
    decodeHuffman("test.huf", "test_decoded.txt");

    ifstream a("test.txt"), b("test_decoded.txt");
    string sa((istreambuf_iterator<char>(a)), istreambuf_iterator<char>());
    string sb((istreambuf_iterator<char>(b)), istreambuf_iterator<char>());
    cout << "Match: " << (sa == sb ? "YES" : "NO") << "\n";

    return 0;
}
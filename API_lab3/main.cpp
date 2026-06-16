#include "header.h"

void printBits(const uint8_t* buffer, uint32_t n) {
    for (uint32_t i = 0; i < n; i++)
        cout << ((buffer[i / 8] >> (i % 8)) & 1);
}

int main() {
    uint8_t a1[] = { 0xE1, 0x01 };
    uint8_t a2[] = { 0xEE, 0x00 };
    uint8_t a3[] = { 0xFF };
    uint8_t a4[] = { 0xAA };

    BitWriter w("test.bin");
    w.writeBits(a1, 9);
    w.writeBits(a2, 9);
    w.writeBits(a3, 8);
    w.writeBits(a4, 4);
    w.close();

    cout << "Written:\n";
    cout << "a1  (9 bits): "; 
    printBits(a1, 9); 
    cout << "\n";
    
    cout << "a2  (9 bits): "; 
    printBits(a2, 9); 
    cout << "\n";
    
    cout << "a3  (8 bits): "; 
    printBits(a3, 8); 
    cout << "\n";
    
    cout << "a4  (4 bits): "; 
    printBits(a4, 4); 
    cout << "\n\n";

    cout << "Full stream:\n";
    ifstream f("test.bin", ios::binary);
    uint8_t byte;
    while (f.read(reinterpret_cast<char*>(&byte), 1)) {
        for (int i = 0; i < 8; i++)
            cout << ((byte >> i) & 1);
        cout << " ";
    }
    cout << "\n\n";

    uint8_t b1[2], b2[1], b3[2], b4[1];

    BitReader r("test.bin");
    r.readBits(b1, 11);
    r.readBits(b2, 7);
    r.readBits(b3, 8);
    r.readBits(b4, 4);
    r.close();

    cout << "Read:\n";
    cout << "b1 (11 bits): "; 
    printBits(b1, 11); cout << "\n";
    
    cout << "b2  (7 bits): "; 
    printBits(b2, 7); 
    cout << "\n";
    
    cout << "b3  (8 bits): "; 
    printBits(b3, 8); 
    cout << "\n";
    
    cout << "b4  (4 bits): "; 
    printBits(b4, 4); 
    cout << "\n";

    return 0;
}
#include "header.h"

long long fileSize(const string& path) {
    ifstream f(path, ios::binary | ios::ate);
    return f.tellg();
}

void analyze(const string& input) {
    long long orig = fileSize(input);
    string tmp = "_tmp.huf";

    encodeHuffman(input, tmp);
    cout << "  Huffman:  " << (double)fileSize(tmp) / orig * 100 << "%\n";

    encodeHuffman(input, tmp, true, false);
    cout << "  +BWT:     " << (double)fileSize(tmp) / orig * 100 << "%\n";

    encodeHuffman(input, tmp, false, true);
    cout << "  +MTF:     " << (double)fileSize(tmp) / orig * 100 << "%\n";

    encodeHuffman(input, tmp, true, true);
    cout << "  +BWT+MTF: " << (double)fileSize(tmp) / orig * 100 << "%\n";
}

int main() {
    cout << "\n--- JSON ---\n";
    cout << "fingerprint.json:\n";        
    analyze("File_lab4/json/fingerprint.json");
    cout << "simd_f32x4_cmp.json:\n";    
    analyze("File_lab4/json/simd_f32x4_cmp.json");
    cout << "simd_f32x4_pmin_pmax:\n";   
    analyze("File_lab4/json/simd_f32x4_pmin_pmax.json");
    cout << "simd_f64x2_cmp.json:\n";    
    analyze("File_lab4/json/simd_f64x2_cmp.json");
    cout << "simd_f64x2_pmin_pmax:\n";   
    analyze("File_lab4/json/simd_f64x2_pmin_pmax.json");

    cout << "\n--- EXE ---\n";
    cout << "AJAZZ_AK820.exe:\n";           
    analyze("File_lab4/exe/AJAZZ_AK820.exe");
    cout << "anki.exe:\n";                  
    analyze("File_lab4/exe/anki.exe");
    cout << "mpv.exe:\n";                   
    analyze("File_lab4/exe/mpv.exe");
    cout << "setup.exe:\n";                 
    analyze("File_lab4/exe/setup.exe");
    cout << "WargamingErrorMonitor.exe:\n"; 
    analyze("File_lab4/exe/WargamingErrorMonitor.exe");

    cout << "\n--- TXT ---\n";
    cout << "dump.txt:\n";    
    analyze("File_lab4/txt/dump.txt");
    cout << "matr_A.txt:\n";  
    analyze("File_lab4/txt/matr_A.txt");
    cout << "raj.txt:\n";     
    analyze("File_lab4/txt/raj.txt");
    cout << "results.txt:\n"; 
    analyze("File_lab4/txt/results.txt");
    cout << "text.txt:\n";    
    analyze("File_lab4/txt/text.txt");

    cout << "\n--- JPG ---\n";
    cout << "7.jpg:\n";  
    analyze("File_lab4/jpg/7.jpg");
    cout << "8.jpg:\n";  
    analyze("File_lab4/jpg/8.jpg");
    cout << "9.jpg:\n";  
    analyze("File_lab4/jpg/9.jpg");
    cout << "10.jpg:\n"; 
    analyze("File_lab4/jpg/10.jpg");
    cout << "11.jpg:\n"; 
    analyze("File_lab4/jpg/11.jpg");

    cout << "\n--- MP3 ---\n";
    cout << "music1.mp3:\n"; 
    analyze("File_lab4/mp3/music1.mp3");
    cout << "music2.mp3:\n"; 
    analyze("File_lab4/mp3/music2.mp3");
    cout << "music3.mp3:\n"; 
    analyze("File_lab4/mp3/music3.mp3");
    cout << "music4.mp3:\n"; 
    analyze("File_lab4/mp3/music4.mp3");
    cout << "music5.mp3:\n"; 
    analyze("File_lab4/mp3/music5.mp3");

    return 0;
}
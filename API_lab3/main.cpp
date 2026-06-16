#include "header.h"

long long fileSize(const string& path) {
    ifstream f(path, ios::binary | ios::ate);
    return f.tellg();
}

void analyze(const string& input) {
    string output = input + ".huf";
    encodeHuffman(input, output);

    long long orig = fileSize(input);
    long long comp = fileSize(output);
    double ratio = (double)comp / orig * 100.0;

    cout << input << ": " << orig << " -> " << comp << " bytes (" << ratio << "%)\n";
}

int main() {
    cout << "\n--- JSON ---\n";
    analyze("File_lab4/json/fingerprint.json");
    analyze("File_lab4/json/simd_f32x4_cmp.json");
    analyze("File_lab4/json/simd_f32x4_pmin_pmax.json");
    analyze("File_lab4/json/simd_f64x2_cmp.json");
    analyze("File_lab4/json/simd_f64x2_pmin_pmax.json");

    cout << "\n--- EXE ---\n";
    analyze("File_lab4/exe/AJAZZ_AK820.exe");
    analyze("File_lab4/exe/anki.exe");
    analyze("File_lab4/exe/mpv.exe");
    analyze("File_lab4/exe/setup.exe");
    analyze("File_lab4/exe/WargamingErrorMonitor.exe");

    cout << "\n--- TXT ---\n";
    analyze("File_lab4/txt/dump.txt");
    analyze("File_lab4/txt/matr_A.txt");
    analyze("File_lab4/txt/raj.txt");
    analyze("File_lab4/txt/results.txt");
    analyze("File_lab4/txt/text.txt");

    cout << "\n--- JPG ---\n";
    analyze("File_lab4/jpg/7.jpg");
    analyze("File_lab4/jpg/8.jpg");
    analyze("File_lab4/jpg/9.jpg");
    analyze("File_lab4/jpg/10.jpg");
    analyze("File_lab4/jpg/11.jpg");

    cout << "\n--- MP3 ---\n";
    analyze("File_lab4/mp3/music1.mp3");
    analyze("File_lab4/mp3/music2.mp3");
    analyze("File_lab4/mp3/music3.mp3");
    analyze("File_lab4/mp3/music4.mp3");
    analyze("File_lab4/mp3/music5.mp3");

    return 0;
}
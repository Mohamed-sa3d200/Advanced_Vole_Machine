#include <bits/stdc++.h>
using namespace std;

class DisplayUtils {
public:
    static string padHex(const string& hex, int width = 4);
    static void printLine(int width = 89);
    static void printHeader(const string& title);
};

class Memory {
private:
    string Mem_of_Cells[256];
public:
    static const int size = 256;
    string getCell(int index);
    string getInstruction(int index);
    bool setInstruction(int &index, const string& instruction);
    void setCell(int index, const string& hexInstr);
    void printMemory();
    bool validateHexInput(const string& input);
};
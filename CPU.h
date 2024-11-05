#include <bits/stdc++.h>
#include "Memory.h"
using namespace std;

class Alu;
class Register {
private:
    int memory[16]{};
    static const int size = 16;

public:
    Register();

    int getCell(int address) const;
    void setCell(int address, int value);
    int decToFloat(float num);
    float floatToDec(int num);
    void printRegisters(Alu alu);
};

class Alu {
private:
    map<char, int> weight{
            {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}
    };
    map<int, char> rev_weight{
            {10, 'A'}, {11, 'B'}, {12, 'C'}, {13, 'D'}, {14, 'E'}, {15, 'F'}
    };

public:
    int hexToDec(const string& s);
    string decToHex(int num);
    bool isValid(int address) const;
    void add(int idx1, int idx2, int idx3, Register& reg);
    void addFloat(int idxR, int idxS, int idxT, Register& reg);
    void bitwiseOr(int idxR, int idxS, int idxT, Register &reg);
    void bitwiseAnd(int idxR, int idxS, int idxT, Register& reg);
    void bitwiseXor(int idxR, int idxS, int idxT, Register& reg);
    void rotateRight(int regIdx, int steps, Register& reg);
};


class Cu {
public:
    void load(int regIdx, int memAddr, Register& reg, Memory& mem);
    void load(int idx_reg,int val,Register &re);
    void store(int regIdx, int memAddr, Register& reg, Memory& mem);
    void move(int regIdx1, int regIdx2, Register& reg);
    void jump(int regIdx, int memAddr, Register& reg, int& pc);
    void jumpGreater(int regIdx, int memAddr, Register& reg, int& pc);
    void halt();
};

class CPU {
private:
    int programCounter;
    string instructionRegister;
    Register reg;
    Alu alu;
    Cu cu;
    void showState(Memory& mem) {
        DisplayUtils::printHeader("CPU State");
        cout << "Program Counter: " << programCounter << endl;
        cout << "Instruction Register: " << instructionRegister << endl;
        DisplayUtils::printLine();
        reg.printRegisters(alu);
        mem.printMemory();
    }


public:
    CPU() : programCounter(0) {}

    void runNextInstruction(Memory& mem);
    void fetch(Memory& mem);
    void decode(Memory& mem);
    void execute(char opcode, Memory& mem);
};
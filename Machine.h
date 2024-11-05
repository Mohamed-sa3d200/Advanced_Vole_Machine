#include <bits/stdc++.h>
#include "CPU.h"


class Machine {
private:
    Memory memory;
    CPU cpu;
    int counter = 16;

public:
    void loadFile(const string& filename);
    void instrInput();
    void outputCells();
    void implement_instructions();
};

class MainUI {
private:
    Machine machine;

public:
    void menu();
    void inputChoice();
    void loadFile();
};

#include "Memory.h"

string Memory::getCell(int index) {
    if (index >= 0 && index < size) {
        return Mem_of_Cells[index].empty() ? "00" : Mem_of_Cells[index];
    }
    else
        return "00";
}

void Memory::setCell(int index, const string &hexInstr) {
    if (index >= 0 && index < size) {
        Mem_of_Cells[index] = hexInstr.substr(0, 2);
    }
}

string Memory::getInstruction(int index) {
    if (index >= 0 && index < size - 1) {
        string firstHalf = getCell(index);
        string secondHalf = getCell(index + 1);
        if (firstHalf == "00"){
            return "0000";
        }
        return firstHalf + secondHalf;
    }
    return "0000";
}

bool Memory::validateHexInput(const string& input) {
    if (input.length() != 4) return false;
    return all_of(input.begin(), input.end(), [](char c) {
        return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
    });
}

bool Memory::setInstruction(int &index, const string& instruction) {
    if (instruction.length() != 4) {
        cout << "Error: Invalid instruction format. Instruction must be exactly 4 hex digits." << endl;
        return false;
    }

    if (index >= 0 && index < size - 1) {
        setCell(index, instruction.substr(0, 2));
        setCell(index + 1, instruction.substr(2, 2));
        return true;
    }

    return false;
}


void Memory::printMemory() {
    DisplayUtils::printHeader("Memory Contents (16x16 Matrix)");

    // Print column headers
    cout << "|     ";
    for (int i = 0; i < 16; i++) {
        cout << "|  " << hex << uppercase << i << "  ";
    }
    cout << "|" << endl;
    DisplayUtils::printLine(89);

    // Print memory contents
    for (int row = 0; row < 16; row++) {
        cout << "| " << hex << uppercase << setw(3) << row << " ";
        for (int col = 0; col < 16; col++) {
            int idx = row * 16 + col;
            string value = Mem_of_Cells[idx];
            if (value.empty()) value = "00";
            cout << "| " << setw(4) << value;
        }
        cout << " |" << endl;
    }
    DisplayUtils::printLine(89);
    cout << dec;
}

string DisplayUtils::padHex(const string &hex, int width) {
    string result = hex;
    if (result.empty()) result = "0";
    while (result.length() < 2) {
        result = "0" + result;
    }
    return result;
}

void DisplayUtils::printLine(int width) {
    cout << string(width, '-') << endl;
}

void DisplayUtils::printHeader(const string &title) {
    printLine();
    cout << "| " << setw(85) << left << title << " |" << endl;
    printLine();
}

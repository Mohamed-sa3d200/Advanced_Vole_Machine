#include "Machine.h"

void Machine::loadFile(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cout << "This file doesn't exist: " << filename << endl;
        return;
    }
    cout << "if you want to start storing from a certain cell. enter 1" << endl;
    cout << "else enter any other key (default start from cell 0x10)" << endl;
    string cell_start_choice;
    cin >> cell_start_choice;
    if (cell_start_choice == "1"){
        cout << "from which cell you want to start (default)" << endl;
        cin >> counter;
    }
    string hexWord;
    while (file >> hexWord && counter < memory.size) {
        memory.setInstruction(counter, hexWord);
        counter += 2;
    }
    // memory.setCell(counter++, "C000");
}

void Machine::instrInput() {
    cout << "if you want to start storing from a certain cell. enter 1" << endl;
    cout << "else enter any other key (default start from cell 10)" << endl;
    string cell_start_choice;
    cin >> cell_start_choice;
    if (cell_start_choice == "1"){
        cout << "from which cell you want to start (default)" << endl;
        cin >> counter;
    }
    cout << "Enter the number of instructions: ";
    int numOfInstr;
    cin >> numOfInstr;

    if (counter + (numOfInstr * 2) >= Memory::size) {
        cout << "Error: Too many instructions for available memory." << endl;
        return;
    }
    if (numOfInstr <= 0) {
        cout << "Invalid number of instructions." << endl;
        return;
    }
    string hexInstr;
    for (int i = 0; i < numOfInstr; i++) {
        cout << "Enter instruction " << (i + 1) << " (4 hex digits): ";
        cin >> hexInstr;

        if (!memory.validateHexInput(hexInstr)) {
            cout << "Error: Invalid instruction format. Try again." << endl;
            i--;  // Retry this instruction
            continue;
        }

        memory.setInstruction(counter, hexInstr);
        counter += 2;
    }
}

void Machine::outputCells() {
    for (int i = 0; i < counter; ++i) {
        if (!memory.getCell(i).empty())
        cout << "Cell " << i << " : " << memory.getCell(i) << endl;
    }
}

void Machine::implement_instructions() {
    cpu.runNextInstruction(memory);
}

void MainUI::menu() {
    cout << "=========== Menu ===========" << endl;
    cout << "1. Load Instruction File" << endl;
    cout << "2. Insert Instructions" << endl;
    cout << "3. Output Active Cells" << endl;
    cout << "4. implement instructions" << endl;
    cout << "5. Exit" << endl;
}

void MainUI::inputChoice() {
    int choice;
    do {
        menu();
        cin >> choice;
        switch (choice) {
            case 1:
                loadFile();
                break;
            case 2:
                machine.instrInput();
                break;
            case 3:
                machine.outputCells();
                break;
            case 4:
                machine.implement_instructions();
                break;
            case 5:
                cout << "Program Ended" << endl;
                break;
            default:
                cout << "Invalid Input" << endl;
                break;
        }
    } while (choice != 4);
}

void MainUI::loadFile() {
    cout << "Enter the file name: ";
    string filename;
    cin >> filename;
    machine.loadFile(filename);
}

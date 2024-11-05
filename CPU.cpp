#include "CPU.h"
Register::Register() {
    for (int & i : memory)
        i = 0;
}

int Register::getCell(int address) const {
    return (address >= 0 && address < size) ? memory[address] : 0;
}

void Register::setCell(int address, int value) {
    if (address >= 0 && address < size) {
        memory[address] = value;
    }
}

int Register::decToFloat(float num) {
    int result = 0;

    // Handle sign bit (1 bit)
    if (num < 0) {
        result |= 0x80;  // Set the sign bit
        num = -num;
    }

    // Split into integral and fractional parts
    int intPart = (int)num;
    float fracPart = num - intPart;

    // Calculate exponent and mantissa
    int exponent = 0;
    float normalizedNum = num;

    if (num != 0) {
        while (normalizedNum >= 2.0) {
            normalizedNum /= 2.0;
            exponent++;
        }
        while (normalizedNum < 1.0) {
            normalizedNum *= 2.0;
            exponent--;
        }
    }

    // Add bias to exponent (bias = 4)
    exponent += 4;
    if (exponent < 0) exponent = 0;
    if (exponent > 7) exponent = 7;

    // Set exponent bits (3 bits)
    result |= (exponent & 0x7) << 4;

    // Calculate mantissa (4 bits)
    normalizedNum -= 1.0; // Remove the 1. from normalized number
    int mantissa = (int)(normalizedNum * 16);
    result |= mantissa & 0xF;

    return result;
}

float Register::floatToDec(int num) {
    // Extract components
    bool sign = (num & 0x80) != 0;
    int exponent = ((num >> 4) & 0x7);
    int mantissa = num & 0xF;

    // Convert to decimal
    float result = 1.0 + (mantissa / 16.0);
    result *= pow(2.0, exponent - 4); // Subtract bias

    return sign ? -result : result;
}

int Alu::hexToDec(const string &s) {
    int decValue = 0;
    for (int i = 0; i < s.size(); ++i) {
        int value = (s[i] >= '0' && s[i] <= '9') ? s[i] - '0' : weight[s[i]];
        decValue = decValue * 16 + value;
    }
    return decValue;
}

string Alu::decToHex(int num) {
    string s;
    while (num > 0) {
        int temp = num % 16;
        s += (temp < 10) ? char(temp + '0') : rev_weight[temp];
        num /= 16;
    }
    reverse(s.begin(), s.end());
    if (s.size() < 2) {
        s.insert(0, 2 - s.size(), '0');
    }
    return s;
}

bool Alu::isValid(int address) const {
    if (address > 255 || address < 0) {
        cout << "Out of range error" << endl;
        return false;
    }
    return true;
}

void Alu::add(int idx1, int idx2, int idx3, Register &reg) {
    cout << "register " << idx2 << " :" << reg.getCell(idx2) << endl;
    cout << "register " << idx3 << " :" << reg.getCell(idx3) << endl;
    reg.setCell(idx1, reg.getCell(idx2) + reg.getCell(idx3));
    cout <<  "register " << idx2 << " + " << "register " << idx3 << " => " << idx1 << endl;
    cout << "register " << idx1 << " :" << reg.getCell(idx1) << endl;

}

void Alu::addFloat(int idxR, int idxS, int idxT, Register &reg) {
    // Get values from registers
    float val1 = reg.floatToDec(reg.getCell(idxS));
    float val2 = reg.floatToDec(reg.getCell(idxT));

    // Perform floating point addition
    float result = val1 + val2;

    // Convert result back to 8-bit float format
    int floatResult = reg.decToFloat(result);

    // Store result
    reg.setCell(idxR, floatResult);

    // Print operation details
    cout << "Floating Point Addition:" << endl;
    cout << "Register " << idxS << " (float): " << val1 << endl;
    cout << "Register " << idxT << " (float): " << val2 << endl;
    cout << "Result in Register " << idxR << " (float): " << result << endl;
}

void Alu::bitwiseOr(int idxR, int idxS, int idxT, Register &reg) {
    int result = reg.getCell(idxS) | reg.getCell(idxT);
    reg.setCell(idxR, result);

    cout << "Bitwise OR Operation:" << endl;
    cout << "Register " << idxS << " : " << reg.getCell(idxS) << endl;
    cout << "Register " << idxT << " : " << reg.getCell(idxT) << endl;
    cout << "Result in Register " << idxR << " : " << result << endl;
}

void Alu::bitwiseAnd(int idxR, int idxS, int idxT, Register &reg) {
    int result = reg.getCell(idxS) & reg.getCell(idxT);
    reg.setCell(idxR, result);

    cout << "Bitwise AND Operation:" << endl;
    cout << "Register " << idxS << " : " << reg.getCell(idxS) << endl;
    cout << "Register " << idxT << " : " << reg.getCell(idxT) << endl;
    cout << "Result in Register " << idxR << " : " << result << endl;
}

void Alu::bitwiseXor(int idxR, int idxS, int idxT, Register &reg) {
    int result = reg.getCell(idxS) ^ reg.getCell(idxT);
    reg.setCell(idxR, result);

    cout << "Bitwise XOR Operation:" << endl;
    cout << "Register " << idxS << " : " << reg.getCell(idxS) << endl;
    cout << "Register " << idxT << " : " << reg.getCell(idxT) << endl;
    cout << "Result in Register " << idxR << " : " << result << endl;
}

void Alu::rotateRight(int regIdx, int steps, Register &reg) {
    int value = reg.getCell(regIdx);
    // Ensure steps is within 0-7 range (for 8-bit values)
    steps = steps & 0x7;

    // Perform rotation
    int result = (value >> steps) | (value << (8 - steps));
    result = result & 0xFF; // Keep only 8 bits

    reg.setCell(regIdx, result);

    cout << "Rotate Right Operation:" << endl;
    cout << "Register " << regIdx << " rotated right by " << steps << " steps" << endl;
    cout << "Original value: " << value << endl;
    cout << "Result: " << result << endl;
}

void Register::printRegisters(Alu alu) {
    DisplayUtils::printHeader("Register Contents");

    // Print register headers
    cout << "|    ";
    for (int i = 0; i < 4; i++) {
        cout << "| R" << setw(2) << left << i << "  ";
    }
    cout << "|" << endl;
    DisplayUtils::printLine();

    // Print register values in 4x4 matrix
    for (int row = 0; row < 4; row++) {
        cout << "| " << setw(2) << row * 4 << " ";
        for (int col = 0; col < 4; col++) {
            int idx = row * 4 + col;
            string hexValue = DisplayUtils::padHex(alu.decToHex(memory[idx]), 4);
            cout << "| " <<  setw(4) << hexValue << " ";
        }
        cout << "|" << endl;
    }
    DisplayUtils::printLine();
}

void Cu::load(int regIdx, int memAddr, Register &reg, Memory &mem) {
    Alu alu;
    reg.setCell(regIdx, alu.hexToDec(mem.getCell(memAddr)));
    cout << " cell " << memAddr << "has been loaded to register " << regIdx << " successfully :)" << endl;
    cout << "register " << regIdx << " :" << reg.getCell(regIdx) << endl;
    cout << "memory cell " << memAddr << " :" << mem.getCell(memAddr) << endl;
    cout<<"-------------------------------------------------------------------------" << endl;
}

void Cu::load(int idx_reg, int val, Register &re) {
    Alu a1;
    re.setCell(idx_reg, val);
    cout << val << " has been loaded to " << idx_reg << " register successfully :)" << endl;
    cout << "register " << idx_reg << " :" << re.getCell(idx_reg) << endl;
    cout << "value :" << val << endl;
    cout<<"-------------------------------------------------------------------------" << endl;
}

void Cu::store(int regIdx, int memAddr, Register &reg, Memory &mem) {
    Alu alu;
    mem.setCell(memAddr, alu.decToHex(reg.getCell(regIdx)));
    cout  << "register " << regIdx << " has been stored to cell " << memAddr << " successfully :)" << endl;
    cout << "register " << regIdx << " : " << reg.getCell(regIdx) << endl;
    cout << "memory " << memAddr << " : " << mem.getCell(memAddr) << endl;
    cout<<"-------------------------------------------------------------------------" << endl;
}

void Cu::move(int regIdx1, int regIdx2, Register &reg) {
    cout << "register " << regIdx1 << " : " << reg.getCell(regIdx1) << endl;
    cout << "register " << regIdx2 << " : " << reg.getCell(regIdx2) << endl;
    reg.setCell(regIdx2, reg.getCell(regIdx1));
    cout << regIdx1 << " register has been copied to " << regIdx2 << " register successfully :)" << endl;
    cout << "register " << regIdx2 << " : " << reg.getCell(regIdx2) << endl;
    cout<<"-------------------------------------------------------------------------" << endl;
}

void Cu::jump(int regIdx, int memAddr, Register &reg, int &pc) {
    if (reg.getCell(regIdx) == reg.getCell(0)) {
        pc = memAddr;
        cout << "Jump to cell " << memAddr << " has been done successfully :)" << endl;
        cout << "now program counter = " << pc << endl;
        cout<<"-------------------------------------------------------------------------" << endl;
    }
}

void Cu::jumpGreater(int regIdx, int memAddr, Register &reg, int &pc) {
    if (reg.getCell(regIdx) > reg.getCell(0)) {
        pc = memAddr;
        cout << "Jump to address " << memAddr << " executed (R" << regIdx << " > R0)" << endl;
    } else {
        cout << "Jump condition not met (R" << regIdx << " <= R0)" << endl;
    }
    cout << "Program Counter: " << pc << endl;
    cout << "-------------------------------------------------------------------------" << endl;
}

void Cu::halt() {
    exit(0);
}

void CPU::runNextInstruction(Memory &mem) {
    while (programCounter < mem.size - 1) {
        string instruction = mem.getInstruction(programCounter);
        if (instruction == "0000") {
            programCounter++;
            continue;
        }

        // Print first memory cell after each instruction
        cout << "Memory Cell 0: " << mem.getCell(0) << endl;

        fetch(mem);
        decode(mem);
        showState(mem);
    }
}

void CPU::fetch(Memory &mem) {
    instructionRegister = mem.getInstruction(programCounter);
    programCounter += 2;
}

void CPU::decode(Memory &mem) {
    char opcode = instructionRegister[0];
    execute(opcode, mem);
}

void CPU::execute(char opcode, Memory &mem) {
    int regNum, memAddr, value;
    switch (opcode) {
        case '1':
            regNum = alu.hexToDec(instructionRegister.substr(1, 1));
            memAddr = alu.hexToDec(instructionRegister.substr(2));
            if (alu.isValid(memAddr)) {
                cu.load(regNum, memAddr, reg, mem);
            }
            break;
        case '2': {
            regNum = alu.hexToDec(instructionRegister.substr(1, 1));
            value = alu.hexToDec(instructionRegister.substr(2));
            if (alu.isValid(value)) {
                cu.load(regNum, value, reg);
            }
            break;
        }
        case '3': {
            regNum = alu.hexToDec(instructionRegister.substr(1, 1));
            value = alu.hexToDec(instructionRegister.substr(2));
            if(alu.isValid(value)){
                cu.store(regNum, value, reg, mem);
            }
            break;
        }
        case '4': {
            regNum = alu.hexToDec(instructionRegister.substr(2, 1));
            int regNum2 = alu.hexToDec(instructionRegister.substr(3, 1));
            if(alu.isValid(regNum2)){
                cu.move(regNum, regNum2, reg);
            }
            break;
        }
        case '5': {
            int regNum0 = alu.hexToDec(instructionRegister.substr(1, 1));
            int regNum1 = alu.hexToDec(instructionRegister.substr(2, 1));
            int regNum2 = alu.hexToDec(instructionRegister.substr(3, 1));
            if(alu.isValid(regNum2)){
                alu.add(regNum0, regNum1, regNum2, reg);
            }
            break;
        }
        case '6': {
            int regNum0 = alu.hexToDec(instructionRegister.substr(1, 1));
            int regNum1 = alu.hexToDec(instructionRegister.substr(2, 1));
            int regNum2 = alu.hexToDec(instructionRegister.substr(3, 1));
            if (alu.isValid(regNum2)) {
                alu.addFloat(regNum0, regNum1, regNum2, reg);
            }
            break;
        }
        case '7': {
            int regNum0 = alu.hexToDec(instructionRegister.substr(1, 1));
            int regNum1 = alu.hexToDec(instructionRegister.substr(2, 1));
            int regNum2 = alu.hexToDec(instructionRegister.substr(3, 1));
            if(alu.isValid(regNum2)){
                alu.bitwiseOr(regNum0, regNum1, regNum2, reg);
            }
            break;
        }
        case '8': {
            int regNum0 = alu.hexToDec(instructionRegister.substr(1, 1));
            int regNum1 = alu.hexToDec(instructionRegister.substr(2, 1));
            int regNum2 = alu.hexToDec(instructionRegister.substr(3, 1));
            if(alu.isValid(regNum2)){
                alu.bitwiseAnd(regNum0, regNum1, regNum2, reg);
            }
            break;
        }

        case '9': {
            int regNum0 = alu.hexToDec(instructionRegister.substr(1, 1));
            int regNum1 = alu.hexToDec(instructionRegister.substr(2, 1));
            int regNum2 = alu.hexToDec(instructionRegister.substr(3, 1));
            if(alu.isValid(regNum2)){
                alu.bitwiseXor(regNum0, regNum1, regNum2, reg);
            }
            break;
        }

        case 'A': {
            int regNum = alu.hexToDec(instructionRegister.substr(1, 1));
            int steps = alu.hexToDec(instructionRegister.substr(3, 1));
            if(alu.isValid(steps)){
                alu.rotateRight(regNum, steps, reg);
            }
            break;
        }


        case 'B': {
            int regNum0 = alu.hexToDec(instructionRegister.substr(1, 2));
            int memAddr = alu.hexToDec(instructionRegister.substr(2));
            if(alu.isValid(memAddr)){
                cu.jump(regNum0, memAddr, reg, programCounter);
            }
            break;
        }
        case 'D': {
            int regNum = alu.hexToDec(instructionRegister.substr(1, 1));
            int memAddr = alu.hexToDec(instructionRegister.substr(2));
            if(alu.isValid(memAddr)){
                cu.jumpGreater(regNum, memAddr, reg, programCounter);
            }
            break;
        }
        case 'C':
            cout << "exit program" << endl;
            cu.halt();
            break;
        default:
            cout << endl << "Invalid Opcode" << endl;
            break;
    }
}

#include <iostream>
#include <iomanip>

using Data = unsigned char;
using Address = unsigned short;

class Bus
{
    private:
        int count = 0;
        Data memory[65536] = {0}; // 64KB of memory

public:
    Data read(Address addr){
        return memory[addr];
    }
    void write(Address addr, Data data){
        memory[addr] = data;
    }


};

class Cpu
{
private:
    Bus *bus;
    Address pc;
    Data a, x, y, sp, status;

    void printHex(Address addr)
    {
        std::cout << std::hex
                  << std::uppercase
                  << std::setw(4)
                  << std::setfill('0')
                  << addr;
    };
    void printHex(Data data)
    {
        std::cout << std::hex
                  << std::uppercase
                  << std::setw(2)
                  << std::setfill('0')
                  << (int)data;
    };
    Data readPC(){
        Data data = bus->read(pc);
        pc++;
        return data;
    }

public:
    Cpu(Bus *bus) : bus(bus)
    {
        reset();
    };
    void reset()
    {
        pc = 0x0000;
        a = x = y = sp = status = 0;
    };
    int execute()
    {
        Data opcode = readPC();
        switch (opcode)
        {
        case 0xA9: // LDA Immediate
            a = readPC();
            break;
        default:
            std::cerr << "Unknown opcode: " << std::hex << (int)opcode << std::endl;
            return 0; // Stop execution on unknown opcode
        }
        return 1;
    }
    void debug()
    {
        std::cout << "PC: ";
        printHex(pc);
        std::cout << " A: ";
        printHex(a);
        std::cout << " X: ";
        printHex(x);
        std::cout << " Y: ";
        printHex(y);
        std::cout << " SP: ";
        printHex(sp);
        std::cout << " STATUS: " << std::bitset<sizeof(Data) * 8>(+status);
        std::cout << std::endl; 
        ;
    }
};

int main()
{
    Bus bus;

    bus.write(0x0000, 0xA9);
    bus.write(0x0001, 0x44);
    
    Cpu cpu(&bus);
    int run = 1;
    while (run)
    {
        cpu.debug();
        run = cpu.execute();
    }
    cpu.debug();
    return 0;
}

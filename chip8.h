#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <array>
#include <windows.h>

class chip8 {
private:
    //Data
	uint8_t memory[4096];
	std::array<uint16_t, 16> stack;
	uint8_t v[16];
	uint16_t opcode, x, y, kk, n, iReg, pc, sp;
    uint8_t delayTimer;
    uint8_t soundTimer;
	bool drawFlag;

public:
    //These need to be accessed outside the class, using setters and getters seems cumbersome
    uint32_t display[64 * 32];
    uint8_t keys[16];
    bool drawHack;

    //Main Functions
	chip8();
	bool loadROM(std::string const& path);
	void run();

private:
    //Opcodes - I'm not writing a big definition for each, go to cowgods chip8 tech ref
    //Array of function pointers coming soon (hence the void funcs)
    void op00E0(); // CLS
    void op00EE(); // RET
    void op1nnn(); // JP addr
    void op2nnn(); // CALL addr
    void op3xkk(); // SE Vx, byte
    void op4xkk(); // SNE Vx, byte
    void op5xy0(); // SE Vx, Vy
    void op6xkk(); // LD Vx, byte
    void op7xkk(); // ADD Vx, byte
    void op8xy0(); // LD Vx, Vy
    void op8xy1(); // OR Vx, Vy
    void op8xy2(); // AND Vx, Vy
    void op8xy3(); // XOR Vx, Vy
    void op8xy4(); // ADD Vx, Vy
    void op8xy5(); // SUB Vx, Vy
    void op8xy6(); // SHR Vx{ , Vy }
    void op8xy7(); // SUBN Vx, Vy
    void op8xyE(); // SHL Vx{ , Vy }
    void op9xy0(); // SNE Vx, Vy
    void opAnnn(); // LD I, addr
    void opBnnn(); // JP V0, addr
    void opCxkk(); // RND Vx, byte
    void opDxyn(); // DRW Vx, Vy, nibble
    void opEx9E(); // SKP Vx
    void opExA1(); // SKNP Vx
    void opFx07(); // LD Vx, DT
    void opFx0A(); // LD Vx, K
    void opFx15(); // LD DT, Vx
    void opFx18(); // LD ST, Vx
    void opFx1E(); // ADD I, Vx
    void opFx29(); // LD F, Vx
    void opFx33(); // LD B, Vx
    void opFx55(); // LD[I], Vx
    void opFx65(); // LD Vx, [I]
};


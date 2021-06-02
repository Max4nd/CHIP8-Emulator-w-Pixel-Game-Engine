#include "chip8.h"

chip8::chip8() :
	memory(), stack(), v(), opcode(), x(), y(), kk(), n(),
	iReg(0), pc(0x200), sp(0), delayTimer(0), soundTimer(0),
	drawFlag(true), display(), keys(), drawHack(true) {
	uint8_t fontset[80] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
		0x20, 0x60, 0x20, 0x20, 0x70,  // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
		0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
		0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
		0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
		0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
		0xF0, 0x80, 0xF0, 0x80, 0x80   // F
	};
	for (int i = 0; i < 80; ++i)
		memory[i] = fontset[i];
}

bool chip8::loadROM(std::string const& path) {
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	if (!file) return false;

	std::ifstream::pos_type file_size = file.tellg();
	if (file_size == std::ifstream::pos_type(-1))
		return false;

	file.seekg(0, std::ios::beg);
	if (!file) return false;

	if (file_size > 4096 - 512) return false;

	std::vector<std::uint8_t> buffer(file_size);
	file.read(reinterpret_cast<char*>(buffer.data()), file_size);

	if (!file) return false;

	for (int i = 0; i < file_size; i++)
		memory[i + pc] = buffer[i];
}

void chip8::run() {
	opcode = memory[pc] << 8 | memory[pc + 1];

	x = (opcode & 0x0F00) >> 8;
	y = (opcode & 0x00F0) >> 4;
	kk = opcode & 0x00FF;
	n = opcode & 0x000F;

	#ifdef _DEBUG
		std::cout << std::hex << "Program Counter: " << pc << " Opcode: " << opcode << "\n";
	#endif

	switch (opcode & 0xF000) {
	case 0x0000:
		switch (opcode) {
		case 0x00E0: op00E0(); break;
		case 0x00EE: op00EE(); break;
		}
		break;
	case 0x1000: op1nnn(); break;
	case 0x2000: op2nnn(); break;
	case 0x3000: op3xkk(); break;
	case 0x4000: op4xkk(); break;
	case 0x5000: op5xy0(); break;
	case 0x6000: op6xkk(); break;
	case 0x7000: op7xkk(); break;
	case 0x8000:
		switch (n) {
		case 0x0000: op8xy0(); break;
		case 0x0001: op8xy1(); break;
		case 0x0002: op8xy2(); break;
		case 0x0003: op8xy3(); break;
		case 0x0004: op8xy4(); break;
		case 0x0005: op8xy5(); break;
		case 0x0006: op8xy6(); break;
		case 0x0007: op8xy7(); break;
		case 0x000E: op8xyE(); break;
		}
		break;
	case 0x9000: op9xy0(); break;
	case 0xA000: opAnnn(); break;
	case 0xB000: opBnnn(); break;
	case 0xC000: opCxkk(); break;
	case 0xD000: opDxyn(); break;
	case 0xE000:
		switch (kk) {
		case 0x009E: opEx9E(); break;
		case 0x00A1: opExA1(); break;
		}
		break;
	case 0xF000:
		switch (kk) {
		case 0x0007: opFx07(); break;
		case 0x000A: opFx0A(); break;
		case 0x0015: opFx15(); break;
		case 0x0018: opFx18(); break;
		case 0x001E: opFx1E(); break;
		case 0x0029: opFx29(); break;
		case 0x0033: opFx33(); break;
		case 0x0055: opFx55(); break;
		case 0x0065: opFx65(); break;
		}
		break;
	}

	if (delayTimer > 0)
		--delayTimer;

	if (soundTimer > 0) {
		if (soundTimer == 1)
			Beep(250, 250); //std::cout << "\a"; lol
		--soundTimer;
	}
}

void chip8::op00E0(){
	pc += 2;
	memset(display, 0, sizeof(display));
	drawFlag = true;
}

void chip8::op00EE(){
	pc = stack[--sp];
	pc += 2;
}

void chip8::op1nnn(){
	pc = opcode & 0x0FFF;
}

void chip8::op2nnn(){
	stack[sp++] = pc;
	pc = opcode & 0x0FFF;
}

void chip8::op3xkk(){
	pc += 2;
	if (v[x] == kk)
		pc += 2;
}

void chip8::op4xkk(){
	pc += 2;
	if (v[x] != kk)
		pc += 2;
}

void chip8::op5xy0(){
	pc += 2;
	if (v[x] == v[y])
		pc += 2;
}

void chip8::op6xkk(){
	pc += 2;
	v[x] = kk;
}

void chip8::op7xkk(){
	pc += 2;
	v[x] += kk;
}

void chip8::op8xy0(){
	pc += 2;
	v[x] = v[y];
}

void chip8::op8xy1(){
	pc += 2;
	v[x] |= v[y];
}

void chip8::op8xy2(){
	pc += 2;
	v[x] &= v[y];
}

void chip8::op8xy3(){
	pc += 2;
	v[x] ^= v[y];
}

void chip8::op8xy4(){
	pc += 2;
	v[0xF] = (v[x] + v[y]) > 0xFF;
	v[x] += v[y];
}

void chip8::op8xy5(){
	pc += 2;
	v[0xF] = v[x] > v[y];
	v[x] -= v[y];
}

void chip8::op8xy6(){
	pc += 2;
	v[0xF] = v[x] & 1;
	v[x] >>= 1;
}

void chip8::op8xy7(){
	pc += 2;
	v[0xF] = v[y] > v[x];
	v[x] = v[y] - v[x];
}

void chip8::op8xyE(){
	pc += 2;
	v[0xF] = v[x] >> 7;
	v[x] <<= 1;
}

void chip8::op9xy0(){
	pc += 2;
	if (v[x] != v[y])
		pc += 2;
}

void chip8::opAnnn(){
	pc += 2;
	iReg = opcode & 0xFFF;
}

void chip8::opBnnn(){
	pc = (opcode & 0xFFF) + v[0];
}

void chip8::opCxkk(){
	pc += 2;
	v[x] = rand() % 255 & kk;
}

void chip8::opDxyn(){
	//This was tough, very much taken from a stackexchange forum post, will link in readme
	pc += 2;
	drawFlag = true;
	drawHack = !drawHack;
	v[0xF] = 0;
	std::uint8_t pixel_row;
	for (int col = 0; col < n; ++col) {
		pixel_row = memory[iReg + col];
		for (int row = 0; row < 8; ++row) {
			if (pixel_row & (0b10000000 >> row)) {
				std::uint16_t cord = (v[x] + row + ((v[y] + col) * 64)) % 2048;
				bool hit = (display[cord] == 1);
				v[0xF] |= hit;
				display[cord] ^= 1;
			}
		}
	}
}

void chip8::opEx9E(){
	pc += 2;
	if (keys[v[x]])
		pc += 2;
}

void chip8::opExA1(){
	pc += 2;
	if (!keys[v[x]]) 
		pc += 2;
}

void chip8::opFx07(){
	pc += 2;
	v[x] = delayTimer;
}

void chip8::opFx0A(){
	pc += 2;
	bool waiting = true;
	for (int i = 0; i < 16; ++i) {
		if (keys[i] != 0) {
			v[x] = i;
			waiting = false;
			break;
		}
	}
	if (waiting) 
		return;
}

void chip8::opFx15(){
	pc += 2;
	delayTimer = v[x];
}

void chip8::opFx18(){
	pc += 2;
	soundTimer = v[x];
}

void chip8::opFx1E(){
	pc += 2;
	v[0xF] = (iReg + v[x]) > 0xFFF;
	iReg += v[x];
}

void chip8::opFx29(){
	pc += 2;
	iReg = v[x] * 5;
}

void chip8::opFx33(){
	pc += 2;
	memory[iReg] = v[x] / 100;
	memory[iReg + 1] = (v[x] / 10) % 10;
	memory[iReg + 2] = v[x] % 10;
}

void chip8::opFx55(){
	pc += 2;
	for (int i = 0; i <= x; i++)
		memory[iReg + i] = v[i];
}

void chip8::opFx65(){
	pc += 2;
	for (int i = 0; i <= x; i++)
		v[i] = memory[iReg + i];
}
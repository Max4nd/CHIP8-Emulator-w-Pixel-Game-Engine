#include "olcPixelGameEngine.h"
#include "chip8.h"

class Example : public olc::PixelGameEngine {
public:
	Example() {
		sAppName = "Example";
	}
private:
	chip8 testChip8;
	int upScale = 10;

public:
	bool OnUserCreate() override {
		//Called once at the start
		testChip8.loadROM("ROMS\\TESTROM.bin");
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		//Called each frame

		//Clears the screen every second draw func, not sure if this makes it look better
		if (testChip8.drawHack == true)
			Clear(olc::BLACK);

		if (GetKey(olc::Key::K1).bHeld) testChip8.keys[0] = 1;
		else if (GetKey(olc::Key::K2).bHeld) testChip8.keys[1] = 1;
		else if (GetKey(olc::Key::K3).bHeld) testChip8.keys[2] = 1;
		else if (GetKey(olc::Key::K4).bHeld) testChip8.keys[3] = 1;
		else if (GetKey(olc::Key::Q).bHeld) testChip8.keys[4] = 1;
		else if (GetKey(olc::Key::W).bHeld) testChip8.keys[5] = 1;
		else if (GetKey(olc::Key::E).bHeld) testChip8.keys[6] = 1;
		else if (GetKey(olc::Key::R).bHeld) testChip8.keys[7] = 1;
		else if (GetKey(olc::Key::A).bHeld) testChip8.keys[8] = 1;
		else if (GetKey(olc::Key::S).bHeld) testChip8.keys[9] = 1;
		else if (GetKey(olc::Key::D).bHeld) testChip8.keys[10] = 1;
		else if (GetKey(olc::Key::F).bHeld) testChip8.keys[11] = 1;
		else if (GetKey(olc::Key::Z).bHeld) testChip8.keys[12] = 1;
		else if (GetKey(olc::Key::X).bHeld) testChip8.keys[13] = 1;
		else if (GetKey(olc::Key::C).bHeld) testChip8.keys[14] = 1;
		else if (GetKey(olc::Key::V).bHeld) testChip8.keys[15] = 1;

		if (GetKey(olc::Key::K1).bReleased) testChip8.keys[0] = 0;
		else if (GetKey(olc::Key::K2).bReleased) testChip8.keys[1] = 0;
		else if (GetKey(olc::Key::K3).bReleased) testChip8.keys[2] = 0;
		else if (GetKey(olc::Key::K4).bReleased) testChip8.keys[3] = 0;
		else if (GetKey(olc::Key::Q).bReleased) testChip8.keys[4] = 0;
		else if (GetKey(olc::Key::W).bReleased) testChip8.keys[5] = 0;
		else if (GetKey(olc::Key::E).bReleased) testChip8.keys[6] = 0;
		else if (GetKey(olc::Key::R).bReleased) testChip8.keys[7] = 0;
		else if (GetKey(olc::Key::A).bReleased) testChip8.keys[8] = 0;
		else if (GetKey(olc::Key::S).bReleased) testChip8.keys[9] = 0;
		else if (GetKey(olc::Key::D).bReleased) testChip8.keys[10] = 0;
		else if (GetKey(olc::Key::F).bReleased) testChip8.keys[11] = 0;
		else if (GetKey(olc::Key::Z).bReleased) testChip8.keys[12] = 0;
		else if (GetKey(olc::Key::X).bReleased) testChip8.keys[13] = 0;
		else if (GetKey(olc::Key::C).bReleased) testChip8.keys[14] = 0;
		else if (GetKey(olc::Key::V).bReleased) testChip8.keys[15] = 0;

		testChip8.run();

		for (int i = 0; i < 64 * 32; i++) {
			int x = i % 64 * upScale;
			int y = i / 64 * upScale;
			if (testChip8.display[i])
				FillRect(x, y, upScale, upScale, olc::GREEN);
		}

		return true;
	}
};

int main() {
	Example demo;
	if (demo.Construct(640, 320, 2, 2))
		demo.Start();

	return 0;
}
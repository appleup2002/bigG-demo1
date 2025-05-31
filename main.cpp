#include <graphics.h>
#include <string>
#include <vector>

void putImageAlpha(int x, int y, IMAGE* img);
void playerMove(bool* isMove);
void drawPlayer(bool* isMove);

class Animation {
public:
	Animation(std::wstring path, int num, int interval) {
		intervalMs = interval;
		totalFrame = num;
		for (size_t i = 0; i < num; i++)
		{
			std::wstring nowPath;
			if (num > 1)
				nowPath = path + std::to_wstring(i) + L".png";
			else
				nowPath = path;
			IMAGE* frame = new IMAGE();
			loadimage(frame, nowPath.c_str());
			frameList.push_back(frame);
		}
	}
	~Animation() {
		for (size_t i = 0; i < totalFrame; i++)
		{
			delete frameList[i];
		}
	}

	void Play(int x, int y, int delta,bool* isMove) {
		timer += delta;
		if (timer >= intervalMs) {
			playerMove(isMove);
			frameIdx = (frameIdx + 1) % totalFrame;
			timer = 0;
		}
		putImageAlpha(x, y, frameList[frameIdx]);
	}

 private:
	int totalFrame = 0;
	int timer = 0;
	int frameIdx = 0;
	int intervalMs = 0;
	std::vector<IMAGE*> frameList;
};

int FPS = 120;

bool running = true;
bool isMove[4] = { false, false, false, false };

int playerSpeed = 1;

Animation mainCharStand(L"img/characters/mainchar/stand/stand-", 4, 1000 / 60 * 4);
Animation mainCharDown(L"img/characters/mainchar/walk/down", 4, 1000 / 60 * 4);
Animation mainCharUp(L"img/characters/mainchar/walk/up", 4, 1000 / 60 * 4);
Animation mainCharLeft(L"img/characters/mainchar/walk/left", 4, 1000 / 60 * 4);
Animation mainCharRight(L"img/characters/mainchar/walk/right", 4, 1000 / 60 * 4);
Animation bkgLanscape(L"img/basckgrounds/landscape.png", 1, 45);

POINT playerPos = {500, 500};

ExMessage msg;

void putImageAlpha(int x, int y, IMAGE* img) {
	int width = img->getwidth();
	int hight = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, width, hight,
		GetImageHDC(img), 0, 0, width, hight, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

void playerMove(bool* isMove) {
	if (isMove[0]) 
		playerPos.y -= playerSpeed;
	if (isMove[1])
		playerPos.y += playerSpeed;
	if (isMove[2])
		playerPos.x -= playerSpeed;
	if (isMove[3])
		playerPos.x += playerSpeed;
}

void drawPlayer(bool* isMove) {
	if (isMove[2])
		mainCharLeft.Play(playerPos.x, playerPos.y, 1000 / FPS, isMove);
	else if (isMove[3])
		mainCharRight.Play(playerPos.x, playerPos.y, 1000 / FPS, isMove);
	else if (isMove[0])
		mainCharUp.Play(playerPos.x, playerPos.y, 1000 / FPS, isMove);
	else if(isMove[1])
		mainCharDown.Play(playerPos.x, playerPos.y, 1000 / FPS, isMove);
	else
		mainCharStand.Play(playerPos.x, playerPos.y, 1000 / FPS, isMove);
}


int main() {

	initgraph(1280, 720);

	BeginBatchDraw();

	while (running) {

		DWORD start_time = GetTickCount();

		while (peekmessage(&msg)) {
			if (msg.message == WM_KEYDOWN)
			{
				switch (msg.vkcode) {
				case VK_UP:
					isMove[0] = true;
					break;
				case VK_DOWN:
					isMove[1] = true;
					break;
				case VK_LEFT:
					isMove[2] = true;
					break;
				case VK_RIGHT:
					isMove[3] = true;
					break;
				}
			}
			else if (msg.message == WM_KEYUP)
			{
				switch (msg.vkcode) {
				case VK_UP:
					isMove[0] = false;
					break;
				case VK_DOWN:
					isMove[1] = false;
					break;
				case VK_LEFT:
					isMove[2] = false;
					break;
				case VK_RIGHT:
					isMove[3] = false;
					break;
				}
			}
		}

		//playerMove(isMove);

		cleardevice();

		bkgLanscape.Play(0, 0, 1000 / FPS, isMove);
		drawPlayer(isMove);

		FlushBatchDraw();
		
		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;

		if (delta_time < 1000 / FPS) {
			Sleep(1000 / FPS - delta_time);
		}

	}
}
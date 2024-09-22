#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <iostream>
 
struct ball {
	float x, y;
	float speedX, speedY;
	float radius;

	void draw() {
		DrawCircle((int) x, (int) y, radius, WHITE);
	}
	void reset() {
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;
		speedX *= -0.5f;
		speedY *= -0.5f;
	}
};

struct paddle {
	float x, y;
	float speed;
	float width, hight;

	Rectangle GetReact() {
		return Rectangle{ x - width / 2, y - hight / 2, 10, 100 };
	}
	void draw() {
		DrawRectangleRec(GetReact(), WHITE);
	}
	void reset() {
		y = GetScreenHeight() / 2;
		speed = 500;
	}
};

struct score {
	int score;
};

int main() {

	InitWindow(800, 600, "Pong");
	InitAudioDevice();
	SetTargetFPS(1000);

	ball Bll;
	Bll.x = GetScreenWidth() / 2.0f;
	Bll.y = GetScreenHeight() / 2.0f;
	Bll.radius = 5;
	Bll.speedX = 0;
	Bll.speedY = 0;

	paddle lpad;
	lpad.x = 50;
	lpad.y = GetScreenHeight() / 2.0f;
	lpad.width = 10;
	lpad.hight = 100;
	lpad.speed = 500;

	paddle rpad;
	rpad.x = GetScreenWidth() - 50;
	rpad.y = GetScreenHeight() / 2;
	rpad.width = 10;
	rpad.hight = 100;
	rpad.speed = 500;

	score player;
	score comp;
	player.score = 0;
	comp.score = 0;

	const char* initialtext = "Press space to start";
	const int textw = MeasureText(initialtext, 40);

	//loading sound
	Sound collison = LoadSound("paddcollison.wav");
	Sound score = LoadSound("score.wav");

	while (!WindowShouldClose()) {

		Bll.x += Bll.speedX * GetFrameTime();
		Bll.y += Bll.speedY * GetFrameTime();

		if (IsKeyPressed(KEY_SPACE)) {
			initialtext = nullptr;
		}

		//input
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
			lpad.y -= lpad.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
			lpad.y += lpad.speed * GetFrameTime();
		}

		//movementlimit
		if (lpad.y-lpad.hight/2 < 0) {
			lpad.y = lpad.hight/2;
		}
		if (lpad.y+lpad.hight/2 > GetScreenHeight()) {
			lpad.y = GetScreenHeight()-lpad.hight/2;
		}
		
		//collision check
		if (CheckCollisionCircleRec(Vector2{Bll.x,Bll.y},Bll.radius, rpad.GetReact())) {
			if (Bll.speedX > 0) {
				Bll.speedX *= -1.3f;
				Bll.speedY = (Bll.y - lpad.y) / (lpad.hight / 2) * Bll.speedX;
				rpad.speed *= 1.005f;
			}
			PlaySound(collison);
		}
		if (CheckCollisionCircleRec(Vector2{ Bll.x,Bll.y }, Bll.radius, lpad.GetReact())) {
			if (Bll.speedX < 0) {
				Bll.speedX *= -1.3f;
				Bll.speedY = (Bll.y - rpad.y) / (rpad.hight / 2) * Bll.speedX;
				lpad.speed *= 1.01;
			}
			PlaySound(collison);
		}
		//ball physics
		if (Bll.speedY > 600) {
			Bll.speedY = 600;
		}
		if (abs((int) Bll.speedX) < 50) {
			Bll.speedX *= 2;
		}
		if (abs((int)Bll.speedX > 600)) {
			Bll.speedX = 600;
		}		
		
		if (Bll.y < 0 || Bll.y > GetScreenHeight()) {
			if (Bll.y < 0) {
				Bll.y = 0;
			}
			if (Bll.y > GetScreenHeight()) {
				Bll.y = GetScreenHeight();
			}
			Bll.speedY *= -1;
			PlaySound(collison);
		}
		
		//scoring system
		if (Bll.x < 0) {
			Bll.reset();
			lpad.reset();
			rpad.reset();
			comp.score++;
			PlaySound(score);
		}
		if (Bll.x > GetScreenWidth()) {
			Bll.reset();
			lpad.reset();
			rpad.reset();
			player.score++;
			PlaySound(score);
		}

		//movement control
		if (Bll.y > rpad.y) {
			rpad.y += rpad.speed * GetFrameTime();
		}
		if (Bll.y < rpad.y) {
			rpad.y -= rpad.speed * GetFrameTime();
		}
		if (rpad.y - rpad.hight / 2 < 0) {
			rpad.y = rpad.hight / 2;
		}
		if (rpad.y + rpad.hight / 2 > GetScreenHeight()) {
			rpad.y = GetScreenHeight() - rpad.hight / 2;
		}

		//rendering
		BeginDrawing();
			ClearBackground(BLACK);

			DrawText(initialtext, GetScreenWidth() / 2 - textw / 2, GetScreenHeight() / 2, 40, GRAY);
			DrawText(TextFormat("Player: %d",player.score), GetScreenWidth() / 4-50, 5, 20, WHITE);
			DrawText(TextFormat("Computer: %d",comp.score), 3 * GetScreenWidth() / 4-50, 5, 20, WHITE);

			if (initialtext == nullptr) {
				Bll.draw();
				if (Bll.speedX == 0 && Bll.speedY == 0) {
					Bll.speedX = 200;
					Bll.speedY = 600;
				}
			}
			lpad.draw();
			rpad.draw();

		EndDrawing();
	}	
	CloseAudioDevice();
	CloseWindow();

	return 0;
}
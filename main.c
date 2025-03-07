/*
Before the Merge, the Chris Chan FNAF game.

This is the third time I'm trying to make this game in a third engine.
Hopefully, this will go okay, as I really like the idea.

1/6/25
*/
#include <stdio.h>
#include <time.h>
#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define WINDOW_W 1000
#define WINDOW_H 600

#define CAM_NUM 6

typedef enum GameScreen { MENU = 0, OFFICE, CAMERAS } GameScreen;

typedef struct Chris {
	Texture texture;
	Vector2 position;
	int height, width;
	int cam_location;
        int AI; int stress;
	bool active;
	bool girlfriendFree;
}Chris;

typedef struct CWC{ //CWC is chrischan sonichu
	Texture texture;
	Vector2 position;
	int height, width;
	int cam_location;
	int AI;
	bool active, primed_gen, primed_attack, primed_cam;
	bool girlfriendFree;
}CWC;

typedef struct Sonichu {
	Texture texture;
	Vector2 position;
	int height, width;
	int cam_location;
	int AI;
	bool primed;
	bool active;
	bool girlfriendFree;
}Sonichu;

typedef struct Rosechu {
	Texture texture;
	Vector2 position;
	int height, width;
	int cam_location;
        int AI;
	bool primed;
	bool active;
	bool boyfriendFree;
}Rosechu;

typedef struct Combo{
	Texture texture;
	Sound laugh;
	Vector2 position;
	int height, width;
	int cam_location;
	int AI;
	bool active;
	char *name;
}Combo;

typedef struct Cams{
	Texture texture;
	bool out_of_order;
}Cams;

static Chris chris = { 0 }; static CWC chrischan_sonichu = { 0 };
static Sonichu sonichu = { 0 };
static Rosechu rosechu = { 0 };
static Combo chaotic_combo[4] = { 0 };  //INIT array of 4 chaotic combo fellers
static GameScreen currentScreen;

static Texture menu;
static Cams cams[CAM_NUM]; static Texture ooo_cam;
static Texture office[3];
static Texture door; static Texture shadow; static Texture darkness; bool light;
static Texture you_win, you_lose, the_end;

static Sound call;
static Sound scream;
static Sound sigh[3];
static Sound end_lines[4];
static Sound shock[3];
static Sound footsteps, combo_footsteps;
static Sound cam_fail;

static Sound fan;
static Sound v_static;
static Sound cam_click;
static Sound open_cam;
static Sound cwc_power;
static Sound whistle;

//basic vars
int curr_cam;
int curr_office;
int battery;
char night_char; static int night_int; char time_string[5] = "12 AM";
char call_string[28] = "audio/calls/night_0_call.wav"; 
static bool win, lose, saved;

bool door_closed; Sound close_door; Sound open_door;
bool cam_scare;
//night timing vars
int attack_frames, combo_frames;
int night_frames;

int combo_no; static bool magichan_summon;

//jumpscare vars
static Texture chris_scare; static Texture cwc_scare;
int jumpscare_timer;
int shake_interval, scare_x, scare_frames; //coding a jumpscare in C is very interesting...

static void Menu(void);
static void InitGame(void);
static void InitCams(void);
static void InitNight(void);
static void Update(void);
static void Draw(void);
static void Unload(void);
static void UpdateDrawFrame(void);
static void GameWin(void);

//logic functions
static void InitCombo(void); //for initializing chaotic combo
static void Chris_Move(void);
static void Sonichu_Move(void);
static void Rosechu_Move(void);
static void Combo_Move(void);
static void Deactivate_All(void);
static void BatteryZero(void);

static void SummonMagiChan(void);
static void InitCWC(void);
static void CWCMove(void);

static bool isMenu; static bool aSound;

int main ()
{
	//File i/o stuff for selecting night
	FILE* ptr;
	ptr = fopen("resources/night.txt", "r");


	if (ptr == NULL){ 
		printf("Error in opening file\n");
		return 1;
	}
	printf("File Opened\n");

	fscanf(ptr, "%d", &night_int);
	printf("Night number %d\n", night_int);

	fclose(ptr);

	//random seed
	time_t currentTime;
	time(&currentTime);
	SetRandomSeed(currentTime);

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(WINDOW_W, WINDOW_H, "Before the Merge");
//	SetWindowMaxSize(WINDOW_W*2, WINDOW_H*2);
//	SetWindowMinSize(WINDOW_W, WINDOW_H);


	SetTargetFPS(60);
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	InitAudioDevice();

	SetWindowIcon(LoadImage("images/thumbnail.png"));
	isMenu = true;

	Menu();

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{	

		//where the game happens
		UpdateDrawFrame();
		
	}

	// cleanup
	// unload our texture so it can be cleaned up
	Unload();

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

void Menu(){ //currently doesnt work. Find a way to fix this, I think im on the right track.
	menu = LoadTexture("images/cover_image.png"); Music song = LoadMusicStream("audio/So_need_a_cute_girl_NES.wav");
	currentScreen = MENU;
	SetMusicVolume(song, 0.5);
	PlayMusicStream(song);

	while (isMenu){
		if (currentScreen == MENU){
			BeginDrawing();
				ClearBackground(PURPLE);
				DrawTexture(menu, 0, 0, WHITE);
				if (night_int > 1 && night_int < 6){
					DrawText(TextFormat("Press SPACE for Night %d", night_int), 40, 460, 30, WHITE);
					DrawText("Press N for New Game", 40, 500, 30, WHITE);
				}
				else if (night_int >= 6){
					DrawText("Press SPACE for THE MERGE", 40, 460, 30, RED);
					DrawText("Press N for New Game", 40, 500, 30, WHITE);
				}
				else{
					DrawText("Press SPACE to Start", 40, 480, 30, WHITE);
				}
				DrawText("Copyright EldieSoft, 2025.	Based on FNAF by Scott Cawthon", 20, 580, 20, GREEN);
			EndDrawing();
			if (!IsMusicStreamPlaying(song)){
				PlayMusicStream(song);
			}
			UpdateMusicStream(song);
	
			if (IsKeyPressed(KEY_SPACE)){
				BeginDrawing();ClearBackground(BLACK); DrawText("Loading...", 20, 580, 20, WHITE); EndDrawing();
				currentScreen = OFFICE;
				isMenu = false;
				StopMusicStream(song);
		
			}
			if (IsKeyPressed(KEY_N)){
				BeginDrawing();ClearBackground(BLACK); DrawText("Loading...", 20, 580, 20, WHITE); EndDrawing();
				currentScreen = OFFICE; night_int = 1;
				isMenu = false;
				StopMusicStream(song);
			}
		}
	
	}
	InitGame();
	UnloadTexture(menu);
	UnloadMusicStream(song);
}

void InitGame(void) {
	chris.width = 230; chris.height = 394;
	chris.texture = LoadTexture("images/enemies/Chris.png");
	chris.position = (Vector2) { (float)WINDOW_W/2 - chris.width/2, (float)WINDOW_H/2 - chris.height/2 };
	chris.cam_location = -1;
	chris.stress = 0; //movement mechanic
	chris.girlfriendFree = true; chris.active = true;
	
	sonichu.width = 248; sonichu.height = 382;
	sonichu.texture = LoadTexture("images/enemies/Sonichu.png");
	sonichu.position = (Vector2) { 700, (float)WINDOW_H/2 - sonichu.height/2};
	sonichu.cam_location = -1;
	sonichu.girlfriendFree = false; sonichu.active = true; sonichu.primed = false;

	rosechu.width = 192; rosechu.height = 324;
	rosechu.texture = LoadTexture("images/enemies/Rosechu.png");
	rosechu.position = (Vector2) {(float)WINDOW_W/2 - rosechu.width/2, (float)WINDOW_H/2 - rosechu.height/2};
	rosechu.cam_location = -1;
	rosechu.boyfriendFree = false; rosechu.active = true; rosechu.primed = false;


	InitCombo();

	InitCams();

	office[0] = LoadTexture("images/office_pics/office_left.png");
	office[1] = LoadTexture("images/office_pics/office_forward.png");
	office[2] = LoadTexture("images/office_pics/office_right.png");
	door = LoadTexture("images/office_pics/door.png"); shadow = LoadTexture("images/office_pics/chris_shadow.png"); darkness = LoadTexture("images/office_pics/darkness.png");
	light = false;
	curr_cam = 0;
	curr_office = 1;
	battery = 10000;

	you_win = LoadTexture("images/you_win.png"); you_lose = LoadTexture("images/you_lose.png");
	the_end = LoadTexture("images/the_end.png");
	door_closed = false;
	cam_scare = true; //mechanic for scaring sonichu or rosechu or chaotic combo back to -1
       	InitNight();
	call_string[18] = night_char;
	call = LoadSound(call_string);

	//init jumpscare
	scream = LoadSound("audio/cwc_lines/jumpscare_cwc.wav"); //scream is about 240 frames long
	chris_scare = LoadTexture("images/enemies/Chris_scare.png");
	jumpscare_timer = shake_interval = scare_x = scare_frames = 0; 

	sigh[0] = LoadSound("audio/cwc_lines/sigh1.wav");
	sigh[1] = LoadSound("audio/cwc_lines/sigh2.wav");
	sigh[2] = LoadSound("audio/cwc_lines/sigh3.wav");

	end_lines[0] = LoadSound("audio/cwc_lines/and_now_for_something_completely_different.wav");
	end_lines[1] = LoadSound("audio/cwc_lines/CURSEYEHAMEHA.wav");
	end_lines[2] = LoadSound("audio/cwc_lines/electric_hedgehog_power.wav");
	end_lines[3] = LoadSound("audio/cwc_lines/My_name_is_not_Ian.wav");

	shock[0] = LoadSound("audio/sfx/electric1.wav");
	shock[1] = LoadSound("audio/sfx/electric2.wav");
	shock[2] = LoadSound("audio/sfx/electric3.wav");

	cam_fail = LoadSound("audio/sfx/cam_fail.wav");
	SetSoundVolume(cam_fail, 0.2);

	footsteps = LoadSound("audio/sfx/footsteps.wav");

	fan = LoadSound("audio/sfx/fan.wav"); SetSoundVolume(fan, 0.2);
	v_static = LoadSound("audio/sfx/static.wav");

	cam_click = LoadSound("audio/sfx/click_cam.wav");
	open_cam = LoadSound("audio/sfx/open_cam.wav");
	SetSoundVolume(open_cam, 0.45);

	close_door = LoadSound("audio/sfx/door_close.wav"); open_door = LoadSound("audio/sfx/door_open.wav");

	whistle = LoadSound("audio/sfx/whistle.wav"); SetSoundVolume(whistle, 0.5);

	attack_frames = 0; night_frames = 1;

	currentScreen = OFFICE; win = lose = saved = aSound = false;

	PlaySound(call);
}

void AllCamsInOrder(){ //helper function for InitCams
	for (int i = 0; i < 6; i++)
		cams[i].out_of_order = false;
	printf("All cams are working!\n");
}

void InitCams(){
	cams[0].texture = LoadTexture("images/cameras/door_camera.png");
	cams[1].texture = LoadTexture("images/cameras/office_camera.png");
	cams[2].texture = LoadTexture("images/cameras/generator_camera.png");
	cams[3].texture = LoadTexture("images/cameras/hallway_camera.png");
	cams[4].texture = LoadTexture("images/cameras/kitchen_camera.png");
	cams[5].texture = LoadTexture("images/cameras/executive_hallway_camera.png");

	ooo_cam = LoadTexture("images/cameras/out_of_order.png");
	AllCamsInOrder();
}

void InitNight()
{
	switch (night_int){ 
		case 1:
			night_char = '1';
			chris.AI = 0;sonichu.active = false;rosechu.active = false;
			chaotic_combo[0].active = false;
			break;
		case 2:
			night_char = '2';
			chris.AI = 2; sonichu.AI = 1; rosechu.active = false;
			chaotic_combo[0].active = false;
			break;
		case 3:
			night_char = '3';
			chris.AI = 4; sonichu.AI = 4; rosechu.AI = 2;
			chaotic_combo[0].active = false;
			break;
		case 4:
			night_char = '4';
			chris.AI = 8; sonichu.AI = 4; rosechu.AI = 8;
			for (int i = 0; i < 4; i++)
				chaotic_combo[i].AI = 1;
			break;
		case 5:
			night_char = '5';
			chris.AI = 10; sonichu.AI = 6; rosechu.AI = 10;
			for (int i = 0; i < 4; i++)
				chaotic_combo[i].AI = 5;
			break;
		default:
			night_char = '6';
			chris.AI = 20; sonichu.AI = 20; rosechu.AI = 20;
			for (int i = 0; i < 4; i++)
				chaotic_combo[i].AI = 20;
			break;
	}
}

void Draw(void) {
	BeginDrawing();
		ClearBackground(PURPLE);


		if (win){
			if (night_int <= 4){
				DrawTexture(you_win, 0, 0, WHITE);
				DrawText("Press SPACE to continue...", 330, 438, 30, WHITE);
				if (IsKeyPressed(KEY_SPACE)){
					isMenu = true;
					Menu();
				}
			}
			else if (night_int == 5){
				DrawTexture(the_end, 0, 0, WHITE);
				if (IsKeyPressed(KEY_SPACE)){
					isMenu = true;
					Menu();
					}
				}
			else {
				DrawTexture(the_end, 0, 0, WHITE);
				DrawText("Congrats! Look in the resource folders for some goodies!!", 20, 560, 30, BLACK);
				if (IsKeyPressed(KEY_SPACE)){
					isMenu = true;
					Menu();
				}
			}
		}
		
		else if (lose){
			DrawTexture(you_lose, 0, 0, WHITE);
			DrawText("Press SPACE to continue...", 330, 438, 30, WHITE);
			if (IsKeyPressed(KEY_SPACE)){
				isMenu = true;
				Menu();
			}
		}
		else {
		//draw cams
		switch (currentScreen) {
			case OFFICE:
				DrawTexture(office[curr_office], 0, 0, WHITE);
				if (night_frames < 1800)
					DrawText("Press LEFT and RIGHT to look around the office. Press Q to close door, and hold L CTRL for flashlight.", 50, 60, 18, BLACK);
				if (!light && curr_office == 1 && !door_closed)
					DrawTexture(darkness, 440, 115, WHITE);
				if ((chris.cam_location == 6 || chrischan_sonichu.cam_location == 6) && curr_office == 1 && light)
					DrawTexture(shadow, 440, 115, WHITE);
				if (door_closed && curr_office == 1)
					DrawTexture(door, 440, 115, WHITE);
				if (curr_office == 0){
					if (IsKeyPressed(KEY_X))
						DrawText("FIXING CAMS...", 207, 297, 20, BLACK);
					if (IsKeyDown(KEY_Z))
						DrawText("CHARGING...", 290, 480, 20, BLACK);
					if (cam_scare)
						DrawText("SCARE USE: 1", 600, 404, 30, GREEN);
					else
						DrawText("SCARE USE: 0", 600, 404, 30, RED);
				}
				if (curr_office == 2){
					if (chaotic_combo[0].cam_location == 8)
						DrawTexture(chaotic_combo[0].texture, 90, 0, WHITE);
					if (chaotic_combo[1].cam_location == 8)
						DrawTexture(chaotic_combo[1].texture, 582, 0, WHITE);
					if (chaotic_combo[2].cam_location == 8)
						DrawTexture(chaotic_combo[2].texture, 160, 330, WHITE);
					if (chaotic_combo[3].cam_location == 8)
						DrawTexture(chaotic_combo[3].texture, 732, 331, WHITE);
				}
				break;
			case CAMERAS:
				if (cams[curr_cam].out_of_order || battery <= 0){
					if (!IsSoundPlaying(v_static))
						PlaySound(v_static);
					DrawTexture(ooo_cam, 0, 0, WHITE);}
				else
					DrawTexture(cams[curr_cam].texture, 0, 0, WHITE);
				battery-=((1*night_int)/2)*3;
				if (night_frames < 1800)
					DrawText("Press UP and DOWN to navigate the cameras. Press C to scare enemies.", 150, 60, 20, BLACK);
				break;
		}	
		//draw hud
		if (currentScreen == CAMERAS){
		       	DrawText(TextFormat("CAMERA NO. %d", curr_cam+1), 20, 20, 20, BLACK);
			if (cam_scare){
				DrawText("SCARE BUTTON: C", 800, 550, 20, GREEN);
				DrawText("SCARE USE: 1", 800, 580, 20, GREEN);
			}
			else
				DrawText("SCARE USE: 0", 820, 580, 20, RED);
		}
		DrawText(TextFormat("BATTERY: %d", battery/100), 20, 580, 20, BLACK);
		DrawText(time_string, 900, 20, 20, BLACK); DrawText(TextFormat("Night %d", night_int), 900, 40 ,20, BLACK);

		//draw characters in proper cams
		if (chris.cam_location == curr_cam && currentScreen == CAMERAS && !cams[curr_cam].out_of_order)
			DrawTexture(chris.texture, chris.position.x, chris.position.y, WHITE);
		
		if (sonichu.cam_location == curr_cam && currentScreen == CAMERAS && !cams[curr_cam].out_of_order)
			DrawTexture(sonichu.texture, sonichu.position.x, sonichu.position.y, WHITE);
		if (rosechu.cam_location == curr_cam && currentScreen == CAMERAS && !cams[curr_cam].out_of_order)
			DrawTexture(rosechu.texture, rosechu.position.x, rosechu.position.y, WHITE);

		if (chrischan_sonichu.cam_location == curr_cam && currentScreen == CAMERAS && !cams[curr_cam].out_of_order)
			DrawTexture(chrischan_sonichu.texture, chrischan_sonichu.position.x, chrischan_sonichu.position.y, WHITE); //I immediately regret calling him chrischan_sonichu

		for (int i = 0; i < 4; i++)
			if (chaotic_combo[i].cam_location == curr_cam && currentScreen == CAMERAS && !cams[curr_cam].out_of_order)
				DrawTexture(chaotic_combo[i].texture, chaotic_combo[i].position.x, chaotic_combo[i].position.y, WHITE);
		}
	EndDrawing();
} 

void Update(void) {
	if (!IsSoundPlaying(fan) && !win && !lose)
		PlaySound(fan);

	if (IsKeyPressed(KEY_M))
		if (IsSoundPlaying(call))
			StopSound(call);

	//camera input
	if (IsKeyPressed(KEY_UP) && currentScreen == CAMERAS){ 
		PlaySound(cam_click);
		if (curr_cam == 0)
			curr_cam = 5;	
		else
			curr_cam--;
	}
	if (IsKeyPressed(KEY_DOWN) && currentScreen == CAMERAS){
		PlaySound(cam_click);
		if (curr_cam == 5)
			curr_cam = 0;
		else
			curr_cam++;
	}
	if (IsKeyPressed(KEY_C) && currentScreen == CAMERAS && cam_scare){
		PlaySound(whistle);
		if (curr_cam == sonichu.cam_location){
			sonichu.cam_location = -1; sonichu.primed = false;
			cam_scare = false;
		}
		if (curr_cam == rosechu.cam_location){
			rosechu.cam_location = -1; rosechu.primed = false;
			cam_scare = false;
		}
	
		else
			cam_scare = false;
	}
	//office input
	if (IsKeyPressed(KEY_LEFT) && currentScreen == OFFICE){
		if (curr_office > 0)
			curr_office--;
	}
	if (IsKeyPressed(KEY_RIGHT) && currentScreen == OFFICE){
		if (curr_office < 2)
			curr_office++;
	}

	//camera toggle
	if (IsKeyPressed(KEY_SPACE)){
		if(!win && !lose){
		switch (currentScreen){
			case CAMERAS:
				PlaySound(open_cam);
				currentScreen = OFFICE;
				break;
			case OFFICE:
				if (curr_office == 1){
					if (battery > 1){
						PlaySound(open_cam);
						currentScreen = CAMERAS;
					}
				}
				break;
			}	
		}
	}

	//charge battery
	if (IsKeyDown(KEY_Z)){
		if (currentScreen == OFFICE && curr_office == 0)
			if (battery <= 9999)
				battery+=(night_int*2);	
	}

	if (battery <= 0)
		BatteryZero();
	//fix cams
	if (IsKeyPressed(KEY_X))
		if(currentScreen == OFFICE && curr_office == 0)
			AllCamsInOrder();
	if (IsKeyPressed(KEY_C))
		if (currentScreen == OFFICE && curr_office == 0 && !cam_scare)
			cam_scare = true;

	//toggle door
	if (IsKeyPressed(KEY_Q)){
		door_closed = !door_closed; 
		if (door_closed)
			PlaySound(close_door);
		else
			PlaySound(open_door);
	}

	//door light
	if (IsKeyDown(KEY_LEFT_CONTROL)){
		light = true;
		battery-=((night_int)/2)*3;
	}
	else
		light = false;

	if (door_closed)
		if (battery>=0)
			battery-=((1*night_int)/2)*3;


	//attack timer for all enemies except for Chaotic combo
	if (attack_frames >= 300){
		if (chris.active)
			Chris_Move();
		if (sonichu.active)
			Sonichu_Move();
		if (rosechu.active)
			Rosechu_Move();
		if (chrischan_sonichu.active)
			CWCMove();
		attack_frames = 0;
	}
	else
		attack_frames++;

	if (combo_frames >= 150){
		Combo_Move(); //combo_move will test for activeness?
		combo_frames = 0;
			
	}
	else
		combo_frames++;

	//night timer
	if (night_frames >= 21600){
		//printf("Night is OVRE!!!\n");
		Deactivate_All();
		GameWin();
	}
	else{
		night_frames++;
		if (night_frames < 3600){
			time_string[0] = '1';
			time_string[1] = '2';
		}

		else if (night_frames == 3600){
			time_string[0] = ' ';
			time_string[1] = '1';
		}
		else if (night_frames == 7200)
			time_string[1] = '2';
		else if (night_frames == 10800){
			time_string[1] = '3';
			chris.AI+=2;
		}
		else if (night_frames == 14400)
			time_string[1] = '4';
		else if (night_frames == 18000)
			time_string[1] = '5';
			
	}

	//debug - summon magichan
/*	if (IsKeyPressed(KEY_T))
		SummonMagiChan(); //TODO: remove or comment out
*/
}

void GameWin(){
	win = true;
	StopSound(fan);
	if (!saved){
	//more file i/o to make the next night work
	FILE* file;
	night_int++;
	int number = night_int;

	file = fopen("night.txt", "w");
	if (file == NULL){
		printf("Saving Error\nSould not save!\n");
		saved = true; //not really, but otherwise an infinite loop happens
		return;
	}
	
	fprintf(file, "%d", number);
	fclose(file);
	saved = true;

	if (!aSound){
		int rand = GetRandomValue(0,2);
		PlaySound(sigh[rand]);
		aSound = true;
		}
	}
}

void UpdateDrawFrame(void){
	Update();
	Draw();
}

void InitCombo(){
	chaotic_combo[0].texture = LoadTexture("images/enemies/Bubbles.png"); chaotic_combo[0].laugh = LoadSound("audio/sfx/bubbles_laugh.wav");
	chaotic_combo[0].name = "Bubbles";
	chaotic_combo[0].height = 318; chaotic_combo[0].width = 298; 

	chaotic_combo[1].texture = LoadTexture("images/enemies/Angelica.png"); chaotic_combo[1].laugh = LoadSound("audio/sfx/angelica_laugh.wav");
	chaotic_combo[1].name = "Angelica";
	chaotic_combo[1].height = 339; chaotic_combo[1].width = 471;
	
	chaotic_combo[2].texture = LoadTexture("images/enemies/Punchy.png"); chaotic_combo[2].laugh = LoadSound("audio/sfx/punchy_laugh.wav");
	chaotic_combo[2].name = "Punchy";
	chaotic_combo[2].height = 278; chaotic_combo[2].width = 184;

	chaotic_combo[3].texture = LoadTexture("images/enemies/Wild.png"); chaotic_combo[3].laugh = LoadSound("audio/sfx/wild_laugh.wav");
	chaotic_combo[3].name = "Wild";
	chaotic_combo[3].height = 264; chaotic_combo[3].width = 181;

	for (int i = 0; i < 4; i++){
		chaotic_combo[i].cam_location = -1;
		chaotic_combo[i].position = (Vector2) {WINDOW_W/2 - chaotic_combo[i].width/2, WINDOW_H/2 - chaotic_combo[i].height/2} ; 
	}

	magichan_summon = false; cwc_power = LoadSound("audio/sfx/cwc_powered_up.wav");
	combo_no = combo_frames = 0; combo_footsteps = LoadSound("audio/sfx/combo_footsteps.wav");
	chaotic_combo[0].active = true; 
}

void Chris_Jumpscare(Texture scare){
	PlaySound(scream);
	while (jumpscare_timer <= 260){
		BeginDrawing();
			ClearBackground(BLACK);
			DrawTexture(scare, scare_x, 0, WHITE);
		EndDrawing();
		if (scare_frames == 2){
			switch (shake_interval) {
				case 0:
					scare_x = -40;
					shake_interval = 1;
					break;
				case 1:
					scare_x = 80;
					shake_interval = 2;
					break;
				case 2:
					scare_x = -40;
					shake_interval = 3;
					break;
				case 3:
					scare_x = 0;
					shake_interval = 0;
					break;
			
			}
			scare_frames = 0;
		}
		scare_frames++;
		jumpscare_timer++;
	}
	lose = true;
	StopSound(fan);

	int rand = GetRandomValue(0, 3);
	PlaySound(end_lines[rand]);
}

void Chris_Move(void) { 
	int rand = GetRandomValue(1, 20);
	printf("Chris: Random int = %d\n", rand);
	
		if (chris.AI >= rand){
			int second_rand = GetRandomValue(1, (4 + chris.stress));
			if (second_rand == 1)	
				printf("Chris could move, but he chose not to.\n");
			else {
				printf("Chris moved.\n");
				PlaySound(footsteps);
				chris.cam_location++;
			}
			if (chris.cam_location == 7){ 
				if (door_closed){
					int i = GetRandomValue(0,2);
					PlaySound(sigh[i]); chris.stress++; //the more stressed he is, the less likely he is to choose not to move. 
					chris.cam_location = GetRandomValue(0,2);
				}
				else{
					Deactivate_All();
					Chris_Jumpscare(chris_scare);
				}
			}
		}
		else
			printf("Chris did not move\n");
}

void Sonichu_Move(void){
	int rand = GetRandomValue(1,20);
	printf("Sonichu: Random int = %d\n", rand);

	if (sonichu.AI >= rand){
		if (!sonichu.primed){
			sonichu.cam_location = 2;
			sonichu.primed = true;
			printf("Sonichu is primed\n");
		}
		else { 
			int one_more = GetRandomValue(0,2);
			sonichu.primed = false;
			sonichu.cam_location = -1;
			PlaySound(shock[one_more]);
			battery-=500;
			printf("Zap to the extreme!\n");
		}
	}
}
void Rosechu_Move(void){
	int rand = GetRandomValue(1,20);
	printf("Rosechu: Random Int = %d\n", rand);

	if (rosechu.AI >= rand){
		if (!rosechu.primed){
			rosechu.cam_location = GetRandomValue(0,5);
			rosechu.primed = true;
			printf("Rosechu is primed\n");
		}
		else {
			int i = rosechu.cam_location;
			rosechu.cam_location = -1;
			cams[i].out_of_order = true;
			PlaySound(cam_fail);
			rosechu.primed = false;
			printf("Rosechu knocked out Camera %d!\n", i);

		}
		
	}
}

void Combo_Move(){

	if (!chaotic_combo[combo_no].active)
		return;
	else{
		if (combo_no < 4){
			int rand = GetRandomValue(1,20);
			printf("%s: Random Int = %d\n", chaotic_combo[combo_no].name, rand);
			if (chaotic_combo[combo_no].AI >= rand) chaotic_combo[combo_no].cam_location++;

			if (chaotic_combo[combo_no].cam_location == 7){
				if (door_closed){
					PlaySound(combo_footsteps);
					chaotic_combo[combo_no].cam_location = -1;
				}
				else{
					PlaySound(chaotic_combo[combo_no].laugh); chaotic_combo[combo_no].cam_location++;
					if (combo_no < 3){
						chaotic_combo[combo_no+1].active = true; 
						chaotic_combo[combo_no].active = false;
						combo_no++;
					}
					else{
						if (!magichan_summon){
							SummonMagiChan();
						}
						for (int i = 0; i < 4; i++){
							chaotic_combo[i].cam_location = -1;chaotic_combo[i].active = false;
						}
					}

				}
			}
		}
	}
}

void BatteryZero(){
	//printf("uh oh.");
	door_closed = false;
}

void SummonMagiChan(){
	Texture MagiChan = LoadTexture("images/enemies/MagiChan.png");
	int width = 600; int height = 600;
	int MagiChan_y = 600;
	while (MagiChan_y > -600 ) {
	BeginDrawing();
		ClearBackground(YELLOW);
		DrawTexture(MagiChan, WINDOW_W/2-width/2, MagiChan_y, WHITE);
	EndDrawing();

	MagiChan_y -= 20;
	}
	magichan_summon = true;
	UnloadTexture(MagiChan);
	InitCWC();
}

void InitCWC() {
	PlaySound(cwc_power);
	printf("CWC is being Initiated\n");
	chrischan_sonichu.texture = LoadTexture("images/enemies/CWCSonichu.png");
	chrischan_sonichu.height = 437; chrischan_sonichu.width = 222;
	chrischan_sonichu.position = (Vector2) { WINDOW_W/2 - chrischan_sonichu.width/2, WINDOW_H/2 - chrischan_sonichu.height/2 };
	chrischan_sonichu.cam_location = chris.cam_location; chrischan_sonichu.AI = chris.AI;
	chrischan_sonichu.active = chrischan_sonichu.girlfriendFree = true; chrischan_sonichu.primed_gen = chrischan_sonichu.primed_attack = chrischan_sonichu.primed_cam = false;

	cwc_scare = LoadTexture("images/enemies/CWC_Scare.png");

	chris.active = false; chris.cam_location = -1;
		
}

void CWCMove() { //i definitely shouldve named him something different

	printf("CWC moved\n");
	if (!chrischan_sonichu.primed_gen && !chrischan_sonichu.primed_attack && !chrischan_sonichu.primed_cam){
		int choice = GetRandomValue(1,4);
		printf("CWC: Random int = %d\n", choice);
		switch (choice){
			case 1: //jump to cam 5, attack next turn
				int rand = GetRandomValue(1, 20);
				if (chrischan_sonichu.AI >= rand){
					PlaySound(end_lines[1]);
					chrischan_sonichu.cam_location = 5;
					chrischan_sonichu.primed_attack = true;
				}
				break;
			case 2: //jump to genarator, drop 10% power next turn
				int rand_also = GetRandomValue(1, 20);
				if (chrischan_sonichu.AI >= rand_also){
					PlaySound(end_lines[2]);
					chrischan_sonichu.cam_location = 2;
					chrischan_sonichu.primed_gen = true;
				}
				break;
			case 3: //jump to random cam, break next turn
				int rand_aswell = GetRandomValue(1, 20);
				if (chrischan_sonichu.AI >= rand_aswell){
					PlaySound(end_lines[0]);
					chrischan_sonichu.cam_location = GetRandomValue(0,5);
					chrischan_sonichu.primed_cam = true;
				}
				break;
			case 4: //break for player
				PlaySound(end_lines[3]);
				break;
		}
		return;
	}
	if (chrischan_sonichu.primed_attack){
		int rand = GetRandomValue(1,20);
		if (chrischan_sonichu.AI >= rand){
			PlaySound(footsteps);
			chrischan_sonichu.cam_location++;
			if (chrischan_sonichu.cam_location == 7){
				if (door_closed){
					int i = GetRandomValue(0,2);
					PlaySound(sigh[i]);
					chrischan_sonichu.cam_location = 1; chrischan_sonichu.primed_attack = false;
				}
				else{
					Deactivate_All();
					Chris_Jumpscare(cwc_scare);
				}
			}
		}
	}
	if (chrischan_sonichu.primed_gen){
		int rand = GetRandomValue(1, 20);
		if (chrischan_sonichu.AI >= rand){
			int one_more = GetRandomValue(0,2);
			chrischan_sonichu.cam_location = 1; chrischan_sonichu.primed_gen = false;
			PlaySound(shock[one_more]);
			battery-=1000;
		}
	
	}
	if (chrischan_sonichu.primed_cam){
		int rand = GetRandomValue(1, 20);
		if (chrischan_sonichu.AI >= rand){
			int i = chrischan_sonichu.cam_location;
			chrischan_sonichu.cam_location = 1; chrischan_sonichu.primed_cam = false;
			cams[i].out_of_order = true;
			PlaySound(cam_fail);
		}
	
	}
}


void Deactivate_All(void){
	chris.active = false; chris.cam_location = -1;
	sonichu.active= false; sonichu.cam_location = -1;
	rosechu.active = false;	rosechu.cam_location = -1;

	chrischan_sonichu.active = false; chrischan_sonichu.cam_location = -1;

	for (int i = 0; i < 3; i++){
		chaotic_combo[i].active = false; chaotic_combo[i].cam_location = -1;
	}
}
void Unload(void){
	UnloadTexture(chris.texture);
	printf("Chris Deleted.\n");
	UnloadTexture(sonichu.texture);
	printf("Sonichu deleted.\n");
	UnloadTexture(rosechu.texture);
	printf("Rosechu deleted.\n");
	for (int i = 0; i < CAM_NUM; i++){
		UnloadTexture(cams[i].texture);
		printf("Cam %d deleted.\n", i);
	}
	for (int i = 0; i < 3; i++){
		UnloadTexture(office[i]);
		printf("Office %d deleted.\n", i);
	}
	for (int i = 0; i < 4; i++){
		UnloadTexture(chaotic_combo[i].texture);
		printf("%s deleted\n", chaotic_combo[i].name);
	}
	UnloadTexture(door); UnloadTexture(darkness);
	UnloadTexture(chris_scare);
	UnloadTexture(shadow);
	UnloadTexture(the_end); UnloadTexture(you_win); UnloadTexture(you_lose);
	UnloadSound(call);

	UnloadSound(scream);
	for (int i = 0; i < 3; i++)
		UnloadSound(sigh[i]);
	for (int i = 0; i < 4; i++)
		UnloadSound(end_lines[i]);
	for (int i = 0; i < 3; i++)
		UnloadSound(shock[i]);
	for (int i = 0; i < 4; i++)
		UnloadSound(chaotic_combo[i].laugh);
	UnloadSound(fan);
	UnloadSound(v_static);
	UnloadSound(cam_fail);
	UnloadSound(cam_click);
	UnloadSound(open_cam);

	UnloadSound(whistle);

	UnloadSound(cwc_power);
	UnloadSound(open_door); UnloadSound(close_door);

	UnloadSound(footsteps); UnloadSound(combo_footsteps);
	CloseAudioDevice();

}

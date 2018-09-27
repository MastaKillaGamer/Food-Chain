#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>


using namespace std;

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 400;
const int GRID_SIZE = 20;
const int GRID_TOTAL = GRID_SIZE * GRID_SIZE;
int grid[GRID_SIZE][GRID_SIZE];
constexpr static int START_BIRTH = 100;
constexpr static int START_HUNGER = 100;
constexpr static int MAX_HEALTH = 100;
struct plant
{
	int x;
	int y;
	int birth;
};
struct herbivore
{
	int x;
	int y;
	int hunger;
	int birth;
};
struct carnivore
{
	int x;
	int y;
	int hunger;
	int birth;
};
std::vector<plant> plantId;
std::vector<herbivore> herbivoreId;
std::vector<carnivore> carnivoreId;
ifstream inFile;

bool init();

bool loadMedia();

void close();

SDL_Texture* loadTexture(std::string path);

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Project 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int createCreature(int x, int y, int c)
{
	if (c == 1)
	{
		plantId.push_back(plant());
		int n = static_cast<int>(plantId.size());
		plantId[n].x = x;
		plantId[n].y = y;
		plantId[n].birth = 0;
		grid[x][y] = 1;
	};
	if (c == 2)
	{
		herbivoreId.push_back(herbivore());
		int n = static_cast<int>(herbivoreId.size());
		herbivoreId[n].x = x;
		herbivoreId[n].y = y;
		herbivoreId[n].hunger = START_BIRTH;
		herbivoreId[n].birth = START_HUNGER;
		grid[x][y] = 2;
	};
	if (c == 3)
	{
		carnivoreId.push_back(carnivore());
		int n = static_cast<int>(carnivoreId.size());
		carnivoreId[n].x = x;
		carnivoreId[n].y = y;
		carnivoreId[n].hunger = START_BIRTH;
		carnivoreId[n].birth = START_HUNGER;
		grid[x][y] = 3;
	};

	return 1;
}

int creatureInit()
{
	for (int i = 0; i < 3; i++)
	{
		int rx = (rand() % 20) + 1;
		int ry = (rand() % 20) + 1;
		createCreature(rx, ry, 1);
	}
	for (int i = 0; i < 3; i++)
	{
		int rx = (rand() % 20) + 1;
		int ry = (rand() % 20) + 1;
		createCreature(rx, ry, 2);
	}
	for (int i = 0; i < 3; i++)
	{
		int rx = (rand() % 20) + 1;
		int ry = (rand() % 20) + 1;
		createCreature(rx, ry, 3);
	}

	return 1;
};

int killCreature(int c,int i)
{
	if (c == 1)
	{
		int x = plantId[i].x;
		int y = plantId[i].y;
		grid[x][y] = 0;
		plantId.erase(plantId.begin() + (i - 1));
	};
	if (c == 2)
	{
		int x = herbivoreId[i].x;
		int y = herbivoreId[i].y;
		grid[x][y] = 0;
		herbivoreId.erase(herbivoreId.begin() + (i - 1));
	};
	if (c == 3)
	{
		int x = carnivoreId[i].x;
		int y = carnivoreId[i].y;
		grid[x][y] = 0;
		carnivoreId.erase(carnivoreId.begin() + (i - 1));
	};
	return 1;
};


int initGrid()
{
	inFile.open("C:\\Dev\\map.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(EXIT_FAILURE);   // call system to stop
	};

	for (int y = 0; y < GRID_SIZE; y++)
	{
		for (int x = 0; x < GRID_SIZE; x++)
		{
			inFile >> grid[x][y];
		}
	}
	creatureInit();
	return 1;
}

int drawCube(int xPos, int yPos)
{
	SDL_Rect fillRect = {xPos * GRID_SIZE,yPos * GRID_SIZE, GRID_SIZE, GRID_SIZE };
	if (grid[xPos][yPos] == 0 ) SDL_SetRenderDrawColor(gRenderer,0,0,0,255);
	if (grid[xPos][yPos] == 1 ) SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
	if (grid[xPos][yPos] == 2 ) SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
	if (grid[xPos][yPos] == 3 ) SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect(gRenderer, &fillRect);
	return 1;
};

int gameDraw()
{
	for (int y = 0; y < GRID_SIZE; y++)
	{
		for (int x = 0; x < GRID_SIZE; x++)
		{
			drawCube(x, y);
		}
	}
	return 1;

};

int gameLogic()
{


	return 1;
};

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			initGrid();

			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//gameLogic();
				gameDraw();
				//cin.get();


				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	close();

	return 0;
};
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <thread>

constexpr static int START_BIRTH = 100;
constexpr static int START_HUNGER = 100;
constexpr static int MAX_HEALTH = 100;
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 400;
const int GRID_SIZE = 20;
const int GRID_TOTAL = GRID_SIZE * GRID_SIZE;
int grid[GRID_SIZE][GRID_SIZE];
int plants = 0;
int herbivores = 0;
int carnivores = 0;
int xmod;
int ymod;
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
enum directions
{
	up = 0, down = 1, left = 2, right = 3, up_right = 4, up_left = 5, down_right = 6, down_left = 7
};
std::vector<plant> plantId;
std::vector<herbivore> herbivoreId;
std::vector<carnivore> carnivoreId;


bool init();

bool loadMedia();

void close();

SDL_Texture* loadTexture(std::string path);

SDL_Window* gWindow = nullptr;

SDL_Renderer* gRenderer = nullptr;

int randRange(int low, int high)
{
	return rand() % (high - low) + low;
}

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
		if (gWindow == nullptr)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == nullptr)
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
	gWindow = nullptr;
	gRenderer = nullptr;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == nullptr)
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
		plantId.emplace_back(plant());
		int n = plants;
		plantId[n].x = x;
		plantId[n].y = y;
		plantId[n].birth = 0;
		grid[x][y] = 1;
		plants++;
	};
	if (c == 2)
	{
		herbivoreId.emplace_back(herbivore());
		int n = herbivores;
		herbivoreId[n].x = x;
		herbivoreId[n].y = y;
		herbivoreId[n].hunger = START_BIRTH;
		herbivoreId[n].birth = START_HUNGER;
		grid[x][y] = 2;
		herbivores++;
	};
	if (c == 3)
	{
		carnivoreId.emplace_back(carnivore());
		int n = carnivores;
		carnivoreId[n].x = x;
		carnivoreId[n].y = y;
		carnivoreId[n].hunger = START_BIRTH;
		carnivoreId[n].birth = START_HUNGER;
		grid[x][y] = 3;
		carnivores++;
	};

	return 1;
}

int creatureInit()
{
	for (int i = 0; i < 3; i++)
	{
		int rx = randRange(1, 20);
		int ry = randRange(1, 20);
		createCreature(rx, ry, 1);
		rx = 0;
		ry = 0;
	}
	for (int i = 0; i < 3; i++)
	{
		int rx = randRange(1, 20);
		int ry = randRange(1, 20);
		createCreature(rx, ry, 2);
		rx = 0;
		ry = 0;
	}
	for (int i = 0; i < 3; i++)
	{
		int rx = randRange(1, 20);
		int ry = randRange(1, 20);
		createCreature(rx, ry, 3);
		rx = 0;
		ry = 0;
	}

	return 1;
};

int killCreature(int c,int i)
{
	if (c == 1)
	{
		int x = plantId[i].x;
		int y = plantId[i].y;
		//plantId.erase(plantId.begin() + (i - 1));
		plants--;
	};
	if (c == 2)
	{
		int x = herbivoreId[i].x;
		int y = herbivoreId[i].y;
		//herbivoreId.erase(herbivoreId.begin() + (i - 1));
		herbivores--;
	};
	if (c == 3)
	{
		int x = carnivoreId[i].x;
		int y = carnivoreId[i].y;
		//carnivoreId.erase(carnivoreId.begin() + (i - 1));
		carnivores--;
	};
	return 1;
};

int dieCreature(int c, int i)
{
	if (c == 1)
	{
		int x = plantId[i].x;
		int y = plantId[i].y;
		grid[x][y] = 0;
		//plantId.erase(plantId.begin() + (i - 1));
		plants--;
	};
	if (c == 2)
	{
		int x = herbivoreId[i].x;
		int y = herbivoreId[i].y;
		grid[x][y] = 0;
		//herbivoreId.erase(herbivoreId.begin() + (i - 1));
		herbivores--;
	};
	if (c == 3)
	{
		int x = carnivoreId[i].x;
		int y = carnivoreId[i].y;
		grid[x][y] = 0;
		//carnivoreId.erase(carnivoreId.begin() + (i - 1));
		carnivores--;
	};
	return 1;
};

int getDirection(directions x)
{
	if ((x = up) || (x = up_left) || (x = up_right))
	{
		ymod = 1;
	}
	if ((x = down) || (x = down_left) || (x = down_right))
	{
		ymod = -1;
	}
	if ((x = left) || (x = down_left) || (x = up_left))
	{
		xmod = -1;
	}
	if ((x = right) || (x = down_right) || (x = up_right))
	{
		xmod = 1;
	}
	return 1;
};

int moveCreature(directions x, int c, int i)
{
	getDirection(x);
	if (c == 1)
	{
		plantId[i].x += xmod;
		plantId[i].y += ymod;
	};
	if (c == 2)
	{
		herbivoreId[i].x += xmod;
		herbivoreId[i].y += ymod;
	};
	if (c == 3)
	{
		carnivoreId[i].x += xmod;
		carnivoreId[i].y += ymod;
	};
	xmod = 0;
	ymod = 0;
	return 1;
}

int initGrid()
{
	for (int y = 0; y < GRID_SIZE; y++)
	{
		for (auto & x : grid)
		{
			x[y] = 0; 
		}
	}

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

int lookAround(int c, int i)
{
	int x;
	int y;
	if (c == 1)
	{
		x = plantId[i].x;
		y = plantId[i].y;
		for (int gx = -1; !(gx = 2); gx++)
		{
			for (int gy = -1; !(gx = 2); gy++)
			{
				//Reproduction
				std::cout << "1" << std::endl;
				if ((grid[x + gx][y + gy] = 0) && (plantId[i].birth = 100))
				{
					createCreature(x - gx, y - gy, 1);
					herbivoreId[i].birth = 90;
				}
			}
		}
	};
	if (c == 2)
	{
		x = herbivoreId[i].x;
		y = herbivoreId[i].y;
		for (int gx = -1; !(gx = 2); gx++)
		{
			for (int gy = -1; !(gx = 2); gy++)
			{
				//Reproduction
				std::cout << "21" << std::endl;
				if ((grid[x + gx][y + gy] = 0) && (herbivoreId[i].birth = 100))
				{
					createCreature(x - gx, y - gy, 2);
					herbivoreId[i].birth = 0;
					return 1;
				}
				//Eating
				std::cout << "22" << std::endl;
				if ((grid[x + gx][y + gy] = 1) && (herbivoreId[i].hunger <= 80))
				{
					grid[herbivoreId[i].x][herbivoreId[i].y] = 0;
					herbivoreId[i].x = x + gx;
					herbivoreId[i].y = y + gy;
					grid[x + gx][y + gy] = 2;
					herbivoreId[i].hunger += 20;
					return 1;
				}
				//Running
				std::cout << "23" << std::endl;
				if (grid[x + gx][y + gy] = 3)
				{
					grid[herbivoreId[i].x][herbivoreId[i].y] = 0;
					herbivoreId[i].x = x - gx;
					herbivoreId[i].y = y - gy;
					grid[x - gx][y - gy] = 2;
					return 1;
				}
			}
		}
		//Moving
		std::cout << "24" << std::endl;
		int r = randRange(0, 7);
		auto moveDir = static_cast<directions>(r);
		moveCreature(moveDir, 2, i);
	}

	if (c == 3)
	{
		x = carnivoreId[i].x;
		y = carnivoreId[i].y;
		for (int gx = -1; !(gx = 2); gx++)
		{
			for (int gy = -1; !(gy = 2); gy++)
			{
				//Reproduction
				if ((grid[x - gx][y - gy] = 0) && (carnivoreId[i].birth = 100))
				{
					createCreature(x - gx, y - gy, 3);
					carnivoreId[i].birth = 0;
					return 1;
				}
				//Eating
				if ((grid[x + gx][y + gy] = 2) && (carnivoreId[i].hunger <= 80))
				{
					grid[carnivoreId[i].x][carnivoreId[i].y] = 0;
					carnivoreId[i].x = x + gx;
					carnivoreId[i].y = y + gy;
					grid[x + gx][y + gy] = 2;
					carnivoreId[i].hunger += 20;
					return 1;
				}
			}
		}
		//Moving
		std::cout << "33" << std::endl;
		int r = randRange(0, 7);
		auto moveDir = static_cast<directions>(r);
		moveCreature(moveDir, 3, i);
	};
	return 1;
};

int turn()
{
	for (int i = 0; i < carnivores; i++)
	{
		if ((grid[carnivoreId[i].x][carnivoreId[i].x] = 1) || (grid[carnivoreId[i].x][carnivoreId[i].x] = 2) || (grid[carnivoreId[i].x][carnivoreId[i].x] = 0));dieCreature(3, i);
		if (carnivoreId[i].hunger <= 0) killCreature(3, i);
		lookAround(3, i);
	

	};
	for (int i = 0; i < herbivores; i++)
	{
		if ((grid[carnivoreId[i].x][carnivoreId[i].x] = 0) || (grid[carnivoreId[i].x][carnivoreId[i].x] = 3) || (grid[carnivoreId[i].x][carnivoreId[i].x] = 1))dieCreature(2, i);
		if (herbivoreId[i].hunger <= 0) killCreature(2, i);
		lookAround(2, i);

	};
	for (int i = 0; i < plants; i++)
	{
		if ((grid[carnivoreId[i].x][carnivoreId[i].x] = 3) || (grid[carnivoreId[i].x][carnivoreId[i].x] = 2) || (grid[carnivoreId[i].x][carnivoreId[i].x] = 0))dieCreature(1, i);
		lookAround(1, i);

	};

	return 1;
};


int main(int argc, char* args[])
{
	srand(time(nullptr));
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
			creatureInit();
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

				turn();
				using namespace std::this_thread; // sleep_for, sleep_until
				using namespace std::chrono; // nanoseconds, system_clock, seconds
				sleep_for(nanoseconds(10));
				gameDraw();
				
				


				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	close();

	return 0;
};
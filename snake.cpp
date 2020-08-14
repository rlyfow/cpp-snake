#include <ncurses.h>
#include <time.h>
#include <string>
using namespace std;

struct node
{
	node * next;
	int y;
	int x;
};

struct gameState
{
	int length;
	int dir;
	node * head;
	node * last;
	int board[20][20];
	char input;
	int lost;
};

gameState * setupGame();
int setupScreen();

int handleInput(gameState * game);
int handleMove(gameState * game);
int checkPlace(gameState * game, int x, int y);
int drawBoard(gameState * game);
int spawnPoint(gameState * game);
int deleteSnake(gameState * game);

int main()
{
	setupScreen();
	gameState * game = setupGame();
	srand(time(NULL));
	
	while(true)
	{
		clear();
		refresh();
		
		if(game->lost==1) break;
		
		handleInput(game);
		handleMove(game);
		drawBoard(game);
		
		move(22, 0);
		attron(A_REVERSE);
		printw("######################");
		move(22, 1);
		printw("SCORE: %d", game->length);
		attroff(A_REVERSE);
		move(22, 0);
		
		game->input = getch();
	}
	
	deleteSnake(game);
	delete game;
	
	endwin();
	return 0;
}

int deleteSnake(gameState * game)
{
	if(game->last->next != nullptr)
	{
		node * tmp = game->last;
		game->last = game->last->next;
		delete tmp;
	}
	delete game->last;
	
	return 0;
}

int spawnPoint(gameState * game)
{
	int x;
	int y;
	while(true)
	{
		x = rand()%20;
		y = rand()%20;
		
		if(game->board[y][x] == 0)
			break;
	}
	game->board[y][x] = 3;
	
	return 0;
}

int drawBoard(gameState * game)
{
	move(0,0);
	printw("######################");
	int y = 1;
	for(int i=19; i>=0; i--)
	{
		move(y, 0);
		printw("#");
		for(int j=0; j<20; j++)
		{
			if(game->board[i][j]==0) printw(" ");
			if(game->board[i][j]==1)
			{
				attron(A_REVERSE);
				printw("o");
				attroff(A_REVERSE);
			}
			if(game->board[i][j]==2)
			{
				attron(A_REVERSE);
				printw("@");
				attroff(A_REVERSE);
			}
			if(game->board[i][j]==3) printw("x");
		}
		printw("#");
		y++;
	}
	move(y, 0);
	printw("######################");
	
	return 0;
}

int checkPlace(gameState * game, int x, int y)
{
	if(x<20 && y<20 && x>=0 && y >= 0)
	{
		return game->board[y][x];
	}
	
	return -1;
}

int handleMove(gameState * game)
{
	int destination_x;
	int destination_y;
	
	if(game->dir == 1)
	{
		destination_x = game->head->x+1;
		destination_y = game->head->y;
	}
	
	if(game->dir == 2)
	{
		destination_x = game->head->x-1;
		destination_y = game->head->y;
	}
	
	if(game->dir == 3)
	{
		destination_x = game->head->x;
		destination_y = game->head->y+1;
	}
	
	if(game->dir == 4)
	{
		destination_x = game->head->x;
		destination_y = game->head->y-1;
	}
	
	int destination_check = checkPlace(game, destination_x, destination_y);
	
	if(destination_check == 0)
	{
		node * new_head = new node;
		game->head->next = new_head;
		
		game->board[game->head->y][game->head->x]=1;
		
		game->head = new_head;
		game->head->y = destination_y;
		game->head->x = destination_x;
		game->head->next = nullptr;
		
		game->board[game->head->y][game->head->x]=2;
		game->board[game->last->y][game->last->x]=0;
		
		node * tmp = game->last;
		game->last = game->last->next;
		delete tmp;
	}
	
	if(destination_check == 1)
	{
		game->lost = 1;
	}
	
	if(destination_check == -1)
	{
		game->lost = 1;
	}
	
	if(destination_check == 3)
	{
		
		node * new_head = new node;
		game->head->next = new_head;
		
		game->board[game->head->y][game->head->x]=1;
		
		game->head = new_head;
		game->head->y = destination_y;
		game->head->x = destination_x;
		game->head->next = nullptr;
		
		game->board[game->head->y][game->head->x]=2;
		game->length++;
		spawnPoint(game);
	}
	
	return 0;
}

int handleInput(gameState * game)
{
	if(game->input =='d' || game->input == 'D')
	{
		if(game->dir != 2)
		{
			game->dir = 1;
		}
	}
	
	if(game->input =='a' || game->input == 'A')
	{
		if(game->dir != 1)
		{
			game->dir = 2;
		}
	}
	
	if(game->input =='w' || game->input == 'W')
	{
		if(game->dir != 4)
		{
			game->dir = 3;
		}
	}
	
	if(game->input =='s' || game->input == 'S')
	{
		if(game->dir != 3)
		{
			game->dir = 4;
		}
	}
	
	return 0;
}

gameState * setupGame()
{
	gameState * new_game = new gameState;
	
	new_game->length = 0;
	new_game->dir = 1;
	new_game->lost = 0;
	new_game->input = ' ';
	
	new_game->head = new node;
	new_game->head->y = 10;
	new_game->head->x = 10;
	
	new_game->last = new node;
	new_game->last->y = 10;
	new_game->last->x = 9;
	new_game->last->next = new_game->head;
	
	for(int y=0; y<20; y++)
		for(int x = 0; x<20; x++)
			new_game->board[y][x] = 0;
	
	new_game->board[10][10] = 2;
	new_game->board[10][9] = 1;
	
	new_game->board[5][5] = 3;
	
	return new_game;
}

int setupScreen()
{
	initscr();
	noecho();
	timeout(300);
	
	return 0;
}

// Program : Checkers Game
//programmer : Rosemol Thanjappan
//Date : 11/24/2018

#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <Windows.h>
#include "MMSystem.h"
#include <vector>
#include<fstream>
#include <iomanip>
using namespace std;

class game
{
public:
	void mainLine();

private:


	int row;//current row
	int col;//current column
	int destRow;//destination row
	int destCol;//destination column
	char playerTurn;
	bool endFlag;//shows whether game ended
	string gameResults;
	long long clockTick;
	string currentActionTaken;


	bool newGame;//shows whether new game
	bool startFlag;//shows whether game started
	string cells[8][8];//the array that holds the game pieces
	int iTotalR, iTotalB;//total number of game pieces




	int dir;//direction to move
	bool flag;// the flag showing the game is playing
	int player;//which player is playing the game

	ifstream rulesFile;//ifstream object holding rules file
	ofstream logFile;//ofstream object holding log file
	ifstream inputLogFile;//ifstream object holding log file
	int toRow;//the row to move
	int toCol;//the column to move
	int jRow;//the row to jump
	int jCol;//the column to jump
	int rowLimit;//the limit of row boundry
	int colLimit;//the limit of column boundry
	string currentPiece;//the current playing piece red or black
	string currentKPiece;//the current King piece "B" or "R"
	string directions[4];//the 4 directions
	vector<int> vDirs;//the vector to hold directions
	bool bEndFlag;//the flag to read files


	// Member Modules
	void init(void);
	void gameMenu(void);
	void eoj(void);
	void displayMenu(void);
	int menuPic(void);
	void startGameOver(void);
	void displayBoard(void);
	void soundModule(int);
	char chooseStartPlayer(void);
	void makeAMove(void);
	void playerR_move(void);
	void playerB_move(void);

	bool checkGameOver(void);
	void writeStatsDataFile(string);
	void setBoard();
	void writeIt();
	void calculateValidDirections(int, int);
	void setOffset(int);
	bool isLegal();
	void accumulate();
	void OpenFile(void);
	bool showDirMenu();


	void readRules();
	void writeLogFile(string);
	bool checkForWinner();
	void jumpOver();
	void clearCell(int, int);
	void moveToTheCell();
	bool isValidPiece(int, int);
	int getCoOrdinate(string);
	bool isLastRank(int);
	bool isValidBoundary();
	bool isEmpty(int, int);
	bool isJumpable();
	bool isOpponentPiece();
	int displayDirMenu();
	bool isValidDir(int);
	void displayScore();
	bool makeFinalMove();
	bool openLogFile();
	void readLogFile();
};




int main(void)
{
	game g1;
	g1.mainLine();

	system("pause");
}

void game::mainLine(void)
{
	init();

	while (endFlag == false)
	{
		gameMenu();
	}

	eoj();
}
void game::init(void)
{
	clockTick = time(0);

	//initializing the number of pieces to 12
	iTotalR = 12; iTotalB = 12;
	//setting the 4 directions
	directions[0] = "1 (Upper-Left Diagonal)";
	directions[1] = "2 (Upper-Right Diagonal)";
	directions[2] = "3 (Lower-Left Diagonal)";
	directions[3] = "4 (Lower-Right Diagonal)";


	// Setup other start items.
	endFlag = false;
	playerTurn = chooseStartPlayer();
	gameResults = "No game results yet.";

	// Like a priming for the game.
	startFlag = false;

	setBoard();
	startFlag = true;
	displayBoard();

	soundModule(1);


}


void game::gameMenu(void)
{
	int aPic = 1;
	displayMenu();
	aPic = menuPic();

	switch (aPic)
	{

		// Start the game over.
	case 1:
		startGameOver();
		currentActionTaken = "Start the game over.";
		cout << currentActionTaken << endl;
		writeStatsDataFile("Start the game over.");
		break;
	case 2:
		playerR_move();
		writeStatsDataFile("Make a move player R.");
		break;
	case 3:

		playerB_move();
		writeStatsDataFile("Make a move player B.");
		break;
	case 4:
		displayBoard();
		writeStatsDataFile("Display the current board");
		break;
	case 5:
		checkForWinner();
		writeStatsDataFile("Check for winner.");
		displayScore();
		break;
	case 6:
		if (!checkForWinner())
		{
			if (!checkGameOver())
			{
				cout << "The game is not over yet." << endl;
				displayScore();
			}

		}
		writeStatsDataFile("Check for end of game.");
		break;


	case 7://Displaying the rules
		bEndFlag = false;
		OpenFile();
		while (!bEndFlag) {
			readRules();
		}
		rulesFile.close();
		writeStatsDataFile("Displaying the rules of the game.");
		system("pause");
		break;
	case 8://Display log file
		system("cls");
		logFile.close();
		if (openLogFile())
		{
			bEndFlag = false;

			while (!bEndFlag)
			{
				readLogFile();
			}
		}

		system("pause");
		inputLogFile.close();

		break;
	case 10:
		endFlag = true;
		writeStatsDataFile("Ending the game.");
		break;
	default:
		cout << "No option picked." << endl;

	}
}
void game::OpenFile(void)
{
	rulesFile.open("CheckersGameRules.txt");
	if (rulesFile.fail())
	{
		cout << "Checkers game rules file open failes" << endl;
		system("pause");

	}

	return;
}
void game::readRules()
{
	string str;

	if (!rulesFile.fail())
	{
		getline(rulesFile, str);

		cout << str << endl;
		if (rulesFile.eof() == true)


			bEndFlag = true;
		else
			bEndFlag = false;
	}
	else {
		bEndFlag = true;
	}


	return;
}


void game::writeLogFile(string str)
{


	logFile << setiosflags(ios::fixed)
		<< setiosflags(ios::showpoint) << setprecision(2)
		<< setw(4) << (row + 1) << "," << setw(2) << col + 1
		<< "    to  "
		<< setw(4) << destRow + 1 << "," << setw(2) << destCol + 1
		<< "   " << str << endl;
	return;
}
bool game::openLogFile(void)
{
	inputLogFile.open("stats.txt");
	if (inputLogFile.fail())
	{
		cout << "Checkers log file open fails" << endl;
		system("pause");
		return false;
	}

	return true;
}
void game::readLogFile()
{
	string str;

	if (!inputLogFile.fail())
	{
		getline(inputLogFile, str);

		cout << str << endl;

		if (inputLogFile.eof() == true)


			bEndFlag = true;
		else
			bEndFlag = false;
	}
	else {
		bEndFlag = true;
	}

	return;
}
void game::eoj(void)
{

	displayScore();
	logFile.close();
	cout << endl << "Game Over" << endl << endl;
	//soundModule(2);

}
void game::displayScore()
{

	if (startFlag) {
		cout << "The player 1 (red) has " << iTotalR << " pieces left." << endl;
		cout << "The player 2 (black)has " << iTotalB << " pieces left." << endl;
		if (iTotalR == iTotalB)
		{
			cout << "Player 1 and player 2 has the same number of pieces left." << endl;
			cout << "It is not time to tell who is winning." << endl << endl;
		}
		else if (iTotalR > iTotalB)
		{
			cout << "\nPlayer 1 (red) is winning. " << endl;
		}
		else {
			cout << "\nPlayer 2 (black) is winning. " << endl;
		}
	}
	else {
		cout << "Game is not started yet." << endl;
	}

	system("pause");
}

void game::displayMenu(void)
{

	cout << "\nCheckers Game";
	cout << "\nMenu Options";
	cout << "\n******************************";
	cout << "\n1. Start a new game.";
	cout << "\n2. Pick a move for player 1.";
	cout << "\n3. Pick a move for player 2.";
	cout << "\n4. Show the current checkers board.";
	cout << "\n5. Check for winner.";
	cout << "\n6. Check for game over status.";
	cout << "\n7. Display the rules.";
	cout << "\n8. Display the game log.";

	cout << "\n10. End the game.";
	cout << "\nEnter choice (1 - 10) ===> ";

}

void game::startGameOver(void)
{


	// Setup other start items.
	endFlag = false;
	playerTurn = chooseStartPlayer();
	gameResults = "No game results yet.";

	startFlag = false;
	//initializing the board
	setBoard();
	startFlag = true;
	//displaying the board
	displayBoard();
	//initializing the variables for start over
	iTotalR = 12; iTotalB = 12;


	soundModule(1);



}

int game::menuPic(void)
{
	int pick;

	cin >> pick;

	system("cls");

	return pick;
}

void game::setBoard()
{

	string startBoard[8][8] = {
		{"#","r","#","r","#","r","#","r"},
		{"r","#","r","#","r","#","r","#"},
		{"#","r","#","r","#","r","#","r"},
		{" ","#"," ","#"," ","#"," ","#"},
		{"#"," ","#"," ","#"," ","#"," "},
		{"b","#","b","#","b","#","b","#"},
		{"#","b","#","b","#","b","#","b"},
		{"b","#","b","#","b","#","b","#"} };
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cells[i][j] = startBoard[i][j];
		}
	}
}


void game::displayBoard(void)
{

	long long howLong;

	howLong = time(0) - clockTick;
	system("cls");
	cout << "The computer clock is at this many seconds: " << clockTick << endl;
	cout << "The game has been running for this many seconds: " << howLong << endl << endl;


	if (startFlag) {
		//if game had already started that is old game ,then display the board
		writeIt();
	}
	else {
		//else this is a new game ,then reset the board and display
		setBoard();
		writeIt();

	}
}
void game::writeIt()
{

	int bigcount;
	int counter;

	cout << "  |----|----|----|----|----|----|----|----|" << endl;

	for (bigcount = 0; bigcount < 8; bigcount++)
	{
		cout << bigcount + 1 << " ";
		for (counter = 0; counter < 8; counter++)
		{
			cout << "| " << cells[bigcount][counter] << "  ";
		}

		cout << "|" << endl << "  |----|----|----|----|----|----|----|----|" << endl;
	}
	cout << "    1    2    3    4    5    6    7    8   " << endl;
	return;
}
//read the co-ordinates and direction and make move
void game::makeAMove()
{
	bool bFlag = false;
	cout << "Player " << player << "'s  ( " << currentPiece << " ) turn : " << endl;

	while (!bFlag)
	{

		row = getCoOrdinate("row");//get the row value
		if (row == -999) { return; }//if -999 return to menu
		col = getCoOrdinate("column");//get the column value
		if (col == -999) { return; }
		bFlag = isValidPiece(row, col);
	}
	//checking whether there is any valid moves for the current piece selected

	//clearing the vector that holds valid directions
	vDirs.clear();

	if (cells[row][col] == "r")
	{
		calculateValidDirections(3, 4);

	}
	else if (cells[row][col] == "b")
	{
		calculateValidDirections(1, 2);

	}
	else if (cells[row][col] == "R" || cells[row][col] == "B")
	{
		calculateValidDirections(1, 4);

	}

	//if the vector comes out empty there is no valid directions,
	//so another piece has to be selected
	if (vDirs.size() == 0)
	{

		cout << "There is no valid moves." << endl;
		cout << "Choose another piece." << endl;
		dir = 0;

	}
	else {
		if (showDirMenu())
		{
			//if the row ,column and direction are valid make final move.
			makeFinalMove();
		}

	}


}


char game::chooseStartPlayer(void)
{
	char startPlayer;

	cout << "The Checkers Game " << endl;
	cout << "***********************************" << endl;
	cout << "Who would you like to start the game? :" << endl;
	cout << "Player \"r\" or Player \"b\" : ";

	cin >> startPlayer;

	return startPlayer;
}

void game::playerR_move(void)
{

	player = 1;
	currentPiece = 'r';
	currentKPiece = 'R';


	makeAMove();

	player = 2;
	currentPiece = 'b';
	currentKPiece = 'B';
}

void game::playerB_move(void)
{
	player = 2;
	currentPiece = 'b';
	currentKPiece = 'B';

	makeAMove();

	player = 1;
	currentPiece = 'r';
	currentKPiece = 'R';
}
void game::calculateValidDirections(int from, int to)
{
	for (int i = from; i <= to; i++)
	{
		setOffset(i);
		if (isLegal())
		{
			vDirs.push_back(i);

		}
	}
}
//calculating the off sets for 4 directions
//toRow ,toCol the row and column to move
//jRow,jCol the row and column to jump
//rowLimit,colLimit the boundary limit of the move
void game::setOffset(int dir)
{
	switch (dir)
	{
	case 1://"1 (Upper-Left Diagonal)
		toRow = -1;
		toCol = -1;
		jRow = -2;
		jCol = -2;
		rowLimit = 0;
		colLimit = 0;

		break;
	case 2: //2 (Upper-Right Diagonal)
		toRow = -1;
		toCol = +1;
		jRow = -2;
		jCol = +2;
		rowLimit = 0;
		colLimit = 7;

		break;
	case 3: //3 (Lower-Left Diagonal)
		toRow = +1;
		toCol = -1;
		jRow = +2;
		jCol = -2;
		rowLimit = 7;
		colLimit = 0;
		break;
	case 4://4 (Lower-Right Diagonal)
		toRow = 1;
		toCol = 1;
		jRow = +2;
		jCol = +2;
		rowLimit = 7;
		colLimit = 7;

		break;
	default:
		break;
	}
}
//checks whether the move is legal
bool game::isLegal()
{
	if (isValidBoundary())
	{
		if (isEmpty(row + toRow, col + toCol))
		{
			return true;
		}
		if (isJumpable())
		{
			if (isOpponentPiece())
			{
				if (isEmpty(row + jRow, col + jCol))
				{
					return true;
				}
				else { return false; }
			}
		}
	}

	return false;
}
//checks for valid boundary
bool game::isValidBoundary()
{
	return (row == rowLimit || col == colLimit) ? false : true;
}
//checks whether the cell is empty
bool game::isEmpty(int r, int c)
{
	return cells[r][c] == " " ? true : false;
}
//checks whether the boundary is legal for jumping over
bool  game::isJumpable()
{
	return row == (rowLimit + 1) || col == (colLimit + 1) ? false : true;
}
//checks whether it is opponent piece
bool game::isOpponentPiece()
{

	return (cells[row + toRow][col + toCol] == currentPiece ||
		cells[row + toRow][col + toCol] == currentKPiece) ? false : true;
}
//moves to the cell 
void game::moveToTheCell()
{
	if (isLastRank(toRow))
	{
		cells[row + toRow][col + toCol] = currentKPiece;
	}
	else {
		cells[row + toRow][col + toCol] = cells[row][col];

	}
	destRow = row + toRow;
	destCol = col + toCol;

	clearCell(row, col);

}
//jump over the opponent 
void game::jumpOver()
{
	if (isLastRank(jRow))
	{
		cells[row + jRow][col + jCol] = currentKPiece;
	}
	else {
		cells[row + jRow][col + jCol] = cells[row][col];
	}
	destRow = row + jRow;
	destCol = col + jCol;

	clearCell((row + toRow), (col + toCol));
	clearCell(row, col);


}
//checks whether in the last rank
bool game::isLastRank(int offset)
{
	if (player == 1) {
		if ((row + offset) == 7) {
			return true;
		}
	}
	else if (player == 2) {
		if ((row + offset) == 0) {
			return true;
		}
	}
	return false;
}
//clears a cell
void game::clearCell(int r, int c)
{
	cells[r][c] = " ";
}
//reads row or column value from the user
int game::getCoOrdinate(string str)
{
	int value;
	bool bFlag = false;
	while (!bFlag) {
		cout << "Please enter the " << str << " value of the piece to move (1 to 8)(-999 for the menu) :";
		cin >> value;
		if (value == -999)
		{
			flag = false;
			return -999;
		}
		while (value < 1 || value>8)
		{
			cout << "Error:Please enter the row value (1 to 8)(-999 for the menu) :";
			cin >> value;
			if (value == -999)
			{
				flag = false;
				return -999;
			}
		}

		value = value - 1;

		return value;
	}
}

//checks for valid piece
bool game::isValidPiece(int row, int col) {
	if (cells[row][col] == " " || cells[row][col] == "#")
	{
		cout << "There is no piece in that position." << endl;
		cout << "Choose another position." << endl;
		return false;
	}
	else if ((cells[row][col] == currentPiece) ||
		(cells[row][col] == currentKPiece))
	{
		return true;

	}
	else {
		cout << "Current player is : " << player << "  and current piece is : "
			<< currentPiece << endl;

		cout << "This is opponent's piece.Play your own piece." << endl;
		cout << endl;
		return false;
	}


}
//Displays directions menu
bool game::showDirMenu()
{
	cout << "Which Direction do you want to move?(-999 for the menu) " << endl;
	displayDirMenu();

	if (dir == -999)
	{
		flag = false;
		return false;
	}

	setOffset(dir);

}
//checks whether valid direction
bool game::isValidDir(int dir)
{
	for (int i = 0; i < vDirs.size(); i++)
	{
		if (dir == vDirs[i])
		{
			return true;
		}

	}
	return false;

}
//makes final move
bool game::makeFinalMove()
{
	if (isValidBoundary())
	{
		if (isEmpty(row + toRow, col + toCol))
		{
			moveToTheCell();
			checkGameOver();
			return true;
		}
		if (isJumpable())
		{
			if (isOpponentPiece())
			{
				if (isEmpty(row + jRow, col + jCol))
				{
					jumpOver();

					accumulate();

					if (!checkForWinner())
					{
						checkGameOver();
					}
					return true;
				}

			}
		}
	}

	return false;
}
//calculates the pieces left
void game::accumulate()
{
	if (player == 1) {
		iTotalB -= 1;
	}
	else {
		iTotalR -= 1;
	}


}
//
bool game::checkForWinner()
{
	if (iTotalR == 0) {
		//endFlag = true;
		flag = false;
		cout << "\nThe winner is player 2 (black).\n" << endl;
		soundModule(2);
		return true;

	}
	else if (iTotalB == 0) {
		//endFlag = true;
		flag = false;
		cout << "\nThe winner is player 1 (red).\n" << endl;
		soundModule(2);
		return true;
	}
	else {
		return false;
	}

}
//displays direction menu
int game::displayDirMenu() {
	if (vDirs.size() >= 1) {
		do {
			cout << directions[vDirs[0] - 1];
			for (int i = 1; i < vDirs.size(); i++)
			{
				cout << " , " << directions[vDirs[i] - 1];
			}
			cout << " : ";
			cin >> dir;
			if (dir == -999)
			{
				flag = false;
				return -999;
			}

		} while (!isValidDir(dir));

	}

}

//checks for whether any valid moves left
bool game::checkGameOver(void)
{
	int status1 = 1;
	int status2 = 1;
	vDirs.clear();
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{


			if (cells[i][j] == "r")
			{
				row = i; col = j;
				calculateValidDirections(3, 4);

			}
			if (cells[i][j] == "R")
			{
				row = i; col = j;
				calculateValidDirections(1, 4);

			}
		}
	}
	if (vDirs.empty())
	{

		status1 = 0;
	}

	vDirs.clear();
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{


			if (cells[i][j] == "b")
			{
				row = i; col = j;
				calculateValidDirections(1, 2);

			}
			if (cells[i][j] == "B")
			{
				row = i; col = j;
				calculateValidDirections(1, 4);

			}
		}
	}
	if (vDirs.empty())
	{
		//cout << "The player 2 (black) has no legal moves." << endl;
		status2 = 0;

	}


	if (status1 == 0 && status2 == 0)
	{
		cout << "\nPlayer 1 and player 2 has no legal moves." << endl;
		cout << "Game is a tie.\n" << endl;
		soundModule(2);
		return true;

	}
	else if (status1 == 0) {
		cout << "\nThe player 1 (red) has no legal moves." << endl;
		cout << "Player 2 (black ) won the game.\n" << endl;
		soundModule(2);
		return true;
	}
	else if (status2 == 0)
	{
		cout << "\nThe player 2 (black )  has no legal moves." << endl;
		cout << "Player 1 (red) won the game.\n" << endl;
		soundModule(2);
		return true;
	}
	else {
		return false;
	}

}




void game::soundModule(int pick)
{
	char song[60];


	if (pick == 1)
		strcpy_s(song, "StartSound_Final.wav");
	else
		strcpy_s(song, "EndGame.wav");

	//PlaySound(TEXT(song), NULL, SND_ASYNC);
}

void game::writeStatsDataFile(string dataItem)
{
	// Use this module to write stat data to stat.txt data file
	// Open file first (use append to setup for adding records to existing file)
	// Write data to file
	// Close data file
	long long currentTime;

	currentTime = time(0);
	logFile.open("stats.txt", ios::app);
	if (logFile.fail())
	{
		cout << "Checkers log file open failes" << endl;
		system("pause");
	}
	else {
		logFile << currentTime << "   " << dataItem << endl;

	}
	logFile.close();
}


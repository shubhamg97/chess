#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

const int NUM_ROWS = 8;
const int NUM_COLS = 8;

// **************** CLASS: CELL *******************
class Cell {
	char piece;
	char color;
public:
	Cell();
	void place(char color, char piece);
	string pickup();
	string getPiece();
	string look();
};

Cell::Cell() {
	piece = ' ';
	color = ' ';
}
string Cell::look() {
	string result = "";
	result = result.append(1, color);
	result = result.append(1, piece);
	return result;
}
string Cell::pickup() {
	string result = "";
	result = result.append(1, color);
	result = result.append(1, piece);
	piece = ' ';
	color = ' ';
	return result;
}
void Cell::place(char newColor, char newPiece) {
	assert((newColor == 'W') || (newColor == 'B'));
	color = newColor;
	assert((newPiece == 'R') || (newPiece == 'K') || (newPiece == 'B') ||
		(newPiece == 'Q') || (newPiece == 'K') || (newPiece == 'N') ||
		(newPiece == 'P'));
	piece = newPiece;
}
string Cell::getPiece() {
	string result = "";
	result = result.append(1, color);
	result = result.append(1, piece);
	return result;
}

// **************** CLASS: BOARD *******************
class Board {
	Cell board[NUM_ROWS][NUM_COLS]; // <-- Not a good idea in the long run
	void displayLine();
	bool moveRook(int &row, int &col, string direction, int steps);
	bool moveBishop(int &row, int &col, string direction, int steps);
	bool moveQueen(int &row, int &col, string direction, int steps);
	bool moveKing(int &row, int &col, string direction, int steps);
	bool moveKnight(int row, int col, string direction, int steps);
	bool movePawn(int &row, int &col, string direction, int steps);
	void place(int, int, char, char);
	string pickup(int, int);
	bool cellEmpty(int, int);
	bool movePieceOneStep(string, string, int&, int&);
public:
	Board();
	Board(string, int, int, char, char);
	bool turn(int row, int col, string direction, int numSpaces);
	void displayBoard();
	string look(int, int);
	bool movePiece(int row, int col, string direction, int steps);
};

bool Board::cellEmpty(int row, int col) {
	if (board[row][col].getPiece() == "  ")
		return true;
	return false;
}
void Board::place(int row, int col, char color, char piece) {
	assert((row >= 0) && (row < NUM_ROWS));
	assert((col >= 0) && (col < NUM_COLS));
	board[row][col].place(color, piece);
}
string Board::look(int row, int col) {
	assert((row >= 0) && (row < NUM_ROWS));
	assert((col >= 0) && (col < NUM_COLS));
	return board[row][col].look();
}
string Board::pickup(int row, int col) {
	assert((row >= 0) && (row < NUM_ROWS));
	assert((col >= 0) && (col < NUM_COLS));
	return board[row][col].pickup();
}

Board::Board() {
	board[0][0].place('B', 'R');
	board[0][1].place('B', 'N');
	board[0][2].place('B', 'B');
	board[0][3].place('B', 'Q');
	board[0][4].place('B', 'K');
	board[0][5].place('B', 'B');
	board[0][6].place('B', 'N');
	board[0][7].place('B', 'R');
	for (int c = 0; c < NUM_COLS; c++) {
		board[1][c].place('B', 'P');
	}

	board[NUM_ROWS - 1][0].place('W', 'R');
	board[NUM_ROWS - 1][1].place('W', 'N');
	board[NUM_ROWS - 1][2].place('W', 'B');
	board[NUM_ROWS - 1][4].place('W', 'K');
	board[NUM_ROWS - 1][3].place('W', 'Q');
	board[NUM_ROWS - 1][5].place('W', 'B');
	board[NUM_ROWS - 1][6].place('W', 'N');
	board[NUM_ROWS - 1][7].place('W', 'R');
	for (int c = 0; c < NUM_COLS; c++) {
		board[NUM_ROWS - 2][c].place('W', 'P');
	}
}
void Board::displayLine() {
	cout << endl;
	for (int x = 0; x < NUM_COLS + 1; x++) {
		cout << "    | ";
	}
	cout << endl;
	for (int x = 0; x < NUM_COLS + 1; x++) {
		cout << "----| ";
	}
	cout << endl;
}
void Board::displayBoard() {
	cout << endl << "CURRENT BOARD:" << endl << endl;
	cout << "    | ";
	for (int c = 0; c < NUM_COLS; c++) {
		cout << char('0' + c) << "   | ";
	}
	displayLine();
	for (int r = 0; r < NUM_ROWS; r++) {
		cout << char('A' + r) << "   | ";
		for (int c = 0; c < NUM_COLS; c++) {
			cout << " " << board[r][c].getPiece() << " | ";
		}
		displayLine();
	}
	cout << endl << endl;
}

bool Board::movePieceOneStep(string piece, string direction, int& row, int& col) {
	assert((row >= 0) && (row < NUM_ROWS));
	assert((col >= 0) && (col < NUM_COLS));
	int toRow = row;
	int toCol = col;
	if (direction == "S")
		toRow = row + 1;
	else if (direction == "N")
		toRow = row - 1;
	else if (direction == "E")
		toCol = col + 1;
	else if (direction == "W")
		toCol = col - 1;
	else if (direction == "NW") {
		toRow = row - 1;
		toCol = col - 1;
	}
	else if (direction == "NE") {
		toRow = row - 1;
		toCol = col + 1;
	}
	else if (direction == "SW") {
		toRow = row + 1;
		toCol = col - 1;
	}
	else if (direction == "SE") {
		toRow = row + 1;
		toCol = col + 1;
	}
	else {
		cout << "INVALID DIRECTION!" << endl;
		assert(false); // force a failure
	}

	assert((toRow >= 0) && (toRow < NUM_ROWS));
	assert((toCol >= 0) && (toCol < NUM_COLS));

	if (!cellEmpty(toRow, toCol)) {
		cout << "Space [ " << toRow << " , " << toCol <<
			"] Contains [" << look(toRow, toCol) << "]" << endl;
		return false;
	}

	piece = pickup(row, col);
	place(toRow, toCol, piece.at(0), piece.at(1));
	row = toRow;
	col = toCol;
	return true;
}

bool Board::moveRook(int &row, int &col, string direction, int steps) {
	string piece = look(row, col);
	assert(piece.at(1) == 'R');

	if (!((direction == "N") || (direction == "S") || (direction == "E") || (direction == "W"))) {
		cout << "Rooks cannot move" << direction << "!" << endl;
		return false;
	}
	for (int x = 0; x < steps; x++) {
		if (!movePieceOneStep(piece, direction, row, col)) {
			return false;
		}
	}
	return true;

}

bool Board::moveBishop(int &row, int &col, string direction, int steps) {
	string piece = look(row, col);
	assert(piece.at(1) == 'B');

	if (!((direction == "NE") || (direction == "NW") || (direction == "SE") || (direction == "SW"))) {
		cout << "Bishops cannot move" << direction << "!" << endl;
		return false;
	}
	for (int x = 0; x < steps; x++) {
		if (!movePieceOneStep(piece, direction, row, col)) {
			return false;
		}
	}
	return true;
}

bool Board::moveQueen(int &row, int &col, string direction, int steps) {
	string piece = look(row, col);
	assert(piece.at(1) == 'Q');

	if (!((direction == "NE") || (direction == "NW") || (direction == "SE") || (direction == "SW")
		|| (direction == "N") || (direction == "S") || (direction == "E") || (direction == "W"))) {
		cout << "Queens cannot move" << direction << "!" << endl;
		return false;
	}
	for (int x = 0; x < steps; x++) {
		if (!movePieceOneStep(piece, direction, row, col)) {
			return false;
		}
	}
	return true;
}

bool Board::moveKing(int &row, int &col, string direction, int steps) {
	string piece = look(row, col);
	assert(piece.at(1) == 'K');

	if (!((direction == "NE") || (direction == "NW") || (direction == "SE") || (direction == "SW")
		|| (direction == "N") || (direction == "S") || (direction == "E") || (direction == "W"))) {
		cout << "Kings cannot move" << direction << "!" << endl;
		return false;
	}
	if (steps > 1) {
		cout << "Knights cannot move " << steps << " steps at a time!" << endl;
		return false;
	}
	for (int x = 0; x < steps; x++) {
		if (!movePieceOneStep(piece, direction, row, col)) {
			return false;
		}
	}
	return true;
}

bool Board::moveKnight(int row, int col, string direction, int steps) {
	string piece = look(row, col);
	assert(piece.at(1) == 'N');

	if (steps > 1) {
		cout << "Knights cannot move " << steps << " steps at a time!" << endl;
		return false;
	}
	int toRow = row;
	int toCol = col;
	if (direction == "NNE") {
		toRow -= 2;
		toCol += 1;
	}
	else if (direction == "NEE") {
		toRow -= 1;
		toCol += 2;
	}
	else if (direction == "SEE") {
		toRow += 1;
		toCol += 2;
	}
	else if (direction == "SSE") {
		toRow += 2;
		toCol += 1;
	}
	else if (direction == "SSW") {
		toRow += 2;
		toCol -= 1;
	}
	else if (direction == "SWW") {
		toRow += 1;
		toCol -= 2;
	}
	else if (direction == "NWW") {
		toRow -= 1;
		toCol -= 2;
	}
	else if (direction == "NNW") {
		toRow -= 2;
		toCol -= 1;
	}
	else {
		cout << "Knights cannot move " << direction << "!" << endl;
		return false;
	}

	if (!cellEmpty(toRow, toCol)) {
		cout << "Space [ " << toRow << " , " << toCol <<
			"] Contains [" << look(toRow, toCol) << "]" << endl;
		return false;
	}

	piece = pickup(row, col);
	place(toRow, toCol, piece.at(0), piece.at(1));

	return true;
}

bool Board::movePawn(int &row, int &col, string direction, int steps) {
	string piece = look(row, col);
	assert(piece.at(1) == 'P');

	if ((piece.at(0) == 'W') && (direction != "N") && (direction != "NW") && (direction != "NE")) {
		cout << "White pawns cannot move " << direction << "!" << endl;
		return false;
	}
	if ((piece.at(0) == 'B') && (direction != "S") && (direction != "SW") && (direction != "SE")) {
		cout << "Black pawns cannot move " << direction << "!" << endl;
		return false;
	}

	if (steps == 1) {
		for (int x = 0; x < steps; x++) {
			if (!movePieceOneStep(piece, direction, row, col))
				return false;
		}
	}

	if ((steps == 2) && ((row == 1) || (row == 6))) {
		for (int x = 0; x < steps; x++) {
			if (!movePieceOneStep(piece, direction, row, col))
				return false;
		}
	}

	else {
		cout << "Pawns can only move 1 step after their first move!" << endl;
		return false;
	}
	return true;
}

bool Board::movePiece(int row, int col, string direction, int steps) {
	int holdRow = row, holdCol = col;
	bool result = false;
	string piece = look(row, col);
	if (piece.at(1) == 'R')
		result = moveRook(row, col, direction, steps);
	else if (piece.at(1) == 'P')
		result = movePawn(row, col, direction, steps);
	else if (piece.at(1) == 'B')
		result = moveBishop(row, col, direction, steps);
	else if (piece.at(1) == 'Q')
		result = moveQueen(row, col, direction, steps);
	else if (piece.at(1) == 'K')
		result = moveKing(row, col, direction, steps);
	else if (piece.at(1) == 'N')
		result = moveKnight(row, col, direction, steps);
	else {
		cout << "Invalid piece " << piece << " at position [" << row << " , " << col << "]" << endl;
		assert(false);
	}
	if (result)
		return true;

	piece = pickup(row, col);
	place(holdRow, holdCol, piece.at(0), piece.at(1));

	return false;
}

bool mated() {
	return false;
}

bool makeMove(Board &board, char color) {
	char row;
	int col, steps;
	string direction;

	assert((color == 'W') || (color == 'B'));

	if (color == 'W') {
		cout << "White's Turn: " << endl;
	}
	else {
		cout << "Black's Turn: " << endl;
	}
	cout << "CURRENT BOARD:" << endl;
	
	cout << "Enter cell (row/col) to move: ";
	cin >> row >> col;
	row = row - 65;

	if (((row < 0) || (row >= NUM_ROWS)) && ((col < 0) || (col >= NUM_COLS))) {
		cout << "OUT OF BOUNDS!" << endl;
		return false;
	}
	string piece = board.look(row, col);
	if (piece.at(0) != color) {
		cout << "Wrong color" << endl;
		return false;
	}

	cout << endl << "Enter direction: ";
	cin >> direction;
	cout << endl << "Enter number of steps: ";
	cin >> steps;

	return board.movePiece(row, col, direction, steps);
}


int main() {

	Board board;
	string goOn = "";
	cout << "BEGINNING BOARD:" << endl;
	board.displayBoard();

	while ((goOn != "N") && (goOn != "n")) {
		while (!makeMove(board, 'W'));
		board.displayBoard();
		while (!makeMove(board, 'B'));
		board.displayBoard();
		cout << "Continue? (Y/N) ";
		cin >> goOn;
	}

	cout << "ENDING BOARD:" << endl;
	board.displayBoard();
	return 0;
}
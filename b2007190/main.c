# include <stdio.h>

# define MAX_LENGTH 100
# define ROWS 9
# define COLUMNS 9
# define MAX_VALUE 10
# define EMPTY 0
# define AREA_SQUARE_SIZE 3
# define INF 999999

typedef struct {
	int x, y;
}Coord;

typedef struct {
	Coord data[MAX_LENGTH];
	int size;
}ListCoord;

void initListCoord(ListCoord *list) {
	list->size = 0;
}

void appendListCoord(ListCoord *list, Coord coord) {
	list->data[list->size++] = coord;
}

typedef struct {
	int data[MAX_LENGTH];
	int size;
}List;

void initList(List *list) {
	list->size = 0;
}

void appendList(List *list, int x) {
	list->data[list->size++] = x;
}

typedef struct {
	int data[ROWS * COLUMNS][ROWS * COLUMNS];
	int n;
}Constrain;

void initConstrain(Constrain *constrain) {
	int i, j;
	for (i=0; i < ROWS * COLUMNS; i++)
		for (j=0; j < ROWS * COLUMNS; j++)
			constrain->	data[i][j] = 0;
	constrain->n = ROWS * COLUMNS;
		
}

int indexOf(Coord coord) {
	return (ROWS * coord.x + coord.y);
}

Coord positionOfVertex(int vertex) {
	Coord coord;
	coord.x = vertex / ROWS;
	coord.y = vertex % COLUMNS;
	return coord;
}

int addConstrain(Constrain *constrain, Coord source, Coord target) {
	int u = indexOf(source);
	int v = indexOf(target);
	if (constrain->data[u][v] == 0) {
		constrain->data[u][v] = 1;
		constrain->data[v][u] = 1;
		return 1;
	}
	return 0;
}

ListCoord getConstrain(Constrain constrain, Coord coord) {
	int i;
	int v = indexOf(coord);
	ListCoord result;
	initListCoord(&result);
	for (i=0; i < constrain.n; i++) {
		if (constrain.data[v][i] == 1)
			appendListCoord(&result, positionOfVertex(i));
	}
	return result;
}

typedef struct {
	int cells[ROWS][COLUMNS];
	Constrain constrain;	
}Sudoku;

void initSudoku(Sudoku *sudoku) {
	int i, j;
	for (i=0; i < ROWS; i++)
		for (j=0; j < COLUMNS; j++)
			sudoku->cells[i][j] = EMPTY;
	initConstrain(&sudoku->constrain);
}

void initSudokuWithValue(Sudoku *sudoku, int input[ROWS][COLUMNS]) {
	int i, j;
	for (i=0; i < ROWS; i++)
		for (j=0; j < COLUMNS; j++)
			sudoku->cells[i][j] = input[i][j];
	initConstrain(&sudoku->constrain);
}

void printSudoku(Sudoku sudoku) {
	int i, j;
	printf("Sudoku:\n");
	for (i=0; i < ROWS; i++) {
		if (i % AREA_SQUARE_SIZE == 0)
			printf("-------------------------\n");
		for (j=0; j < COLUMNS; j++) {
			if (j % AREA_SQUARE_SIZE == 0)
				printf("| ");
			printf("%d ", sudoku.cells[i][j]);
		}
		printf("|\n");
	}
	printf("-------------------------\n");
}

int isFillSudoku(Sudoku sudoku) {
	int i, j;
	for (i=0; i < ROWS; i++)
		for (j=0; j < COLUMNS; j++)
			if (sudoku.cells[i][j] == EMPTY)
				return 0;
	return 1;
}

void spreadConstrainFrom(Coord position, Constrain *constrain, ListCoord *changeds) {
	int row = position.x, column = position.y;
	int i, j;
	for (i=0; i < ROWS; i++)
		if (i != row) {
			Coord pos = {i, column};
			if (addConstrain(constrain, position, pos))
				appendListCoord(changeds, pos);
		}
		
	for (i=0; i < COLUMNS; i++)
		if (i != column) {
			Coord pos = {row, i};
			if (addConstrain(constrain, position, pos))
				appendListCoord(changeds, pos);
		}
		
	for (i=0; i < AREA_SQUARE_SIZE; i++)
		for (j=0; j < AREA_SQUARE_SIZE; j++) {
			int areaX = (row / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
			int areaY = (column / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
			if (areaX+i != row || areaY+i != column) {
				Coord pos = {areaX+i, areaY+i};
				if (addConstrain(constrain, position, pos))
					appendListCoord(changeds, pos);
			}
		}
}

List getAvailableValues(Coord position, Sudoku sudoku) {
	ListCoord posList = getConstrain(sudoku.constrain, position);
	int available[MAX_VALUE];
	int i;
	for (i=1; i < MAX_VALUE; i++)
		available[i] = 1;
	for (i=0; i < posList.size; i++) {
		Coord pos = posList.data[i];
		if (sudoku.cells[pos.x][pos.y] != EMPTY)
			available[sudoku.cells[pos.x][pos.y]] = 0;                                                                     
	}
	List result;
	initList(&result);
	for (i=1; i < MAX_VALUE; i++)
		if (available[i])
			appendList(&result, i);
	return result;
}

Coord getNextEmptyCell(Sudoku sudoku) {
	int i, j;
	for (i=0; i < ROWS; i++)
		for (j=0; j < COLUMNS; j++) {
			Coord pos = {i, j};
			if (sudoku.cells[i][j] = EMPTY)
				return pos;
		}
}

Coord getNextMinDomainCell(Sudoku sudoku) {
	int minLength = INF;
	int i, j;
	Coord result;
	for (i=0; i < ROWS; i++)
		for (j=0; j < COLUMNS; j++) {
			if (sudoku.cells[i][j] == EMPTY) {
				Coord pos = {i, j};
				int availableLength = getAvailableValues(pos, sudoku).size;
				if (availableLength < minLength) {
					minLength = availableLength;
					result = pos;
				}
			}
		}
	return result;
}

int exploredCounter = 0;
int sudokuBackTracking(Sudoku *sudoku) {
	if (isFillSudoku(*sudoku))
		return 1;
	Coord position = getNextMinDomainCell(*sudoku);
	List available = getAvailableValues(position, *sudoku);
	if (available.size == 0)
		return 0;
	int j;
	for (j=0; j < available.size; j++) {
		int value = available.data[j];
		sudoku->cells[position.x][position.y] = value;
		exploredCounter++;
		ListCoord history;
		initListCoord(&history);
		if (sudokuBackTracking(sudoku))
			return 1;
		sudoku->cells[position.x][position.y]  = EMPTY;
	}
	return 0;
}

Sudoku solveSudoku(Sudoku sudoku) {
	int i, j;
	initConstrain(&sudoku.constrain);
	for (i=0; i < ROWS; i++)
		for (j=0; j < COLUMNS; j++) {
			if (sudoku.cells[i][j] != EMPTY) {
				ListCoord history;
				initListCoord(&history);
				Coord pos = {i, j};
				spreadConstrainFrom(pos, &sudoku.constrain, &history);
			}
		}
	exploredCounter = 0;
	if (sudokuBackTracking(&sudoku))
		printf("Solved\n");
	else
		printf("Can not solve\n");
	printf("Explored %d states\n", exploredCounter);
	return sudoku;
}

int main() {
	int input[9][9] = {
		{5, 3, 0, 0, 7, 0, 0, 0, 0},
		{6, 0, 0, 1, 9, 5, 0, 0, 0},
		{0, 9, 8, 0, 0, 0, 0, 6, 0},
		{8, 0, 0, 0, 6, 0, 0, 0, 3},
		{4, 0, 0, 8, 0, 3, 0, 0, 1},
		{7, 0, 0, 0, 2, 0, 0, 0, 6},
		{0, 6, 0, 0, 0, 0, 2, 8, 0},
		{0, 0, 0, 4, 1, 9, 0, 0, 5},
		{0, 0, 0, 0, 8, 0, 0, 7, 9},
	};
	Sudoku sudoku;
	initSudokuWithValue(&sudoku, input);
	printSudoku(sudoku);
	Sudoku result = solveSudoku(sudoku);
	printSudoku(result);
	return 0;
}

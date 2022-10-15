# include <stdio.h>

# define MAX_LENGTH 100
# define ROWS 9
# define COLUMNS 9
# define MAX_VALUE 10
# define EMPTY 0
# define AREA_SQUARE_SIZE 3
# define INF 999999

/* khai bao cau truc toa do xac dinh vi tri cua mot o */
typedef struct {
	int x, y;
}Coord;

/* khai bao cau truc danh sach toa do */
typedef struct {
	Coord data[MAX_LENGTH];
	int size;
}ListCoord;

/* khoi tao danh sach toa do */
void initListCoord(ListCoord *list) {
	list->size = 0;
}

/* them phan tu vao danh sach toa do */
void appendListCoord(ListCoord *list, Coord coord) {
	list->data[list->size++] = coord;
}

/* khai bao cau truc danh sach */
typedef struct {
	int data[MAX_LENGTH];
	int size;
}List;

/* khoi tao danh sach */
void initList(List *list) {
	list->size = 0;
}

/* them phan tu vao danh sach */
void appendList(List *list, int x) {
	list->data[list->size++] = x;
}

/* khai bao cau truc rang buoc 
   bieu dien bang ma tran dinh dinh */
typedef struct {
	int data[ROWS * COLUMNS][ROWS * COLUMNS];
	int n;
}Constrain;

/* khoi tao ma tran rang buoc */
void initConstrain(Constrain *constrain) {
	int i, j;
	for (i=0; i < ROWS * COLUMNS; i++)
		for (j=0; j < ROWS * COLUMNS; j++)
			constrain->	data[i][j] = 0;
	constrain->n = ROWS * COLUMNS;
		
}

/* ham chuyen doi toa do cua mot o sang mot dinh */
int indexOf(Coord coord) {
	return (ROWS * coord.x + coord.y);
}

/* ham chuyen doi gia tri cua mot dinh sang toa do cua mot o */
Coord positionOfVertex(int vertex) {
	Coord coord;
	coord.x = vertex / ROWS;
	coord.y = vertex % COLUMNS;
	return coord;
}

/* 
   them rang buoc giua hai dinh
   neu them rang buoc thanh cong: tra ve 1
   nguoc lai tra ve 0
*/
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

/* 
	ham tra ve danh sach cac o trong sudoku co rang buoc
	voi mot o
*/
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

/* khai bao cau truc Sudoku. Trong do:
	- cells: ma tran 9x9 chua gia tri trong Sudoku
	- constrain: ma tran rang buoc cac o trong Sudoku
*/
typedef struct {
	int cells[ROWS][COLUMNS];
	Constrain constrain;
}Sudoku;

/* khoi tao Sudoku rong */
void initSudoku(Sudoku *sudoku) {
	int i, j;
	for (i=0; i < ROWS; i++)
		for (j=0; j < COLUMNS; j++)
			sudoku->cells[i][j] = EMPTY;
	initConstrain(&sudoku->constrain);
}

/* khoi tao Sudoku voi ma tran co san */
void initSudokuWithValue(Sudoku *sudoku, int input[ROWS][COLUMNS]) {
	int i, j;
	for (i=0; i < ROWS; i++)
		for (j=0; j < COLUMNS; j++)
			sudoku->cells[i][j] = input[i][j];
	initConstrain(&sudoku->constrain);
}

/* in Sudoku */
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

/* ham kiem tra trang thai ket thuc cua bai toan */
int isFillSudoku(Sudoku sudoku) {
	int i, j;
	for (i=0; i < ROWS; i++)
		for (j=0; j < COLUMNS; j++)
			if (sudoku.cells[i][j] == EMPTY)
				return 0;
	return 1;
}

/* ham lan truyen rang buoc tu mot o */
void spreadConstrainFrom(Coord position, Constrain *constrain, ListCoord *changeds) {
	int row = position.x, column = position.y;
	int i, j;

	// lan truyen rang buoc trong cot chua o do
	for (i=0; i < ROWS; i++)
		if (i != row) {
			Coord pos = {i, column};
			if (addConstrain(constrain, position, pos))
				appendListCoord(changeds, pos);
		}
	
	// lan truyen rang buoc trong dong chua o do
	for (i=0; i < COLUMNS; i++)
		if (i != column) {
			Coord pos = {row, i};
			if (addConstrain(constrain, position, pos))
				appendListCoord(changeds, pos);
		}
	
	// lan truyen rang buoc trong khoi chua o do
	for (i=0; i < AREA_SQUARE_SIZE; i++)
		for (j=0; j < AREA_SQUARE_SIZE; j++) {
			int areaX = (row / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
			int areaY = (column / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
			if (areaX+i != row || areaY+j != column) {
				Coord pos = {areaX+i, areaY+j};
				if (addConstrain(constrain, position, pos))
					appendListCoord(changeds, pos);
			}
		}
}

/* ham tim mien gia tri cho o trong */
List getAvailableValues(Coord position, Sudoku sudoku) {
	// tra ve danh sach cac o co rang buoc voi position
	ListCoord posList = getConstrain(sudoku.constrain, position);

	int available[MAX_VALUE];
	int i;
	for (i=1; i < MAX_VALUE; i++)
		available[i] = 1;

	/* duyet qua danh sach, o nao chua gia tri thi loai gia tri cua
	o do ra khoi mien gia tri cua position */
	for (i=0; i < posList.size; i++) {
		Coord pos = posList.data[i];
		if (sudoku.cells[pos.x][pos.y] != EMPTY)
			available[sudoku.cells[pos.x][pos.y]] = 0;                                                        
	}
	List result;
	initList(&result);
	for (i=1; i < MAX_VALUE; i++) {
		if (available[i])
			appendList(&result, i);
	}
	return result;
}

/* ham tra ve mot o trong trong Sudoku */
Coord getNextEmptyCell(Sudoku sudoku) {
	int i, j;
	for (i=0; i < ROWS; i++)
		for (j=0; j < COLUMNS; j++) {
			Coord pos = {i, j};
			if (sudoku.cells[i][j] = EMPTY)
				return pos;
		}
}

/* ham tra ve o co mien gia tri it nhat */
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

/* thuat toan Quay lui giai Sudoku */
int exploredCounter = 0;
int sudokuBackTracking(Sudoku *sudoku) {
	// dieu kien dung de quy: neu Sudoku da duoc dien het gia tri thi dung
	if (isFillSudoku(*sudoku))
		return 1;

	/* 
		tim o trong co mien gia tri thap nhat & tim mien gia tri cua o trong do
		neu mien gia tri la rong thi tra ve 0 (conflict)
	*/
	Coord position = getNextMinDomainCell(*sudoku);
	List available = getAvailableValues(position, *sudoku);
	if (available.size == 0)
		return 0;
	int j;
	/*
		lan luot dien cac gia tri trong mien gia tri vao o trong
	*/
	for (j=0; j < available.size; j++) {
		int value = available.data[j];
		sudoku->cells[position.x][position.y] = value;
		exploredCounter++;

		/*
			- neu trang thai con tra ve 1 thi bai toan da giai xong
			- neu trang thai con tra ve 0 thi co conflict, 
			xoa gia tri hien tai cua o trong & tiep tuc gan gia tri ke tiep
			trong mien gia tri
		*/
		if (sudokuBackTracking(sudoku))
			return 1;
		sudoku->cells[position.x][position.y]  = EMPTY;
	}
	return 0;
}

/* lan truyen rang buoc & giai bai toan */
Sudoku solveSudoku(Sudoku sudoku) {
	int i, j;
	// lan truyen rang buoc cho tat ca cac o trong Sudoku
	for (i=0; i < ROWS; i++)
		for (j=0; j < COLUMNS; j++) {
			ListCoord history;
			initListCoord(&history);
			Coord pos = {i, j};
			spreadConstrainFrom(pos, &sudoku.constrain, &history);
		}
	
	// giai bai toan
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

	/* khoi tao trang thai dau cho bai toan */
	Sudoku sudoku;
	initSudokuWithValue(&sudoku, input);
	printSudoku(sudoku);

	/* giai bai toan */
	Sudoku result = solveSudoku(sudoku);
	printSudoku(result);
	return 0;
}
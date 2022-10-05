# include <stdio.h>
# include <stdlib.h>

# define ROWS 3 // so luong dong
# define COLS 3 // so luong cot
# define EMPTY 0
# define MAX_OPERATOR 4 // so luong hanh dong
# define MAXLENGTH 500

/* cac hanh dong */
const char* action[] = {"First State", "Move cell EMPTY to UP", "Move cell EMPTY to DOWN", "Move cell EMPTY to LEFT", "Move cell EMPTY to RIGHT"};

/* khai bao State */
typedef struct {
    int eightPuzzle[ROWS][COLS];
    int emptyRow;
    int emptyCol;
}State;

/* hien thi trang thai */
void printState(State state) {
    int row, col;
    printf("\n----------\n");
    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++)
            printf("|%d ", state.eightPuzzle[row][col]);
        printf("|\n");
    }
    printf("----------\n");
}

/* so sanh hai trang thai */
int compareState(State state1, State state2) {
    // neu vi tri cua o trong trong hai trang khac nhau thi tra ve 0
    if (state1.emptyCol != state2.emptyCol || state1.emptyRow != state2.emptyRow)
        return 0;
    int row, col;
    // so sanh het tat ca cac vi tri trong hai trang thai
    for (row = 0; row < ROWS; row++) 
        for (col = 0; col < COLS; col++)
            if (state1.eightPuzzle[row][col] != state2.eightPuzzle[row][col])
                return 0;

    return 1;
}

/* kiem tra ket qua */
int goalCheck(State state, State goal) {
    return compareState(state, goal);
}

/* ham di chuyen o trong len tren */
int upOperator(State currentState, State *result) {
    *result = currentState;
    int emptyRowCurrent = currentState.emptyRow, emptyColCurrent = currentState.emptyCol;
    if (emptyRowCurrent > 0){
        result->emptyRow = emptyRowCurrent - 1;
        result->emptyCol = emptyColCurrent;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = result->eightPuzzle[emptyRowCurrent-1][emptyColCurrent];
        result->eightPuzzle[emptyRowCurrent-1][emptyColCurrent] = EMPTY;
        return 1;
    }
    return 0;
}

/* ham di chuyen o trong xuong duoi */
int downOperator(State currentState, State *result) {
    *result = currentState;
    int emptyRowCurrent = currentState.emptyRow, emptyColCurrent = currentState.emptyCol;
    if (emptyRowCurrent < ROWS-1){
        result->emptyRow = emptyRowCurrent + 1;
        result->emptyCol = emptyColCurrent;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = result->eightPuzzle[emptyRowCurrent+1][emptyColCurrent];
        result->eightPuzzle[emptyRowCurrent+1][emptyColCurrent] = EMPTY;
        return 1;
    }
    return 0;
}

/* ham di chuyen o trong sang trai */
int leftOperator(State currentState, State *result) {
    *result = currentState;
    int emptyRowCurrent = currentState.emptyRow, emptyColCurrent = currentState.emptyCol;
    if (emptyColCurrent > 0) {
        result->emptyRow = emptyRowCurrent;
        result->emptyCol = emptyColCurrent - 1;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = result->eightPuzzle[emptyRowCurrent][emptyColCurrent-1];
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent-1] = EMPTY;
        return 1;
    }
    return 0;
}

/* ham di chuyen o trong sang phai */
int rightOperator(State currentState, State *result) {
    *result = currentState;
    int emptyRowCurrent = currentState.emptyRow, emptyColCurrent = currentState.emptyCol;
    if (emptyColCurrent < COLS-1) {
        result->emptyRow = emptyRowCurrent;
        result->emptyCol = emptyColCurrent + 1;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = result->eightPuzzle[emptyRowCurrent][emptyColCurrent+1];
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent+1] = EMPTY;
        return 1;
    }
    return 0;
}

/* ham thuc hien cac hanh dong len mot trang thai */
int callOperator(State currentState, State *result, int option) {
    switch (option) {
        case 1: return upOperator(currentState, result);
        case 2: return downOperator(currentState, result);
        case 3: return leftOperator(currentState, result);
        case 4: return rightOperator(currentState, result);

        default:
            printf("Error\n");
            return 0;
    }
}

int main() {
    State state1, result;
    state1.eightPuzzle[0][0] = 3;
    state1.eightPuzzle[0][1] = 4;
    state1.eightPuzzle[0][2] = 5;
    state1.eightPuzzle[1][0] = 1;
    state1.eightPuzzle[1][1] = 0;
    state1.eightPuzzle[1][2] = 2;
    state1.eightPuzzle[2][0] = 6;
    state1.eightPuzzle[2][1] = 7;
    state1.eightPuzzle[2][2] = 8;
    state1.emptyCol = 1;
    state1.emptyRow = 1;

    printf("Trang thai bat dau\n");
    printState(state1);
    int opt;
    for (opt = 1; opt <= MAX_OPERATOR; opt++) {
        callOperator(state1, &result, opt);
        if (!compareState(state1, result)) {
            printf("Hanh dong %s thanh cong\n", action[opt]);
            printState(result);
        }
        else
            printf("Hanh dong %s khong thanh cong\n", action[opt]);
    }
    return 0;
}
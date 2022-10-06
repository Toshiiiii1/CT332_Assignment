# include <stdio.h>
# include <stdlib.h>
# include <vector>
# include <algorithm>

using namespace std;

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
int compareState(State s1,State s2){
	if(s1.emptyRow != s2.emptyRow || s1.emptyCol != s2.emptyCol)
		return 0;
	int row, col;
	for(row=0; row < ROWS; row++)
		for(col=0; col < COLS; col++)
			if(s1.eightPuzzle[row][col] != s2.eightPuzzle[row][col])
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
            return 0;
    }
}

/* ham heuristic so 1 */
int heuristic1(State state, State goal) {
    int row, col, count = 0;
    for (row = 0; row < ROWS; row++)
        for (col = 0; col < COLS; col++)
            if (state.eightPuzzle[row][col] != goal.eightPuzzle[row][col])
                count++;
    return count;
}

// tim tong so buoc de di chuyen cac o sai ve dung vi tri
int heuristic2(State state, State goal) {
	int count = 0;
	/* 
		- row, col: vi tri cua o dang xet trong state
		- row_g, col_g: vi tri cua o dang xet trong goal
	*/
	int row, col, row_g, col_g;
	for (row=0; row < ROWS; row++) 
		for (col=0; col < COLS; col++)
			if (state.eightPuzzle[row][col] != EMPTY)
				for (row_g=0; row_g < ROWS; row_g++)
					for (col_g=0; col_g < COLS; col_g++)
					/* tinh so buoc di chuyen mot o ve vi tri dung */
						if (state.eightPuzzle[row][col] == goal.eightPuzzle[row_g][col_g]) {
							count += abs(row - row_g) + abs(col - col_g);
							col_g = COLS; // ket thuc vong lap col_g
							row_g = ROWS; // key thuc vong lap row_g
						}
	return count;
}

/* khai bao Node */
typedef struct Node{
    State state;
    struct Node *parent;
    int no_function;
    int heuristic;
}Node;

Node* findState(State state, vector<Node*> v, vector<Node*>::iterator *pos){
	vector<Node*>::iterator it = v.begin();
    if (v.size() == 0)
        return NULL;
    while (it != v.end()) {
        if (compareState((*it)->state, state)) {
            *pos = it;
            return *it;
        }
        it = v.erase(it);
    }
    return NULL;
}

bool compareHeuristic(Node *a, Node *b) {
    return a->heuristic > b->heuristic;
}

Node* bestFS(State s, State goal){
	vector<Node*> Open(MAXLENGTH);
    Open.clear();
    vector<Node*> Close(MAXLENGTH);
    Close.clear();

	Node* root = (Node*)malloc(sizeof(Node));
	root->state = s;
	root->parent = NULL;
	root->no_function = 0;
	root->heuristic = heuristic1(s, goal);
	Open.push_back(root);

	while(!Open.empty()){
		Node* node = Open.back();
		Open.pop_back();
		Close.push_back(node);
		if(goalCheck(node->state, goal))
			return node;

		int opt;
		for(opt=1; opt <= MAX_OPERATOR; opt++){
			State newstate;
			newstate = node->state;
			if(callOperator(node->state, &newstate, opt)){
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->no_function = opt;
				newNode->parent = node;
				newNode->heuristic = heuristic1(newstate, goal);

				//kiem tra trang thai moi trong hai danh sach Open & Close
				vector<Node*>::iterator pos_Open, pos_Close;
				Node* nodeOpen = findState(newstate, Open, &pos_Open);
				Node* nodeClose = findState(newstate, Close, &pos_Close);

				/* neu trang thai moi chua co trong Open & Close => them trang thai moi vao Open */
				if(nodeOpen == NULL && nodeClose == NULL)
					Open.push_back(newNode);

				/* neu trang thai moi da ton tai trong Open va co gia tri heu tot hon => them trang thai moi vao Open */
				else if(nodeOpen != NULL && nodeOpen->heuristic > newNode->heuristic){
					Open.erase(pos_Open);
                    Open.push_back(newNode);
				}
				
				/* neu trang thai moi da ton tai trong Close va co gia tri heu tot hon => them trang thai moi vao Open */
				else if(nodeClose != NULL && nodeClose->heuristic > newNode->heuristic){
					Close.erase(pos_Close);
                    Open.push_back(newNode);
				}
			}
		}
        sort(Open.begin(), Open.end(), compareHeuristic);
	}
	return NULL;
}

void print_Way(Node* node){
	vector<Node*> vectorPrint;
	while(node->parent != NULL){
		vectorPrint.push_back(node);
		node = node->parent;
	}
    vectorPrint.push_back(node);
	int i, ac = 0;
	for(i = vectorPrint.size() - 1; i>=0; i--){
		printf("\nAction %d: %s ", ac, action[vectorPrint.at(i)->no_function]);
		printState(vectorPrint.at(i)->state);
		ac++;
	}
}

int main() {
    State state1, goal;
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

    goal.eightPuzzle[0][0] = 0;
    goal.eightPuzzle[0][1] = 1;
    goal.eightPuzzle[0][2] = 2;
    goal.eightPuzzle[1][0] = 3;
    goal.eightPuzzle[1][1] = 4;
    goal.eightPuzzle[1][2] = 5;
    goal.eightPuzzle[2][0] = 6;
    goal.eightPuzzle[2][1] = 7;
    goal.eightPuzzle[2][2] = 8;
    goal.emptyCol = 0;
    goal.emptyRow = 0;

    Node* p = bestFS(state1,goal);
	print_Way(p);
    return 0;
}

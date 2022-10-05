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
	/* emptyRow, emptyCol: vi tri cua o trong trong ma tran */
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

/* ham heuristic so 1 */
int heuristic1(State state, State goal) {
    int row, col, count = 0;
    for (row = 0; row < ROWS; row++)
        for (col = 0; col < COLS; col++)
            if (state.eightPuzzle[row][col] != goal.eightPuzzle[row][col])
                count++;
    return count;
}

/* khai bao cau truc Node */
typedef struct Node {
	State state;
	struct Node *parent;
	int no_function, f, g, h;
}Node;

/* khai bao kieu List */

typedef struct {
    Node* Elements[MAXLENGTH];
    int size;
}List;

// khoi tao List
void makeNullList(List *L){
	L->size = 0;
}

// kiem tra danh sach rong
int emptyList(List L){
	return L.size == 0;
}

// kiem tra danh sach day
int full(List L){
	return L.size == MAXLENGTH-1;
}

// tra ve phan tu trong danh sach
Node* elementAt(int p, List L){
	if(emptyList(L)) 
        return NULL;
	else if(p < 1 || p > L.size) 
        return NULL;
	return L.Elements[p-1];
}

// them phan tu vao danh sach
void pushList(Node* x, int pos, List *L){
	if(!full(*L)){
		int q;
		for(q = L->size; q >= pos; q--)
			L->Elements[q] = L->Elements[q-1];
		L->Elements[pos-1] = x;
		L->size++;
	}
}

// xoa phan tu khoi danh sach
void popList(int pos, List *L){
	if(emptyList(*L)) ;
	else if(pos < 1 || pos > L->size) ;
	else{
		int i;
		for(i = pos-1; i< L->size; i++)
			L->Elements[i] = L->Elements[i+1];
		L->size--;
	}
}

// ham tim kiem phan tu trong danh sach
Node* findState(State s, List L, int *pos){
	int i;
	for(i=1; i <= L.size; i++)
		if(compareState(elementAt(i, L)->state, s)){
			*pos = i;
			return elementAt(i, L);
		}
	return NULL;
}

// ham sap xep danh sach
void sort_List(List *L){
	int i,j;
	for(i=0; i < L->size-1; i++)
		for(j = i+1; j < L->size; j++)
			if(L->Elements[i]->f > L->Elements[j]->f ){
				Node* node = L->Elements[i];
				L->Elements[i] = L->Elements[j];
				L->Elements[j] = node;
			}
}

/* giai thuat A* */
Node* A_Star(State state, State goal) {
	List Open, Close;
	makeNullList(&Open);
	makeNullList(&Close);

	Node *root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->no_function = 0;
	root->g = 0;
	root->h = heuristic1(root->state, goal);
	root->f = root->h + root->g;
	pushList(root, Open.size+1, &Open);

	while (!emptyList(Open)) {
		Node *node = elementAt(1, Open);
		popList(1, &Open);
		pushList(node, Close.size+1, &Close);
		if (goalCheck(node->state, goal))
			return node;
		int opt;
		for (opt=1; opt <= MAX_OPERATOR; opt++) {
			State newState;
			if (callOperator(node->state, &newState, opt)) {
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->no_function = opt;
				newNode->g = node->g+1;
				newNode->h = heuristic1(newNode->state, goal);
				newNode->f = newNode->g + newNode->h;
				
				int pos_Open, pos_Close;
				Node *nodeFoundOpen = findState(newState, Open, &pos_Open);
				Node *nodeFoundClose = findState(newState, Close, &pos_Close);

				if (nodeFoundOpen == NULL && nodeFoundClose == NULL)
					pushList(newNode, Open.size+1, &Open);
				else if (nodeFoundOpen != NULL && nodeFoundOpen->g > newNode->g) {
					popList(pos_Open, &Open);
					pushList(newNode, pos_Open, &Open);
				}
				else if (nodeFoundClose != NULL && nodeFoundClose->g > newNode->g) {
					popList(pos_Close, &Close);
					pushList(newNode, Open.size+1, &Open);
				}
			}
		}
		sort_List(&Open);
	}
	return NULL;
}

/* ham in duong di loi giai cua bai toan */
void print_Way(Node* node){
	List L;
	makeNullList(&L);
	while(node->parent != NULL){
		pushList(node, L.size+1, &L);
		node = node->parent;
	}
    pushList(node, L.size+1, &L);
	int i, ac = 0;
	for(i = L.size; i>0; i--){
		printf("\nAction %d: %s ",ac,action[elementAt(i,L)->no_function]);
		printState(elementAt(i,L)->state);
		ac++;
	}
}

int main() {
    State state1, goal;
    state1.eightPuzzle[0][0] = 1;
    state1.eightPuzzle[0][1] = 2;
    state1.eightPuzzle[0][2] = 3;
    state1.eightPuzzle[1][0] = 8;
    state1.eightPuzzle[1][1] = 0;
    state1.eightPuzzle[1][2] = 4;
    state1.eightPuzzle[2][0] = 7;
    state1.eightPuzzle[2][1] = 6;
    state1.eightPuzzle[2][2] = 5;
    state1.emptyCol = 1;
    state1.emptyRow = 1;

    goal.eightPuzzle[0][0] = 2;
    goal.eightPuzzle[0][1] = 8;
    goal.eightPuzzle[0][2] = 1;
    goal.eightPuzzle[1][0] = 0;
    goal.eightPuzzle[1][1] = 4;
    goal.eightPuzzle[1][2] = 3;
    goal.eightPuzzle[2][0] = 7;
    goal.eightPuzzle[2][1] = 6;
    goal.eightPuzzle[2][2] = 5;
    goal.emptyCol = 0;
    goal.emptyRow = 1;

	Node *p = A_Star(state1, goal);
	print_Way(p);
    return 0;
}
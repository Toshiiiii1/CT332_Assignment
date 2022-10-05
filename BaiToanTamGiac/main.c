# include <stdio.h>
# include <stdlib.h>

# define MAXLENGTH 500

const char name[] = {'A', 'B', 'C', 'D', 'G'};

/* khai bao cau truc Dinh */
typedef struct {
	int neighbor[10];
	int h;
}Vertices;

/* xay dung do thi */
typedef struct {
	int num;
	Vertices v[10];
}Graph;

/* khoi tao do thi */
void initGraph(int num, Graph *G) {
	G->num = num;
	int i, j;
	for (i=0; i < num; i++) {
		for (j=0; j < num; j++)
			G->v[i].neighbor[j] = 0;
		G->v[i].h = 0;
	}
}

/* khai bao cau truc State */
typedef struct {
	int vertex;
}State;

/* ham in trang thai */
void printState(State state) {
	printf("%c", name[state.vertex]);
}

/* ham so sanh hai trang thai */
int compareState(State state1, State state2) {
	return state1.vertex == state2.vertex;
}

/* ham kiem tra mot trang thai co phai la trang thai ket qua */
int goalCheck(State state, State goal) {
	return compareState(state, goal);
}

/* khai bao cau truc Node */
typedef struct Node {
	State state;
	struct Node *parent;
	int f, g, h;
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
Node* A_Star(Graph G, State state, State goal) {
	List Open, Close;
	makeNullList(&Open);
	makeNullList(&Close);

	Node *root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->g = 0;
	root->h = G.v[state.vertex].h;
	root->f = root->h + root->g;
	pushList(root, Open.size+1, &Open);

	while (!emptyList(Open)) {
		Node *node = elementAt(1, Open);
		popList(1, &Open);
		pushList(node, Close.size+1, &Close);
		if (goalCheck(node->state, goal))
			return node;
		int opt;
		for (opt=0; opt < G.num; opt++) {
			if (G.v[node->state.vertex].neighbor[opt] > 0) {
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state.vertex = opt;
				newNode->parent = node;
				newNode->g = node->g + G.v[node->state.vertex].neighbor[opt];
				newNode->h = G.v[opt].h;
				newNode->f = newNode->g + newNode->h;
				
				int pos_Open, pos_Close;
				Node *nodeFoundOpen = findState(newNode->state, Open, &pos_Open);
				Node *nodeFoundClose = findState(newNode->state, Close, &pos_Close);

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
		printState(elementAt(i, L)->state);
		if (i != 1) {
			printf("->");
		}
		ac++;
	}
}

int main() {
    int i, j;
	Graph graph;
	freopen("test.txt", "r", stdin);
	initGraph(10, &graph);
	for (i=0; i < 5; i++) {
		int x;
		scanf("%d", &x);
		graph.v[i].h = x;
		for (j=0; j < 5; j++) {
			scanf("%d", &x);
			graph.v[i].neighbor[j] = x;
		}
	}
	State A, G;
	A.vertex = 0;
	G.vertex = 4;
	Node *p = A_Star(graph, A, G);
	print_Way(p);
    return 0;
}
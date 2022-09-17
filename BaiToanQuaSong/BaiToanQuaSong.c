# include <stdio.h>
# include <stdlib.h>

# define MAXLENGTH 10000

const char* action[] = {"first state", "di chuyen mot nguoi", "di chuyen hai nguoi", "di chuyen mot quy", "di chuyen hai quy", "di chuyen mot nguoi va mot quy"};

/* khai bao cau truc State */
typedef struct {
    int tuSi, quy;
    char vi_tri_thuyen;
}State;

/* khoi tao State */
void makeNullState(State *state) {
    state->tuSi = 0;
    state->quy = 0;
    state->vi_tri_thuyen = 'A';
}

/* hien thi trang thai hien tai */
void hienThiTrangThai(State state) {
    printf("(%d, %d, %c)\n", state.tuSi, state.quy, state.vi_tri_thuyen);
}

/* ham kiem tra mot trang thai co phai trang thai dich */
int kiemTraKetQua(State state) {
    return (state.tuSi == 0 && state.quy == 0 && state.vi_tri_thuyen == 'B');
}

/* ham kiem tra rang buoc */
int kiemTraDieuKien(State state) {
    // if (state.quy!=state.tuSi && state.tuSi>0 && state.tuSi <3)
    //     return 0;
	// return 1;
    if (3-state.tuSi == 0 || state.tuSi == 0)
        return 1;
    else if (3-state.tuSi > 0)
        return (state.tuSi >= state.quy && 3-state.tuSi >= 3-state.quy);
}

/* ham so sanh hai trang thai */
int compareStates(State state1, State state2) {
    return (state1.tuSi == state2.tuSi && state1.quy == state2.quy && state1.vi_tri_thuyen == state2.vi_tri_thuyen);
}

/* ham di chuyen 1 nguoi sang bo ben kia */
int diChuyenMotNguoi(State currentState, State *result) {
    if (currentState.vi_tri_thuyen == 'A'){
        if (currentState.tuSi > 0){
            result->tuSi = currentState.tuSi - 1;
            result->quy = currentState.quy;
            result->vi_tri_thuyen = 'B';
        }
    }
    else{
        if (3 - currentState.tuSi > 0){
            result->tuSi = currentState.tuSi + 1;
            result->quy = currentState.quy;
            result->vi_tri_thuyen = 'A';
        }
    }
    return kiemTraDieuKien(*result);
}

/* ham di chuyen 1 quy sang bo ben kia */
int diChuyenMotQuy(State currentState, State *result) {
    if (currentState.vi_tri_thuyen == 'A'){
        if (currentState.quy > 0){
            result->quy = currentState.quy - 1;
            result->tuSi = currentState.tuSi;
            result->vi_tri_thuyen = 'B';
        }
    }
    else{
        if (3 - currentState.quy > 0){
            result->quy = currentState.quy + 1;
            result->tuSi = currentState.tuSi;
            result->vi_tri_thuyen = 'A';
        }
    }
    return kiemTraDieuKien(*result);
}

/* ham di chuyen 2 nguoi sang bo ben kia */
int diChuyenHaiNguoi(State currentState, State *result) {
    if (currentState.vi_tri_thuyen == 'A'){
        if (currentState.tuSi > 1){
            result->tuSi = currentState.tuSi - 2;
            result->quy = currentState.quy;
            result->vi_tri_thuyen = 'B';
        }
    }
    else{
        if (3 - currentState.tuSi > 1){
            result->tuSi = currentState.tuSi + 2;
            result->quy = currentState.quy;
            result->vi_tri_thuyen = 'A';
        }
    }
    return kiemTraDieuKien(*result);
}

/* ham di chuyen 2 quy sang bo ben kia */
int diChuyenHaiQuy(State currentState, State *result) {
    if (currentState.vi_tri_thuyen == 'A'){
        if (currentState.quy > 1){
            result->quy = currentState.quy - 2;
            result->tuSi = currentState.tuSi;
            result->vi_tri_thuyen = 'B';
        }
    }
    else{
        if (3 - currentState.quy > 1){
            result->quy = currentState.quy + 2;
            result->tuSi = currentState.tuSi;
            result->vi_tri_thuyen = 'A';
        }
    }
    return kiemTraDieuKien(*result);
}

/* ham di chuyen 1 nguoi va 1 quy sang bo ben kia */
int diChuyenMotNguoiMotQuy(State currentState, State *result) {
    if (currentState.vi_tri_thuyen == 'A'){
        if (currentState.tuSi > 0 && currentState.quy > 0){
            result->quy = currentState.quy - 1;
            result->tuSi = currentState.tuSi - 1;
            result->vi_tri_thuyen = 'B';
        }
    }
    else{
        if ((3 - currentState.tuSi > 0) && (3 - currentState.quy > 0)){
            result->quy = currentState.quy + 1;
            result->tuSi = currentState.tuSi + 1;
            result->vi_tri_thuyen = 'A';
        }
    }
    return kiemTraDieuKien(*result);
}

/* ham goi cac hanh dong */
int callOperator(State currentState, State *result, int option) {
    switch (option) {
        case 1: return diChuyenMotNguoi(currentState, result);
        case 2: return diChuyenHaiNguoi(currentState, result);
        case 3: return diChuyenMotQuy(currentState, result);
        case 4: return diChuyenHaiQuy(currentState, result);
        case 5: return diChuyenMotNguoiMotQuy(currentState, result);
    
        default:
            printf("Error\n");
            return 0;
    }
}

/* -------------------------------------------------- */

/* xay dung cay tim kiem */

// khai bao cau truc "Node"
typedef struct Node{
    /* state: trang thai hien tai
       parent: luu tru Node cha
       noFunction: so thu tu cua hanh dong */
    State state;
    struct Node *parent;
    int noFunction;
}Node;

/* cai dat cau truc du lieu Ngan xep */

// khai bao cau truc Ngan xep
typedef struct {
    Node* Elements[MAXLENGTH];
    int topIndex;
}Stack;

// khoi tao Ngan xep
void makeNullStack(Stack *stack) {
    stack->topIndex = MAXLENGTH;
}

// kiem tra ngan xep rong
int emptyStack(Stack stack) {
    return stack.topIndex == MAXLENGTH;
}

// kiem tra ngan xep day
int fullStack(Stack stack) {
    return stack.topIndex == 0;
}

// dua p.tu vao ngan xep
void push(Node *x, Stack *stack) {
    if (fullStack(*stack))
        printf("Stack is full\n");
    else{
        stack->topIndex -= 1;
        stack->Elements[stack->topIndex] = x;
    }
}

// xoa phan tu khoi ngan xep
void pop(Stack *stack) {
    if (!emptyStack(*stack))
        stack->topIndex += 1;
    else
        printf("Stack is empty\n");
}
// lay p.tu trong Stack
Node* top(Stack stack) {
    if (!emptyStack(stack))
        return stack.Elements[stack.topIndex];
    else
        printf("Stack is empty\n");
}

// kiem tra trang thai trong Open(Close) stack
int findStateInStack(State state, Stack stack) {
    while (!emptyStack(stack)) {
        if (compareStates(top(stack)->state, state))
            return 1;
        pop(&stack);
    }
    return 0;
}

/* cai dat cau truc du lieu Hang doi */

// khai bao hang doi
typedef struct {
    Node* Elements[MAXLENGTH];
    int front, rear;
}Queue;

// khoi tao hang doi
void makeNullQueue(Queue *queue) {
    queue->front = -1;
    queue->rear = -1;
}

// kiem tra hang doi day
int fullQueue(Queue queue) {
    return ((queue.rear - queue.front + 1) % MAXLENGTH) == 0;
}

// kiem tra hang doi rong
int emptyQueue(Queue queue) {
    return queue.front == -1;
}

// tra ve p.tu dau hang doi
Node* getFront(Queue queue) {
    if (emptyQueue(queue))
        printf("Queue is empty\n");
    else
        return queue.Elements[queue.front];
}

// xoa p.tu khoi hang doi
void deleteElement(Queue *queue) {
    if (!emptyQueue(*queue)){
        if (queue->front == queue->rear)
            makeNullQueue(queue);
        else
            queue->front = (queue->front + 1) % MAXLENGTH;
    }
    else
        printf("Queue is empty\n");
}

// them p.tu vao hang doi
void pushElement(Node *x, Queue *queue) {
    if (!fullQueue(*queue)){
        if (emptyQueue(*queue))
            queue->front = 0;
        queue->rear = (queue->rear + 1) % MAXLENGTH;
        queue->Elements[queue->rear] = x;
    }
    else
        printf("Queue is full\n");
}

// tim mot trang thai trong hang doi
int findStateInQueue(State state, Queue queue) {
    while (!emptyQueue(queue)) {
        if (compareStates(getFront(queue)->state, state))
            return 1;
        deleteElement(&queue);
    }
    return 0;
}

// in cac phan tu trong hang doi
void printQueue(Queue queue) {
    while (!emptyQueue(queue)){
        State element = getFront(queue)->state;
        hienThiTrangThai(element);
        deleteElement(&queue);
    }
}

// thuat toan duyet theo chieu rong
Node* BFS(State state) {
    /* khai bao hai hang doi:
        - Open: luu tru cac Node chuan bi duoc trien khai
        - Close: luu tru cac Node da duoc trien khai */
    Queue Open, Close;
    makeNullQueue(&Open);
    makeNullQueue(&Close);

    /* khoi tao trang thai ban dau */
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->noFunction = 0;
    pushElement(root, &Open);

    /* thuc hien qua trinh duyet */
    while (!emptyQueue(Open)) {
        Node* node = getFront(Open);
        deleteElement(&Open);
        pushElement(node, &Close);

        // kiem tra trang thai hien tai co phai trang thai muc tieu
        if (kiemTraKetQua(node->state))
            return node;

        // thuc hien cac hanh dong len trang thai hien tai
        int option;
        for (option = 1; option <= 5; option++) {
            State newState;
            makeNullState(&newState);
            if (callOperator(node->state, &newState, option)) {
                if (findStateInQueue(newState, Close) || findStateInQueue(newState, Open))
                    continue;
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->noFunction = option;
                pushElement(newNode, &Open);
            }
        }
    }
    return NULL;
}

// thuat toan duyet theo chieu sau
Node* DFS(State state) {
    /* khai bao hai ngan xep:
        - Open: luu tru cac Node chuan bi duoc trien khai
        - Close: luu tru cac Node da duoc trien khai */
    Stack Open, Close;
    makeNullStack(&Open);
    makeNullStack(&Close);

    /* khoi tao trang thai ban dau */
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->noFunction = 0;
    push(root, &Open);

    /* thuc hien qua trinh duyet */
    while (!emptyStack(Open)) {
        Node* node = top(Open);
        pop(&Open);
        push(node, &Close);

        // kiem tra trang thai hien tai co phai trang thai muc tieu
        if (kiemTraKetQua(node->state))
            return node;

        // thuc hien cac hanh dong len trang thai hien tai
        int option;
        for (option = 1; option <= 5; option++) {
            State newState;
            makeNullState(&newState);
            if (callOperator(node->state, &newState, option)) {
                if (findStateInStack(newState, Close) || findStateInStack(newState, Open))
                    continue;
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->noFunction = option;
                push(newNode, &Open);
            }
        }
    }
    return NULL;
}

// in ket qua cua duong di loi giai
void printWaysToGoal(Node* node) {
    Stack stackPrint;
    makeNullStack(&stackPrint);

    while (node->parent != NULL) {
        push(node, &stackPrint);
        node = node->parent;
    }
    push(node, &stackPrint);

    int noAction = 0;
    while (!emptyStack(stackPrint)) {
        printf("\nAction %d: %s\n", noAction, action[top(stackPrint)->noFunction]);
        hienThiTrangThai(top(stackPrint)->state);
        pop(&stackPrint);
        noAction++;
    }
}

/* -------------------------------------------------- */

int main() {
    State state = {3, 3, 'A'};
    Node *result1 = BFS(state);
    printWaysToGoal(result1);

    Node *result2 = BFS(state);
    printWaysToGoal(result2);
    return 0;
}
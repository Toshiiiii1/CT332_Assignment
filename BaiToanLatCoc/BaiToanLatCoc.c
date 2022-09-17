# include <stdio.h>
# include <stdlib.h>

# define MAXLENGTH 10000

const char* trangThai[] = {"Up", "Ngua"};
const char* action[] = {"first state", "lat coc 1", "lat coc 2", "lat coc 3", "lat coc 4", "lat coc 5", "lat coc 6"};

/* khai bao cau truc State */
typedef struct {
    /* - 0: cup up
       - 1: cup ngua */
    int cup1, cup2, cup3, cup4, cup5, cup6;
}State;

/* khoi tai trang thai */
void makeNullState(State *state) {
    state->cup1 = 0;
    state->cup2 = 0;
    state->cup3 = 0;
    state->cup4 = 0;
    state->cup5 = 0;
    state->cup6 = 0;
}

/* ham kiem tra mot trang thai co phai trang thai ket qua khong */
int kiemTraKetQua(State state) {
    return (state.cup1 == 1 && state.cup2 == 1 && state.cup3 == 1 && state.cup4 == 1 && state.cup5 == 1 && state.cup6 == 1) || (state.cup1 == 0 && state.cup2 == 0 && state.cup3 == 0 && state.cup4 == 0 && state.cup5 == 0 && state.cup6 == 0);
}

/* ham so sanh hai trang thai */
int compareStates(State state1, State state2) {
    return (state1.cup1 == state2.cup1 && state1.cup2 == state2.cup2 && state1.cup3 == state2.cup3 && state1.cup4 == state2.cup4 && state1.cup5 == state2.cup5 && state1.cup6 == state2.cup6);
}

/* ham hien thi trang thai */
void hienThiTrangThai(State state) {
    printf("%s %s %s %s %s %s\n", trangThai[state.cup1], trangThai[state.cup2], trangThai[state.cup3], trangThai[state.cup4], trangThai[state.cup5], trangThai[state.cup6]);
}

/* lat coc so 1 */
void latCoc1(State current, State *result) {
    result->cup1 = (int)abs(current.cup1 - 1);
    result->cup2 = (int)abs(current.cup2 - 1);
    result->cup3 = current.cup3;
    result->cup4 = current.cup4;
    result->cup5 = current.cup5;
    result->cup6 = current.cup6;
}

/* lat coc so 2 */
void latCoc2(State current, State *result) {
    result->cup1 = (int)abs(current.cup1 - 1);
    result->cup2 = (int)abs(current.cup2 - 1);
    result->cup3 = (int)abs(current.cup3 - 1);
    result->cup4 = current.cup4;
    result->cup5 = current.cup5;
    result->cup6 = current.cup6;
}

/* lat coc so 3 */
int latCoc3(State current, State *result) {
    result->cup1 = current.cup1;
    result->cup2 = (int)abs(current.cup2 - 1);
    result->cup3 = (int)abs(current.cup3 - 1);
    result->cup4 = (int)abs(current.cup4 - 1);
    result->cup5 = current.cup5;
    result->cup6 = current.cup6;
}

/* lat coc so 4 */
int latCoc4(State current, State *result) {
    result->cup1 = current.cup1;
    result->cup2 = current.cup2;
    result->cup3 = (int)abs(current.cup3 - 1);
    result->cup4 = (int)abs(current.cup4 - 1);
    result->cup5 = (int)abs(current.cup5 - 1);
    result->cup6 = current.cup6;
}

/* lat coc so 5 */
int latCoc5(State current, State *result) {
    result->cup1 = current.cup1;
    result->cup2 = current.cup2;
    result->cup3 = current.cup3;
    result->cup4 = (int)abs(current.cup4 - 1);
    result->cup5 = (int)abs(current.cup5 - 1);
    result->cup6 = (int)abs(current.cup6 - 1);
}

/* lat coc so 6 */
int latCoc6(State current, State *result) {
    result->cup1 = current.cup1;
    result->cup2 = current.cup2;
    result->cup3 = current.cup3;
    result->cup4 = current.cup4;
    result->cup5 = (int)abs(current.cup5 - 1);
    result->cup6 = (int)abs(current.cup6 - 1);
}

/* ham thuc hien cac hanh dong len mot trang thai */
int callOperator(State state, State *result, int option) {
    switch (option) {
        case 1: latCoc1(state, result); return 1;
        case 2: latCoc2(state, result); return 1;
        case 3: latCoc3(state, result); return 1;
        case 4: latCoc4(state, result); return 1;
        case 5: latCoc5(state, result); return 1;
        case 6: latCoc6(state, result); return 1;

        default:
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
        printf("\nHanh dong %d: %s\n", noAction, action[top(stackPrint)->noFunction]);
        hienThiTrangThai(top(stackPrint)->state);
        pop(&stackPrint);
        noAction++;
    }
}

/* -------------------------------------------------- */

int main() {
    State state = {1,0,1,0,1,0};
    Node *result1 = DFS(state);
    printWaysToGoal(result1);

    Node *result2 = BFS(state);
    printWaysToGoal(result2);
    return 0;
}
# include <stdio.h>
# include <stdlib.h>

# define TANKCAPCITY_X 10 // suc chua cua binh X
# define TANKCAPCITY_Y 5 // suc chua cua binh Y
# define TANKCAPCITY_Z 6 // suc chua cua binh Z
# define EMPTY 0
# define GOAL 8 // ket qua bai toan
# define MAXLENGTH 100 // so luong phan tu trong Stack

const char* action[] = {"first state", "pour water full X", "pour water full Y", "pour water full Z", "pour water empty X", "pour water empty Y", "pour water empty Z", "pour water X to Y", "pour water X to Z", "pour water Y to X", "pour water Y to Z", "pour water Z to X", "pour water Z to Y"}; // mang chua cac hanh dong

/* -------------------------------------------------- */

/* khai bao cau truc "State" */
typedef struct {
    /* x: luong nuoc trong binh X
       y: luong nuoc trong binh Y
       z: luong nuoc trong binh Z */
    int x, y, z;
}State;

/* khoi tao trang thai ban dau cho binh X & binh Y & binh Z */
void makeNullState(State *state) {
    state->x = 0;
    state->y = 0;
    state->z = 0;
}

/* in trang thai */
void printState(State state) {
    printf("X: %d - Y: %d - Z: %d\n", state.x, state.y, state.z);
}

/* kiem tra mot trang thai co phai trang thai ket qua hay khong */
int checkGoal(State state) {
    return state.x == GOAL;
}

/* so sanh hai trang thai */
int compareStates(State state1, State state2) {
    return (state1.x == state2.x && state1.y == state2.y && state1.z == state2.z);
}

/* - cac hanh dong ap dung len mot trang thai
   - neu hanh dong thuc hien thanh cong tra ve 1. Nguoc lai tra ve 0 
   - currentState: trang thai hien tai
   - *result: trang thai ket qua sau khi ap dung thanh cong mot hanh dong */

// do day binh X
int pourWaterFullX(State currentState, State *result) {
    if (currentState.x < TANKCAPCITY_X){
        result->x = TANKCAPCITY_X;
        result->y = currentState.y;
        result->z = currentState.z;
        return 1;
    }
    return 0;
}

// do day binh Y
int pourWaterFullY(State currentState, State *result) {
    if (currentState.y < TANKCAPCITY_Y){
        result->y = TANKCAPCITY_Y;
        result->x = currentState.x;
        result->z = currentState.z;
        return 1;
    }
    return 0;
}

// do day binh Z
int pourWaterFullZ(State currentState, State *result) {
    if (currentState.y < TANKCAPCITY_Z){
        result->z = TANKCAPCITY_Z;
        result->x = currentState.x;
        result->y = currentState.y; 
        return 1;
    }
    return 0;
}

// lam rong binh X
int pourWaterEmptyX(State currentState, State *result) {
    if (currentState.x > 0){
        result->x = EMPTY;
        result->y = currentState.y;
        result->z = currentState.z;
        return 1;
    }
    return 0;
}

// lam rong binh Y
int pourWaterEmptyY(State currentState, State *result) {
    if (currentState.y > 0){
        result->y = EMPTY;
        result->x = currentState.x;
        result->z = currentState.z;
        return 1;
    }
    return 0;
}

// lam rong binh Z
int pourWaterEmptyZ(State currentState, State *result) {
    if (currentState.z > 0){
        result->z = EMPTY;
        result->x = currentState.x;
        result->y = currentState.y;
        return 1;
    }
    return 0;
}

// chuyen nuoc tu binh X sang binh Y
int pourWaterXY(State currentState, State *result) {
    if (currentState.x > 0 && currentState.y < TANKCAPCITY_Y){
        result->x = (currentState.x + currentState.y - TANKCAPCITY_Y > EMPTY)? currentState.x + currentState.y - TANKCAPCITY_Y : EMPTY;

        result->y = (currentState.x + currentState.y < TANKCAPCITY_Y)? currentState.x + currentState.y : TANKCAPCITY_Y;

        result->z = currentState.z;
        return 1;
    }
    return 0;
}

// chuyen nuoc tu binh X sang binh Z
int pourWaterXZ(State currentState, State *result) {
    if (currentState.x > 0 && currentState.z < TANKCAPCITY_Z){
        result->x = (currentState.x + currentState.z - TANKCAPCITY_Z > EMPTY)? currentState.x + currentState.z - TANKCAPCITY_Z : EMPTY;

        result->z = (currentState.x + currentState.z < TANKCAPCITY_Z)? currentState.x + currentState.z : TANKCAPCITY_Z;

        result->y = currentState.y;
        return 1;
    }
    return 0;
}

// chuyen nuoc tu binh Y sang binh X
int pourWaterYX(State currentState, State *result) {
    if (currentState.y > 0 && currentState.x < TANKCAPCITY_X){
        result->y = ((currentState.x + currentState.y - TANKCAPCITY_X) > EMPTY)? currentState.x + currentState.y - TANKCAPCITY_X : EMPTY;

        result->x = ((currentState.x + currentState.y) < TANKCAPCITY_X)? currentState.x + currentState.y : TANKCAPCITY_X;

        result->z = currentState.z;
        return 1;
    }
    return 0;
}

// chuyen nuoc tu binh Y sang binh Z
int pourWaterYZ(State currentState, State *result) {
    if (currentState.y > 0 && currentState.z < TANKCAPCITY_Z){
        result->y = ((currentState.z + currentState.y - TANKCAPCITY_Z) > EMPTY)? currentState.z + currentState.y - TANKCAPCITY_Z : EMPTY;

        result->z = ((currentState.z + currentState.y) < TANKCAPCITY_Z)? currentState.z + currentState.y : TANKCAPCITY_Z;

        result->x = currentState.x;
        return 1;
    }
    return 0;
}

// chuyen nuoc tu binh Z sang binh X
int pourWaterZX(State currentState, State *result) {
    if (currentState.z > 0 && currentState.x < TANKCAPCITY_X){
        result->z = ((currentState.x + currentState.z - TANKCAPCITY_X) > EMPTY)? currentState.x + currentState.z - TANKCAPCITY_X : EMPTY;

        result->x = ((currentState.x + currentState.z) < TANKCAPCITY_X)? currentState.x + currentState.z : TANKCAPCITY_X;

        result->y = currentState.y;
        return 1;
    }
    return 0;
}

// chuyen nuoc tu binh Z sang binh Y
int pourWaterZY(State currentState, State *result) {
    if (currentState.z > 0 && currentState.y < TANKCAPCITY_Y){
        result->z = ((currentState.y + currentState.z - TANKCAPCITY_Y) > EMPTY)? currentState.y + currentState.z - TANKCAPCITY_Y : EMPTY;

        result->y = ((currentState.y + currentState.z) < TANKCAPCITY_Y)? currentState.y + currentState.z : TANKCAPCITY_Y;

        result->x = currentState.x;
        return 1;
    }
    return 0;
}

/* ham goi cac hanh dong:
   - currentState: trang thai hien tai
   - *result: trang thai ket qua sau khi ap dung mot hanh dong
   - option: so nguyen tu 1 den 6 tuong ung voi 6 hanh dong cua bai toan */
int callOperator(State currentState, State *result, int option) {
    switch (option) {
        case 1: return pourWaterFullX(currentState, result);
        case 2: return pourWaterFullY(currentState, result);
        case 3: return pourWaterFullZ(currentState, result);
        case 4: return pourWaterEmptyX(currentState, result);
        case 5: return pourWaterEmptyY(currentState, result);
        case 6: return pourWaterEmptyZ(currentState, result);
        case 7: return pourWaterXY(currentState, result);
        case 8: return pourWaterXZ(currentState, result);
        case 9: return pourWaterYX(currentState, result);
        case 10: return pourWaterYZ(currentState, result);
        case 11: return pourWaterZX(currentState, result);
        case 12: return pourWaterZY(currentState, result);

        default:
            printf("Error calls operators\n");
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
// int findState(State state, Stack stack) {
//     while (!emptyStack(stack)) {
//         if (compareStates(top(stack)->state, state))
//             return 1;
//         pop(&stack);
//     }
//     return 0;
// }

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
int findState(State state, Queue queue) {
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
        printState(element);
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
        if (checkGoal(node->state))
            return node;

        // thuc hien cac hanh dong len trang thai hien tai
        int option;
        for (option = 1; option <= 12; option++) {
            State newState;
            makeNullState(&newState);
            if (callOperator(node->state, &newState, option)) {
                if (findState(newState, Close) || findState(newState, Open))
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
        printState(top(stackPrint)->state);
        pop(&stackPrint);
        noAction++;
    }
}

/* -------------------------------------------------- */

int main() {
    State state = {10, 0, 0};
    Node *result = BFS(state);
    printWaysToGoal(result);
    return 0;
}
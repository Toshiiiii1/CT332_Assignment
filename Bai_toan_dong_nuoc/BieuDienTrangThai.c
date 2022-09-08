# include <stdio.h>

# define TANKCAPCITY_X 9 // suc chua cua binh X
# define TANKCAPCITY_Y 4 // suc chua cua binh Y
# define EMPTY 0
# define GOAL 6 // ket qua bai toan
# define MAXLENGTH 100 // so luong phan tu trong Stack

const char* action[] = {"First State", "Pour water full X", "Pour water full Y", "Pour water empty X", "Pour water empty Y", "Pour water X to Y", "Pour water Y to X"}; // mang chua cac hanh dong

/* khai bao cau truc "State" */
typedef struct {
    /* x: luong nuoc trong binh X
       y: luong nuoc trong binh Y */
    int x, y;
}State;

/* khoi trang thai ban dau cho binh X & binh Y */
void makeNullState(State *state) {
    state->x = 0;
    state->y = 0;
}

/* In trang thai */
void printState(State state) {
    printf("X: %d - Y: %d\n", state.x, state.y);
}

/* kiem tra mot trang thai co phai trang thai ket qua hay khong */
int checkGoal(State state) {
    return (state.x == GOAL || state.y == GOAL);
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
        return 1;
    }
    return 0;
}

// do day binh Y
int pourWaterFullY(State currentState, State *result) {
    if (currentState.y < TANKCAPCITY_Y){
        result->y = TANKCAPCITY_Y;
        result->x = currentState.x;
        return 1;
    }
    return 0;
}

// lam rong binh X
int pourWaterEmptyX(State currentState, State *result) {
    if (currentState.x > 0){
        result->x = EMPTY;
        result->y = currentState.y;
        return 1;
    }
    return 0;
}

// lam rong binh Y
int pourWaterEmptyY(State currentState, State *result) {
    if (currentState.y > 0){
        result->y = EMPTY;
        result->x = currentState.x;
        return 1;
    }
    return 0;
}

// chuyen nuoc tu binh X sang binh Y
int pourWaterXY(State currentState, State *result) {
    if (currentState.x > 0 && currentState.y < TANKCAPCITY_Y){
        result->x = (currentState.x + currentState.y - TANKCAPCITY_Y > EMPTY)? currentState.x + currentState.y - TANKCAPCITY_Y : EMPTY;

        result->y = (currentState.x + currentState.y < TANKCAPCITY_Y)? currentState.x + currentState.y : TANKCAPCITY_Y;
        return 1;
    }
    return 0;
}

// chuyen nuoc tu binh Y sang binh X
int pourWaterYX(State currentState, State *result) {
    if (currentState.y > 0 && currentState.x < TANKCAPCITY_X){
        result->y = ((currentState.x + currentState.y - TANKCAPCITY_X) > EMPTY)? currentState.x + currentState.y - TANKCAPCITY_X : EMPTY;

        result->x = ((currentState.x + currentState.y) < TANKCAPCITY_X)? currentState.x + currentState.y : TANKCAPCITY_X;
        return 1;
    }
    return 0;
}

/* - ham goi cac hanh dong
   - currentState: trang thai hien tai
   - *result: trang thai ket qua sau khi ap dung mot hanh dong
   - option: so nguyen tu 1 den 6 tuong ung voi 6 hanh dong cua bai toan */
int callOperator(State currentState, State *result, int option) {
    switch (option) {
        case 1: return pourWaterFullX(currentState, result);
        case 2: return pourWaterFullY(currentState, result);
        case 3: return pourWaterEmptyX(currentState, result);
        case 4: return pourWaterEmptyY(currentState, result);
        case 5: return pourWaterXY(currentState, result);
        case 6: return pourWaterYX(currentState, result);

        default:
            printf("Error calls operators\n");
            return 0;
    }
}

int main() {
    State currentState = {5, 4}, result;
    printf("Trang thai bat dau\n");
    printState(currentState);

    int option;
    for (option = 1; option <= 6; option++) {
        int flag = callOperator(currentState, &result, option);
        if (flag) {
            printf("\nHanh dong %s thanh cong\n", action[option]);
            printState(result);
        }
        else
            printf("\nHanh dong %s khong thanh cong\n", action[option]);
    }
    return 0;
}
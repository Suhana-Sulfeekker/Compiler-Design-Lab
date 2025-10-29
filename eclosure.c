#include <stdio.h>
#include <stdlib.h>

// -----------------------------
// STRUCTURE DEFINITIONS
// -----------------------------

// Each transition is represented as a linked list node
struct node {
    int st;                  // destination state
    struct node *link;       // pointer to next destination (if multiple)
};

// -----------------------------
// GLOBAL VARIABLES
// -----------------------------

struct node *transition[20][20] = {NULL}; // transition[state][symbol_index]
static int e_closure[20][20] = {0};       // stores e-closure results

char alphabet[20];          // list of input symbols (last one must be 'e' if epsilon is used)
static int set[20];         // helper array (not used heavily here)
static int buffer[20];      // marks visited states to avoid infinite loops

int nostate, noalpha, notransition;  // counts
int c;                               // counter for closure array indexing

// -----------------------------
// FUNCTION DECLARATIONS
// -----------------------------

void insert_trantbl(int from, char sym, int to);
int  findalpha(char sym);
void findclosure(int x, int sta);
void print_e_closure(int i);

// -----------------------------
// MAIN FUNCTION
// -----------------------------

int main() {
    int i, j, r, s;
    char c_sym;
    struct node *temp;

    printf("Enter the number of alphabets: ");
    scanf("%d", &noalpha);
    getchar();

    printf("\nNOTE: Use letter 'e' as epsilon (must be last if present)\n");
    printf("Enter alphabets one by one:\n");

    for (i = 0; i < noalpha; i++) {
        alphabet[i] = getchar();
        getchar(); // consume newline
    }

    printf("\nEnter number of states: ");
    scanf("%d", &nostate);

    printf("\nEnter number of transitions: ");
    scanf("%d", &notransition);

    printf("\nNOTE: Enter transitions in form -> q_from alphabet q_to\n");
    printf("      Example: 1 a 2 means (q1 --a--> q2)\n");

    for (i = 0; i < notransition; i++) {
        scanf("%d %c%d", &r, &c_sym, &s);
        insert_trantbl(r, c_sym, s);
    }

    // -----------------------------
    // COMPUTE AND PRINT E-CLOSURES
    // -----------------------------

    printf("\nEpsilon-closures of all states\n");
    printf("----------------------------------\n");

    for (i = 1; i <= nostate; i++) {
        c = 0;

        // clear buffers
        for (j = 0; j < 20; j++) {
            buffer[j] = 0;
            e_closure[i][j] = 0;
        }

        // find epsilon closure of state i
        findclosure(i, i);

        printf("e-closure(q%d): ", i);
        print_e_closure(i);
        printf("\n");
    }

    return 0;
}

// -----------------------------
// FUNCTION DEFINITIONS
// -----------------------------

// Function: insert_trantbl
// -----------------------------
// Inserts a transition (from -> to) on symbol sym into the transition table.
void insert_trantbl(int from, char sym, int to) {
    int j = findalpha(sym);
    if (j == 999) {
        printf("Error: invalid symbol '%c'\n", sym);
        exit(0);
    }

    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->st = to;
    temp->link = transition[from][j];
    transition[from][j] = temp;
}

// Function: findalpha
// -----------------------------
// Returns the index of the given symbol in the alphabet array.
int findalpha(char sym) {
    for (int i = 0; i < noalpha; i++)
        if (alphabet[i] == sym)
            return i;
    return 999; // not found
}

// Function: findclosure
// -----------------------------
// Recursively finds all states reachable from 'x' on epsilon (including x itself).
void findclosure(int x, int sta) {
    struct node *temp;

    // Avoid revisiting the same state
    if (buffer[x])
        return;

    // Include the current state in e-closure(sta)
    e_closure[sta][c++] = x;
    buffer[x] = 1; // mark visited

    // If epsilon exists as last symbol in alphabet and there are epsilon transitions
    if (alphabet[noalpha - 1] == 'e' && transition[x][noalpha - 1] != NULL) {
        temp = transition[x][noalpha - 1];

        // Recursively follow all epsilon transitions
        while (temp != NULL) {
            findclosure(temp->st, sta);
            temp = temp->link;
        }
    }
}

// Function: print_e_closure
// -----------------------------
// Prints the epsilon closure set for a given state
void print_e_closure(int i) {
    int j;
    printf("{ ");
    for (j = 0; e_closure[i][j] != 0; j++) {
        printf("q%d ", e_closure[i][j]);
        if (e_closure[i][j + 1] != 0)
            printf(", ");
    }
    printf("}");
}

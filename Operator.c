#include <stdio.h>
#include <string.h>
#define MAX 50

char stack[MAX], input[MAX];
char table[10][10], terminals[10];
int n;

// Function to find index of a terminal
int indexOf(char c) {
    for (int i = 0; i < n; i++)
        if (terminals[i] == c)
            return i;
    return -1;
}

// Function to find rightmost terminal in the stack
int findRightmostTerminal(int top) {
    while (top >= 0 && indexOf(stack[top]) == -1)
        top--;
    return top;
}

int main() {
    int i, j, step = 1, top = 0;
    char grammar[200];

    // Step 1: Input terminals
    printf("Enter number of terminals: ");
    scanf("%d", &n);

    printf("Enter the terminals (single characters like + * i $):\n");
    scanf("%s", terminals);

    // Step 2: Input precedence relations
    printf("\nEnter operator precedence table values (<, >, =):\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("Enter precedence between %c and %c: ", terminals[i], terminals[j]);
            scanf(" %c", &table[i][j]);
        }
    }

    // Step 3: Display operator precedence table
    printf("\n**** OPERATOR PRECEDENCE TABLE ****\n\t");
    for (i = 0; i < n; i++)
        printf("%c\t", terminals[i]);
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("%c\t", terminals[i]);
        for (j = 0; j < n; j++)
            printf("%c\t", table[i][j]);
        printf("\n");
    }

    // Step 4: Input grammar (for display only)
    printf("\nEnter grammar used (example: E->E+E|E*E|(E)|id): ");
    scanf("%s", grammar);
    printf("Grammar stored (not used in parsing): %s\n", grammar);

    // Step 5: Input expression
    char temp[MAX];
    printf("\nEnter input expression (example: id+id*id): ");
    scanf("%s", temp);

    // Convert "id" → single symbol 'i'
    int k = 0, m = 0;
    while (temp[k] != '\0') {
        if (temp[k] == 'i' && temp[k + 1] == 'd') {
            input[m++] = 'i';
            k += 2;
        } else {
            input[m++] = temp[k++];
        }
    }
    input[m++] = '$';
    input[m] = '\0';

    // Initialize stack
    stack[top] = '$';
    stack[top + 1] = '\0';

    printf("\nStep\tStack\t\tInput\t\tAction\n");

    i = 0;
    while (1) {
        // Find topmost terminal
        int t = findRightmostTerminal(top);
        char a = stack[t];
        char b = input[i];
        int col = indexOf(a);
        int row = indexOf(b);

        // Print current parser state
        printf("%d\t%s\t\t%s\t\t", step++, stack, &input[i]);

        // Acceptance condition
        if (a == '$' && b == '$') {
            printf("accept\n");
            break;
        }

        // Invalid symbols
        if (col == -1 || row == -1) {
            printf("error (invalid symbol)\n");
            break;
        }

        // SHIFT
        if (table[col][row] == '<' || table[col][row] == '=') {
            stack[++top] = b;
            stack[top + 1] = '\0';
            printf("shift %c\n", b);
            i++;
        }

        // REDUCE
        else if (table[col][row] == '>') {
            if (stack[top] == 'i') {
                stack[top] = 'E';
                stack[top+1] = '\0';
                printf("reduce id -> E\n");
            } else if (stack[top] == 'E' && stack[top - 1] == '+' && stack[top - 2] == 'E') {
                top -= 2;
                stack[top] = 'E';
                stack[top + 1] = '\0';
                printf("reduce E + E -> E\n");
            } else if (stack[top] == 'E' && stack[top - 1] == '*' && stack[top - 2] == 'E') {
                top -= 2;
                stack[top] = 'E';
                stack[top + 1] = '\0';
                printf("reduce E * E → E\n");
            } else {
                printf("error (no valid handle)\n");
                break;
            }
        }

        // ERROR
        else {
            printf("error (invalid precedence)\n");
            break;
        }
    }

    return 0;
}

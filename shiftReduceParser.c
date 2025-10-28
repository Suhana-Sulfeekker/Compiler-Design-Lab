#include <stdio.h>
#include <string.h>

#define MAX_INPUT 100
#define MAX_STACK 100

// Global variables
char input[MAX_INPUT], stack[MAX_STACK];
int input_len, stack_top = -1;  // stack_top keeps track of top index
int curr_pos = 0;               // current position in input string

// Function declarations
void check();       // function to check for possible reductions
void print_stack(); // function to display stack content

int main() {
    printf("GRAMMAR is:\nE -> E+E | E*E | (E) | id\n");
    printf("Enter input string: ");
    scanf("%s", input);

    input_len = strlen(input);

    printf("\nStack\t\tInput\t\tAction\n");

    // --- SHIFT PHASE ---
    while (curr_pos < input_len) {
        // If "id" is detected as one token
        if (input[curr_pos] == 'i' && input[curr_pos + 1] == 'd') {
            stack[++stack_top] = 'I';  // push symbol 'I' for id
            stack[stack_top + 1] = '\0';

            printf("$");
            print_stack();
            printf("\t\t%s$\t\tSHIFT -> id\n", input + curr_pos + 2);

            curr_pos += 2;  // move input pointer by 2 characters
            check();         // check for possible reductions
        } 
        else { // for operators or parentheses
            stack[++stack_top] = input[curr_pos];
            stack[stack_top + 1] = '\0';

            printf("$");
            print_stack();
            printf("\t\t%s$\t\tSHIFT -> %c\n", input + curr_pos + 1, input[curr_pos]);

            curr_pos++;
            check();
        }
    }

    // --- FINAL CHECK ---
    if (stack_top == 0 && stack[0] == 'E') {
        printf("\nInput string is VALID.\n");
    } else {
        printf("\nInput string is INVALID.\n");
    }

    return 0;
}

// ---------------------------------------------------------------
// Function: check()
// Purpose : Continuously check the stack for handles (patterns)
//           and perform reductions (e.g., E+E -> E)
// ---------------------------------------------------------------
void check() {
    int handle_size;
    char *handle;

    while (1) {
        // Check for reducible patterns (handles)
        if (stack_top >= 0 && stack[stack_top] == 'I') {
            handle = "id"; handle_size = 1;
        } 
        else if (stack_top >= 2 && stack[stack_top - 2] == 'E' && stack[stack_top - 1] == '+' && stack[stack_top] == 'E') {
            handle = "E+E"; handle_size = 3;
        } 
        else if (stack_top >= 2 && stack[stack_top - 2] == 'E' && stack[stack_top - 1] == '*' && stack[stack_top] == 'E') {
            handle = "E*E"; handle_size = 3;
        } 
        else if (stack_top >= 2 && stack[stack_top - 2] == '(' && stack[stack_top - 1] == 'E' && stack[stack_top] == ')') {
            handle = "(E)"; handle_size = 3;
        } 
        else {
            // No more reductions possible
            return;
        }

        // Perform reduction — replace handle with E
        stack_top -= handle_size - 1;
        stack[stack_top] = 'E';
        stack[stack_top + 1] = '\0';

        // Display reduction step
        printf("$");
        print_stack();
        printf("\t\t%s$\t\tREDUCE -> %s\n", input + curr_pos, handle);
    }
}

// ---------------------------------------------------------------
// Function: print_stack()
// Purpose : Prints current content of the stack with id shown as "id"
// ---------------------------------------------------------------
void print_stack() {
    for (int i = 0; i <= stack_top; i++) {
        if (stack[i] == 'I')
            printf("id");
        else
            printf("%c", stack[i]);
    }
}

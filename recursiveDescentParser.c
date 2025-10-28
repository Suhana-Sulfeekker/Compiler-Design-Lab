#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Input string and helper arrays
char ip_sym[50], op[100], tmp[100];
int ip_ptr = 0;  // Pointer to track current input symbol

// Function declarations
void e();
void e_prime();
void t();
void t_prime();
void f();
void advance();

//--------------------
// Function: e()
// Implements rule: E -> T E'
//--------------------
void e() {
    strcpy(op, "TE'");  // Initial expansion of E
    printf("E = %-25s E -> TE'\n", op);
    t();                // Parse T
    e_prime();          // Parse E'
}

//--------------------
// Function: e_prime()
// Implements rule: E' -> + T E' | ε
//--------------------
void e_prime() {
    int i, n = 0, l;

    // Remove '#' if present (used as placeholder cleanup)
    for (i = 0; i <= strlen(op); i++)
        if (op[i] != '#') tmp[n++] = op[i];
    tmp[n] = '\0';
    strcpy(op, tmp);

    l = strlen(op);

    // Find first occurrence of E in the output string
    for (n = 0; n < l && op[n] != 'E'; n++);

    // If next input symbol is '+', apply E' -> + T E'
    if (ip_sym[ip_ptr] == '+') {
        // Make space to insert "+TE'"
        for (i = l; i >= n + 1; i--)
            op[i + 2] = op[i];
        op[n++] = '+';
        op[n++] = 'T';
        op[n++] = 'E';
        // op[n++] = '\'';
        printf("E = %-25s E' -> +TE'\n", op);

        advance();  // Consume '+'
        t();        // Parse T
        e_prime();  // Parse next E'
    } 
    // Else, E' -> ε (empty production)
    else {
        for (i = n; i <= strlen(op); i++)
            op[i] = op[i + 2];
        printf("E = %-25s E' -> e\n", op);
    }
}

//--------------------
// Function: t()
// Implements rule: T -> F T'
//--------------------
void t() {
    int i, n = 0, l;

    // Cleanup
    for (i = 0; i <= strlen(op); i++)
        if (op[i] != '#') tmp[n++] = op[i];
    tmp[n] = '\0';
    strcpy(op, tmp);
    l = strlen(op);

    // Find first T
    for (n = 0; n < l && op[n] != 'T'; n++);

    // Insert "FT'"
    for (i = l; i >= n + 1; i--)
        op[i + 2] = op[i];
    op[n++] = 'F';
    op[n++] = 'T';
    op[n++] = '\'';

    printf("E = %-25s T -> FT'\n", op);

    f();        // Parse F
    t_prime();  // Parse T'
}

//--------------------
// Function: t_prime()
// Implements rule: T' -> * F T' | ε
//--------------------
void t_prime() {
    int i, n = 0, l;

    // Cleanup
    for (i = 0; i <= strlen(op); i++)
        if (op[i] != '#') tmp[n++] = op[i];
    tmp[n] = '\0';
    strcpy(op, tmp);
    l = strlen(op);

    // Find T
    for (n = 0; n < l && op[n] != 'T'; n++);

    // If next input symbol is '*', apply T' -> * F T'
    if (ip_sym[ip_ptr] == '*') {
        for (i = l; i >= n + 1; i--)
            op[i + 2] = op[i];
        op[n++] = '*';
        op[n++] = 'F';
        op[n++] = 'T';

        printf("E = %-25s T' -> *FT'\n", op);

        advance();  // Consume '*'
        f();        // Parse F
        t_prime();  // Parse next T'
    } 
    // Else T' -> ε
    else {
        for (i = n; i <= strlen(op); i++)
            op[i] = op[i + 2];
        printf("E = %-25s T' -> e\n", op);
    }
}

//--------------------
// Function: f()
// Implements rule: F -> (E) | i
//--------------------
void f() {
    int n = 0;

    // Remove '#' if present
    for (int i = 0; i <= strlen(op); i++)
        if (op[i] != '#') tmp[n++] = op[i];
    tmp[n] = '\0';
    strcpy(op, tmp);

    // Find F
    for (n = 0; n < strlen(op) && op[n] != 'F'; n++);

    // F -> i
    if (ip_sym[ip_ptr] == 'i') {
        op[n] = 'i';
        printf("E = %-25s F -> i\n", op);
        advance();
    } 
    // F -> (E)
    else if (ip_sym[ip_ptr] == '(') {
        advance();
        op[n] = '('; op[n + 1] = 'E'; op[n + 2] = ')';
        printf("E = %-25s F -> (E)\n", op);
        e();
        if (ip_sym[ip_ptr] == ')') advance();
        else {
            printf("Syntax error: expected ')'\n");
            exit(1);
        }
    } 
    // Invalid symbol
    else {
        printf("Syntax error: invalid symbol '%c'\n", ip_sym[ip_ptr]);
        exit(1);
    }
}

//--------------------
// Function: advance()
// Moves to next input symbol
//--------------------
void advance() {
    ip_ptr++;
}

int main() {
    printf("\nGrammar without left recursion:\n");
    printf("E -> TE'\nE' -> +TE' | e\nT -> FT'\nT' -> *FT' | e\nF -> (E) | i\n\n");

    printf("Enter the input expression: ");
    scanf("%s", ip_sym);

    printf("\nExpression\t\tSequence of Production Rules\n");
    e();

    if (ip_sym[ip_ptr] == '\0')
        printf("\nParsing successful!\n");
    else
        printf("\nSyntax error at '%c'\n", ip_sym[ip_ptr]);

    return 0;
}

#include <stdio.h>
#include <string.h>

void gen_code_for_operator(char *expr, char operator, char *reg) {
    int i = 0, j = 0;
    char temp[100];  // Temporary string to hold modified expression

    while (expr[i] != '\0') {
        // When current character is the target operator
        if (expr[i] == operator) {
            // Print the corresponding three-address code
            // Format: Operator  Destination  Operand1  Operand2
            printf("%c\t%c\t%c\t%c\n", operator, *reg, expr[i - 1], expr[i + 1]);

            // Replace the left operand with the result register in the new expression
            temp[j - 1] = *reg;

            // Skip the operator and right operand in the original expression
            i += 2;

            // Move to the next available register (Z → Y → X → ...)
            (*reg)--;

            continue;
        }

        // Copy current character to temp (if no operation found)
        temp[j++] = expr[i++];
    }

    temp[j] = '\0';      // Null-terminate the new expression
    strcpy(expr, temp);  // Update the expression with replaced registers
}

void gen_code(char *expr) {
    char reg = 'Z';  // Start using registers from Z backward

    // Generate code in operator precedence order
    gen_code_for_operator(expr, '/', &reg);
    gen_code_for_operator(expr, '*', &reg);
    gen_code_for_operator(expr, '+', &reg);
    gen_code_for_operator(expr, '-', &reg);
    gen_code_for_operator(expr, '=', &reg);
}

int main() {
    char expr[100];

    printf("Enter expression:\n");
    scanf("%s", expr);

    printf("Op\tDestn\tArg1\tArg2\n");
    gen_code(expr);

    return 0;
}

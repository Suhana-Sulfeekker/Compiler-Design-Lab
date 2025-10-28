#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 50
#define NT 26   // number of possible non-terminals (A–Z)

// Global variables
char prods[MAX][MAX];          // Array of productions
char firstSet[NT][MAX];        // FIRST sets for each non-terminal
char followSet[NT][MAX];       // FOLLOW sets for each non-terminal
int n;                         // Number of productions

// ---------------------- Utility Functions ----------------------
int isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');   // A–Z are non-terminals
}

int isInSet(char set[], char c) {
    for (int i = 0; set[i] != '\0'; i++)
        if (set[i] == c) return 1;
    return 0;
}

void addToSet(char set[], char c) {
    if (c == ' ') return;
    if (!isInSet(set, c)) {
        int len = strlen(set);
        set[len] = c;
        set[len + 1] = '\0';
    }
}

// ---------------------- FIRST Computation ----------------------
void computeFirst() {
    int changed;

    do {
        changed = 0; // track if any change occurs in this iteration

        for (int i = 0; i < n; i++) {
            char lhs = prods[i][0];   // left-hand side non-terminal
            int start = (prods[i][1] == '-' || prods[i][1] == '=') ? 3 : 2;
            int before = strlen(firstSet[lhs - 'A']);

            // Traverse right-hand side of production
            for (int j = start; prods[i][j] != '\0'; j++) {
                char sym = prods[i][j];

                if (!isNonTerminal(sym)) {
                    // Terminal directly goes into FIRST(lhs)
                    addToSet(firstSet[lhs - 'A'], sym);
                    break;
                } else {
                    // Add FIRST(sym) except epsilon
                    for (int k = 0; firstSet[sym - 'A'][k] != '\0'; k++)
                        if (firstSet[sym - 'A'][k] != 'e')
                            addToSet(firstSet[lhs - 'A'], firstSet[sym - 'A'][k]);

                    // If sym can produce epsilon, continue to next symbol
                    if (isInSet(firstSet[sym - 'A'], 'e'))
                        continue;
                    else
                        break;
                }
            }

            // If all symbols in RHS can produce epsilon, add epsilon to FIRST(lhs)
            int allEps = 1;
            for (int j = start; prods[i][j] != '\0'; j++) {
                char sym = prods[i][j];
                if (!isNonTerminal(sym) || !isInSet(firstSet[sym - 'A'], 'e')) {
                    allEps = 0;
                    break;
                }
            }
            if (allEps) addToSet(firstSet[lhs - 'A'], 'e');

            if (strlen(firstSet[lhs - 'A']) > before)
                changed = 1;
        }
    } while (changed);
}

// ---------------------- FOLLOW Computation ----------------------
void computeFollow() {
    int changed;

    // Add $ to FOLLOW of start symbol
    addToSet(followSet[prods[0][0] - 'A'], '$');

    do {
        changed = 0;

        for (int i = 0; i < n; i++) {
            char lhs = prods[i][0];
            int start = (prods[i][1] == '-' || prods[i][1] == '=') ? 3 : 2;

            for (int j = start; prods[i][j] != '\0'; j++) {
                char B = prods[i][j];
                if (!isNonTerminal(B)) continue;

                int before = strlen(followSet[B - 'A']);
                int k = j + 1;

                // Case 1: symbols after B (beta)
                while (prods[i][k] != '\0') {
                    char beta = prods[i][k];
                    if (!isNonTerminal(beta)) {
                        addToSet(followSet[B - 'A'], beta);
                        break;
                    } else {
                        // Add FIRST(beta) except epsilon
                        for (int m = 0; firstSet[beta - 'A'][m] != '\0'; m++)
                            if (firstSet[beta - 'A'][m] != 'e')
                                addToSet(followSet[B - 'A'], firstSet[beta - 'A'][m]);

                        // If beta has epsilon, move further
                        if (isInSet(firstSet[beta - 'A'], 'e')) {
                            k++;
                            if (prods[i][k] == '\0') {
                                // If epsilon at end, add FOLLOW(lhs)
                                for (int m = 0; followSet[lhs - 'A'][m] != '\0'; m++)
                                    addToSet(followSet[B - 'A'], followSet[lhs - 'A'][m]);
                            }
                        } else break;
                    }
                }

                // Case 2: if B is the last symbol, add FOLLOW(lhs)
                if (prods[i][j + 1] == '\0') {
                    for (int m = 0; followSet[lhs - 'A'][m] != '\0'; m++)
                        addToSet(followSet[B - 'A'], followSet[lhs - 'A'][m]);
                }

                if (strlen(followSet[B - 'A']) > before)
                    changed = 1;
            }
        }
    } while (changed);
}

// ---------------------- MAIN FUNCTION ----------------------
int main() {
    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions (e.g., E->E+T or E=E+T):\n");
    for (int i = 0; i < n; i++)
        scanf("%s", prods[i]);

    // Initialize FIRST and FOLLOW sets to empty
    for (int i = 0; i < 26; i++) {
        firstSet[i][0] = '\0';
        followSet[i][0] = '\0';
    }

    // Compute FIRST and FOLLOW
    computeFirst();
    computeFollow();

    // Print results
    int printed[26] = {0};
    printf("\nFIRST sets:\n");
    for (int i = 0; i < n; i++) {
        char nt = prods[i][0];
        if (!printed[nt - 'A']) {
            printf("%c : { %s }\n", nt, firstSet[nt - 'A']);
            printed[nt - 'A'] = 1;
        }
    }

    memset(printed, 0, sizeof(printed));
    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < n; i++) {
        char nt = prods[i][0];
        if (!printed[nt - 'A']) {
            printf("%c : { %s }\n", nt, followSet[nt - 'A']);
            printed[nt - 'A'] = 1;
        }
    }

    return 0;
}

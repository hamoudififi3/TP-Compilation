
// MAAFIFI MOHAMED RAID (Security)
// RAGAA CHARFEDDINE (Security)
// BERRI NOUH (Security)



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_SYMBOLS 10

typedef struct {
    int from;
    char symbol;   
    int to;
} Transition;

int n_states, n_symbols, n_transitions;
char symbols[MAX_SYMBOLS];
Transition transitions[100];
int start_state;
int final_states[MAX_STATES];
int n_finals;


int eclosure[MAX_STATES][MAX_STATES];
int eclosure_computed[MAX_STATES] = {0};


void readNFA() {
    int i;
    
    printf("Number of states: ");
    scanf("%d", &n_states);
    if (n_states <= 0 || n_states > MAX_STATES) {
        printf("Invalid number of states!\n");
        exit(1);
    }
    
    printf("Number of input symbols: ");
    scanf("%d", &n_symbols);
    if (n_symbols <= 0 || n_symbols > MAX_SYMBOLS) {
        printf("Invalid number of symbols!\n");
        exit(1);
    }
    
    printf("Symbols (no epsilon): ");
    for (i = 0; i < n_symbols; i++) {
        scanf(" %c", &symbols[i]);
        if (symbols[i] == 'e') {
            printf("'e' is reserved for epsilon!\n");
            exit(1);
        }
    }
    
    printf("Number of transitions: ");
    scanf("%d", &n_transitions);
    if (n_transitions <= 0 || n_transitions > 100) {
        printf("Invalid number of transitions!\n");
        exit(1);
    }
    
    printf("Transitions (from symbol to) , use 'e' for epsilon:\n");
    for (i = 0; i < n_transitions; i++) {
        scanf("%d %c %d", &transitions[i].from,
                          &transitions[i].symbol,
                          &transitions[i].to);
        if (transitions[i].from < 0 || transitions[i].from >= n_states ||
            transitions[i].to < 0 || transitions[i].to >= n_states) {
            printf("Invalid state number!\n");
            exit(1);
        }
    }
    
    printf("Start state: ");
    scanf("%d", &start_state);
    if (start_state < 0 || start_state >= n_states) {
        printf("Invalid start state!\n");
        exit(1);
    }
    
    printf("Number of final states: ");
    scanf("%d", &n_finals);
    if (n_finals <= 0 || n_finals > MAX_STATES) {
        printf("Invalid number of final states!\n");
        exit(1);
    }
    
    printf("Final states: ");
    for (i = 0; i < n_finals; i++) {
        scanf("%d", &final_states[i]);
        if (final_states[i] < 0 || final_states[i] >= n_states) {
            printf("Invalid final state!\n");
            exit(1);
        }
    }
}


void dfs_epsilon(int state, int current, int visited[]) {
    int i;
    visited[current] = 1;
    
    for (i = 0; i < n_transitions; i++) {
        if (transitions[i].from == current && 
            transitions[i].symbol == 'e' &&
            !visited[transitions[i].to]) {
            dfs_epsilon(state, transitions[i].to, visited);
        }
    }
}


void computeEclosure() {
    int i, j;
    int visited[MAX_STATES];
    
    for (i = 0; i < n_states; i++) {
        for (j = 0; j < n_states; j++) {
            eclosure[i][j] = 0;
            visited[j] = 0;
        }
        

        eclosure[i][i] = 1;
        

        dfs_epsilon(i, i, visited);
        

        for (j = 0; j < n_states; j++) {
            if (visited[j]) {
                eclosure[i][j] = 1;
            }
        }
    }
}


void computeNewTransitions() {
    int i, j, k, s;
    int reachable[MAX_STATES];  
    
    printf("\nNew transitions (without epsilon):\n");
    
    for (i = 0; i < n_states; i++) {
        for (s = 0; s < n_symbols; s++) {

            for (j = 0; j < n_states; j++)
                reachable[j] = 0;
            

            for (j = 0; j < n_states; j++) {
                if (eclosure[i][j]) {

                    for (k = 0; k < n_transitions; k++) {
                        if (transitions[k].from == j &&
                            transitions[k].symbol == symbols[s]) {
                            int to = transitions[k].to;

                            for (int x = 0; x < n_states; x++) {
                                if (eclosure[to][x]) {
                                    reachable[x] = 1;
                                }
                            }
                        }
                    }
                }
            }
            

            for (j = 0; j < n_states; j++) {
                if (reachable[j]) {
                    printf("δ'(%d, %c) = %d\n", i, symbols[s], j);
                }
            }
        }
    }
}


void computeNewFinalStates() {
    int i, j;
    int is_final[MAX_STATES] = {0};
    
    printf("\nNew final states:\n");
    for (i = 0; i < n_states; i++) {
        for (j = 0; j < n_finals; j++) {
            if (eclosure[i][final_states[j]]) {
                is_final[i] = 1;
                break;
            }
        }
    }
    
    printf("{ ");
    for (i = 0; i < n_states; i++) {
        if (is_final[i]) {
            printf("%d ", i);
        }
    }
    printf("}\n");
}


void displayEclosure() {
    int i, j;
    printf("\nEpsilon-closure:\n");
    for (i = 0; i < n_states; i++) {
        printf("ε-closure(%d) = { ", i);
        for (j = 0; j < n_states; j++) {
            if (eclosure[i][j]) {
                printf("%d ", j);
            }
        }
        printf("}\n");
    }
}

int main() {
    readNFA();
    computeEclosure();
    displayEclosure();
    computeNewTransitions();
    computeNewFinalStates();
    
    printf("\nConversion complete!\n");
    printf("Note: The start state remains: %d\n", start_state);
    
    #ifdef _WIN32
    system("pause");
    #else
    printf("Press Enter to continue...\n");
    getchar();
    getchar();
    #endif
    
    return 0;
}

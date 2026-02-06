#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char ch;
    struct Node *dot, *dash;
} Node;

Node* newNode() {
    Node* n = (Node*)malloc(sizeof(Node));
    n->ch = 0;
    n->dot = n->dash = NULL;
    return n;
}

void insert(Node* root, const char* morse, char c) {
    Node* cur = root;
    for (int i = 0; morse[i]; i++) {
        if (morse[i] == '.') {
            if (!cur->dot) cur->dot = newNode();
            cur = cur->dot;
        } else if (morse[i] == '-') {
            if (!cur->dash) cur->dash = newNode();
            cur = cur->dash;
        }
    }
    cur->ch = c;
}

typedef struct {
    char c;
    const char* m;
} Map;

Map table[] = {
    {'A',".-"},{'B',"-..."},{'C',"-.-."},{'D',"-.."},
    {'E',"."},{'F',"..-."},{'G',"--."},{'H',"...."},
    {'I',".."},{'J',".---"},{'K',"-.-"},{'L',".-.."},
    {'M',"--"},{'N',"-."},{'O',"---"},{'P',".--."},
    {'Q',"--.-"},{'R',".-."},{'S',"..."},
    {'T',"-"},{'U',"..-"},{'V',"...-"},
    {'W',".--"},{'X',"-..-"},{'Y',"-.--"},{'Z',"--.."},
    {'0',"-----"},{'1',".----"},{'2',"..---"},
    {'3',"...--"},{'4',"....-"},{'5',"....."},
    {'6',"-...."},{'7',"--..."},{'8',"---.."},{'9',"----."}
};

int N = sizeof(table) / sizeof(table[0]);

Node* buildTree() {
    Node* root = newNode();
    for (int i = 0; i < N; i++)
        insert(root, table[i].m, table[i].c);
    return root;
}

#define MAX_MORSE_LEN 8

/* ---------------- DECODE ---------------- */
void decode(Node* root, const char* line) {
    char buf[MAX_MORSE_LEN + 1];
    int bi = 0;
    Node* cur;

    for (int i = 0; line[i]; i++) {
        if (line[i] == ' ') {
            if (bi > 0) {
                buf[bi] = '\0';
                cur = root;
                for (int k = 0; buf[k] && cur; k++)
                    cur = (buf[k] == '.') ? cur->dot : cur->dash;
                printf("%c", (cur && cur->ch) ? cur->ch : '?');
                bi = 0;
            }
        } else if (line[i] == '/') {
            printf(" ");
        } else {
            if (bi < MAX_MORSE_LEN)
                buf[bi++] = line[i];
        }
    }

    if (bi > 0) {
        buf[bi] = '\0';
        cur = root;
        for (int k = 0; buf[k] && cur; k++)
            cur = (buf[k] == '.') ? cur->dot : cur->dash;
        printf("%c", (cur && cur->ch) ? cur->ch : '?');
    }
    printf("\n");
}

/* ---------------- ENCODE ---------------- */
const char* encodeChar(char c) {
    c = toupper((unsigned char)c);
    for (int i = 0; i < N; i++)
        if (table[i].c == c)
            return table[i].m;
    return NULL;
}

void encode(const char* text) {
    for (int i = 0; text[i]; i++) {
        if (text[i] == ' ')
            printf("/ ");
        else {
            const char* m = encodeChar(text[i]);
            if (m) printf("%s ", m);
            else printf("? ");
        }
    }
    printf("\n");
}

/* ---------------- MAIN ---------------- */
int main() {
    Node* root = buildTree();
    char input[256];

    printf("MORSE CODE TRANSLATOR\n");
    printf("D = Decode | E = Encode | EXIT = Quit\n\n");

    while (1) {
        printf("Enter command: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "EXIT") == 0) {
            printf("Exiting program...\n");
            break;
        } else if (strcmp(input, "D") == 0) {
            printf("Enter Morse code: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            printf("Decoded text: ");
            decode(root, input);
        } else if (strcmp(input, "E") == 0) {
            printf("Enter text: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            printf("Encoded morse: ");
            encode(input);
        } else {
            printf("Invalid command.\n");
        }
    }
    return 0;
}
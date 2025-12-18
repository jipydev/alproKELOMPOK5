#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    printf("\nTekan ENTER untuk melanjutkan...");
    getchar();
}

int inputInt(const char *msg) {
    char buffer[50];
    int value;
    while (1) {
        printf("%s", msg);
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &value) == 1)
            return value;
        printf("Input harus angka!\n");
    }
}

float inputFloat(const char *msg) {
    char buffer[50];
    float value;
    while (1) {
        printf("%s", msg);
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%f", &value) == 1)
            return value;
        printf("Input harus angka desimal!\n");
    }
}

void inputString(const char *msg, char *buffer, int size) {
    printf("%s", msg);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int validTanggal(const char *tgl) {
    if (strlen(tgl) != 10) return 0;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (tgl[i] != '-') return 0;
        } else {
            if (!isdigit(tgl[i])) return 0;
        }
    }
    return 1;
}

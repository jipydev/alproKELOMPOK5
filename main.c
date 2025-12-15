#include <stdio.h>

int main() {
    int umur;
    float tinggi;
    float median, sd, z;

    printf("=== CEK STUNTING BALITA (Z-SCORE WHO) ===\n");
    printf("Masukkan umur balita (bulan): ");
    scanf("%d", &umur);

    printf("Masukkan tinggi badan (cm): ");
    scanf("%f", &tinggi);

    // Data WHO (Laki-laki, disederhanakan)
    if (umur <= 12) {
        median = 75.7;
        sd = 2.9;
    } else if (umur <= 24) {
        median = 87.1;
        sd = 3.2;
    } else if (umur <= 36) {
        median = 96.1;
        sd = 3.4;
    } else if (umur <= 48) {
        median = 103.3;
        sd = 3.7;
    } else {
        median = 110.0;
        sd = 4.0;
    }

    // Hitung Z-score
    z = (tinggi - median) / sd;

    printf("\nMedian WHO : %.1f cm\n", median);
    printf("Z-Score   : %.2f\n", z);

    // Klasifikasi
    if (z < -3) {
        printf("Status: SANGAT PENDEK (STUNTING BERAT)\n");
    } else if (z < -2) {
        printf("Status: PENDEK (STUNTING)\n");
    } else if (z <= 2) {
        printf("Status: NORMAL\n");
    } else {
        printf("Status: TINGGI\n");
    }

    return 0;
}

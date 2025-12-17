#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Inventaris.h"
#include "utils.h"


#define FILE_INV  "inventaris.txt"

typedef struct 
{
    int id;
    char namaBarang[50];
    int stok;
}inventaris;

int generateId() {
    FILE *fp = fopen(FILE_INV, "r");
    if (!fp) return 1;

    Inventaris i;
    int lastId = 0;

    while (fscanf(fp, "%d %s %d", &i.id, i.barang, &i.jumlah) != BATAS) {
        if (i.id > lastId)
            lastId = i.id;
    }

    fclose(fp);
    return lastId + 1;
}

//Fitur tambah 
void tambahInv() {
    FILE *fp = fopen(FILE_INV, "r");
    int count = 0;
    Inventaris temp;

    if (fp) {
        while (fscanf(fp, "%d %s %d", &temp.id, temp.barang, &temp.jumlah) != BATAS)
            count++;
        fclose(fp);
    }

    if (count >= BATAS) {
        printf("Inventory penuh!\n");
        return;
    }

    Inventaris i;
    i.id = generateId();

    printf("Nama barang (1 kata): ");
    scanf("%s", i.barang);

    printf("Jumlah: ");
    scanf("%d", &i.jumlah);

    fp = fopen(FILE_INV, "a");
    fprintf(fp, "%d %s %d\n", i.id, i.barang, i.jumlah);
    fclose(fp);

}







//Menu utama 
void menuInventaris(){

printf("|=========================|\n");
printf("|       Inventory         |\n");
printf("|=========================|\n");
printf("|1.Lihat Barang           |\n");
printf("|-------------------------|\n");
printf("|2.Tambah Barang          |\n");
printf("|-------------------------|\n");
printf("|3.Edit Barang            |\n");
printf("|-------------------------|\n");
printf("|4.Cari Barang            |\n");
printf("|-------------------------|\n");
printf("|5.Hapus Barang           |\n");
printf("|-------------------------|\n");
}








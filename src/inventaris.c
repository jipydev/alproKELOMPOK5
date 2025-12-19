#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventaris.h"
#include "utils.h"

#define BATAS 100
#define FILE_INV  "db/inventaris.txt"

typedef struct 
{
    int id;
    char namaBarang[50];
    int stok;
}inventaris;



//Fitur tambah 
void tambahInv() {
    clearScreen();
    FILE *fp = fopen(FILE_INV, "r");
    int count = 0;
    inventaris temp;

    if (fp) {
        while (fscanf(fp, "%d %s %d", &temp.id, temp.namaBarang, &temp.stok) == 3)
            count++;
        fclose(fp);
    }

    if (count >= BATAS) {
        printf("Inventory penuh!\n");
        return;
    }

    inventaris i;
    i.id = count+1;

    printf("Nama barang : ");
    scanf("%s", i.namaBarang);

    printf("Jumlah: ");
    scanf("%d", &i.stok);
    getchar();

    fp = fopen(FILE_INV, "a");
if (!fp) {
    printf("Gagal membuka file!\n");
    return;
}

fprintf(fp, "%d %s %d\n", i.id, i.namaBarang, i.stok);
fclose(fp);

printf("Data Berhasil Disimpan");
pauseScreen();
 
}
// Tampilkan semua data
void tampilkanSemuaInv() {
    clearScreen();
     FILE *fp = fopen(FILE_INV, "r");
    if (!fp) {
        printf("Belum ada data.\n");
        return;
    }

    inventaris i;

    printf("\n      === DATA INVENTARIS ===    \n");
printf("------------------------------------------------\n");
printf("| %-4s | %-20s | %-6s |\n", "ID", "Nama Barang", "Stok");
printf("------------------------------------------------\n");

while (fscanf(fp, "%d %s %d", &i.id, i.namaBarang, &i.stok) == 3) {
    printf("| %-4d | %-20s | %-6d |\n",
           i.id, i.namaBarang, i.stok);
}

printf("------------------------------------------------\n");
pauseScreen();
    fclose(fp);
}
void editInv(){
clearScreen();
    FILE *fp, *temp;
    inventaris i;
    int idCari;
    int found = 0;

    printf("Masukkan ID yang ingin diedit: ");
    scanf("%d", &idCari);

    fp = fopen("db.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }

    for (int x = 0; x < BATAS; x++) {

        if (fscanf(fp, "%d %s %d",
                   &i.id, i.namaBarang, &i.stok) != 3) {
            break; 
        }

        if (i.id == idCari) {
            printf("Data ditemukan!\n");
            printf("Nama baru: ");
            scanf("%s", i.namaBarang);
            printf("Jumlah baru: ");
            scanf("%d", &i.stok);



            
            found = 1;
        }

        fprintf(temp, "%d %s %d\n",
                i.id, i.namaBarang, i.stok);
    }

    fclose(fp);
    fclose(temp);

    remove("db.txt");
    rename("temp.txt", "db.txt");

    if (found)
        printf("Data berhasil diedit!\n");
    else
        printf("ID tidak ditemukan!\n");
}
void hapusInv() {
    clearScreen();
    FILE *fp, *temp;
    int idHapus;
    inventaris i;
    int ditemukan = 0;

    printf("Masukkan ID barang yang ingin dihapus: ");
    scanf("%d", &idHapus);

    fp = fopen(FILE_INV, "r");
    if (!fp) {
        printf("Data belum ada!\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Gagal membuat file sementara!\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d %s %d", &i.id, i.namaBarang, &i.stok) == 3) {
        if (i.id == idHapus) {
            ditemukan = 1;   
        } else {
            fprintf(temp, "%d %s %d\n", i.id, i.namaBarang, i.stok);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_INV);
    rename("temp.txt", FILE_INV);

    if (ditemukan)
        printf("Barang dengan ID %d berhasil dihapus.\n", idHapus);
    else
        printf("ID tidak ditemukan.\n");
}
void cariInv() {
    clearScreen();
    FILE *fp, *temp;
    char namaInv[50];
    inventaris i;
    int ditemukan = 0;

    printf("Masukkan Nama barang yang ingin dicari: ");
    scanf("%s", namaInv);

    fp = fopen(FILE_INV, "r");
    if (!fp) {
        printf("Data belum ada!\n");
        return;
    }
    printf("\n=== HASIL PENCARIAN ===\n");

    while (fscanf(fp, "%d %s %d", &i.id, i.namaBarang, &i.stok) == 3) {

        if (strcmp(i.namaBarang, namaInv) == 0) {
            printf("ID   : %d\n", i.id);
            printf("Nama : %s\n", i.namaBarang);
            printf("Stok : %d pcs\n", i.stok);
            ditemukan = 1;
            break; 
        }
    }

    if (!ditemukan) {
        printf("Barang dengan nama '%s' tidak ditemukan.\n", namaInv);
    }

    fclose(fp);
}
    






//Menu utama 
void menuInventaris(){
    clearScreen();
int pilih;
printf("|=========================|\n");
printf("|       Inventory         |\n");
printf("|=========================|\n");
printf("|1.Lihat Barang           |\n");
printf("|-------------------------|\n");
printf("|2.Tambah Barang          |\n");
printf("|-------------------------|\n");
printf("|3.Edit Barang            |\n");
printf("|-------------------------|\n");
printf("|4.Hapus Barang           |\n");
printf("|-------------------------|\n");
printf("|5.Cari Inventory         |\n");
printf("|-------------------------|\n");
pilih=inputInt ("Pilihan Anda :");

switch (pilih)
{
case 1 : tampilkanSemuaInv();break;
case 2 : tambahInv();break;
case 3 : editInv();break;
case 4 : hapusInv();break;
case 5 : hapusInv();break;
case 6 : exit(0);break;
default:
    printf("Kesalahan");
}
}








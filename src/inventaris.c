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


//id
int generateId() {
    FILE *fp = fopen(FILE_INV, "r");
    if (!fp) return 1;

    inventaris i;
    int lastId = 0;

    while (fscanf(fp, "%d %s %d",
                  &i.id, i.namaBarang, &i.stok) == 3) {
        if (i.id > lastId)
            lastId = i.id;
    }

    fclose(fp);
    return lastId + 1;
}
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
    i.id = generateId();
printf("=====================================\n");
printf("|        TAMBAH DATA BARANG          |\n");
printf("=====================================\n");
printf("| Nama Barang : ");
scanf(" %s", i.namaBarang);
printf("| Jumlah      : ");
scanf("%d", &i.stok);
printf("=====================================\n");
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
printf("Tekan 0 Untuk Melanjutkan..");
getchar();
    fclose(fp);
}
void tampilkanSemuaInvDoang() {
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
}
void editInv(){
clearScreen();
    FILE *fp, *temp;
    inventaris i;
    int idCari;
    int found = 0;
    
printf("=====================================\n");
printf("|            EDIT BARANG             |\n");
printf("=====================================\n");
    printf("Masukkan ID yang ingin diedit: ");
    scanf("%d", &idCari);
printf("=====================================\n");

    fp = fopen("in.txt", "r");
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
            printf("\nTekan 0 untuk kembali ke menu...");
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
    inventaris i, barangHapus;
    int ditemukan = 0;
    tampilkanSemuaInvDoang();
    printf("------------------------------------------------\n");


    
    printf("Masukkan ID barang yang ingin dihapus: ");
    scanf("%d", &idHapus);

    fp = fopen(FILE_INV, "r");
    if (!fp) {
        printf("Data belum ada!\n");
        pauseScreen();
        return;
    }

    temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Gagal membuat file sementara!\n");
        pauseScreen();
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d %s %d",
                  &i.id, i.namaBarang, &i.stok) == 3) {

        if (i.id == idHapus) {
            barangHapus = i;   
            ditemukan = 1;
        } else {
            fprintf(temp, "%d %s %d\n",
                    i.id, i.namaBarang, i.stok);
        }
    }

    fclose(fp);
    fclose(temp);

    if (ditemukan) {
        remove(FILE_INV);
        rename("temp.txt", FILE_INV);

        printf("\nBarang BERHASIL dihapus:\n");
        printf("------------------------------------------------\n");
        printf("| %-4s | %-20s | %-6s |\n", "ID", "Nama Barang", "Stok");
        printf("------------------------------------------------\n");
        printf("| %-4d | %-20s | %-6d |\n",
               barangHapus.id,
               barangHapus.namaBarang,
               barangHapus.stok);
        printf("------------------------------------------------\n");
    } else {
        remove("temp.txt");
        printf("ID tidak ditemukan.\n");
    }

    printf("\nTekan 0 untuk kembali ke menu...");
    int back;
    do {
        scanf("%d", &back);
    } while (back != 0);
}

void cariInv() {
    clearScreen();
    FILE *fp, *temp;
    char namaInv[50];
    inventaris i;
    int ditemukan = 0;

    printf("Masukkan Nama barang yang ingin dicari: ");
    scanf(" %s", namaInv);

    fp = fopen(FILE_INV, "r");
    if (!fp) {
        printf("Data belum ada!\n");
        return;
    }
    printf("------------------------------------------------\n");
    printf("| %-4s | %-20s | %-6s |\n", "ID", "Nama Barang", "Stok");
    printf("------------------------------------------------\n");

    while (fscanf(fp, "%d %s %d",
                  &i.id, i.namaBarang, &i.stok) == 3) {

        if (strcmp(i.namaBarang, namaInv) == 0) {
            printf("| %-4d | %-20s | %-6d |\n",
                   i.id, i.namaBarang, i.stok);
            ditemukan = 1;
            break;
        }
    }

    if (!ditemukan) {
        printf("| %-4s | %-20s | %-6s |\n", "-", "Data tidak ditemukan", "-");
    }

    printf("------------------------------------------------\n");
    fclose(fp);

    printf("\nTekan 0 untuk kembali ke menu...");
    getchar();
    getchar();
}





//Menu utama 
void menuInventaris(){
    clearScreen();
int pilih;
do{
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
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");

pilih=inputInt("Pilihan Anda : ");
if (pilih==1) tampilkanSemuaInv();
else if (pilih==2) tambahInv();
else if (pilih==3) editInv();
else if (pilih==4) hapusInv();
else if (pilih==5) cariInv();

} while (pilih !=0);
}










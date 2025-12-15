#ifndef REKAM_MEDIS_H
#define REKAM_MEDIS_H

typedef struct {
    int id;
    char nama[50];
    char keluhan[100];
} Rekam;

/* Menu utama */
void menuRekam();

/* CRUD */
void tambahRekam();
void lihatRekam();
void editRekam();
void hapusRekam();

/* Submenu lihat */
void tampilkanSemuaRekam();
void cariRekamById();
void cariRekamByNama();

#endif
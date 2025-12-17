#ifndef REKAM_MEDIS_H
#define REKAM_MEDIS_H

/* =======================
   STRUKTUR DATA
   ======================= */
typedef struct {
    int id;
    char nama[50];
    char keluhan[100];
} Rekam;

/* =======================
   MENU UTAMA
   ======================= */
void menuRekam();

/* =======================
   CRUD
   ======================= */
void tambahRekam();
void lihatRekam();
void editRekam();
void hapusRekam();

/* =======================
   SUBMENU LIHAT
   ======================= */
void tampilkanSemuaRekam();
void cariRekamById();
void cariRekamByNama();

#endif

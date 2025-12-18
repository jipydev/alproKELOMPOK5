#ifndef JADWAL_KEGIATAN_H
#define JADWAL_KEGIATAN_H

typedef struct {
    int id;
    char kegiatan[50];
    char tanggal[30];

    } Jadwal;

/* Menu utama */
void menuJadwal();

/* CRUD */
void tambahJadwal();
void lihatJadwal();
void editJadwal();
void hapusJadwal();

/* Submenu lihat */
void tampilkanSemuaJadwal();
void cariJadwalById();
void cariJadwalByNama();
void cariJadwalByTanggal();

#endif

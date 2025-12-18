#ifndef JADWAL_KEGIATAN_H
#define JADWAL_KEGIATAN_H

typedef struct {
    int id;
    char kegiatan[50];
    char tanggal[30];

    } Jadwal;

/* Menu utama */
void menuJadwal();

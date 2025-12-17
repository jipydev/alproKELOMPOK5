#ifndef UTILS_H
#define UTILS_H

/* Tampilan */
void clearScreen();
void pauseScreen();

/* Input aman */
int inputInt(const char *msg);
float inputFloat(const char *msg);
void inputString(const char *msg, char *buffer, int size);

/* Validasi */
int validTanggal(const char *tgl);

#endif

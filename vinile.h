#ifndef VINILE_H
#define VINILE_H
#include "audiovisivo.h"

class Vinile: public Audiovisivo {
private:
    string artista;
    int numeroTracce;
public:
    Vinile(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, int durata, string produzione,
string artista, int numeroTracce);
    string getArtista() const;
    int getNumeroTracce() const;
};

#endif // VINILE_H

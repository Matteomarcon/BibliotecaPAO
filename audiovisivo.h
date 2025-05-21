#ifndef AUDIOVISIVO_H
#define AUDIOVISIVO_H
#include "media.h"

class Audiovisivo: public Media
{
private:
    int durata;
    string produzione;
public:
    Audiovisivo(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, int durata, string produzione);
    int getDurata() const;
    string getProduzione() const;
};

#endif // AUDIOVISIVO_H

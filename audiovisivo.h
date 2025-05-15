#ifndef AUDIOVISIVO_H
#define AUDIOVISIVO_H
#include "media.h"

class audiovisivo: public Media
{
private:
    int durata;
    string produzione;
public:
    audiovisivo(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, int durata, string produzione);
    virtual ~audiovisivo();
    int getDurata() const;
    string getProduzione() const;
};

#endif // AUDIOVISIVO_H

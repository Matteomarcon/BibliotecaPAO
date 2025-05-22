#ifndef AUDIOVISIVO_H
#define AUDIOVISIVO_H
#include "media.h"

class Audiovisivo: public Media
{
private:
    int durata;
    string produzione;
public:
    virtual ~Audiovisivo();

    Audiovisivo(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, int durata, string produzione);
    Audiovisivo();
    int getDurata() const;
    string getProduzione() const;

    virtual QIcon getIcon() const = 0;
};

#endif // AUDIOVISIVO_H

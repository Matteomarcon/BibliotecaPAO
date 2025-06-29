#ifndef AUDIOVISIVO_H
#define AUDIOVISIVO_H

#include "media.h"

class Audiovisivo: public Media {
private:
    int durata;
    QString produzione;
public:
    virtual ~Audiovisivo();

    Audiovisivo(int idMedia, QString immagine, QString titolo, float prezzo, QDate dataPubblicazione, QString genere, bool disponibilita, int copie, int durata, QString produzione);
    int getDurata() const;
    QString getProduzione() const;

    virtual QIcon getIcon() const = 0;
};

#endif // AUDIOVISIVO_H

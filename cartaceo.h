#ifndef CARTACEO_H
#define CARTACEO_H

#include "media.h"

class Cartaceo: public Media {
private:
    QString autore;
    QString editore;
public:
    virtual ~Cartaceo();

    Cartaceo(int idMedia, QString immagine, QString titolo, float prezzo, QDate dataPubblicazione, QString genere, bool disponibilita,
             int copie, QString autore, QString editore);
    QString getAutore() const;
    QString getEditore() const;

    virtual QIcon getIcon() const = 0;
};

#endif // CARTACEO_H

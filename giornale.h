#ifndef GIORNALE_H
#define GIORNALE_H

#include "cartaceo.h"

class Giornale: public Cartaceo {
private:
    QString testata;

public:
    Giornale(int idMedia, QString immagine, QString titolo, float prezzo, QDate dataPubblicazione, QString genere, bool disponibilita, int copie, QString autore, QString editore,
QString testata);
    QString getTestata() const;

    QIcon getIcon() const override;
};

#endif // GIORNALE_H

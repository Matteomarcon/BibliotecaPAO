#ifndef LIBRO_H
#define LIBRO_H

#include "cartaceo.h"

class Libro: public Cartaceo {
private:
    QString formato;
    QString lingua;
public:
    Libro(int idMedia, QString immagine, QString titolo, float prezzo, QDate dataPubblicazione, QString genere, bool disponibilita,
          int copie, QString autore, QString editore, QString lingua, QString formato);
    QString getFormato() const;
    QString getLingua() const;

    QIcon getIcon() const override;
};

#endif // LIBRO_H

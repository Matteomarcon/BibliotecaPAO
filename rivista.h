#ifndef RIVISTA_H
#define RIVISTA_H

#include "cartaceo.h"

class Rivista: public Cartaceo {
private:
    int numero;
    QString periodicita;
public:
    Rivista(int idMedia, QString immagine, QString titolo, float prezzo, QDate dataPubblicazione, QString genere,
            bool disponibilita, int copie, QString autore, QString editore, int numero, QString periodicita);
    int getNumero() const;
    QString getPeriodicita() const;

    QIcon getIcon() const override;
};

#endif // RIVISTA_H

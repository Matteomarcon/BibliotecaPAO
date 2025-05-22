#ifndef RIVISTA_H
#define RIVISTA_H
#include "cartaceo.h"

class Rivista: public Cartaceo
{
private:
    int numero;
    string periodicita;
public:
    Rivista(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere,
            bool disponibilita, int copie, string autore, string editore, int numero, string periodicita);
    int getNumero() const;
    string getPeriodicita() const;

    QIcon getIcon() const override;
};

#endif // RIVISTA_H

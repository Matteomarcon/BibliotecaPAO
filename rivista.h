#ifndef RIVISTA_H
#define RIVISTA_H
#include "cartaceo.h"

class rivista: public cartaceo
{
private:
    int numero;
    enum periodicita {
    settimanale,
    mensile,
    bimestrale,
    trimestrale,
    semestrale,
    annuale
    };
    periodicita tipoPeriodicita;
public:
    rivista(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere,
            bool disponibilita, int copie, string autore, string editore, int numero, periodicita tipoPeriodicita);
    virtual ~rivista();
    int getNumero() const;
    periodicita getPeriodicita() const;
};

#endif // RIVISTA_H

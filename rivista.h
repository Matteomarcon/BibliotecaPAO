#ifndef RIVISTA_H
#define RIVISTA_H
#include "cartaceo.h"

class Rivista: public Cartaceo {
private:
    int numero;
    QString periodicita;
public:
    Rivista(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere,
            bool disponibilita, int copie, QString autore, QString editore, int numero, QString periodicita);
    int getNumero() const;
    QString getPeriodicita() const;

    QIcon getIcon() const override;
    static void creaForm(QFormLayout *layout);
};

#endif // RIVISTA_H

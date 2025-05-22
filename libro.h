#ifndef LIBRO_H
#define LIBRO_H
#include "cartaceo.h"
#include <QString>

class Libro: public Cartaceo {
private:
    string formato;
    string lingua;
public:
    Libro(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita,
          int copie, string autore, string editore, string lingua, string formato);
    string getFormato() const;
    string getLingua() const;

    QIcon getIcon() const override;
};

#endif // LIBRO_H

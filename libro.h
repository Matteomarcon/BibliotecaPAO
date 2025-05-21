#ifndef LIBRO_H
#define LIBRO_H
#include "cartaceo.h"
#include <QString>

class Libro: public Cartaceo {
private:
    enum formato {
        copertinaRigida,
        tascabile,
        ebook
    };
    string lingua;
    formato tipoFormato;
public:
    Libro(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita,
          int copie, string autore, string editore, string lingua, formato tipoFormato);
    formato getFormato() const;
    string getLingua() const;
};

#endif // LIBRO_H

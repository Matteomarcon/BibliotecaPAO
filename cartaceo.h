#ifndef CARTACEO_H
#define CARTACEO_H
#include "media.h"

class Cartaceo: public Media
{
private:
    string autore;
    string editore;
public:
    Cartaceo(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita,
             int copie, string autore, string editore);
    string getAutore() const;
    string getEditore() const;
};

#endif // CARTACEO_H

#ifndef CARTACEO_H
#define CARTACEO_H
#include "media.h"

class cartaceo: public Media
{
private:
    string autore;
    string editore;
public:
    cartaceo(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita,
             int copie, string autore, string editore);
    virtual ~cartaceo();
    string getAutore() const;
    string getEditore() const;
};

#endif // CARTACEO_H

#ifndef GIORNALE_H
#define GIORNALE_H
#include "cartaceo.h"

class Giornale: public Cartaceo {
private:
    string testata;

public:
    Giornale(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita, int copie, string autore, string editore,
string testata);
    string getTestata() const;

    QIcon getIcon() const override;
    static void creaForm(QFormLayout *layout);
};

#endif // GIORNALE_H

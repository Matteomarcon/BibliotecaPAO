#ifndef CARTACEO_H
#define CARTACEO_H
#include "media.h"
#include <QLineEdit>
#include <QFormLayout>

class Cartaceo: public Media
{
private:
    string autore;
    string editore;
public:
    virtual ~Cartaceo();

    Cartaceo(QImage immagine, string titolo, float prezzo, Data dataPubblicazione, string genere, bool disponibilita,
             int copie, string autore, string editore);
    string getAutore() const;
    string getEditore() const;

    virtual QIcon getIcon() const = 0;
    static void creaForm(QFormLayout *layout);
};

#endif // CARTACEO_H

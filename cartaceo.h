#ifndef CARTACEO_H
#define CARTACEO_H
#include "media.h"
#include <QLineEdit>
#include <QFormLayout>

class Cartaceo: public Media
{
private:
    QString autore;
    QString editore;
public:
    virtual ~Cartaceo();

    Cartaceo(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere, bool disponibilita,
             int copie, QString autore, QString editore);
    QString getAutore() const;
    QString getEditore() const;

    virtual QIcon getIcon() const = 0;
    static void creaForm(QFormLayout *layout);
};

#endif // CARTACEO_H

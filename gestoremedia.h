#ifndef GESTOREMEDIA_H
#define GESTOREMEDIA_H

#include "gestorejson.h"
#include <QListWidget>
#include <QFormLayout>

class GestoreMedia {
private:
    GestoreJson gestoreJson;
    QListWidget* listaMedia;
public:
    GestoreMedia(QListWidget* lista, QString percorso = "dati.json");

    QStringList getTipiDisponibili() const;
    void salvaMediaDaForm(QString tipo, QFormLayout *layout);
    void caricaBiblioteca();

    void creaForm(const QString& tipo, QFormLayout *layout) const;
    Media* creaFilm(QFormLayout* layout);
    //Media* creaGiornale(QFormLayout* layout);
    //Media* creaLibro(QFormLayout* layout);
    //Media* creaRivista(QFormLayout* layout);
    //Media* creaVinile(QFormLayout* layout);
};

#endif // GESTOREMEDIA_H

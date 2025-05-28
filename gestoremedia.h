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
    void inserisciNuovoMedia(Media* media);
    void caricaBiblioteca();

    void creaForm(const QString& tipo, QFormLayout* layout) const;
};

#endif // GESTOREMEDIA_H

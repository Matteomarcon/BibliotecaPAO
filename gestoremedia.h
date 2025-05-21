#ifndef GESTOREMEDIA_H
#define GESTOREMEDIA_H

#include "gestorejson.h"
#include <QListWidget>

class GestoreMedia {
private:
    GestoreJson gestoreJson;
    QListWidget* listaMedia;
public:
    GestoreMedia(QListWidget* lista, const QString& percorso = "dati.json");

    void inserisciNuovoMedia(Media* media);
};

#endif // GESTOREMEDIA_H

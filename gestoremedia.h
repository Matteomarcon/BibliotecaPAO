#ifndef GESTOREMEDIA_H
#define GESTOREMEDIA_H

#include "gestorejson.h"
#include <QListWidget>
#include <QFormLayout>

class GestoreMedia {
private:
    GestoreJson gestoreJson;
    QListWidget* listaMedia;
    QFormLayout* formLayout;

public:
    GestoreMedia(QListWidget* listaMedia, QFormLayout* formLayout, QString percorsoJson = "BibliotecaDefault.json");

    //Utilità
    static QStringList getTipiDisponibili();

    //Gestione Media
    void salvaMediaDaForm(const QString& tipo);
    void caricaBiblioteca();

    //Creazione Form
    void creaForm(const QString& tipo);
    void creaFormAudiovisivo();
    void creaFormCartaceo();
    void creaFormFilm();
    void creaFormGiornale();
    void creaFormLibro();
    void creaFormRivista();
    void creaFormVinile();

    //Creazione Media
    Media* creaFilm();
    Media* creaGiornale();
    Media* creaLibro();
    Media* creaRivista();
    Media* creaVinile();

    //Eliminazione Media
    void eliminaMedia(int indice);
};

#endif // GESTOREMEDIA_H

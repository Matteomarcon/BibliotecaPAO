#ifndef GESTOREMEDIA_H
#define GESTOREMEDIA_H

#include "gestorejson.h"

#include <QListWidget>
#include <QFormLayout>
#include <QLabel>

class GestoreMedia {
private:
    QListWidget* listaMedia;
    QListWidget* listaPrestiti;
    QFormLayout* formLayout;
    QLabel* imagePreview;
    GestoreJson gestoreJson;

public:
    GestoreMedia(QListWidget* listaMedia, QListWidget* listaPrestiti, QFormLayout* formLayout, QLabel* imagePreview, QString percorsoJson = "BibliotecaDefault.json");

    //Utilità
    static QStringList getTipiDisponibili();

    //Gestione Media
    void salvaMediaDaForm(const QString& tipo, int indice = -1);
    void caricaFormDaMedia(int indice);
    void modificaMedia(int indice, int copie, bool disponibilita);
    void caricaBiblioteca(QLabel* risultatiLabel);

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

    //Prestiti
    void creaPrestito();
    void caricaPrestiti(QLabel* labelRisultatiPrestiti);
    void salvaPrestito(Prestito* prestito);
    void eliminaPrestito(int indice);
};

#endif // GESTOREMEDIA_H

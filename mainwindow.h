#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidgetItem>
#include <QLabel>
#include "GestoreMedia.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private slots:
    void showPaginaPrincipale();
    void showPaginaNuovoMedia();
    void mostraInfo(QListWidgetItem *item);
    void caricaBiblioteca();
    void eliminaMedia();

private:
    GestoreMedia *gestore;
    QFormLayout* formLayout;
    QLabel* imagePreview;
    QLabel* risultatiLabel;

    QStackedWidget *stackedWidget;
    QWidget *paginaPrincipale;
    QWidget *paginaNuovoMedia;

    QListWidget *listaMedia;

    QWidget *rightPanel;
    QLabel *labelIcona;
    QLabel *labelCategoria;
    QLabel *labelAutore;
    QLabel *labelEditore;
    QLabel *labelTestata;
    QLabel *labelNumeroTracce;
    QLabel *labelArtista;
    QLabel *labelProduzione;
    QLabel *labelDurata;
    QLabel *labelCopie;
    QLabel *labelDisponibilita;
    QLabel *labelGenere;
    QLabel *labelData;
    QLabel *labelPrezzo;
    QLabel *labelNumero;
    QLabel *labelPeriodicita;
    QLabel *labelLingua;
    QLabel *labelLinguaOriginale;
    QLabel *labelFormato;
    QLabel *labelTitoloTopBar;
    QLabel *labelTitolo;
    QLabel *labelDescrizione;
    QLabel *labelPaese;
    QLabel *labelRegista;
    QLabel *labelImmagine;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H

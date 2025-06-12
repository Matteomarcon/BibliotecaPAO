#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QComboBox>
#include "GestoreMedia.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //Funzionalità
    void inizializzaGUI();
    void creaPaginaPrincipale();
    void creaPaginaForm();
    void mostraPaginaPrincipale();
    void mostraPaginaForm();
    void mostraInfo(QListWidgetItem *item);
    QLabel* creaLabel(QWidget *parent, QVBoxLayout *layout);
    void caricaBiblioteca();
    void eliminaMedia();

    //GUI
    QStackedWidget *stackPagine;
    QWidget *paginaPrincipale;
    QWidget *paginaForm;
    QWidget *pannelloInfo;

    //pannelloSinistro
    QListWidget *listaMedia;
    QFormLayout *formLayout;
    QLabel *labelRisultati;
    QComboBox *selettoreMedia;
    QLabel *labelImmagine;

    //pannelloInfo
    QLabel *labelIconaTopBar;
    QLabel *labelTitoloTopBar;
    QLabel *labelAnteprimaImmagine;
    QLabel *labelCategoria;
    QLabel *labelTitolo;
    QLabel *labelAutore;
    QLabel *labelEditore;
    QLabel *labelTestata;
    QLabel *labelPrezzo;
    QLabel *labelGenere;
    QLabel *labelData;
    QLabel *labelDisponibilita;
    QLabel *labelCopie;
    QLabel *labelFormato;
    QLabel *labelLingua;
    QLabel *labelLinguaOriginale;
    QLabel *labelRegista;
    QLabel *labelProduzione;
    QLabel *labelDurata;
    QLabel *labelPaese;
    QLabel *labelNumero;
    QLabel *labelPeriodicita;
    QLabel *labelArtista;
    QLabel *labelNumeroTracce;

    //Backend
    GestoreMedia *gestore;


/*private slots:
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
    QLabel *labelImmagine;*/
};

#endif // MAINWINDOW_H

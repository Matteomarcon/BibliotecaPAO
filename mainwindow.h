#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GestoreMedia.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QDoubleSpinBox>

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
    void creaPaginaPrestiti();
    void mostraPaginaPrincipale();
    void mostraPaginaForm();
    void mostraPaginaPrestiti();
    void mostraInfo();
    void pulisciInfo();
    void pulisciForm();
    QLabel* creaLabel(QWidget *parent, QVBoxLayout *layout);
    void caricaBiblioteca();
    void eliminaMedia();

    //filtri
    QLineEdit* barraRicerca;
    QDialog* dialogFiltri;
    QDialog* dialog;
    QDoubleSpinBox* spinPrezzoMin;
    QDoubleSpinBox* spinPrezzoMax;
    QComboBox* comboCategoria;
    QComboBox* comboDisponibilita;
    void aggiornaFiltroMedia(const QString& testoRicerca, const QString& categoriaSelezionata, const QString& disponibilitaSelezionata, double minPrezzo, double maxPrezzo);

    //GUI
    QStackedWidget *stackPagine;
    QWidget *paginaPrincipale;
    QWidget *paginaForm;
    QWidget *paginaPrestiti;
    QWidget *pannelloInfo;

    // Bottoni pagina principale
    QPushButton *bottoneModifica;
    QPushButton *bottoneElimina;
    QPushButton *bottoneNuovoPrestito;

    // Bottoni pagina form
    QPushButton *bottoneSalva;

    // Bottoni pagina prestiti
    QPushButton *bottoneRestituzione;

    //prestiti
    QListWidget *listaPrestiti;

    //pannelloSinistro
    QListWidget *listaMedia;
    QFormLayout *formLayout;
    QLabel *labelRisultatiMedia;
    QLabel *labelRisultatiPrestiti;
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
};

#endif // MAINWINDOW_H

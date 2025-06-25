#include "gestoremedia.h"
#include "media.h"
#include "film.h"
#include "vinile.h"
#include "rivista.h"
#include "giornale.h"
#include "libro.h"

#include <QHBoxLayout>
#include <QCheckBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QBuffer>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>

GestoreMedia::GestoreMedia(QListWidget* listaMedia, QListWidget* listaPrestiti, QFormLayout* formLayout, QLabel* imagePreview, QString percorso)
    : listaMedia(listaMedia), listaPrestiti(listaPrestiti), formLayout(formLayout), imagePreview(imagePreview), gestoreJson(percorso) {}

//Utilità
QStringList GestoreMedia::getTipiDisponibili() {
    return {"Film", "Giornale", "Libro", "Rivista", "Vinile"};
}

void GestoreMedia::caricaFormDaMedia(int indice) {
    Media* media = listaMedia->item(indice)->data(Qt::UserRole).value<Media*>();

    // Funzione interna per trovare il widget del formLayout con una certa label (da ripetere ogni volta)
    auto findWidgetByLabel = [this](const QString& labelText) -> QWidget* {
        for (int i = 0; i < formLayout->rowCount(); ++i) {
            QLabel* label = qobject_cast<QLabel*>(formLayout->itemAt(i, QFormLayout::LabelRole)->widget());
            if (label && label->text() == labelText) {
                return formLayout->itemAt(i, QFormLayout::FieldRole)->widget();
            }
        }
        return nullptr;
    };

    if (QLineEdit* titolo = qobject_cast<QLineEdit*>(findWidgetByLabel("Titolo"))) titolo->setText(media->getTitolo());
    if (QDoubleSpinBox* prezzo = qobject_cast<QDoubleSpinBox*>(findWidgetByLabel("Prezzo"))) prezzo->setValue(media->getPrezzo());
    if (QDateEdit* data = qobject_cast<QDateEdit*>(findWidgetByLabel("Data"))) data->setDate(media->getData());
    if (QLineEdit* genere = qobject_cast<QLineEdit*>(findWidgetByLabel("Genere"))) genere->setText(media->getGenere());
    if (QSpinBox* copie = qobject_cast<QSpinBox*>(findWidgetByLabel("Copie"))) copie->setValue(media->getCopie());

    if (Film* film = dynamic_cast<Film*>(media)) {
        if (QSpinBox* durata = qobject_cast<QSpinBox*>(findWidgetByLabel("Durata (min)"))) durata->setValue(film->getDurata());
        if (QLineEdit* produzione = qobject_cast<QLineEdit*>(findWidgetByLabel("Produzione"))) produzione->setText(film->getProduzione());
        if (QLineEdit* regista = qobject_cast<QLineEdit*>(findWidgetByLabel("Regista"))) regista->setText(film->getRegista());
        if (QLineEdit* lingua = qobject_cast<QLineEdit*>(findWidgetByLabel("Lingua originale"))) lingua->setText(film->getLinguaOriginale());
        if (QLineEdit* paese = qobject_cast<QLineEdit*>(findWidgetByLabel("Paese di produzione"))) paese->setText(film->getPaeseProduzione());
    }
    else if (Giornale* giornale = dynamic_cast<Giornale*>(media)) {
        if (QLineEdit* autore = qobject_cast<QLineEdit*>(findWidgetByLabel("Autore"))) autore->setText(giornale->getAutore());
        if (QLineEdit* editore = qobject_cast<QLineEdit*>(findWidgetByLabel("Editore"))) editore->setText(giornale->getEditore());
        if (QLineEdit* testata = qobject_cast<QLineEdit*>(findWidgetByLabel("Testata"))) testata->setText(giornale->getTestata());
    }
    else if (Libro* libro = dynamic_cast<Libro*>(media)) {
        if (QLineEdit* autore = qobject_cast<QLineEdit*>(findWidgetByLabel("Autore"))) autore->setText(libro->getAutore());
        if (QLineEdit* editore = qobject_cast<QLineEdit*>(findWidgetByLabel("Editore"))) editore->setText(libro->getEditore());
        if (QLineEdit* formato = qobject_cast<QLineEdit*>(findWidgetByLabel("Formato"))) formato->setText(libro->getFormato());
        if (QLineEdit* lingua = qobject_cast<QLineEdit*>(findWidgetByLabel("Lingua"))) lingua->setText(libro->getLingua());
    }
    else if (Rivista* rivista = dynamic_cast<Rivista*>(media)) {
        if (QLineEdit* autore = qobject_cast<QLineEdit*>(findWidgetByLabel("Autore"))) autore->setText(rivista->getAutore());
        if (QLineEdit* editore = qobject_cast<QLineEdit*>(findWidgetByLabel("Editore"))) editore->setText(rivista->getEditore());
        if (QSpinBox* numero = qobject_cast<QSpinBox*>(findWidgetByLabel("Numero"))) numero->setValue(rivista->getNumero());
        if (QComboBox* periodicita = qobject_cast<QComboBox*>(findWidgetByLabel("Periodicità"))) {
            int index = periodicita->findText(rivista->getPeriodicita());
            if (index != -1)
                periodicita->setCurrentIndex(index);
        }
    }
    else if (Vinile* vinile = dynamic_cast<Vinile*>(media)) {
        if (QSpinBox* durata = qobject_cast<QSpinBox*>(findWidgetByLabel("Durata (min)"))) durata->setValue(vinile->getDurata());
        if (QLineEdit* produzione = qobject_cast<QLineEdit*>(findWidgetByLabel("Produzione"))) produzione->setText(vinile->getProduzione());
        if (QLineEdit* artista = qobject_cast<QLineEdit*>(findWidgetByLabel("Artista"))) artista->setText(vinile->getArtista());
        if (QSpinBox* tracce = qobject_cast<QSpinBox*>(findWidgetByLabel("Numero tracce"))) tracce->setValue(vinile->getNumeroTracce());
    }

}

//Gestione Media
void GestoreMedia::salvaMediaDaForm(const QString& tipo, int indice) {
    Media* media = nullptr;

    if (tipo == "Film") media = creaFilm();
    else if (tipo == "Giornale") media = creaGiornale();
    else if (tipo == "Libro") media = creaLibro();
    else if (tipo == "Rivista") media = creaRivista();
    else if (tipo == "Vinile") media = creaVinile();

    gestoreJson.salvaMedia(media, indice);
}

void GestoreMedia::caricaBiblioteca(QLabel* risultatiLabel) {
    listaMedia->clear();
    QList<Media*> lista = gestoreJson.caricaBiblioteca();

    for (Media* media : lista) {
        QListWidgetItem* item = new QListWidgetItem(media->getIcon(), media->getTitolo(), listaMedia);
        item->setData(Qt::UserRole, QVariant::fromValue<Media*>(media));

        QFont font;
        font.setBold(true);
        item->setFont(font);

        listaMedia->setIconSize(QSize(48, 48));
    }


    // Aggiorna conteggio all’avvio (tutti visibili)
    int visibili = 0;
    for (int i = 0; i < listaMedia->count(); ++i) {
        QListWidgetItem *item = listaMedia->item(i);
        item->setHidden(false);  // Mostra tutto
        ++visibili;
    }
    risultatiLabel->setText(QString("Risultati: %1").arg(visibili));

}

//Creazione Form
void GestoreMedia::creaForm(const QString& tipo) {
    QPushButton* caricaImmagine = new QPushButton("Carica Immagine");
    caricaImmagine->setObjectName("BottoneImmagine");
    formLayout->addRow("Immagine", caricaImmagine);

    QTextEdit* base64Edit = new QTextEdit();
    base64Edit->setObjectName("Immagine");
    base64Edit->setVisible(false);
    formLayout->addRow("Immagine", base64Edit);

    QLabel* label = qobject_cast<QLabel*>(formLayout->labelForField(base64Edit));
    if (label) label->setVisible(false);

    QObject::connect(caricaImmagine, &QPushButton::clicked, [=]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Scegli immagine", "", "Immagini (*.png *.jpg *.bmp)");

        if (fileName.isEmpty())
            return;

        QPixmap pixmap(fileName);
        if (pixmap.isNull()) {
            QMessageBox::warning(nullptr, "Errore", "Impossibile caricare l'immagine.");
            return;
        }

        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        bool saved = pixmap.save(&buffer, "PNG");

        if (!saved) {
            QMessageBox::warning(nullptr, "Errore", "Errore nel salvataggio dell'immagine in memoria.");
            return;
        } else {
            imagePreview->setPixmap(pixmap);
        }

        QString base64 = QString::fromLatin1(byteArray.toBase64());
        base64Edit->setText(base64);
    });

    formLayout->addRow("Titolo", new QLineEdit());
    formLayout->addRow("Prezzo", new QDoubleSpinBox());
    formLayout->addRow("Data", new QDateEdit());
    formLayout->addRow("Genere", new QLineEdit());
    formLayout->addRow("Copie", new QSpinBox());

    if (tipo == "Film") creaFormFilm();
    else if (tipo == "Giornale") creaFormGiornale();
    else if (tipo == "Libro") creaFormLibro();
    else if (tipo == "Rivista") creaFormRivista();
    else if (tipo == "Vinile") creaFormVinile();
}

void GestoreMedia::creaFormAudiovisivo() {
    formLayout->addRow("Durata (min)", new QSpinBox());
    formLayout->addRow("Produzione", new QLineEdit());
}

void GestoreMedia::creaFormCartaceo() {
    formLayout->addRow("Autore", new QLineEdit());
    formLayout->addRow("Editore", new QLineEdit());
}

void GestoreMedia::creaFormFilm() {
    creaFormAudiovisivo();
    formLayout->addRow("Regista", new QLineEdit());
    formLayout->addRow("Lingua originale", new QLineEdit());
    formLayout->addRow("Paese di produzione", new QLineEdit());
}

void GestoreMedia::creaFormGiornale() {
    creaFormCartaceo();
    formLayout->addRow("Testata", new QLineEdit());
}

void GestoreMedia::creaFormLibro() {
    creaFormCartaceo();
    formLayout->addRow("Formato", new QLineEdit());
    formLayout->addRow("Lingua", new QLineEdit());
}

void GestoreMedia::creaFormRivista() {
    creaFormCartaceo();
    formLayout->addRow("Numero", new QSpinBox());
    QComboBox* periodicita = new QComboBox();
    periodicita->addItems({"Seleziona periocità...", "Settimanale", "Mensile", "Trimestrale", "Quadrimestrale", "Semestrale", "Annuale"});
    periodicita->setItemData(0, 0, Qt::UserRole - 1); //Disabilita la voce
    periodicita->setCurrentIndex(-1);
    formLayout->addRow("Periodicità", periodicita);
}

void GestoreMedia::creaFormVinile() {
    creaFormAudiovisivo();
    formLayout->addRow("Artista", new QLineEdit());
    formLayout->addRow("Numero tracce", new QSpinBox());
}

//Creazione Media
Media* GestoreMedia::creaFilm() {
    QString immagineStr, titoloStr, genereStr, produzioneStr, registaStr, linguaOriginaleStr, paeseProduzioneStr;
    double prezzoVal = 0;
    QDate dataVal;
    bool disponibilitaVal;
    int copieVal = 0, durataVal = 0;

    for (int i = 1; i < formLayout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(formLayout->itemAt(i, QFormLayout::LabelRole)->widget());
        QWidget* field = formLayout->itemAt(i, QFormLayout::FieldRole)->widget();

        QString labelText = label->text();

        if (labelText == "Immagine") {
            if (auto lineEdit = qobject_cast<QTextEdit*>(field)) {
                immagineStr = lineEdit->toPlainText();
            }
        } else if (labelText == "Titolo") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                titoloStr = lineEdit->text();
            }
        } else if (labelText == "Prezzo") {
            if (auto doubleSpin = qobject_cast<QDoubleSpinBox*>(field)) {
                prezzoVal = doubleSpin->value();
            }
        } else if (labelText == "Data") {
            if (auto dateEdit = qobject_cast<QDateEdit*>(field)) {
                dataVal = dateEdit->date();
            }
        } else if (labelText == "Genere") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                genereStr = lineEdit->text();
            }
        } else if (labelText == "Copie") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                copieVal = spinBox->value();
                if (copieVal) disponibilitaVal = true;
                else disponibilitaVal = false;
            }
        } else if (labelText == "Durata (min)") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                durataVal = spinBox->value();
            }
        } else if (labelText == "Produzione") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                produzioneStr = lineEdit->text();
            }
        } else if (labelText == "Regista") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                registaStr = lineEdit->text();
            }
        } else if (labelText == "Lingua originale") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                linguaOriginaleStr = lineEdit->text();
            }
        } else if (labelText == "Paese di produzione") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                paeseProduzioneStr = lineEdit->text();
            }
        }
    }

    int idMedia = 0;
    if (listaMedia->count()) {
        Media* media = listaMedia->item(listaMedia->count()-1)->data(Qt::UserRole).value<Media*>();
        idMedia = media->getId()+1;
    }

    return new Film(idMedia, immagineStr, titoloStr, static_cast<float>(prezzoVal), dataVal, genereStr, disponibilitaVal, copieVal,
                    durataVal, produzioneStr, registaStr, linguaOriginaleStr, paeseProduzioneStr);
}

Media* GestoreMedia::creaGiornale() {
    QString immagineStr, titoloStr, genereStr, autoreStr, editoreStr, testataStr;
    double prezzoVal = 0;
    QDate dataVal;
    bool disponibilitaVal;
    int copieVal = 0;

    for (int i = 1; i < formLayout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(formLayout->itemAt(i, QFormLayout::LabelRole)->widget());
        QWidget* field = formLayout->itemAt(i, QFormLayout::FieldRole)->widget();

        QString labelText = label->text();


        if (labelText == "Immagine") {
            if (auto lineEdit = qobject_cast<QTextEdit*>(field)) {
                immagineStr = lineEdit->toPlainText();
            }
        } else if (labelText == "Titolo") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                titoloStr = lineEdit->text();
            }
        } else if (labelText == "Prezzo") {
            if (auto doubleSpin = qobject_cast<QDoubleSpinBox*>(field)) {
                prezzoVal = doubleSpin->value();
            }
        } else if (labelText == "Data") {
            if (auto dateEdit = qobject_cast<QDateEdit*>(field)) {
                dataVal = dateEdit->date();
            }
        } else if (labelText == "Genere") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                genereStr = lineEdit->text();
            }
        } else if (labelText == "Copie") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                copieVal = spinBox->value();
                if (copieVal) disponibilitaVal = true;
                else disponibilitaVal = false;
            }
        } else if (labelText == "Autore") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                autoreStr = lineEdit->text();
            }
        } else if (labelText == "Editore") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                editoreStr = lineEdit->text();
            }
        } else if (labelText == "Testata") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                testataStr = lineEdit->text();
            }
        }
    }

    int idMedia = 0;
    if (listaMedia->count()) {
        Media* media = listaMedia->item(listaMedia->count()-1)->data(Qt::UserRole).value<Media*>();
        idMedia = media->getId()+1;
    }

    return new Giornale(idMedia, immagineStr, titoloStr, static_cast<float>(prezzoVal), dataVal, genereStr, disponibilitaVal, copieVal,
                        autoreStr, editoreStr, testataStr);
}


Media* GestoreMedia::creaLibro() {
    QString immagineStr, titoloStr, genereStr, autoreStr, editoreStr, formatoStr, linguaStr;
    double prezzoVal = 0;
    QDate dataVal;
    bool disponibilitaVal;
    int copieVal = 0;

    for (int i = 1; i < formLayout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(formLayout->itemAt(i, QFormLayout::LabelRole)->widget());
        QWidget* field = formLayout->itemAt(i, QFormLayout::FieldRole)->widget();

        QString labelText = label->text();


        if (labelText == "Immagine") {
            if (auto lineEdit = qobject_cast<QTextEdit*>(field)) {
                immagineStr = lineEdit->toPlainText();
            }
        } else if (labelText == "Titolo") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                titoloStr = lineEdit->text();
            }
        } else if (labelText == "Prezzo") {
            if (auto doubleSpin = qobject_cast<QDoubleSpinBox*>(field)) {
                prezzoVal = doubleSpin->value();
            }
        } else if (labelText == "Data") {
            if (auto dateEdit = qobject_cast<QDateEdit*>(field)) {
                dataVal = dateEdit->date();
            }
        } else if (labelText == "Genere") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                genereStr = lineEdit->text();
            }
        } else if (labelText == "Copie") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                copieVal = spinBox->value();
                if (copieVal) disponibilitaVal = true;
                else disponibilitaVal = false;
            }
        } else if (labelText == "Autore") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                autoreStr = lineEdit->text();
            }
        } else if (labelText == "Editore") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                editoreStr = lineEdit->text();
            }
        } else if (labelText == "Lingua") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                linguaStr = lineEdit->text();
            }
        } else if (labelText == "Formato") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                formatoStr = lineEdit->text();
            }
        }
    }

    int idMedia = 0;
    if (listaMedia->count()) {
        Media* media = listaMedia->item(listaMedia->count()-1)->data(Qt::UserRole).value<Media*>();
        idMedia = media->getId()+1;
    }

    return new Libro(idMedia, immagineStr, titoloStr, static_cast<float>(prezzoVal), dataVal, genereStr, disponibilitaVal, copieVal,
                     autoreStr, editoreStr, linguaStr, formatoStr);
}


Media* GestoreMedia::creaRivista() {
    QString immagineStr, titoloStr, genereStr, autoreStr, editoreStr, periodicitaStr;
    double prezzoVal = 0;
    QDate dataVal;
    bool disponibilitaVal;
    int copieVal = 0, numeroVal = 0;

    for (int i = 1; i < formLayout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(formLayout->itemAt(i, QFormLayout::LabelRole)->widget());
        QWidget* field = formLayout->itemAt(i, QFormLayout::FieldRole)->widget();

        QString labelText = label->text();


        if (labelText == "Immagine") {
            if (auto lineEdit = qobject_cast<QTextEdit*>(field)) {
                immagineStr = lineEdit->toPlainText();
            }
        } else if (labelText == "Titolo") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                titoloStr = lineEdit->text();
            }
        } else if (labelText == "Prezzo") {
            if (auto doubleSpin = qobject_cast<QDoubleSpinBox*>(field)) {
                prezzoVal = doubleSpin->value();
            }
        } else if (labelText == "Data") {
            if (auto dateEdit = qobject_cast<QDateEdit*>(field)) {
                dataVal = dateEdit->date();
            }
        } else if (labelText == "Genere") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                genereStr = lineEdit->text();
            }
        } else if (labelText == "Copie") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                copieVal = spinBox->value();
                if (copieVal) disponibilitaVal = true;
                else disponibilitaVal = false;
            }
        } else if (labelText == "Autore") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                autoreStr = lineEdit->text();
            }
        } else if (labelText == "Editore") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                editoreStr = lineEdit->text();
            }
        } else if (labelText == "Numero") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                numeroVal = spinBox->value();
            }
        } else if (labelText == "Periodicita'") {
            if (auto comboBox = qobject_cast<QComboBox*>(field)) {
                periodicitaStr = comboBox->currentText();
            }
        }

    }

    int idMedia = 0;
    if (listaMedia->count()) {
        Media* media = listaMedia->item(listaMedia->count()-1)->data(Qt::UserRole).value<Media*>();
        idMedia = media->getId()+1;
    }

    return new Rivista(idMedia, immagineStr, titoloStr, static_cast<float>(prezzoVal), dataVal, genereStr, disponibilitaVal, copieVal,
                       autoreStr, editoreStr, numeroVal, periodicitaStr);
}
Media* GestoreMedia::creaVinile() {
    QString immagineStr, titoloStr, genereStr, produzioneStr, artistaStr;
    double prezzoVal = 0;
    QDate dataVal;
    bool disponibilitaVal;
    int copieVal = 0, durataVal = 0, tracceVal = 0;

    for (int i = 1; i < formLayout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(formLayout->itemAt(i, QFormLayout::LabelRole)->widget());
        QWidget* field = formLayout->itemAt(i, QFormLayout::FieldRole)->widget();

        QString labelText = label->text();

        if (labelText == "Immagine") {
            if (auto lineEdit = qobject_cast<QTextEdit*>(field)) {
                immagineStr = lineEdit->toPlainText();
            }
        } else if (labelText == "Titolo") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                titoloStr = lineEdit->text();
            }
        } else if (labelText == "Prezzo") {
            if (auto doubleSpin = qobject_cast<QDoubleSpinBox*>(field)) {
                prezzoVal = doubleSpin->value();
            }
        } else if (labelText == "Data") {
            if (auto dateEdit = qobject_cast<QDateEdit*>(field)) {
                dataVal = dateEdit->date();
            }
        } else if (labelText == "Genere") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                genereStr = lineEdit->text();
            }
        } else if (labelText == "Copie") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                copieVal = spinBox->value();
                if (copieVal) disponibilitaVal = true;
                else disponibilitaVal = false;
            }
        } else if (labelText == "Durata (min)") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                durataVal = spinBox->value();
            }
        } else if (labelText == "Produzione") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                produzioneStr = lineEdit->text();
            }
        } else if (labelText == "Artista") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                artistaStr = lineEdit->text();
            }
        } else if (labelText == "Numero tracce") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                tracceVal = spinBox->value();
            }
        }
    }

    int idMedia = 0;
    if (listaMedia->count()) {
        Media* media = listaMedia->item(listaMedia->count()-1)->data(Qt::UserRole).value<Media*>();
        idMedia = media->getId()+1;
    }

    return new Vinile(idMedia, immagineStr, titoloStr, static_cast<float>(prezzoVal), dataVal, genereStr, disponibilitaVal, copieVal,
                      durataVal, produzioneStr, artistaStr, tracceVal);
}

//Eliminazione Media
void GestoreMedia::eliminaMedia(int indice) {
    gestoreJson.eliminaMedia(indice);
}

void GestoreMedia::eliminaPrestito(int indice) {
    gestoreJson.eliminaPrestito(indice);
}
void GestoreMedia::salvaPrestito(Prestito* prestito) {
    gestoreJson.salvaPrestito(prestito);
}


void GestoreMedia::caricaPrestiti(QLabel* labelRisultatiPrestiti) {
    listaPrestiti->clear();
    QList<Prestito*> lista = gestoreJson.caricaPrestiti();

    for (Prestito* prestito : lista) {
        QListWidgetItem* item = new QListWidgetItem("Prestito: " + prestito->getNome() + " " + prestito->getCognome() + ", Titolo: " + listaMedia->item(prestito->getIdMedia())->data(Qt::UserRole).value<Media*>()->getTitolo() +
                                                        ", dal " + prestito->getDataInizio().toString("dd/MM/yyyy") +
                                                        " al " + prestito->getDataFine().toString("dd/MM/yyyy"), listaPrestiti);

        QFont font;
        font.setBold(true);
        item->setFont(font);
    }

    // Aggiorna conteggio all’avvio (tutti visibili)
    int visibili = 0;
    for (int i = 0; i < listaPrestiti->count(); ++i) {
        QListWidgetItem *item = listaPrestiti->item(i);
        item->setHidden(false);  // Mostra tutto
        ++visibili;
    }
    labelRisultatiPrestiti->setText(QString("Risultati: %1").arg(visibili));

}


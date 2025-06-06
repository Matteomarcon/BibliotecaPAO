#include "gestoremedia.h"
#include <media.h>
#include <film.h>
#include <vinile.h>
#include <rivista.h>
#include <giornale.h>
#include <libro.h>


#include <QHBoxLayout>
#include <QCheckBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QBuffer>
#include <QPushButton>
#include <QFileDialog>

GestoreMedia::GestoreMedia(QListWidget* listaMedia, QFormLayout* formLayout, QString percorso)
    : gestoreJson(percorso), listaMedia(listaMedia), formLayout(formLayout) {}

//Utilità
QStringList GestoreMedia::getTipiDisponibili() {
    return {"Film", "Giornale", "Libro", "Rivista", "Vinile"};
}

//Gestione Media
void GestoreMedia::salvaMediaDaForm(const QString& tipo) {
    Media* media = nullptr;

    if (tipo == "Film") media = creaFilm();
    else if (tipo == "Giornale") media = creaGiornale();
    else if (tipo == "Libro") media = creaLibro();
    else if (tipo == "Rivista") media = creaRivista();
    else if (tipo == "Vinile") media = creaVinile();

    gestoreJson.salvaNuovoMedia(media);
}

void GestoreMedia::caricaBiblioteca() {

    listaMedia->clear();
    QList<Media*> lista = gestoreJson.caricaBiblioteca();

    for (Media* media : lista) {
        QListWidgetItem* item = new QListWidgetItem(media->getIcon(), media->getTitolo(), listaMedia);
        item->setData(Qt::UserRole, QVariant::fromValue<Media*>(media));

        QFont font;
        font.setBold(true);
        item->setFont(font);

        listaMedia->setIconSize(QSize(48, 48));
        listaMedia->addItem(item);
    }
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
        }

        QString base64 = QString::fromLatin1(byteArray.toBase64());
        base64Edit->setText(base64);
    });

    formLayout->addRow("Titolo", new QLineEdit("titoloEdit"));
    formLayout->addRow("Prezzo", new QDoubleSpinBox());
    formLayout->addRow("Data", new QDateEdit());
    formLayout->addRow("Genere", new QLineEdit());
    formLayout->addRow("Disponibilita", new QCheckBox());
    formLayout->addRow("Copie", new QSpinBox());

    if (tipo == "Film") creaFormFilm();
    else if (tipo == "Giornale") creaFormGiornale();
    else if (tipo == "Libro") creaFormFilm();
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
    formLayout->addRow("Lingua Originale", new QLineEdit());
    formLayout->addRow("Paese Produzione", new QLineEdit());
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
    formLayout->addRow("Numero Tracce", new QSpinBox());
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
        } else if (labelText == "Disponibilita") {
            if (auto checkBox = qobject_cast<QCheckBox*>(field)) {
                disponibilitaVal = checkBox->isChecked();
            }
        } else if (labelText == "Copie") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                copieVal = spinBox->value();
            }
        } else if (labelText == "Durata") {
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
        } else if (labelText == "Paese produzione") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                paeseProduzioneStr = lineEdit->text();
            }
        }
    }

    Data dataPubblicazione(dataVal.day(), dataVal.month(), dataVal.year());

    return new Film(immagineStr, titoloStr, static_cast<float>(prezzoVal), dataPubblicazione, genereStr, disponibilitaVal, copieVal,
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
        } else if (labelText == "Disponibilita") {
            if (auto checkBox = qobject_cast<QCheckBox*>(field)) {
                disponibilitaVal = checkBox->isChecked();
            }
        } else if (labelText == "Copie") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                copieVal = spinBox->value();
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

    Data dataPubblicazione(dataVal.day(), dataVal.month(), dataVal.year());

    return new Giornale(immagineStr, titoloStr, static_cast<float>(prezzoVal), dataPubblicazione, genereStr, disponibilitaVal, copieVal,
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
        } else if (labelText == "Disponibilita") {
            if (auto checkBox = qobject_cast<QCheckBox*>(field)) {
                disponibilitaVal = checkBox->isChecked();
            }
        } else if (labelText == "Copie") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                copieVal = spinBox->value();
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

    Data dataPubblicazione(dataVal.day(), dataVal.month(), dataVal.year());

    return new Libro(immagineStr, titoloStr, static_cast<float>(prezzoVal), dataPubblicazione, genereStr, disponibilitaVal, copieVal,
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
        } else if (labelText == "Disponibilita") {
            if (auto checkBox = qobject_cast<QCheckBox*>(field)) {
                disponibilitaVal = checkBox->isChecked();
            }
        } else if (labelText == "Copie") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                copieVal = spinBox->value();
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

    Data dataPubblicazione(dataVal.day(), dataVal.month(), dataVal.year());

    return new Rivista(immagineStr, titoloStr, static_cast<float>(prezzoVal), dataPubblicazione, genereStr, disponibilitaVal, copieVal,
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
        } else if (labelText == "Disponibilita") {
            if (auto checkBox = qobject_cast<QCheckBox*>(field)) {
                disponibilitaVal = checkBox->isChecked();
            }
        } else if (labelText == "Copie") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                copieVal = spinBox->value();
            }
        } else if (labelText == "Durata") {
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

    Data dataPubblicazione(dataVal.day(), dataVal.month(), dataVal.year());

    return new Vinile(immagineStr, titoloStr, static_cast<float>(prezzoVal), dataPubblicazione, genereStr, disponibilitaVal, copieVal,
                      durataVal, produzioneStr, artistaStr, tracceVal);
}

//Eliminazione Media
void GestoreMedia::eliminaMedia(int indice) {
    gestoreJson.eliminaMedia(indice);
}


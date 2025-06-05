#include "gestoremedia.h"
#include <QHBoxLayout>
#include <QCheckBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <typeinfo>
#include <media.h>
#include <film.h>
#include <vinile.h>
#include <rivista.h>
#include <giornale.h>
#include <libro.h>

GestoreMedia::GestoreMedia(QListWidget* lista, QString percorso)
    : gestoreJson(percorso), listaMedia(lista) {}

void GestoreMedia::eliminaMedia(int indice) {
    gestoreJson.eliminaMedia(indice);
}

QStringList GestoreMedia::getTipiDisponibili() const {
    return {"Film", "Giornale", "Libro", "Rivista", "Vinile"};
}

void GestoreMedia::salvaMediaDaForm(QString tipo, QFormLayout *layout) {
    Media* media = nullptr;

    if (tipo == "Film") media = creaFilm(layout);
    else if (tipo == "Giornale") media = creaGiornale(layout);
    else if (tipo == "Libro") media = creaLibro(layout);
    else if (tipo == "Rivista") media = creaRivista(layout);
    else if (tipo == "Vinile") media = creaVinile(layout);

    gestoreJson.salvaNuovoMedia(media);
}

Media* GestoreMedia::creaFilm(QFormLayout* layout) {
    QString immagineStr, titoloStr, genereStr, produzioneStr, registaStr, linguaOriginaleStr, paeseProduzioneStr;
    double prezzoVal = 0;
    QDate dataVal;
    bool disponibilitaVal;
    int copieVal = 0, durataVal = 0;

    for (int i = 1; i < layout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(layout->itemAt(i, QFormLayout::LabelRole)->widget());
        QWidget* field = layout->itemAt(i, QFormLayout::FieldRole)->widget();

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

Media* GestoreMedia::creaGiornale(QFormLayout* layout) {
    QString immagineStr, titoloStr, genereStr, autoreStr, editoreStr, testataStr;
    double prezzoVal = 0;
    QDate dataVal;
    bool disponibilitaVal;
    int copieVal = 0;

    for (int i = 1; i < layout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(layout->itemAt(i, QFormLayout::LabelRole)->widget());
        QWidget* field = layout->itemAt(i, QFormLayout::FieldRole)->widget();

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


Media* GestoreMedia::creaLibro(QFormLayout* layout) {
    QString immagineStr, titoloStr, genereStr, autoreStr, editoreStr, formatoStr, linguaStr;
    double prezzoVal = 0;
    QDate dataVal;
    bool disponibilitaVal;
    int copieVal = 0;

    for (int i = 1; i < layout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(layout->itemAt(i, QFormLayout::LabelRole)->widget());
        QWidget* field = layout->itemAt(i, QFormLayout::FieldRole)->widget();

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


Media* GestoreMedia::creaRivista(QFormLayout* layout) {
    QString immagineStr, titoloStr, genereStr, autoreStr, editoreStr, periodicitaStr;
    double prezzoVal = 0;
    QDate dataVal;
    bool disponibilitaVal;
    int copieVal = 0, numeroVal = 0;

    for (int i = 1; i < layout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(layout->itemAt(i, QFormLayout::LabelRole)->widget());
        QWidget* field = layout->itemAt(i, QFormLayout::FieldRole)->widget();

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
Media* GestoreMedia::creaVinile(QFormLayout* layout) {
    QString immagineStr, titoloStr, genereStr, produzioneStr, artistaStr;
    double prezzoVal = 0;
    QDate dataVal;
    bool disponibilitaVal;
    int copieVal = 0, durataVal = 0, tracceVal = 0;

    for (int i = 1; i < layout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(layout->itemAt(i, QFormLayout::LabelRole)->widget());
        QWidget* field = layout->itemAt(i, QFormLayout::FieldRole)->widget();

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


void GestoreMedia::caricaBiblioteca() {

    listaMedia->clear();
    QList<Media*> lista = gestoreJson.caricaBiblioteca();

    for (Media* media : lista) {
        QListWidgetItem* item = new QListWidgetItem(media->getIcon(), media->getTitolo(), listaMedia);

        item->setData(Qt::UserRole + 1, media->getImmagine());
        item->setData(Qt::UserRole + 2, media->getTitolo());
        item->setData(Qt::UserRole + 3, media->getPrezzo());
        item->setData(Qt::UserRole + 4, media->getData());
        item->setData(Qt::UserRole + 5, media->getGenere());
        item->setData(Qt::UserRole + 6, media->getDisponibilita());
        item->setData(Qt::UserRole + 7, media->getCopie());


        if (Film* film = dynamic_cast<Film*>(media)) {
            item->setData(Qt::UserRole, "Film");
            item->setData(Qt::UserRole + 8, film->getDurata());
            item->setData(Qt::UserRole + 9, film->getProduzione());
            item->setData(Qt::UserRole + 12, film->getRegista());
            item->setData(Qt::UserRole + 13, film->getLinguaOriginale());
            item->setData(Qt::UserRole + 14, film->getPaeseProduzione());
        }
        else if (Giornale* giornale = dynamic_cast<Giornale*>(media)) {
            item->setData(Qt::UserRole, "Giornale");
            item->setData(Qt::UserRole + 10, giornale->getAutore());
            item->setData(Qt::UserRole + 11, giornale->getEditore());
            item->setData(Qt::UserRole + 15, giornale->getTestata());
        }
        else if (Libro* libro = dynamic_cast<Libro*>(media)) {
            item->setData(Qt::UserRole, "Libro");
            item->setData(Qt::UserRole + 10, libro->getAutore());
            item->setData(Qt::UserRole + 11, libro->getEditore());
            item->setData(Qt::UserRole + 16, libro->getFormato());
            item->setData(Qt::UserRole + 17, libro->getLingua());
        }
        else if (Rivista* rivista = dynamic_cast<Rivista*>(media)) {
            item->setData(Qt::UserRole, "Rivista");
            item->setData(Qt::UserRole + 10, rivista->getAutore());
            item->setData(Qt::UserRole + 11, rivista->getEditore());
            item->setData(Qt::UserRole + 18, rivista->getNumero());
            item->setData(Qt::UserRole + 19, rivista->getPeriodicita());
        }
        else if (Vinile* vinile = dynamic_cast<Vinile*>(media)) {
            item->setData(Qt::UserRole, "Vinile");
            item->setData(Qt::UserRole + 8, vinile->getDurata());
            item->setData(Qt::UserRole + 9, vinile->getProduzione());
            item->setData(Qt::UserRole + 20, vinile->getArtista());
            item->setData(Qt::UserRole + 21, vinile->getNumeroTracce());
        }

        // ---------- Stile ----------
        QFont font;
        font.setBold(true);
        item->setFont(font);

        listaMedia->setIconSize(QSize(48, 48));
        listaMedia->addItem(item);
    }
}


void GestoreMedia::creaForm(const QString& tipo, QFormLayout* layout) const {
    if (tipo == "Film") Film::creaForm(layout);
    else if (tipo == "Giornale") Giornale::creaForm(layout);
    else if (tipo == "Libro") Libro::creaForm(layout);
    else if (tipo == "Rivista") Rivista::creaForm(layout);
    else if (tipo == "Vinile") Vinile::creaForm(layout);
}

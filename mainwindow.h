#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

class QPushButton;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;

#include "libro.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QWidget *centralWidget;

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonBarLayout;
    QHBoxLayout *centralBodyLayout;
    QVBoxLayout *leftPanelLayout;

    QPushButton *button1;
    QPushButton *button2;

    QLabel* dettagliLibro;

    QWidget *leftPanel;
    QLabel *rightPanel;

private slots:
    void onItemClicked(QListWidgetItem *item);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H

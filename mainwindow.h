#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QWidget *centralWidget;

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonBarLayout;
    QHBoxLayout *centralBodyLayout;

    QPushButton *button1;
    QPushButton *button2;

    QLabel *leftPanel;
    QLabel *rightPanel;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H

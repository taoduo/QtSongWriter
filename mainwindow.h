#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#ifndef CMIDIPACKET32_H_
#include "CMidiPacket43.h"
#endif
#include <QTimer>
#include "RtMidi.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void txTimerAction();
    void on_pushButton_play_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QDialog>

namespace Ui {
class DisplayWindow;
}

class DisplayWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DisplayWindow(QWidget *parent = 0);
    ~DisplayWindow();

private slots:

    void on_pushButton_close_display_clicked();

private:
    Ui::DisplayWindow *ui;
};

#endif // DISPLAYWINDOW_H

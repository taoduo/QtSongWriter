#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QDialog>
#include "CMidiPacket43.h"
namespace Ui {
class DisplayWindow;
}

class DisplayWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DisplayWindow(QWidget *parent = 0);
    ~DisplayWindow();
    void set_display_text(std::vector<CMidiPacket43> mp_vector);
    void set_channels(int chan[]);

private slots:
    void on_pushButton_close_display_clicked();

private:
    Ui::DisplayWindow *ui;
};

#endif // DISPLAYWINDOW_H

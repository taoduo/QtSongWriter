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

    void on_horizontalSlider_tempo_valueChanged(int value);

    void on_horizontalSlider_1_vol_valueChanged(int value);

    void on_horizontalSlider_2_vol_valueChanged(int value);

    void on_horizontalSlider_3_vol_valueChanged(int value);

    void on_horizontalSlider_4_vol_valueChanged(int value);

    void on_horizontalSlider_5_vol_valueChanged(int value);

    void on_pushButton_Enter_clicked();

    void setup_comboBox_category();

    void setup_instru_name_list();
    void on_comboBox_1_cat_currentIndexChanged(int index);

    void on_comboBox_2_cat_currentIndexChanged(int index);

    void on_comboBox_3_cat_currentIndexChanged(int index);

    void on_comboBox_4_cat_currentIndexChanged(int index);

    void on_comboBox_5_cat_currentIndexChanged(int index);

    void on_comboBox_1_instru_currentIndexChanged(const QString &arg1);

    void on_comboBox_2_instru_currentIndexChanged(const QString &arg1);

    void on_comboBox_3_instru_currentIndexChanged(const QString &arg1);

    void on_comboBox_4_instru_currentIndexChanged(const QString &arg1);

    void on_comboBox_5_instru_currentIndexChanged(const QString &arg1);

    void on_comboBox_input_track_currentIndexChanged(int index);

    void on_clear_button_clicked();

private:
    Ui::MainWindow *ui;
    void init_all_chan_combo_box();
    void setup_trk_combox_box();
};

#endif // MAINWINDOW_H

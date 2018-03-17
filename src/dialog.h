#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "lanedetection.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:

    void processFrameAndUpdateGUI();

    void on_spinBox_valueChanged(int arg1);

    void on_kernelSlider_valueChanged(int value);

    void on_valueHighTHSlider_valueChanged(int value);

    void on_valueLowTHSlider_valueChanged(int value);

    void on_cannyHighTHSlider_valueChanged(int value);

    void on_cannyLowTHSlider_valueChanged(int value);

    void on_rhoSlider_valueChanged(int value);

    void on_thetaSlider_valueChanged(int value);

    void on_THSlider_valueChanged(int value);

    void on_minLenSlider_valueChanged(int value);

    void on_maxGapSlider_valueChanged(int value);

private:
    Ui::Dialog *ui;

    LaneDetection laneDetection;

    QTimer *timer;
};

#endif // DIALOG_H

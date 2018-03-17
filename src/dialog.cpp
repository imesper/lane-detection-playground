#include "dialog.h"
#include "ui_dialog.h"

#include <QDesktopWidget>
#include <QDebug>
#include <QPicture>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    laneDetection("test.mp4")
{
    ui->setupUi(this);
    move(0, 0);

    this->ui->kernelSlider->setValue(this->laneDetection.gausKernel().height);
    this->ui->valueHighTHSlider->setValue(this->laneDetection.vmax());
    this->ui->valueLowTHSlider->setValue(this->laneDetection.vmin());
    this->ui->rhoSlider->setValue(static_cast<int>(this->laneDetection.rho()));
    this->ui->thetaSlider->setValue(static_cast<int>((CV_PI / this->laneDetection.theta())));
    this->ui->minLenSlider->setValue(static_cast<int>(this->laneDetection.minLineLength()));
    this->ui->maxGapSlider->setValue(static_cast<int>(this->laneDetection.maxLineGap()));
    this->ui->cannyLowTHSlider->setValue(this->laneDetection.cannyLow());
    this->ui->cannyHighTHSlider->setValue(this->laneDetection.cannyHigh());
    this->ui->THSlider->setValue(this->laneDetection.threshold());

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));
    timer->start(60);
}

Dialog::~Dialog()
{
    delete ui;
}

//////////////////////////////////////////////////////////////
void Dialog::processFrameAndUpdateGUI(){

    std::vector<cv::Mat> images;

    laneDetection.processFrameAndUpdateGUI(images);

    cv::Mat small = images[0];
    QImage original =  QImage(images[0].data, images[0].cols, images[0].rows, static_cast<int>(images[0].step), QImage::Format_RGB888);
    QImage image1 = QImage(images[1].data, images[1].cols, images[1].rows, static_cast<int>(images[1].step), QImage::Format_RGB888);
    QImage image2 = QImage(images[2].data, images[2].cols, images[2].rows, static_cast<int>(images[2].step), QImage::Format_RGB888);
    QImage edges1 = QImage(images[3].data, images[3].cols, images[3].rows, static_cast<int>(images[3].step), QImage::Format_Indexed8);
    QImage edges2 = QImage(images[4].data, images[4].cols, images[4].rows, static_cast<int>(images[4].step), QImage::Format_Indexed8);
    QImage image3 = QImage(images[5].data, images[5].cols, images[5].rows, static_cast<int>(images[5].step), QImage::Format_RGB888);
    QImage mixed = QImage(images[6].data, images[6].cols, images[6].rows, static_cast<int>(images[6].step), QImage::Format_Indexed8);

    ui->label->setPixmap(QPixmap::fromImage(image1));
//    ui->label_2->setPixmap(QPixmap::fromImage(image2));
    ui->label_3->setPixmap(QPixmap::fromImage(edges1));
//    ui->label_4->setPixmap(QPixmap::fromImage(edges2));
    ui->label_2->setPixmap(QPixmap::fromImage(image3));
    ui->label_4->setPixmap(QPixmap::fromImage(mixed));

}

////////////////////////////////////////////////////////////

void Dialog::on_spinBox_valueChanged(int arg1)
{
    if(arg1 % 2){
        this->ui->kernelSlider->setValue(arg1);
    } else{
        this->ui->kernelSlider->setValue(arg1+1);
    }
}


void Dialog::on_kernelSlider_valueChanged(int value)
{
    cv::Size newSize(value, value);
    if(value % 2){
        this->laneDetection.setGausKernel(newSize);
        this->ui->spinBox->setValue(value);
    }
}

void Dialog::on_valueHighTHSlider_valueChanged(int value)
{
    this->laneDetection.setVmax(value);
    this->ui->lcdNumber->display(value);
}

void Dialog::on_valueLowTHSlider_valueChanged(int value)
{
    this->laneDetection.setVmin(value);
    this->ui->lcdNumber_2->display(value);
}

void Dialog::on_cannyHighTHSlider_valueChanged(int value)
{
    this->laneDetection.setCannyHigh(value);
    this->ui->lcdNumber_3->display(value);
}

void Dialog::on_cannyLowTHSlider_valueChanged(int value)
{
    this->laneDetection.setCannyLow(value);
    this->ui->lcdNumber_4->display(value);
}

void Dialog::on_rhoSlider_valueChanged(int value)
{
    this->laneDetection.setRho(static_cast<double>(value));
    this->ui->lcdNumber_5->display(this->laneDetection.rho());
}

void Dialog::on_thetaSlider_valueChanged(int value)
{
    qDebug() << value;
    this->laneDetection.setTheta(CV_PI/value);
    qDebug() << this->laneDetection.theta();
    this->ui->lcdNumber_6->display(value);
}

void Dialog::on_THSlider_valueChanged(int value)
{
    this->laneDetection.setThreshold(value);
    this->ui->lcdNumber_7->display(this->laneDetection.threshold());

}

void Dialog::on_minLenSlider_valueChanged(int value)
{
    this->laneDetection.setMinLineLength(static_cast<double>(value));
    this->ui->lcdNumber_8->display(this->laneDetection.minLineLength());
}

void Dialog::on_maxGapSlider_valueChanged(int value)
{
    qDebug() << value;
    this->laneDetection.setMaxLineGap(static_cast<double>(value));
    this->ui->lcdNumber_9->display(this->laneDetection.maxLineGap());
}

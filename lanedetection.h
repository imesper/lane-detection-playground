#ifndef LANEDETECTION_H
#define LANEDETECTION_H

#include <string>
#include <QTimer>
#include <QImage>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class LaneDetection : public QObject
{
    QString _filePath;
    QTimer _frameTimer;
    cv::VideoCapture _capWebCam;
    cv::Mat _matOriginal;
    cv::Mat _matHSV;
    cv::Mat _matHSL;
    cv::Mat _matGreyGaussian;
    cv::Mat _matS;
    cv::Mat _matV;
    cv::Mat _matH;
    cv::Mat _matGrey;
    cv::Mat _matEdges;
    cv::Point _center;
    cv::Point _a;
    cv::Point _b;
    cv::VideoWriter _outputCap;

    QImage imageOriginal;
    QImage imageProcessed;
    QImage imageHSV;
    QImage imageEdges;

    std::vector<cv::Vec3f> vecCircles;
    std::vector<cv::Vec3f>::iterator itrCircles;

    QTimer * _timer;
    cv::Size _gausKernel;

    int _vmin;
    int _vmax;
    int _hmin;
    int _hmax;
    int _smin;
    int _smax;
    int _cannyHigh;
    int _cannyLow;
    double _rho; // distance resolution in pixels of the Hough grid
    double _theta;  // angular resolution in radians of the Hough grid
    int _threshold;
    double _minLineLength; // minimum number of pixels making up a line
    double _maxLineGap;  //  maximum gap in pixels between connectable line segments

public:

    LaneDetection() = delete;

    LaneDetection(std::string filePath);

    ~LaneDetection();

    int vmin() const;
    void setVmin(int vmin);

    int vmax() const;
    void setVmax(int vmax);

    int cannyHigh() const;
    void setCannyHigh(int cannyHigh);

    int cannyLow() const;
    void setCannyLow(int cannyLow);

    double rho() const;
    void setRho(double rho);

    double theta() const;
    void setTheta(double theta);

    int threshold() const;
    void setThreshold(int threshold);

    double minLineLength() const;
    void setMinLineLength(double minLineLength);

    double maxLineGap() const;
    void setMaxLineGap(double maxLineGap);

    int hmin() const;
    void setHmin(int hmin);

    int hmax() const;
    void setHmax(int hmax);

    int smin() const;
    void setSmin(int smin);

    int smax() const;
    void setSmax(int smax);

    cv::Mat polygonMask(cv::Mat original);
    cv::Mat whiteMask(cv::Mat original);
    cv::Mat yellowMask(cv::Mat original);

    cv::Mat exctractCannyEdges(cv::Mat original);
    cv::Mat extractLines(cv::Mat original);

    cv::Mat filteredValue(std::vector<cv::Mat> hsv);
    cv::Mat filteredHue(std::vector<cv::Mat> hsv);
    cv::Mat filteredSaturation(std::vector<cv::Mat> hsv);
    std::vector<cv::Mat> splitHSV(cv::Mat original);

    cv::Size gausKernel() const;
    void setGausKernel(const cv::Size &gausKernel);

    void processFrameAndUpdateGUI(std::vector<cv::Mat> &);


};

#endif // LANEDETECTION_H

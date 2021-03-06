#include "lanedetection.h"
#include <QDebug>

cv::Size LaneDetection::gausKernel() const
{
    return _gausKernel;
}

void LaneDetection::setGausKernel(const cv::Size &gausKernel)
{
    _gausKernel = gausKernel;
}

LaneDetection::LaneDetection(std::string filePath) :
    _gausKernel(3,3),
    _vmin(240),
    _vmax(286),
    _cannyHigh(150),
    _cannyLow(50),
    _rho(1),
    _theta(CV_PI/180),
    _threshold(15),
    _minLineLength(40),
    _maxLineGap(1)
{
    this->_capWebCam.open(filePath);
}

LaneDetection::~LaneDetection()
{
    this->_capWebCam.release();
}

int LaneDetection::vmax() const
{
    return _vmax;
}

void LaneDetection::setVmax(int vmax)
{
    _vmax = vmax;
}

int LaneDetection::cannyHigh() const
{
    return _cannyHigh;
}

void LaneDetection::setCannyHigh(int cannyHigh)
{
    _cannyHigh = cannyHigh;
}

int LaneDetection::cannyLow() const
{
    return _cannyLow;
}

void LaneDetection::setCannyLow(int cannyLow)
{
    _cannyLow = cannyLow;
}

double LaneDetection::rho() const
{
    return _rho;
}

void LaneDetection::setRho(double rho)
{
    _rho = rho;
}

double LaneDetection::theta() const
{
    return _theta;
}

void LaneDetection::setTheta(double theta)
{
    _theta = theta;
}

int LaneDetection::threshold() const
{
    return _threshold;
}

void LaneDetection::setThreshold(int threshold)
{
    _threshold = threshold;
}

double LaneDetection::minLineLength() const
{
    return _minLineLength;
}

void LaneDetection::setMinLineLength(double minLineLength)
{
    _minLineLength = minLineLength;
}

double LaneDetection::maxLineGap() const
{
    return _maxLineGap;
}

void LaneDetection::setMaxLineGap(double maxLineGap)
{
    _maxLineGap = maxLineGap;
}

cv::Mat LaneDetection::polygonMask(cv::Mat original)
{
    // Frame Size
    int width = original.cols;
    int height = original.rows;


    // Defining Points to select the region of interest

    std::vector<std::vector<cv::Point>> pts;
    std::vector<cv::Point> tmp;

    tmp.push_back(cv::Point(0, 0));
    tmp.push_back(cv::Point(0, height));
    tmp.push_back(cv::Point((width*5/100), height));
    tmp.push_back(cv::Point((width*48/100),height*57/100));
    tmp.push_back(cv::Point((width*52/100),height*57/100));
    tmp.push_back(cv::Point((width*95/100), height));
    tmp.push_back(cv::Point(width,height));
    tmp.push_back(cv::Point(width,0));

    pts.push_back(tmp);

    cv::fillPoly(original, pts, cv::Scalar(0,0,0), 8);

    return original;
}

cv::Mat LaneDetection::whiteMask(cv::Mat original)
{

    cv::Mat mask;
    // specify the range of colours that you want to include, you can play with the borders here
    cv::Scalar lowerb = cv::Scalar(200,200,200);
    cv::Scalar upperb = cv::Scalar(255,255,255);

    cv::inRange(original, lowerb, upperb, mask);

    return mask;
}

cv::Mat LaneDetection::yellowMask(cv::Mat original)
{

    cv::Mat mask;
    cv::Mat HSL;

    // specify the range of colours that you want to include, you can play with the borders here
    cv::Scalar lowery = cv::Scalar(225,180,0);
    cv::Scalar uppery = cv::Scalar(255,255,170);

    cv::inRange(original, lowery, uppery, mask);

    cv::cvtColor(original, HSL, cv::COLOR_RGB2HLS);

    cv::Mat maskHLS;
    cv::Mat hls_y;

    cv::Scalar lowerHLS = cv::Scalar(20,120,80);
    cv::Scalar upperHLS = cv::Scalar(45,200,255);

    cv::inRange(HSL, lowerHLS, upperHLS, maskHLS);

    cv::add(mask, maskHLS, mask);

    return mask;
}

cv::Mat LaneDetection::exctractCannyEdges(cv::Mat original)
{
    cv::Mat edges;
    qDebug() << this->_cannyHigh << " : " << this->_cannyLow;
    cv::Canny(original, edges, this->_cannyLow, this->_cannyHigh);

    return edges;
}

cv::Mat LaneDetection::extractLines(cv::Mat original)
{
    // Frame Size
    int width = original.cols;
    int height = original.rows;

    std::vector<cv::Vec4i> lines;

    cv::HoughLinesP(original, lines, this->_rho, this->_theta, this->_threshold, this->_minLineLength, this->_maxLineGap );

    cv::Mat linesImage(height, width, CV_8UC3, cv::Scalar(0,0,0));

    qDebug() << lines.size() << " " << _rho << " " << _theta << " " << _threshold;

    for( size_t i = 0; i < lines.size(); i++ )
    {
        cv::Vec4i l = lines[i];

        cv::line(linesImage, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 8, CV_AA);
    }
    return linesImage;
    //    double X1 = 0;
    //    double X2 = 0;
    //    double Y1 = 0;
    //    double Y2 = 0;

    //    for( size_t i = 0; i < lines.size(); i++ )
    //    {
    //        cv::Vec4i l = lines[i];
    //        // Left Lane
    //        X1 = l[0];
    //        X2 = l[2];
    //        Y1 = l[1];
    //        Y2 = l[3];

    //        qDebug() << "X1: " << X1;
    //        qDebug() << "X2: "<< X2;
    //        qDebug() << "Y1: " << Y1;
    //        qDebug() << "Y2: "<< Y2;

    //        double slope = (Y1 - Y2) / (X1 - X2);

    //        double b = Y1 - (slope * X1);
    //        qDebug() << "b: " << b;
    //        qDebug() << "Slope: " << slope;

    //        if(slope == 1. || slope == 0. ){
    //            continue;
    //        }

    ////        cv::line(matOriginal, cv::Point(X1, Y1), cv::Point(X2, Y2), cv::Scalar(0,0,255), 8, CV_AA);

    //        if(slope < 0){
    //            // For x = 0 point1 = b
    //            // For x = width/2
    //            double point2 = slope * width*45/100 + b;
    //            qDebug() << "Point2: " << point2;

    //            cv::line(original, cv::Point(0, b), cv::Point(width*45/100, point2), cv::Scalar(0,0,255), 8, CV_AA);

    //        } else {
    //            // For x = width
    //            double point1 = slope * width + b;
    //            // For x = width/2
    //            double point2 = slope * width*60/100 + b;
    //            cv::line( original, cv::Point(width*60/100, point2), cv::Point(width, point1), cv::Scalar(0,0,255), 8, CV_AA);
    //        }
    //    }
}

cv::Mat LaneDetection::filteredValue(std::vector<cv::Mat>  hsv)
{
    cv::Mat matV;
    cv::inRange(hsv[2], this->_vmin, this->_vmax, matV);
    return matV;
}
cv::Mat LaneDetection::filteredHue(std::vector<cv::Mat>  hsv)
{
    cv::Mat matH;
    cv::inRange(hsv[0], this->_vmin, this->_vmax, matH);
    return matH;
}
cv::Mat LaneDetection::filteredSaturation(std::vector<cv::Mat>  hsv)
{
    cv::Mat matV;
    cv::inRange(hsv[2], this->_vmin, this->_vmax, matV);
    return matV;
}
std::vector<cv::Mat> LaneDetection::splitHSV(cv::Mat original)
{
    cv::Mat matHSV;
    // Convert to HSV
    cv::cvtColor(original, matHSV, cv::COLOR_RGB2HSV);

    std::vector<cv::Mat> hsv;

    cv::split(matHSV, hsv);

    return hsv;
}

void LaneDetection::processFrameAndUpdateGUI(std::vector<cv::Mat> &images)
{
    // Get the frame of the video to process
    this->_capWebCam >> this->_matOriginal;

    // Start Over - Playing in Loop
    if(this->_matOriginal.empty()) {
        this->_capWebCam.set(cv::CAP_PROP_POS_FRAMES, 0.0);
        this->_capWebCam >> this->_matOriginal;
    }

    // Convert to RGB
    cv::cvtColor(this->_matOriginal, this->_matOriginal,CV_BGR2RGB);


    // Convert to HSV
    //    std::vector<cv::Mat> HSV =  this->splitHSV(this->_matOriginal);
    //    cv::Mat matValue =  this->filteredValue(HSV);
    //    cv::Mat matHue =  this->filteredHue(HSV);


    cv::cvtColor(this->_matOriginal, this->_matGrey, cv::COLOR_RGB2GRAY);

    cv::GaussianBlur(this->_matGrey, this->_matGreyGaussian, this->_gausKernel, 0);

    cv::Mat matWhiteMask =  this->whiteMask(this->_matOriginal);
    cv::Mat matYellowMask =  this->yellowMask(this->_matOriginal);

    cv::Mat mixedMask;

    cv::add(matWhiteMask, matYellowMask, mixedMask);

    //    cv::threshold(hls_y, hls_y, 128.0,255.0, cv::THRESH_BINARY);

    //    cv::add(mixedMask, hls_y, mixedMask);


    //    this->_matEdges = mixedMask;
    this->_matEdges = this->exctractCannyEdges(mixedMask);

    cv::Mat matEdges = this->exctractCannyEdges(this->_matGreyGaussian);

    this->_matEdges = this->polygonMask(this->_matEdges);

    matEdges = this->polygonMask(matEdges);

    cv::Mat maskRegion = this->polygonMask(mixedMask);

    cv::Mat lineImage = this->extractLines(this->_matEdges);

    cv::Mat lineImage2 = this->extractLines(matEdges);

    cv::Mat lineImage3 = this->extractLines(maskRegion);

    cv::Mat image1;
    cv::Mat image2;
    cv::Mat image3;

    cv::addWeighted(lineImage2, 1, this->_matOriginal, 0.4, 1, image1);
    cv::addWeighted(lineImage, 1, this->_matOriginal, 0.4, 1, image2);
    cv::addWeighted(lineImage3, 1, this->_matOriginal, 0.4, 1, image3);

    int width = this->_matOriginal.cols/2;
    int height = this->_matOriginal.rows/2;
    cv::Mat small;
    cv::Mat image1Small;
    cv::Mat image2Small;
    cv::Mat image3Small;
    cv::Mat edges1Small;
    cv::Mat edges2Small;
    cv::Mat edges3Small;
    cv::Mat mixedSmall;

    cv::resize(this->_matOriginal, small, cv::Size(width, height));
    cv::resize(image1, image1Small, cv::Size(width, height));
    cv::resize(image2, image2Small, cv::Size(width, height));
    cv::resize(image3, image3Small, cv::Size(width, height));
    cv::resize(matEdges, edges1Small, cv::Size(width, height));
    cv::resize(this->_matEdges, edges2Small, cv::Size(width, height));
    cv::resize(maskRegion, mixedSmall, cv::Size(width, height));

    //    cv::imshow("Large", matEdges);
    //    cv::imshow("Small", edges1Small);
    //    cv::waitKey();

    images.push_back(small);
    images.push_back(image1Small);
    images.push_back(image2Small);
    images.push_back(edges1Small);
    images.push_back(edges2Small);
    images.push_back(image3Small);
    images.push_back(mixedSmall);
}


int LaneDetection::vmin() const
{
    return _vmin;
}

void LaneDetection::setVmin(int vmin)
{
    _vmin = vmin;
}

int LaneDetection::hmin() const
{
    return _hmin;
}

void LaneDetection::setHmin(int hmin)
{
    _hmin = hmin;
}

int LaneDetection::hmax() const
{
    return _hmax;
}

void LaneDetection::setHmax(int hmax)
{
    _hmax = hmax;
}

int LaneDetection::smin() const
{
    return _smin;
}

void LaneDetection::setSmin(int smin)
{
    _smin = smin;
}

int LaneDetection::smax() const
{
    return _smax;
}

void LaneDetection::setSmax(int smax)
{
    _smax = smax;
}

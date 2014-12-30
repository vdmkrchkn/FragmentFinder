#include "worker-thread.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

namespace
{
    //
    QImage Mat2QImage(Mat const& src)
    {
         cv::Mat temp; // make the same cv::Mat
         cvtColor(src, temp, CV_BGR2RGB);
         QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
         dest.bits(); // enforce deep copy, see documentation
         return dest;
    }
    //
    Mat QImage2Mat(QImage const& src)
    {
         QImage srcFmt(src.convertToFormat(QImage::Format_RGB888));
         Mat tmp(srcFmt.height(),srcFmt.width(),CV_8UC3,(uchar*)srcFmt.bits(),srcFmt.bytesPerLine());
         Mat result;
         cvtColor(tmp, result, CV_BGR2RGB);
         return result;
    }
}

const QImage *WorkerThread::getResultImage() const
{
    return m_ptrResult.data();
}

void WorkerThread::startImageMatching(const QImage &rcImageInput, const QImage &rcImagePattern)
{
    if (isRunning())
        return;
    //    
    m_ptrInput.reset(new QImage(rcImageInput));
    m_ptrPattern.reset(new QImage(rcImagePattern));
    QThread::start();
}

void WorkerThread::stop()
{
    requestInterruption();
}

void WorkerThread::run()
{
    if (m_ptrInput && m_ptrPattern)
    {
        Mat image = QImage2Mat(*m_ptrInput);
        Mat templ = QImage2Mat(*m_ptrPattern);
        // Create the result matrix
        Mat result;
        int result_rows = image.rows - templ.rows + 1;
        int result_cols = image.cols - templ.cols + 1;
        result.create(result_rows, result_cols, CV_32FC1);
        // matches
        int match_method = CV_TM_SQDIFF;
        matchTemplate(image,templ,result,match_method);
        // normalize
        normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
        // localize the minimum and maximum values in the result matrix
        double minVal, maxVal;
        Point minLoc, maxLoc, matchLoc;
        minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
        if(match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
            matchLoc = minLoc; // для таких методов нужен min
        else
            matchLoc = maxLoc; // для остальных max
        // выделение границы шаблона
        rectangle(image, matchLoc, Point(matchLoc.x + templ.cols , matchLoc.y + templ.rows), Scalar::all(0));
        m_ptrResult.reset(new QImage(Mat2QImage(image)));
    }
}

#ifndef QT_CV_H
#define QT_CV_H

/**************************************************************************

此处代码引用来源：https://blog.csdn.net/qq_33485434/article/details/81667004

****************************************************************************/

#include <QDebug>
#include <QImage>
#include <QPixmap>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
namespace QT_CV
{
    QImage cvMatToQImage(const cv::Mat& inMat);
    QPixmap cvMatToQPixmap(const cv::Mat& inMat);
    cv::Mat QImageToCvMat(const QImage& inImage, bool inCloneImageData = true);
    cv::Mat QPixmapToCvMat(const QPixmap& inPixmap, bool inCloneImageData = true);

    void CreateAlphaMat(cv::Mat& mat);
}

#endif

#include"QT_CV.h"

namespace QT_CV
{

    // 将Mat转化位QImage
    QImage cvMatToQImage(const cv::Mat& inMat)
    {
        switch (inMat.type())
        {
            // 8-bit, 4 channel
        case CV_8UC4:
        {
            QImage image(inMat.data,inMat.cols, inMat.rows,static_cast<int>(inMat.step),QImage::Format_ARGB32);

            return image;
        }

        // 8-bit, 3 channel
        case CV_8UC3:
        {
            QImage image(inMat.data,inMat.cols, inMat.rows,static_cast<int>(inMat.step),QImage::Format_RGB888);

            return image.rgbSwapped();
        }

        // 8-bit, 1 channel
        case CV_8UC1:
        {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
            QImage image(inMat.data, inMat.cols, inMat.rows,static_cast<int>(inMat.step),QImage::Format_Grayscale8);//Format_Alpha8 and Format_Grayscale8 were added in Qt 5.5
#else//这里还有一种写法，最后给出
            static QVector<QRgb>  sColorTable;

            // only create our color table the first time
            if (sColorTable.isEmpty())
            {
                sColorTable.resize(256);

                for (int i = 0; i < 256; ++i)
                {
                    sColorTable[i] = qRgb(i, i, i);
                }
            }

            QImage image(inMat.data, inMat.cols, inMat.rows,static_cast<int>(inMat.step),QImage::Format_Indexed8);

            image.setColorTable(sColorTable);
#endif

            return image;
        }

        default:
            qWarning() << "CVS::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
        }

        return QImage();
    }

    //将Mat转化为QPixmap
    QPixmap cvMatToQPixmap(const cv::Mat& inMat)
    {
        return QPixmap::fromImage(cvMatToQImage(inMat));
    }
    //将QImage转化为Mat
    cv::Mat QImageToCvMat(const QImage& inImage, bool inCloneImageData)
    {
        switch (inImage.format())
        {
            // 8-bit, 4 channel
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        {
            cv::Mat  mat(inImage.height(), inImage.width(), CV_8UC4,const_cast<uchar*>(inImage.bits()), static_cast<size_t>(inImage.bytesPerLine()));

            return (inCloneImageData ? mat.clone() : mat);
        }

        // 8-bit, 3 channel
        case QImage::Format_RGB32:
        case QImage::Format_RGB888:
        {
            if (!inCloneImageData)
            {
                qWarning() << "CVS::QImageToCvMat() - Conversion requires cloning because we use a temporary QImage";
            }

            QImage   swapped = inImage;

            if (inImage.format() == QImage::Format_RGB32)
            {
                swapped = swapped.convertToFormat(QImage::Format_RGB888);
            }

            swapped = swapped.rgbSwapped();

            return cv::Mat(swapped.height(), swapped.width(),
                CV_8UC3,
                const_cast<uchar*>(swapped.bits()),
                static_cast<size_t>(swapped.bytesPerLine())
            ).clone();
        }

        // 8-bit, 1 channel
        case QImage::Format_Indexed8:
        {
            cv::Mat  mat(inImage.height(), inImage.width(),
                CV_8UC1,
                const_cast<uchar*>(inImage.bits()),
                static_cast<size_t>(inImage.bytesPerLine())
            );

            return (inCloneImageData ? mat.clone() : mat);
        }

        default:
            qWarning() << "CVS::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
            break;
        }

        return cv::Mat();
    }

    //将QPixmap转化为Mat
    cv::Mat QPixmapToCvMat(const QPixmap& inPixmap, bool inCloneImageData)
    {
        return QImageToCvMat(inPixmap.toImage(), inCloneImageData);
    }

    void CreateAlphaMat(cv::Mat& mat) 
    {
        for (int i = 0; i < mat.rows; i++)
        {
            for (int j = 0; j < mat.cols; j++)
            {
                cv::Vec4b& rgba = mat.at<cv::Vec4b>(i, j);//get someone pixel information
                rgba[0] = UCHAR_MAX;//255
                rgba[1] = cv::saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX);//resemble ((n-i)/n)*255
                rgba[2] = cv::saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX);//resemble ((n-i)/n)*255
                rgba[3] = cv::saturate_cast<uchar>(0.5 * (rgba[1] + rgba[2]));//  255*255/2 ~ 0

            }
        }
    }


}
#ifndef CVIMAGEWIDGET_H
#define CVIMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>
#include <qdebug.h>
#include <qevent.h>

using namespace cv;

class CVImageWidget : public QWidget
{

public:

	explicit CVImageWidget(QWidget *parent = 0)
		: QWidget(parent)
	{

	}


	QSize sizeHint() const
	{
		return _qimage.size();
	}

	QSize minimumSizeHint() const
	{
		return _qimage.size();
	}


	void showImage()
	{
		if (!_qimage.isNull())
			this->repaint();
	}

	cv::Mat& getCVImage()
	{
		return _cvmat;
	}

	void setCVImage(const cv::Mat &image)
	{
		_cvmat = image;
		_qimage = QImage(_cvmat.data, _cvmat.cols, _cvmat.rows, _cvmat.step, QImage::Format_RGB888);
	}

	void setImage(QImage image)
	{
		_qimage = image;
	}

	QImage& getImage()
	{
		this->repaint();
		return _qimage;
	}

	~CVImageWidget()
	{

	}


	public slots:

	void showImage(const Mat& image)
	{
		//qDebug() << image.type();
		switch (image.type())
		{
		case CV_8UC1:
			cvtColor(image, _cvmat, CV_GRAY2RGB);
			break;
		case CV_8UC3:
			cvtColor(image, _cvmat, CV_BGR2RGB);
			break;
		case CV_8UC4:
			cvtColor(image, _cvmat, CV_BGRA2RGB);
			break;
		default:
			qDebug() << "unsupported format " << image.type();
			break;
		}

		assert(_cvmat.isContinuous());
		_qimage = QImage(_cvmat.data, _cvmat.cols, _cvmat.rows, _cvmat.step, QImage::Format_RGB888);
		this->setMinimumSize(image.cols, image.rows);

		this->repaint();
	}

protected:

	void paintEvent(QPaintEvent *)
	{
		QPainter painter(this);
		painter.drawImage(QPoint(0, 0), _qimage);
		painter.end();
	}

private:
	QImage _qimage;
	Mat _cvmat;
	Mat _cvmask;
	QImage _grid;


};

#endif //CVIMAGEWIDGET_H
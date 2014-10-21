#ifndef KINECTGUIAIUX_H
#define KINECTGUIAIUX_H

#include <QtCore>
#include <qmainwindow.h>
#include "CVImageWidget.h"
#include "ui_kinectgui.h"


QT_BEGIN_NAMESPACE

class Ui_KinectGuiAuxClass
{

public:
	CVImageWidget* rgbDataImage;
	CVImageWidget* depthDataImage;
	CVImageWidget* bodyMaskImage;
	CVImageWidget* skeletonImage;
	CVImageWidget* audioImage;

	void setupUi(QMainWindow *main, Ui::KinectGUIClass ui)
	{
		rgbDataImage = new CVImageWidget(ui.RGBframe);
		rgbDataImage->setObjectName(QStringLiteral("rgbDataImage"));
		rgbDataImage->setGeometry(QRect(1, 1, ui.RGBframe->geometry().width() - 1, ui.RGBframe->geometry().height() - 1));
		rgbDataImage->setEnabled(false);

		depthDataImage = new CVImageWidget(ui.depthFrame);
		depthDataImage->setObjectName(QStringLiteral("depthDataImage"));
		depthDataImage->setGeometry(QRect(1, 1, ui.depthFrame->geometry().width() - 1, ui.depthFrame->geometry().height() - 1));
		depthDataImage->setEnabled(false);

		bodyMaskImage = new CVImageWidget(ui.maskFrame);
		bodyMaskImage->setObjectName(QStringLiteral("bodyMaskImage"));
		bodyMaskImage->setGeometry(QRect(1, 1, ui.maskFrame->geometry().width() - 1, ui.maskFrame->geometry().height() - 1));
		bodyMaskImage->setEnabled(false);

		skeletonImage = new CVImageWidget(ui.skeletonFrame);
		skeletonImage->setObjectName(QStringLiteral("skeletonImage"));
		skeletonImage->setGeometry(QRect(1, 1, ui.skeletonFrame->geometry().width() - 1, ui.skeletonFrame->geometry().height() - 1));
		skeletonImage->setEnabled(false);

		audioImage = new CVImageWidget(ui.audioFrame);
		audioImage->setObjectName(QStringLiteral("skeletonImage"));
		audioImage->setGeometry(QRect(1, 1, ui.audioFrame->geometry().width() - 1, ui.audioFrame->geometry().height() - 1));
		audioImage->setEnabled(false);
	}
};

namespace UiAux {
	class KinectGuiAuxClass : public Ui_KinectGuiAuxClass {};
} //namepace UiAux

#endif //KINECTGUIAUX
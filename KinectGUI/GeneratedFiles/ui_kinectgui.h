/********************************************************************************
** Form generated from reading UI file 'kinectgui.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KINECTGUI_H
#define UI_KINECTGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KinectGUIClass
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *startRecordButton;
    QPushButton *stopRecordButton;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QFrame *RGBframe;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QFrame *depthFrame;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QFrame *maskFrame;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QFrame *skeletonFrame;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout;
    QPushButton *startCaptureButton;
    QPushButton *stopCaptureButton;
    QGroupBox *groupBox;
    QWidget *layoutWidget3;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *recordLabel;
    QLabel *colorLabel;
    QLabel *depthLabel;
    QLabel *depthFps;
    QCheckBox *depthShowCheck;
    QCheckBox *depthRecordCheck;
    QLabel *maskLabel;
    QLabel *maskFps;
    QLabel *colorFps;
    QCheckBox *colorShowCheck;
    QCheckBox *maskShowCheck;
    QLabel *fpsLabel;
    QLabel *showLabel;
    QCheckBox *audioShowCheck;
    QCheckBox *audioRecordCheck;
    QCheckBox *skeletonRecordCheck;
    QCheckBox *maskRecordCheck;
    QLabel *skeletonLabel;
    QLabel *skeletonFps;
    QLabel *audioLabel;
    QLabel *audioBeam;
    QCheckBox *skeletonShowCheck;
    QCheckBox *colorRecordCheck;
    QFrame *audioFrame;
    QLabel *audioLabel_2;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *KinectGUIClass)
    {
        if (KinectGUIClass->objectName().isEmpty())
            KinectGUIClass->setObjectName(QStringLiteral("KinectGUIClass"));
        KinectGUIClass->resize(1403, 989);
        centralWidget = new QWidget(KinectGUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(1070, 90, 281, 41));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        startRecordButton = new QPushButton(layoutWidget);
        startRecordButton->setObjectName(QStringLiteral("startRecordButton"));

        horizontalLayout_2->addWidget(startRecordButton);

        stopRecordButton = new QPushButton(layoutWidget);
        stopRecordButton->setObjectName(QStringLiteral("stopRecordButton"));

        horizontalLayout_2->addWidget(stopRecordButton);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 10, 1041, 911));
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        RGBframe = new QFrame(layoutWidget1);
        RGBframe->setObjectName(QStringLiteral("RGBframe"));
        RGBframe->setMinimumSize(QSize(514, 426));
        RGBframe->setAutoFillBackground(true);
        RGBframe->setFrameShape(QFrame::Box);
        RGBframe->setFrameShadow(QFrame::Plain);

        verticalLayout->addWidget(RGBframe);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        depthFrame = new QFrame(layoutWidget1);
        depthFrame->setObjectName(QStringLiteral("depthFrame"));
        depthFrame->setMinimumSize(QSize(514, 426));
        depthFrame->setAutoFillBackground(true);
        depthFrame->setFrameShape(QFrame::Box);
        depthFrame->setFrameShadow(QFrame::Plain);

        verticalLayout_2->addWidget(depthFrame);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_3->addWidget(label_3);

        maskFrame = new QFrame(layoutWidget1);
        maskFrame->setObjectName(QStringLiteral("maskFrame"));
        maskFrame->setMinimumSize(QSize(514, 426));
        maskFrame->setAutoFillBackground(true);
        maskFrame->setFrameShape(QFrame::Box);
        maskFrame->setFrameShadow(QFrame::Plain);

        verticalLayout_3->addWidget(maskFrame);


        gridLayout->addLayout(verticalLayout_3, 1, 0, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_4->addWidget(label_4);

        skeletonFrame = new QFrame(layoutWidget1);
        skeletonFrame->setObjectName(QStringLiteral("skeletonFrame"));
        skeletonFrame->setMinimumSize(QSize(514, 426));
        skeletonFrame->setAutoFillBackground(true);
        skeletonFrame->setFrameShape(QFrame::Box);
        skeletonFrame->setFrameShadow(QFrame::Plain);

        verticalLayout_4->addWidget(skeletonFrame);


        gridLayout->addLayout(verticalLayout_4, 1, 1, 1, 1);

        layoutWidget2 = new QWidget(centralWidget);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(1070, 40, 281, 41));
        horizontalLayout = new QHBoxLayout(layoutWidget2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        startCaptureButton = new QPushButton(layoutWidget2);
        startCaptureButton->setObjectName(QStringLiteral("startCaptureButton"));

        horizontalLayout->addWidget(startCaptureButton);

        stopCaptureButton = new QPushButton(layoutWidget2);
        stopCaptureButton->setObjectName(QStringLiteral("stopCaptureButton"));

        horizontalLayout->addWidget(stopCaptureButton);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(1070, 150, 281, 211));
        layoutWidget3 = new QWidget(groupBox);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(10, 40, 263, 137));
        gridLayout_2 = new QGridLayout(layoutWidget3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 0, 1, 1);

        recordLabel = new QLabel(layoutWidget3);
        recordLabel->setObjectName(QStringLiteral("recordLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(recordLabel->sizePolicy().hasHeightForWidth());
        recordLabel->setSizePolicy(sizePolicy);
        recordLabel->setMinimumSize(QSize(60, 0));
        recordLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(recordLabel, 0, 3, 1, 1);

        colorLabel = new QLabel(layoutWidget3);
        colorLabel->setObjectName(QStringLiteral("colorLabel"));

        gridLayout_2->addWidget(colorLabel, 1, 0, 1, 1);

        depthLabel = new QLabel(layoutWidget3);
        depthLabel->setObjectName(QStringLiteral("depthLabel"));

        gridLayout_2->addWidget(depthLabel, 2, 0, 1, 1);

        depthFps = new QLabel(layoutWidget3);
        depthFps->setObjectName(QStringLiteral("depthFps"));
        sizePolicy.setHeightForWidth(depthFps->sizePolicy().hasHeightForWidth());
        depthFps->setSizePolicy(sizePolicy);
        depthFps->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(depthFps, 2, 1, 1, 1);

        depthShowCheck = new QCheckBox(layoutWidget3);
        depthShowCheck->setObjectName(QStringLiteral("depthShowCheck"));

        gridLayout_2->addWidget(depthShowCheck, 2, 2, 1, 1, Qt::AlignHCenter);

        depthRecordCheck = new QCheckBox(layoutWidget3);
        depthRecordCheck->setObjectName(QStringLiteral("depthRecordCheck"));

        gridLayout_2->addWidget(depthRecordCheck, 2, 3, 1, 1, Qt::AlignHCenter);

        maskLabel = new QLabel(layoutWidget3);
        maskLabel->setObjectName(QStringLiteral("maskLabel"));

        gridLayout_2->addWidget(maskLabel, 3, 0, 1, 1);

        maskFps = new QLabel(layoutWidget3);
        maskFps->setObjectName(QStringLiteral("maskFps"));
        maskFps->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(maskFps, 3, 1, 1, 1);

        colorFps = new QLabel(layoutWidget3);
        colorFps->setObjectName(QStringLiteral("colorFps"));
        sizePolicy.setHeightForWidth(colorFps->sizePolicy().hasHeightForWidth());
        colorFps->setSizePolicy(sizePolicy);
        colorFps->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(colorFps, 1, 1, 1, 1);

        colorShowCheck = new QCheckBox(layoutWidget3);
        colorShowCheck->setObjectName(QStringLiteral("colorShowCheck"));
        colorShowCheck->setMinimumSize(QSize(0, 0));
        colorShowCheck->setTristate(false);

        gridLayout_2->addWidget(colorShowCheck, 1, 2, 1, 1, Qt::AlignHCenter);

        maskShowCheck = new QCheckBox(layoutWidget3);
        maskShowCheck->setObjectName(QStringLiteral("maskShowCheck"));

        gridLayout_2->addWidget(maskShowCheck, 3, 2, 1, 1, Qt::AlignHCenter);

        fpsLabel = new QLabel(layoutWidget3);
        fpsLabel->setObjectName(QStringLiteral("fpsLabel"));
        sizePolicy.setHeightForWidth(fpsLabel->sizePolicy().hasHeightForWidth());
        fpsLabel->setSizePolicy(sizePolicy);
        fpsLabel->setMinimumSize(QSize(60, 0));
        fpsLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(fpsLabel, 0, 1, 1, 1);

        showLabel = new QLabel(layoutWidget3);
        showLabel->setObjectName(QStringLiteral("showLabel"));
        sizePolicy.setHeightForWidth(showLabel->sizePolicy().hasHeightForWidth());
        showLabel->setSizePolicy(sizePolicy);
        showLabel->setMinimumSize(QSize(60, 0));
        showLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(showLabel, 0, 2, 1, 1);

        audioShowCheck = new QCheckBox(layoutWidget3);
        audioShowCheck->setObjectName(QStringLiteral("audioShowCheck"));

        gridLayout_2->addWidget(audioShowCheck, 5, 2, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);

        audioRecordCheck = new QCheckBox(layoutWidget3);
        audioRecordCheck->setObjectName(QStringLiteral("audioRecordCheck"));

        gridLayout_2->addWidget(audioRecordCheck, 5, 3, 1, 1, Qt::AlignHCenter);

        skeletonRecordCheck = new QCheckBox(layoutWidget3);
        skeletonRecordCheck->setObjectName(QStringLiteral("skeletonRecordCheck"));

        gridLayout_2->addWidget(skeletonRecordCheck, 4, 3, 1, 1, Qt::AlignHCenter);

        maskRecordCheck = new QCheckBox(layoutWidget3);
        maskRecordCheck->setObjectName(QStringLiteral("maskRecordCheck"));

        gridLayout_2->addWidget(maskRecordCheck, 3, 3, 1, 1, Qt::AlignHCenter);

        skeletonLabel = new QLabel(layoutWidget3);
        skeletonLabel->setObjectName(QStringLiteral("skeletonLabel"));

        gridLayout_2->addWidget(skeletonLabel, 4, 0, 1, 1);

        skeletonFps = new QLabel(layoutWidget3);
        skeletonFps->setObjectName(QStringLiteral("skeletonFps"));
        skeletonFps->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(skeletonFps, 4, 1, 1, 1);

        audioLabel = new QLabel(layoutWidget3);
        audioLabel->setObjectName(QStringLiteral("audioLabel"));

        gridLayout_2->addWidget(audioLabel, 5, 0, 1, 1);

        audioBeam = new QLabel(layoutWidget3);
        audioBeam->setObjectName(QStringLiteral("audioBeam"));
        audioBeam->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(audioBeam, 5, 1, 1, 1);

        skeletonShowCheck = new QCheckBox(layoutWidget3);
        skeletonShowCheck->setObjectName(QStringLiteral("skeletonShowCheck"));

        gridLayout_2->addWidget(skeletonShowCheck, 4, 2, 1, 1, Qt::AlignHCenter);

        colorRecordCheck = new QCheckBox(layoutWidget3);
        colorRecordCheck->setObjectName(QStringLiteral("colorRecordCheck"));

        gridLayout_2->addWidget(colorRecordCheck, 1, 3, 1, 1, Qt::AlignHCenter);

        audioFrame = new QFrame(centralWidget);
        audioFrame->setObjectName(QStringLiteral("audioFrame"));
        audioFrame->setGeometry(QRect(1070, 700, 281, 221));
        audioFrame->setFrameShape(QFrame::Box);
        audioFrame->setFrameShadow(QFrame::Plain);
        audioLabel_2 = new QLabel(centralWidget);
        audioLabel_2->setObjectName(QStringLiteral("audioLabel_2"));
        audioLabel_2->setGeometry(QRect(1070, 680, 281, 16));
        KinectGUIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(KinectGUIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1403, 26));
        KinectGUIClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(KinectGUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        KinectGUIClass->setStatusBar(statusBar);
        mainToolBar = new QToolBar(KinectGUIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        KinectGUIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(KinectGUIClass);

        QMetaObject::connectSlotsByName(KinectGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *KinectGUIClass)
    {
        KinectGUIClass->setWindowTitle(QApplication::translate("KinectGUIClass", "KinectGUI", 0));
        startRecordButton->setText(QApplication::translate("KinectGUIClass", "Start recording", 0));
        stopRecordButton->setText(QApplication::translate("KinectGUIClass", "Stop recording", 0));
        label->setText(QApplication::translate("KinectGUIClass", "RGB data", 0));
        label_2->setText(QApplication::translate("KinectGUIClass", "Depth data", 0));
        label_3->setText(QApplication::translate("KinectGUIClass", "Person mask", 0));
        label_4->setText(QApplication::translate("KinectGUIClass", "Skeleton", 0));
        startCaptureButton->setText(QApplication::translate("KinectGUIClass", "Start capturing", 0));
        stopCaptureButton->setText(QApplication::translate("KinectGUIClass", "Stop capturing", 0));
        groupBox->setTitle(QApplication::translate("KinectGUIClass", "General Information", 0));
        recordLabel->setText(QApplication::translate("KinectGUIClass", "Record", 0));
        colorLabel->setText(QApplication::translate("KinectGUIClass", "Color:", 0));
        depthLabel->setText(QApplication::translate("KinectGUIClass", "Depth: ", 0));
        depthFps->setText(QApplication::translate("KinectGUIClass", "--", 0));
        depthShowCheck->setText(QString());
        depthRecordCheck->setText(QString());
        maskLabel->setText(QApplication::translate("KinectGUIClass", "Mask: ", 0));
        maskFps->setText(QApplication::translate("KinectGUIClass", "--", 0));
        colorFps->setText(QApplication::translate("KinectGUIClass", "--", 0));
        colorShowCheck->setText(QString());
        maskShowCheck->setText(QString());
        fpsLabel->setText(QApplication::translate("KinectGUIClass", "FPS", 0));
        showLabel->setText(QApplication::translate("KinectGUIClass", "Show", 0));
        audioShowCheck->setText(QString());
        audioRecordCheck->setText(QString());
        skeletonRecordCheck->setText(QString());
        maskRecordCheck->setText(QString());
        skeletonLabel->setText(QApplication::translate("KinectGUIClass", "Skeleton: ", 0));
        skeletonFps->setText(QApplication::translate("KinectGUIClass", "--", 0));
        audioLabel->setText(QApplication::translate("KinectGUIClass", "Audio: ", 0));
        audioBeam->setText(QApplication::translate("KinectGUIClass", "--", 0));
        skeletonShowCheck->setText(QString());
        colorRecordCheck->setText(QString());
        audioLabel_2->setText(QApplication::translate("KinectGUIClass", "Audio", 0));
    } // retranslateUi

};

namespace Ui {
    class KinectGUIClass: public Ui_KinectGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KINECTGUI_H

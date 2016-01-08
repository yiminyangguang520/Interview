#include "GLDTipWidget.h"

namespace GlodonMask
{

    GLDTipWidget::GLDTipWidget(const GLDTipInfo & guideInfo, NEXTCLICKEDCALLBACK goCallBack, QWidget * parent)
        : QWidget(parent)
        , m_pHintWidget(nullptr)
        , m_pNextButton(nullptr)
        , m_pCloseButton(nullptr)
        , m_goCallBack(goCallBack)

    {
        setAttribute(Qt::WA_NoSystemBackground);
        setAttribute(Qt::WA_TranslucentBackground);
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

        initHintWidget();

        initNextButton();

        initCloseButton();

        // 提示信息
        setHintWidgetStyle(guideInfo);

        // 关闭按钮
        setCloseButtonStyle(guideInfo);

        // 下一个按钮
        setNextButtonStyle(guideInfo);
    }

    GLDTipWidget::~GLDTipWidget()
    {

    }

    void GLDTipWidget::initHintWidget()
    {
        m_pHintWidget = new QWidget(this);
        m_pHintWidget->setObjectName("m_pHintWidget");
        m_pHintWidget->setAutoFillBackground(true);
    }

    void GLDTipWidget::initNextButton()
    {
        m_pNextButton = new QPushButton(m_pHintWidget);
        m_pNextButton->setObjectName("m_pNextButton");
        m_pNextButton->setAutoFillBackground(true);

        connect(m_pNextButton, &QPushButton::clicked, this, &GLDTipWidget::nextButtonClicked);
    }

    void GLDTipWidget::initCloseButton()
    {
        m_pCloseButton = new QPushButton(m_pHintWidget);
        m_pCloseButton->setObjectName("m_pCloseButton");
        m_pCloseButton->setAutoFillBackground(true);

        connect(m_pCloseButton, &QPushButton::clicked, this, &GLDTipWidget::closeBtnClicked);
    }

    void GLDTipWidget::setHintWidgetStyle(const GLDTipInfo &guideInfo)
    {
        if (guideInfo.m_maskWidgetItem.m_normalImage != "")
        {
            const QString strMaskStyle = hintStyleSheet(guideInfo);
            m_pHintWidget->setStyleSheet(strMaskStyle);
        }
        else
        {
            hide();
        }

        if (guideInfo.m_maskWidgetItem.m_width != -1 && guideInfo.m_maskWidgetItem.m_height != -1)
        {
            m_pHintWidget->setFixedSize(guideInfo.m_maskWidgetItem.m_width, guideInfo.m_maskWidgetItem.m_height);
        }

        m_pHintWidget->move(0, 0);
    }

    void GLDTipWidget::setCloseButtonStyle(const GLDTipInfo &guideInfo)
    {
        if (guideInfo.m_closeButtonItem.m_normalImage != "")
        {
            const QString strCloseStyle = closeStyleSheet(guideInfo);
            m_pCloseButton->setStyleSheet(strCloseStyle);
        }

        if (guideInfo.m_closeButtonItem.m_width != -1 && guideInfo.m_closeButtonItem.m_height != -1)
        {
            m_pCloseButton->setFixedSize(guideInfo.m_closeButtonItem.m_width,
                guideInfo.m_closeButtonItem.m_height);
        }

        if (guideInfo.m_closeButtonItem.m_leftXPos != -1 && guideInfo.m_closeButtonItem.m_leftYPos != -1)
        {
            m_pCloseButton->move(guideInfo.m_closeButtonItem.m_leftXPos,
                guideInfo.m_closeButtonItem.m_leftYPos);
        }
    }

    void GLDTipWidget::setNextButtonStyle(const GLDTipInfo &guideInfo)
    {
        if (guideInfo.m_nextButtonItem.m_normalImage != "")
        {
            const QString strNextStyle = nextStyleSheet(guideInfo);
            m_pNextButton->setStyleSheet(strNextStyle);
        }

        if (guideInfo.m_nextButtonItem.m_width != -1 && guideInfo.m_nextButtonItem.m_height != -1)
        {
            m_pNextButton->setFixedSize(guideInfo.m_nextButtonItem.m_width,
                guideInfo.m_nextButtonItem.m_height);
        }

        if (guideInfo.m_nextButtonItem.m_leftXPos != -1 && guideInfo.m_nextButtonItem.m_leftYPos != -1)
        {
            m_pNextButton->move(guideInfo.m_nextButtonItem.m_leftXPos,
                guideInfo.m_nextButtonItem.m_leftYPos);
        }
    }

    QString GLDTipWidget::hintStyleSheet(const GLDTipInfo &guideInfo)
    {
        return QString("QWidget#m_pHintWidget"
            "{"
            "border-image: url(" + guideInfo.m_maskWidgetItem.m_normalImage + ");"
            "}"
            );
    }

    QString GLDTipWidget::closeStyleSheet(const GLDTipInfo &guideInfo)
    {
        return QString("QPushButton#m_pCloseButton"
            "{"
            "border-image: url(" + guideInfo.m_closeButtonItem.m_normalImage + ");"
            "}"
            "QPushButton#m_pCloseButton:hover"
            "{"
            "border-image: url(" + guideInfo.m_closeButtonItem.m_hoverImage + ");"
            "}"
            "QPushButton#m_pCloseButton:pressed"
            "{"
            "border-image: url(" + guideInfo.m_closeButtonItem.m_pressedImage + ");"
            "}");
    }

    QString GLDTipWidget::nextStyleSheet(const GLDTipInfo &guideInfo)
    {
        return QString("QPushButton#m_pNextButton"
            "{"
            "border-image: url(" + guideInfo.m_nextButtonItem.m_normalImage + ");"
            "}"
            "QPushButton#m_pNextButton:hover"
            "{"
            "border-image: url(" + guideInfo.m_nextButtonItem.m_hoverImage + ");"
            "}"
            "QPushButton#m_pNextButton:pressed"
            "{"
            "border-image: url(" + guideInfo.m_nextButtonItem.m_pressedImage + ");"
            "}");
    }

    void GLDTipWidget::nextButtonClicked()
    {
        if (nullptr != m_goCallBack)
        {
            m_goCallBack();
        }
    }

}

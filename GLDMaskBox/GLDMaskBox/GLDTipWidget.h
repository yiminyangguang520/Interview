#ifndef GLDMASKWIDGET_H
#define GLDMASKWIDGET_H

#include "GLDMaskBox_Global.h"

#include <QWidget>
#include <QPushButton>

#include <functional>

namespace GlodonMask
{
    typedef std::function<void()> NEXTCLICKEDCALLBACK;

    struct GLDTipInfoItem
    {
        GLDTipInfoItem(int width = -1, int height = -1, int leftXPos = -1, int leftYPos = -1,
                       QString normalImage = "", QString hoverImage = "", QString pressedImage = "")
            : m_width(width)
            , m_height(height)
            , m_leftXPos(leftXPos)
            , m_leftYPos(leftYPos)
            , m_normalImage(normalImage)
            , m_hoverImage(hoverImage)
            , m_pressedImage(pressedImage)
        {

        }

        int m_width;               // 图片宽度
        int m_height;              // 图片高度
        int m_leftXPos;            // 左上角X坐标
        int m_leftYPos;            // 左上角Y坐标

        QString m_normalImage;     // 正常情况下图片
        QString m_hoverImage;      // 鼠标划过时效果
        QString m_pressedImage;    // 鼠标按下时效果
    };

    struct GLDTipInfo
    {
        GLDTipInfo()
        {

        }

        GLDTipInfo(GLDTipInfoItem maskWidgetItem,
                     GLDTipInfoItem nextButtonItem,
                     GLDTipInfoItem closeButtonItem)
            : m_maskWidgetItem(maskWidgetItem)
            , m_nextButtonItem(nextButtonItem)
            , m_closeButtonItem(closeButtonItem)
        {

        }

        GLDTipInfoItem m_maskWidgetItem;    // 提示信息
        GLDTipInfoItem m_nextButtonItem;    // 下一步
        GLDTipInfoItem m_closeButtonItem;   // 关闭
    };



    class GLDMASKBOX_EXPORT GLDTipWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit GLDTipWidget(const GLDTipInfo & guideInfo,
                              NEXTCLICKEDCALLBACK goCallBack = nullptr,
                              QWidget * parent = 0);
        ~GLDTipWidget();


    private:
        /**
        * @brief 初始化蒙版主界面
        */
        void initHintWidget();

        /**
        * @brief 初始化下一步按钮
        */
        void initNextButton();

        /**
        * @brief 初始化关闭按钮
        */
        void initCloseButton();

        /**
        * @brief 设置提示信息样式
        * @param guideInfo    当前提示信息
        */
        void setHintWidgetStyle(const GLDTipInfo &guideInfo);

        /**
        * @brief 设置关闭按钮样式
        * @param guideInfo    当前提示信息
        */
        void setCloseButtonStyle(const GLDTipInfo &guideInfo);

        /**
        * @brief 设置下一步按钮样式
        * @param guideInfo    当前提示信息
        */
        void setNextButtonStyle(const GLDTipInfo &guideInfo);

        /**
        * @brief 获取提示样式
        * @param guideInfo    当前提示信息
        * @return
        */
        QString hintStyleSheet(const GLDTipInfo &guideInfo);

        /**
        * @brief 获取关闭按钮样式
        * @param guideInfo    当前提示信息
        * @return
        */
        QString closeStyleSheet(const GLDTipInfo &guideInfo);

        /**
        * @brief 获取下一步按钮样式
        * @param guideInfo    当前提示信息
        * @return
        */
        QString nextStyleSheet(const GLDTipInfo &guideInfo);

    Q_SIGNALS:
        void closeBtnClicked();

    private slots:
        /**
        * @brief 下一步按钮被点击
        */
        void nextButtonClicked();

    private:
        QWidget*            m_pHintWidget;         // 提示图片
        QPushButton*        m_pNextButton;         // 下一步按钮
        QPushButton*        m_pCloseButton;        // 关闭按钮
        NEXTCLICKEDCALLBACK m_goCallBack;          // 回调函数指针
    };
}

#endif // GLDMASKWIDGET_H

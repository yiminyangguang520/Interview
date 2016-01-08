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

        int m_width;               // ͼƬ���
        int m_height;              // ͼƬ�߶�
        int m_leftXPos;            // ���Ͻ�X����
        int m_leftYPos;            // ���Ͻ�Y����

        QString m_normalImage;     // ���������ͼƬ
        QString m_hoverImage;      // ��껮��ʱЧ��
        QString m_pressedImage;    // ��갴��ʱЧ��
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

        GLDTipInfoItem m_maskWidgetItem;    // ��ʾ��Ϣ
        GLDTipInfoItem m_nextButtonItem;    // ��һ��
        GLDTipInfoItem m_closeButtonItem;   // �ر�
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
        * @brief ��ʼ���ɰ�������
        */
        void initHintWidget();

        /**
        * @brief ��ʼ����һ����ť
        */
        void initNextButton();

        /**
        * @brief ��ʼ���رհ�ť
        */
        void initCloseButton();

        /**
        * @brief ������ʾ��Ϣ��ʽ
        * @param guideInfo    ��ǰ��ʾ��Ϣ
        */
        void setHintWidgetStyle(const GLDTipInfo &guideInfo);

        /**
        * @brief ���ùرհ�ť��ʽ
        * @param guideInfo    ��ǰ��ʾ��Ϣ
        */
        void setCloseButtonStyle(const GLDTipInfo &guideInfo);

        /**
        * @brief ������һ����ť��ʽ
        * @param guideInfo    ��ǰ��ʾ��Ϣ
        */
        void setNextButtonStyle(const GLDTipInfo &guideInfo);

        /**
        * @brief ��ȡ��ʾ��ʽ
        * @param guideInfo    ��ǰ��ʾ��Ϣ
        * @return
        */
        QString hintStyleSheet(const GLDTipInfo &guideInfo);

        /**
        * @brief ��ȡ�رհ�ť��ʽ
        * @param guideInfo    ��ǰ��ʾ��Ϣ
        * @return
        */
        QString closeStyleSheet(const GLDTipInfo &guideInfo);

        /**
        * @brief ��ȡ��һ����ť��ʽ
        * @param guideInfo    ��ǰ��ʾ��Ϣ
        * @return
        */
        QString nextStyleSheet(const GLDTipInfo &guideInfo);

    Q_SIGNALS:
        void closeBtnClicked();

    private slots:
        /**
        * @brief ��һ����ť�����
        */
        void nextButtonClicked();

    private:
        QWidget*            m_pHintWidget;         // ��ʾͼƬ
        QPushButton*        m_pNextButton;         // ��һ����ť
        QPushButton*        m_pCloseButton;        // �رհ�ť
        NEXTCLICKEDCALLBACK m_goCallBack;          // �ص�����ָ��
    };
}

#endif // GLDMASKWIDGET_H

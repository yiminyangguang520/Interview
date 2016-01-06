#ifndef GLDMASK_H
#define GLDMASK_H

#include "GLDTipWidget.h"
#include "GLDMaskBox_Global.h"

#include <QList>
#include <QSettings>
#include <QtXml/QDomElement>

class QPushButton;

namespace GlodonMask
{
    static QWidget* topLevelParentWidget(QWidget* pWgt)
    {
        if (!pWgt)
        {
            return nullptr;
        }

        QWidget* widget = nullptr;

        for (widget = pWgt; widget != nullptr; widget = widget->parentWidget())
        {
            if (widget->isWindow() && widget->isWindowType())
            {
                break;
            }

            if (nullptr == widget->parentWidget())
            {
                return widget;
            }
        }

        return widget;
    }

    struct CoordinateParam
    {
    public:
        enum Quadrant
        {
            Zero,
            First,
            Second,
            Third,
            Fourth
        };

        CoordinateParam()
            : m_point(-1, -1)
            , m_quadrant(Zero)
        {

        }

        QPoint                     m_point;
        CoordinateParam::Quadrant  m_quadrant;
    };


    class GLDProxyWidget : public QWidget
    {
        Q_OBJECT

    public:
        GLDProxyWidget(const int index, QWidget* parent = nullptr);
        ~GLDProxyWidget();

    protected:
        virtual void showEvent(QShowEvent *event);

    Q_SIGNALS:
        void tobeShow();

    protected:
        int           m_index;
    };

    class GLDMASKBOX_EXPORT GLDMask : public QWidget
    {
        Q_OBJECT

    public:
        enum MASKCOLOR
        {
            GrayColor,
            CoolGreyColor,
            LightGrayColor,
            DarkGrayColor
        };

    public:
        explicit GLDMask(QWidget * pWgt, GLDTipWidget * pTipWgt,
            const int index, QWidget * parent = nullptr);

        virtual ~GLDMask();

        /**
        * @brief �����ɰ汳��ɫ
        * @param maskColor
        */
        void setMaskColor(MASKCOLOR maskColor);

        /**
        * @brief ���ü�ͷ��ɫ
        * @param color
        */
        void setArrowColor(const QColor& color);

        /**
        * @brief ���ü�ͷ������ϸ
        * @param lineWidth
        */
        void setArrowLineWidth(const int lineWidth);

        /**
        * @brief ����ini�ļ�·��
        * @param iniPath
        */
        void setIniPath(const QString& iniPath);

        /**
        * @brief �ж��ɰ��Ƿ��Ѿ���ʾ
        * @return
        */
        bool canShow(const QString &iniPath);

        /**
        * @brief ��ȡ��Ҫ��ʾ�ɰ��widget
        * @return
        */
        inline QWidget* getClipedWidget()
        {
            return m_pClippedWgt;
        }

    public slots:
        /**
        * @brief �ر��ɰ�
        */
        void slotClose();

        /**
        * @brief ��ʾ�ɰ�
        */
        void showMask();

    protected:
        virtual void paintEvent(QPaintEvent *event);
        virtual bool eventFilter(QObject *watched, QEvent *event);

    private:

        /**
        * @brief ������ʾ��Ϣλ��
        * @return
        */
        CoordinateParam calcPosOfTipInfo();

        /**
        * @brief �����ɰ�
        * @param painter
        */
        void drawMask(QPainter & painter);

        /**
        * @brief ����ָ�����Ͻǵļ�ͷ
        * @param startPoint    ���
        * @param endPoint      �յ�
        * @param painter
        */
        void drawLeftTopArrow(QPoint &startPoint, QPoint &endPoint, QPainter &painter);

        /**
        * @brief ����ָ�����½ǵļ�ͷ
        * @param startPoint    ���
        * @param endPoint      �յ�
        * @param painter
        */
        void drawLeftBottomArrow(QPoint &startPoint, QPoint &endPoint, QPainter &painter);

        /**
        * @brief ����ָ�����Ͻǵļ�ͷ
        * @param startPoint    ���
        * @param endPoint      �յ�
        * @param painter
        */
        void drawTopRightArrow(QPoint &startPoint, QPoint &endPoint, QPainter &painter);

        /**
        * @brief ����ָ�����½ǵļ�ͷ
        * @param startPoint    ���
        * @param endPoint      �յ�
        * @param painter
        */
        void drawRightBottomArrow(QPoint &startPoint, QPoint &endPoint, QPainter &painter);

    private:
        MASKCOLOR               m_maskColor;      // �ɰ汳��ɫ

        QWidget*                m_pClippedWgt;    // ��Ҫ��ʾ�ɰ��widget
        GLDTipWidget*           m_pTipWidget;     // ��ʾ��Ϣ����

        QColor                  m_arrowColor;     // ��ͷ��ɫ
        int                     m_arrowLineWidth; // ��ͷ������ϸ

        GLDProxyWidget*         m_pGLDProxyWidget;// ����widget

        static QString          m_iniPath;        // ini�ļ�·��
    };
}

#endif // GLDMASK_H

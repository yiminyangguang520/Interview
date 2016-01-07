#ifndef GLDMASK_H
#define GLDMASK_H

#include "GLDTipWidget.h"
#include "GLDMaskBox_Global.h"

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
        GLDProxyWidget(QWidget* parent = nullptr);
        ~GLDProxyWidget();

    protected:
        virtual void showEvent(QShowEvent *event);

    Q_SIGNALS:
        void tobeShow();
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
        explicit GLDMask(QWidget* pWgt, GLDTipWidget* pTipWgt,
                         const int index, QWidget* parent = nullptr);

        virtual ~GLDMask();

        /**
        * @brief 设置蒙版背景色
        * @param maskColor
        */
        void setMaskColor(MASKCOLOR maskColor);

        /**
        * @brief 设置箭头颜色
        * @param color
        */
        void setArrowColor(const QColor& color);

        /**
        * @brief 设置箭头线条粗细
        * @param lineWidth
        */
        void setArrowLineWidth(const int lineWidth);

        /**
         * @brief 设置是否已经显示过
         * @param show
         */
        void setIsShown(bool show);

    public slots:
        /**
        * @brief 关闭蒙版
        */
        void slotClose();

        /**
        * @brief 显示蒙版
        */
        void showMask();

    Q_SIGNALS:
        void alreadyShow();

    protected:
        virtual void paintEvent(QPaintEvent *event);
        virtual bool eventFilter(QObject *watched, QEvent *event);

    private:

        /**
        * @brief 计算提示信息位置
        * @return
        */
        CoordinateParam calcPosOfTipInfo();

        /**
        * @brief 绘制蒙版
        * @param painter
        */
        void drawMask(QPainter & painter);

        /**
        * @brief 绘制指向左上角的箭头
        * @param startPoint    起点
        * @param endPoint      终点
        * @param painter
        */
        void drawLeftTopArrow(QPoint &startPoint, QPoint &endPoint, QPainter &painter);

        /**
        * @brief 绘制指向左下角的箭头
        * @param startPoint    起点
        * @param endPoint      终点
        * @param painter
        */
        void drawLeftBottomArrow(QPoint &startPoint, QPoint &endPoint, QPainter &painter);

        /**
        * @brief 绘制指向右上角的箭头
        * @param startPoint    起点
        * @param endPoint      终点
        * @param painter
        */
        void drawTopRightArrow(QPoint &startPoint, QPoint &endPoint, QPainter &painter);

        /**
        * @brief 绘制指向右下角的箭头
        * @param startPoint    起点
        * @param endPoint      终点
        * @param painter
        */
        void drawRightBottomArrow(QPoint &startPoint, QPoint &endPoint, QPainter &painter);

    private:
        MASKCOLOR               m_maskColor;      // 蒙版背景色

        QWidget*                m_pClippedWgt;    // 需要显示蒙版的widget
        GLDTipWidget*           m_pTipWidget;     // 提示信息窗体

        QColor                  m_arrowColor;     // 箭头颜色
        int                     m_arrowLineWidth; // 箭头线条粗细

        GLDProxyWidget*         m_pGLDProxyWidget;// 代理widget

        bool                    m_bIsShown;       // 该蒙版是否显示过
    };
}

#endif // GLDMASK_H

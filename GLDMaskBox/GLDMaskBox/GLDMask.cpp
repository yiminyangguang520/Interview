#include "GLDMask.h"

#include <QPainter>
#include <QPushButton>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>

namespace GlodonMask
{
    GLDProxyWidget::GLDProxyWidget(const int index, QWidget *parent)
        : QWidget(parent)
        , m_index(index)
    {

    }

    GLDProxyWidget::~GLDProxyWidget()
    {

    }

    void GLDProxyWidget::showEvent(QShowEvent *event)
    {
        QWidget::showEvent(event);

        if (m_index == 0)
        {
            emit tobeShow();
        }
    }

    QString GLDMask::m_iniPath = "";

    GLDMask::GLDMask(QWidget *pWgt, GLDTipWidget *pTipWgt, const int index, QWidget *parent)
        : QWidget(parent)
        , m_maskColor(GLDMask::GrayColor)
        , m_pClippedWgt(pWgt)
        , m_pTipWidget(pTipWgt)
        , m_arrowColor(QColor(1, 169, 240))
        , m_arrowLineWidth(2)
        , m_pGLDProxyWidget(new GLDProxyWidget(index, pWgt))
    {
        m_pTipWidget->setParent(this);

        setFixedSize(QApplication::desktop()->width(), QApplication::desktop()->height());

        m_pClippedWgt->installEventFilter(this);

        connect(m_pTipWidget, &GLDTipWidget::tipWidgetClicked, this, &GLDMask::slotClose);

        connect(m_pGLDProxyWidget, &GLDProxyWidget::tobeShow, this, &GLDMask::showMask);
    }

    GLDMask::~GLDMask()
    {

    }

    void GLDMask::paintEvent(QPaintEvent * event)
    {
        QWidget::paintEvent(event);

        QPainter painter(this);

        CoordinateParam param = calcPosOfTipInfo();

        QPoint endPoint;

        switch (param.m_quadrant)
        {
        case CoordinateParam::First:
        {
            drawTopRightArrow(param.m_point, endPoint, painter);
            endPoint += QPoint(-m_pTipWidget->width(), 0);
            m_pTipWidget->move(endPoint);
            break;
        }

        case CoordinateParam::Second:
        {
            drawLeftTopArrow(param.m_point, endPoint, painter);
            m_pTipWidget->move(endPoint);
            break;
        }

        case CoordinateParam::Third:
        {
            drawLeftBottomArrow(param.m_point, endPoint, painter);
            endPoint += QPoint(0, -m_pTipWidget->height());
            m_pTipWidget->move(endPoint);
            break;
        }

        case CoordinateParam::Fourth:
        {
            drawRightBottomArrow(param.m_point, endPoint, painter);
            endPoint += QPoint(-m_pTipWidget->width(), -m_pTipWidget->height());
            m_pTipWidget->move(endPoint);
            break;
        }

        default:
            break;
        }


        QPoint clippedWgtTopLeft = m_pClippedWgt->rect().topLeft();

        const int nClippedWidgetWidth = m_pClippedWgt->size().width();
        const int nClippedWidgetHeight = m_pClippedWgt->size().height();

        QPoint ptGlobalTopLeft = m_pClippedWgt->mapTo(topLevelParentWidget(m_pClippedWgt), clippedWgtTopLeft);
        QRect pOwnerRect(ptGlobalTopLeft.x(), ptGlobalTopLeft.y(), nClippedWidgetWidth, nClippedWidgetHeight);

        QRegion rect = this->rect();
        rect -= pOwnerRect;
        setMask(rect);

        drawMask(painter);

        update();
    }

    void GLDMask::drawMask(QPainter & painter)
    {
        const int topWidgetWidth = topLevelParentWidget(m_pClippedWgt)->size().width();
        const int topWidgetHeight = topLevelParentWidget(m_pClippedWgt)->size().height();

        QColor color;

        switch (m_maskColor)
        {
        case GLDMask::GrayColor:             // 128, 128, 128
            color = QColor(0, 0, 0, 100);
            break;

        case GLDMask::CoolGreyColor:         // 128, 138, 135
            color = QColor(128, 138, 135, 150);
            break;

        case GLDMask::LightGrayColor:        // 160, 160, 164
            color = QColor(160, 160, 164, 100);
            break;

        case GLDMask::DarkGrayColor:         // 128, 128, 128
            color = QColor(128, 128, 128, 100);
            break;

        default:
            color = QColor(160, 160, 164, 100); // 160, 160, 164
        }

        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        painter.setPen(QPen(color));
        painter.setBrush(color);
        painter.drawRect(0, 0, topWidgetWidth, topWidgetHeight);
    }

    void GLDMask::drawLeftTopArrow(QPoint &startPoint, QPoint &endPoint, QPainter &painter)
    {
        int x = startPoint.x();
        int y = startPoint.y();

        int x1 = x + 70;
        int y1 = y + 50;

        endPoint = QPoint(x1, y1);

        painter.setPen(QPen(m_arrowColor, m_arrowLineWidth));
        painter.setRenderHint(QPainter::Antialiasing);

        QPainterPath path;
        path.moveTo(startPoint);

        QPoint point1((x + (x1 - x) * 3 / 10), (y + (y1 - y) * 4 / 7));
        QPoint point2((x + (x1 - x) * 4 / 10), (y + (y1 - y) * 6 / 7));
        QPoint point3((x + (x1 - x) * 8 / 10), (y + (y1 - y) * 3 / 7));
        path.cubicTo(point1, point2, point3);


        QPoint point4((x + (x1 - x) * 5 / 10), (y + (y1 - y) * 1 / 7));
        QPoint point5((x + (x1 - x) * 3 / 10), (y + (y1 - y) * 2 / 7));
        QPoint point6((x + (x1 - x) * 4 / 10), (y + (y1 - y) * 9 / 14));
        path.cubicTo(point4, point5, point6);

        QPoint point7((x + (x1 - x) * 5 / 10), (y + (y1 - y) * 6 / 7));
        QPoint point8((x + (x1 - x) * 5 / 10), (y + (y1 - y) * 7 / 7));
        path.cubicTo(point7, point8, endPoint);

        painter.drawPath(path);

        int xOffset = (x1 - x) / 5;
        QLine line1(startPoint, QPoint(x + 2, y + xOffset));
        int yOffset = (y1 - y) / 7;
        QLine line2(startPoint, QPoint(x + xOffset, y + yOffset));
        painter.drawLine(line1);
        painter.drawLine(line2);
    }

    void GLDMask::drawLeftBottomArrow(QPoint &startPoint, QPoint &endPoint, QPainter &painter)
    {
        int x = startPoint.x();
        int y = startPoint.y();

        int x1 = x + 70;
        int y1 = y + 50;
        endPoint = QPoint(x1, y - 50);

        painter.setPen(QPen(m_arrowColor, m_arrowLineWidth));
        painter.setRenderHint(QPainter::Antialiasing);

        QPainterPath path;
        path.moveTo(startPoint);

        QPoint point1((x + (x1 - x) * 3 / 10), (y - (y1 - y) * 4 / 7));
        QPoint point2((x + (x1 - x) * 4 / 10), (y - (y1 - y) * 6 / 7));
        QPoint point3((x + (x1 - x) * 8 / 10), (y - (y1 - y) * 3 / 7));
        path.cubicTo(point1, point2, point3);


        QPoint point4((x + (x1 - x) * 5 / 10), (y - (y1 - y) * 1 / 7));
        QPoint point5((x + (x1 - x) * 3 / 10), (y - (y1 - y) * 2 / 7));
        QPoint point6((x + (x1 - x) * 4 / 10), (y - (y1 - y) * 9 / 14));
        path.cubicTo(point4, point5, point6);

        QPoint point7((x + (x1 - x) * 5 / 10), y - 50);
        QPoint point8((x + (x1 - x) * 5 / 10), y - 50);
        path.cubicTo(point7, point8, endPoint);

        painter.drawPath(path);

        int xOffset = (x1 - x) * 2 / 10;
        QLine line1(startPoint, QPoint(x + 2, y - xOffset));
        int yOffset = (y1 - y) * 1 / 7;
        QLine line2(startPoint, QPoint(x + xOffset, y - yOffset));
        painter.drawLine(line1);
        painter.drawLine(line2);
    }

    void GLDMask::drawTopRightArrow(QPoint &startPoint, QPoint &endPoint, QPainter &painter)
    {
        int x = startPoint.x();
        int y = startPoint.y();

        int x1 = x - 70;
        int y1 = y + 50;
        endPoint = QPoint(x1, y1);

        painter.setPen(QPen(m_arrowColor, m_arrowLineWidth));
        painter.setRenderHint(QPainter::Antialiasing);

        QPainterPath path;
        path.moveTo(startPoint);

        QPoint point1((x - (x - x1) * 3 / 10), (y + (y1 - y) * 4 / 7));
        QPoint point2((x - (x - x1) * 4 / 10), (y + (y1 - y) * 6 / 7));
        QPoint point3((x - (x - x1) * 8 / 10), (y + (y1 - y) * 3 / 7));
        path.cubicTo(point1, point2, point3);


        QPoint point4((x - (x - x1) * 5 / 10), (y + (y1 - y) * 1 / 7));
        QPoint point5((x - (x - x1) * 3 / 10), (y + (y1 - y) * 2 / 7));
        QPoint point6((x - (x - x1) * 4 / 10), (y + (y1 - y) * 9 / 14));
        path.cubicTo(point4, point5, point6);

        QPoint point7((x - (x - x1) * 5 / 10), (y + (y1 - y) * 6 / 7));
        QPoint point8((x - (x - x1) * 5 / 10), (y + (y1 - y) * 7 / 7));
        path.cubicTo(point7, point8, endPoint);

        painter.drawPath(path);

        int xOffset = (y1 - y) / 5;
        int yOffset = (x - x1) / 7;
        QLine line1(startPoint, QPoint(x - xOffset * 3 / 2, y + yOffset / 2));
        QLine line2(startPoint, QPoint(x, y + yOffset));
        painter.drawLine(line1);
        painter.drawLine(line2);
    }

    void GLDMask::drawRightBottomArrow(QPoint &startPoint, QPoint &endPoint, QPainter &painter)
    {
        int x = startPoint.x();
        int y = startPoint.y();

        int x1 = x - 70;
        int y1 = y - 50;
        endPoint = QPoint(x1, y1);

        painter.setPen(QPen(m_arrowColor, m_arrowLineWidth));
        painter.setRenderHint(QPainter::Antialiasing);

        QPainterPath path;
        path.moveTo(startPoint);

        QPoint point1((x - (x - x1) * 3 / 10), (y - (y - y1) * 4 / 7));
        QPoint point2((x - (x - x1) * 4 / 10), (y - (y - y1) * 6 / 7));
        QPoint point3((x - (x - x1) * 8 / 10), (y - (y - y1) * 3 / 7));
        path.cubicTo(point1, point2, point3);


        QPoint point4((x - (x - x1) * 5 / 10), (y - (y - y1) * 1 / 7));
        QPoint point5((x - (x - x1) * 3 / 10), (y - (y - y1) * 2 / 7));
        QPoint point6((x - (x - x1) * 4 / 10), (y - (y - y1) * 9 / 14));
        path.cubicTo(point4, point5, point6);

        QPoint point7((x - (x - x1) * 5 / 10), (y - (y - y1) * 6 / 7));
        QPoint point8((x - (x - x1) * 5 / 10), (y - (y - y1) * 7 / 7));
        path.cubicTo(point7, point8, endPoint);

        painter.drawPath(path);

        int xOffset = (x - x1) / 5;
        int yOffset = (y - y1) / 7;
        QLine line1(startPoint, QPoint(x - 2, y - xOffset));
        QLine line2(startPoint, QPoint(x - xOffset, y - yOffset));
        painter.drawLine(line1);
        painter.drawLine(line2);
    }

    bool GLDMask::eventFilter(QObject *watched, QEvent *event)
    {
        if (watched == m_pClippedWgt && event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            if (mouseEvent->button() == Qt::LeftButton)
            {
                close();
            }
            else
            {
                return false;
            }
        }

        return QWidget::eventFilter(watched, event);
    }

    void GLDMask::setIniPath(const QString &iniPath)
    {
        m_iniPath = iniPath;
    }

    bool GLDMask::canShow(const QString & iniPath)
    {
        QSettings oInis(iniPath, QSettings::IniFormat);

        return oInis.value("MaskBoxIsShown").toInt();
    }

    void GLDMask::setMaskColor(MASKCOLOR maskColor)
    {
        if (maskColor == m_maskColor)
        {
            return;
        }

        m_maskColor = maskColor;
    }

    void GLDMask::setArrowColor(const QColor &color)
    {
        if (m_arrowColor == color)
        {
            return;
        }

        m_arrowColor = color;
    }

    void GLDMask::setArrowLineWidth(const int lineWidth)
    {
        if (m_arrowLineWidth == lineWidth)
        {
            return;
        }

        m_arrowLineWidth = lineWidth;
    }

    CoordinateParam GLDMask::calcPosOfTipInfo()
    {
        CoordinateParam coordinateParam;

        do
        {
            // 计算owner位置对应屏幕中心的象限
            if (!m_pClippedWgt)
            {
                break;
            }

            QPoint clippedWgtTopLeft = m_pClippedWgt->rect().topLeft();
            QPoint ptParentOwnerTopLeft = m_pClippedWgt->mapTo(topLevelParentWidget(m_pClippedWgt), clippedWgtTopLeft);

            QPoint ptGlobalOwnerCenter = m_pClippedWgt->mapToGlobal(m_pClippedWgt->rect().center());
            QPoint ptGlobalScreen = QApplication::desktop()->screenGeometry().center();
            QPoint ptDelta = ptGlobalOwnerCenter - ptGlobalScreen;

            if (ptDelta.x() >= 0 && ptDelta.y() <= 0)
            {
                // 第一象限
                coordinateParam.m_point = QPoint(ptParentOwnerTopLeft.x() + m_pClippedWgt->width() / 2,
                    ptParentOwnerTopLeft.y() + m_pClippedWgt->height());
                coordinateParam.m_quadrant = CoordinateParam::First;
            }
            else if (ptDelta.x() <= 0 && ptDelta.y() <= 0)
            {
                // 第二象限
                coordinateParam.m_point = QPoint(ptParentOwnerTopLeft.x() + m_pClippedWgt->width() / 2,
                    ptParentOwnerTopLeft.y() + m_pClippedWgt->height());
                coordinateParam.m_quadrant = CoordinateParam::Second;
            }
            else if (ptDelta.x() <= 0 && ptDelta.y() >= 0)
            {
                // 第三象限
                coordinateParam.m_point = QPoint(ptParentOwnerTopLeft.x() + m_pClippedWgt->width() / 2,
                    ptParentOwnerTopLeft.y());
                coordinateParam.m_quadrant = CoordinateParam::Third;

            }
            else if (ptDelta.x() >= 0 && ptDelta.y() >= 0)
            {
                // 第四象限
                coordinateParam.m_point = QPoint(ptParentOwnerTopLeft.x() + m_pClippedWgt->width() / 2,
                    ptParentOwnerTopLeft.y());
                coordinateParam.m_quadrant = CoordinateParam::Fourth;
            }

        } while (0);

        return coordinateParam;
    }

    void GLDMask::slotClose()
    {
        QSettings oInis(m_iniPath, QSettings::IniFormat);
        oInis.setValue("MaskBoxIsShown", 1);
        close();
    }

    void GLDMask::showMask()
    {
        if (canShow(m_iniPath))
        {
            return;
        }

        raise();
        show();
    }

}

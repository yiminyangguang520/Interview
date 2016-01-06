#ifndef GLDMASKBOX_H
#define GLDMASKBOX_H

#include "GLDMask.h"
#include "GLDMaskBox_Global.h"

#include <QAction>

namespace GlodonMask
{

    class GLDMASKBOX_EXPORT GLDMaskBox : public QObject
    {
    public:
        explicit GLDMaskBox(QList<QWidget*> wgtList, const QString & xmlPath,
            const QString & iniPath);

        explicit GLDMaskBox(QList<QAction*> actList, const QString & xmlPath,
            const QString & iniPath);

        ~GLDMaskBox();

        /**
        * @brief 设置蒙版颜色
        * @param color
        */
        void setMaskColor(GLDMask::MASKCOLOR color);

        /**
        * @brief 设置蒙版箭头颜色
        * @param color
        */
        void setMaskArrowColor(const QColor& color);

        /**
        * @brief 设置蒙版箭头粗细
        * @param lineWidth
        */
        void setMaskArrowLineWidth(const int lineWidth);

        /**
        * @brief 返回蒙版个数
        * @return
        */
        size_t count();

    private:
        class InnerMaskBoxImpl;
        QScopedPointer<InnerMaskBoxImpl> d;
    };
}

#endif // GLDMASKBOX_H

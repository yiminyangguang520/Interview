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

        explicit GLDMaskBox(const QString & id, const QList<GLDGuideInfo> & guideInfoList);

        ~GLDMaskBox();


        /**
        * @brief 设置需要显示蒙版的widget
        * @param color
        */
        void setMaskedWgts(QList<QWidget*> & wgtList);

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

        /**
        * @brief 获取MaskBox的ID
        * @return
        */
        QString getID();

        /**
        * @brief MaskBox是否已经显示过
        * @return
        */
        bool isMaskBoxShown();

    private slots:
        void setAllMaskShown();

    private:
        class InnerMaskBoxImpl;
        QScopedPointer<InnerMaskBoxImpl> d;
    };
}

#endif // GLDMASKBOX_H

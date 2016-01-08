#ifndef GLDMASKBOXFACTORY_H
#define GLDMASKBOXFACTORY_H

#include "GLDMaskBox.h"
#include "GLDMaskBox_Global.h"

#include <windows.h>

#include <QHash>
#include <QtXml/QDomElement>

namespace GlodonMask
{
    class GLDMASKBOX_EXPORT GLDMaskBoxInfo
    {
    public:
        GLDMaskBoxInfo(const QString& xmlPath);
        ~GLDMaskBoxInfo();

    public:
        /**
         * @brief 创建并显示蒙版
         * @param id        GLDMaskBox ID,与xml文件中MaskBox节点ID一致
         * @param wgtList   需要显示蒙版的widget
         */
        void showMasks(const QString& id, QList<QWidget*> &wgtList);

        /**
         * @brief 创建并显示蒙版
         * @param id        GLDMaskBox ID,与xml文件中MaskBox节点ID一致
         * @param actList   需要显示蒙版的action
         */
        void showMasks(const QString& id, QList<QAction*> &actList);

        /**
         * @brief 设置蒙版颜色
         * @param id      GLDMaskBox ID,与xml文件中MaskBox节点ID一致
         * @param color   颜色枚举值
         */
        void setMaskBoxColor(const QString& id, GLDMask::MASKCOLOR color);

        /**
         * @brief 设置蒙版箭头的颜色
         * @param id      GLDMaskBox ID,与xml文件中MaskBox节点ID一致
         * @param color   箭头颜色
         */
        void setMaskBoxArrowColor(const QString& id, const QColor& color);

        /**
         * @brief 设置蒙版箭头的线宽
         * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
         * @param lineWidth  线条宽度
         */
        void setMaskArrowLineWidth(const QString& id, const int lineWidth);

        /**
         * @brief 将已经显示过的蒙版所在的GLDMaskBox的ID写入到文件
         */
        void writeMaskBoxIDToFile();

    private:
        class InnerMaskBoxInfoImpl;
        QScopedPointer<InnerMaskBoxInfoImpl> d;
    };
}

#endif // GLDMASKBOXFACTORY_H

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
         * @param wgtHash   需要显示蒙版的widget,其中key为xml文件中tip节点的step属性的值,step需按顺序从1开始编号;value为QWidget*
         */
        STATUS showMasks(const QString& id, QHash<int, QWidget*> &wgtHash);

        /**
         * @brief 创建并显示蒙版
         * @param id        GLDMaskBox ID,与xml文件中MaskBox节点ID一致
         * @param actHash   需要显示蒙版的action,其中key为xml文件中tip节点的step属性的值,step需按顺序从1开始编号;value为QAction*
         */
        STATUS showMasks(const QString& id, QHash<int, QAction*> &actHash);

        /**
        * @brief 蒙版是否已经显示过
        * @param id        GLDMaskBox ID,与xml文件中MaskBox节点ID一致
        */
        bool isShown(const QString& id);

        /**
         * @brief 设置蒙版颜色
         * @param id      GLDMaskBox ID,与xml文件中MaskBox节点ID一致
         * @param color   颜色枚举值
         */
        bool setMaskBoxColor(const QString& id, GLDMask::MASKCOLOR color);

        /**
         * @brief 设置蒙版箭头的颜色
         * @param id      GLDMaskBox ID,与xml文件中MaskBox节点ID一致
         * @param color   箭头颜色
         */
        bool setMaskBoxArrowColor(const QString& id, const QColor& color);

        /**
         * @brief 设置蒙版箭头的线宽
         * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
         * @param lineWidth  线条宽度
         */
        bool setMaskBoxArrowLineWidth(const QString& id, const int lineWidth);

        /**
         * @brief 将已经显示过的蒙版所在的GLDMaskBox的ID写入到文件
         */
        bool writeMaskBoxIDToFile();

    private:
        class InnerMaskBoxInfoImpl;
        QScopedPointer<InnerMaskBoxInfoImpl> d;
    };
}

#endif // GLDMASKBOXFACTORY_H

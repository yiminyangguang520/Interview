#ifndef GLDMASKBOXUTIL_H
#define GLDMASKBOXUTIL_H

#include "GLDMaskBox.h"

#include <windows.h>

namespace GlodonMask
{
    /**
    * @brief 初始化操作
    * @param xmlPath    xml文件路径
    * @return
    */
    GLDMASKBOX_EXPORT bool WINAPI initialize(const QString& xmlPath);

    /**
    * @brief 创建并显示蒙版
    * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
    * @param wgtList    需要显示蒙版的widget
    * @return
    */
    GLDMASKBOX_EXPORT int WINAPI showMasks(const QString& id, QList<QWidget*> &wgtList);

    /**
    * @brief 创建并显示蒙版
    * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
    * @param wgtList    需要显示蒙版的QAction
    * @return
    */
    GLDMASKBOX_EXPORT int WINAPI showMasks(const QString& id, QList<QAction*> &actList);

    /**
    * @brief 为MaskBox中的Mask设置蒙版颜色
    * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
    * @param color      颜色
    * @return
    */
    GLDMASKBOX_EXPORT bool WINAPI setMaskBoxColor(const QString& id, GLDMask::MASKCOLOR color);

    /**
    * @brief 为MaskBox中的Mask设置箭头颜色
    * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
    * @param color      颜色
    * @return
    */
    GLDMASKBOX_EXPORT bool WINAPI setMaskBoxArrowColor(const QString& id, const QColor& color);

    /**
    * @brief 为MaskBox中的Mask设置箭头宽度
    * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
    * @param lineWidth
    * @return
    */
    GLDMASKBOX_EXPORT bool WINAPI setMaskArrowLineWidth(const QString& id, const int lineWidth);

    /**
    * @brief 进行持久化操作,在应用程序关闭后调用
    * @return
    */
    GLDMASKBOX_EXPORT bool WINAPI unInitialize();
}
#endif // GLDMASKBOXUTIL_H

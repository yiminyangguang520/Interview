#ifndef GLDMASKBOXUTIL_H
#define GLDMASKBOXUTIL_H

#include "GLDMaskBoxInfo.h"

#include <windows.h>

namespace GlodonMask
{
    /**
    * @brief 初始化操作
    * @param xmlPath    xml文件路径
    * @return           初始化成功返回true,否则返回false
    */
    GLDMASKBOX_EXPORT bool WINAPI initialize(const QString& xmlPath);

    /**
    * @brief 创建并显示蒙版
    * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
    * @param wgtList    需要显示蒙版的widget,其中key为xml文件中tip节点的step属性的值,step需按顺序从1开始编号;value为QWidget*
    * @return           显示成功返回SUCESS;xml文件中ID为id的GLDMaskBox下的tip节点个数与wgtHash个数不一致,返回UNMATCHED;否则返回FAILURE
    */
    GLDMASKBOX_EXPORT STATUS WINAPI showMasks(const QString& id, QHash<int, QWidget*> &wgtHash);

    /**
    * @brief 创建并显示蒙版
    * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
    * @param wgtList    需要显示蒙版的QAction,其中key为xml文件中tip节点的step属性的值,step需按顺序从1开始编号;value为QAction*
    * @return           显示成功返回SUCESS;xml文件中ID为id的GLDMaskBox下的tip节点个数与wgtHash个数不一致,返回UNMATCHED;否则返回FAILURE
    */
    GLDMASKBOX_EXPORT STATUS WINAPI showMasks(const QString& id, QHash<int, QAction*> &actHash);

    /**
    * @brief 蒙版是否显示过
    * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
    * @return           显示过则返回true,否则返回false
    */
    GLDMASKBOX_EXPORT bool WINAPI isShown(const QString& id);

    /**
    * @brief 为MaskBox中的Mask设置蒙版颜色
    * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
    * @param color      颜色
    * @return           设置成功返回true,否则返回false
    */
    GLDMASKBOX_EXPORT bool WINAPI setMaskBoxColor(const QString& id, GLDMask::MASKCOLOR color);

    /**
    * @brief 为MaskBox中的Mask设置箭头颜色
    * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
    * @param color      颜色
    * @return           设置成功返回true,否则返回false
    */
    GLDMASKBOX_EXPORT bool WINAPI setMaskBoxArrowColor(const QString& id, const QColor& color);

    /**
    * @brief 为MaskBox中的Mask设置箭头宽度
    * @param id         GLDMaskBox ID,与xml文件中MaskBox节点ID一致
    * @param lineWidth  线宽
    * @return           设置成功返回true,否则返回false
    */
    GLDMASKBOX_EXPORT bool WINAPI setMaskBoxArrowLineWidth(const QString& id, const int lineWidth);

    /**
    * @brief  进行持久化操作,在应用程序关闭后调用
    * @return 成功返回true,否则返回false
    */
    GLDMASKBOX_EXPORT bool WINAPI unInitialize();
}
#endif // GLDMASKBOXUTIL_H

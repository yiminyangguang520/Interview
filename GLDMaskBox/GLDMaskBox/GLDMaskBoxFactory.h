#ifndef GLDMASKBOXFACTORY_H
#define GLDMASKBOXFACTORY_H

#include "GLDMaskBox.h"

#include <windows.h>

#include <QHash>
#include <QStringList>
#include <QtXml/QDomElement>

namespace GlodonMask
{
    GLDMASKBOX_EXPORT bool WINAPI Initialize(const QString& xmlPath);

    GLDMASKBOX_EXPORT int WINAPI showMasks(const QString& id, QList<QWidget*> &wgtList);

    GLDMASKBOX_EXPORT bool WINAPI UnInitialize();

    class GLDMaskBoxFactory
    {
    public:
        GLDMaskBoxFactory(const QString& xmlPath);
        ~GLDMaskBoxFactory();

    public:
        void showMasks(const QString& id, QList<QWidget*> &wgtList);
        void showMasks(const QString& id, QList<QAction*> &actList);
        void writeToFile();

    private:
        /**
         * @brief 解析XML文件
         * @param xmlPath
         */
        void doParseXML(const QString& xmlPath);

        /**
         * @brief 解析MaskBox中的一组提示信息
         * @param tipList
         * @param guideInfoList
         */
        void doParseMaskBoxTipInfos(QDomNodeList &tipList, QList<GLDTipInfo> &guideInfoList);

        /**
         * @brief 解析提示信息中的单个Item(hint、close、next)
         * @param element
         * @return
         */
        GLDTipInfoItem doParseTipInfoItem(QDomElement &element);

        /**
         * @brief 解析ini文件,该文件中存放已经显示过的MaskBox,对已经显示过的MaskBox进行持久化操作
         * @param iniPath
         * @param shownMaskBoxIDList
         */
        void parseIniFile(const QString& iniPath, QStringList& shownMaskBoxIDList);

        /**
         * @brief 将wgtList中的Menu转换为button,并添加到list中
         * @param wgtList
         */
        void menuToBtn(QList<QWidget*> & wgtList);

        /**
        * @brief 将action相关的ToolButton形式的widget添加到list中
        * @return
        */
        QList<QWidget*> actionToBtn(QList<QAction*> & actList);

        /**
         * @brief 将需要显示蒙版的widget添加到GLDMaskBox中
         * @param id
         * @param wgtList
         */
        void setWidgets(const QString& id, QList<QWidget*> &wgtList);

    private:
        QHash<QString, GLDMaskBox*> m_maskBoxHash;
    };
}

#endif // GLDMASKBOXFACTORY_H
#pragma once

#include "GLDMaskBox.h"

#include <QHash>
#include <QObject>
#include <QStringList>
#include <QtXml/QDomElement>

namespace GlodonMask
{
    class GLDMASKBOX_EXPORT GLDMaskBoxFactory : public QObject
    {
        Q_OBJECT

    public:
        void initialize(const QString& xmlPath);
        void showMasks(const QString& id, QList<QWidget*> &wgtList);
        void showMasks(const QString& id, QList<QAction*> &actList);

    public:
        GLDMaskBoxFactory(QObject* parent = nullptr);
        virtual ~GLDMaskBoxFactory();

    private:
        void parseXML(const QString& xmlPath);

        void doParseMaskBoxTipInfos(QDomNodeList &tipList, QList<GLDGuideInfo> &guideInfoList);

        GLDGuideInfoItem doParseTipInfoItem(QDomElement &element);

        void parseIniFile(const QString& iniPath, QStringList& shownMaskBoxIDList);

        void menuToBtn(QList<QWidget*> & wgtList);

        /**
        * @brief 将action相关的ToolButton形式的widget添加到list中
        * @return
        */
        QList<QWidget*> actionToBtn(QList<QAction*> & actList);

    private:
        QHash<QString, GLDMaskBox*> m_maskBoxHash;
    };
}

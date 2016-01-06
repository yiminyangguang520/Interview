#pragma once

#include "GLDMaskBox.h"

#include <QHash>
#include <QObject>
#include <QStringList>
#include <QtXml/QDomElement>

namespace GlodonMask
{
    class GLDMaskBoxFactory : public QObject
    {
        Q_OBJECT

    public:
        void initialize(const QString& xmlPath);

    protected:
        GLDMaskBoxFactory(QObject* parent = nullptr);
        virtual ~GLDMaskBoxFactory();

    private:
        void parseXML(const QString& xmlPath);
        GLDMaskBox parseNodeItem(QDomElement &element);
        GLDGuideInfoItem parseTipNodeItem(QDomElement &element);
        void parseIniFile(const QString& iniPath, QStringList& shownMaskBoxIDList);

    private:
        QHash<QString, GLDMaskBox> m_maskBoxHash;
    };
}
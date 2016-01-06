#include "GLDMaskBoxFactory.h"
#include "GLDGetSystemParams.h"
#include <QFile>

namespace GlodonMask
{
    void GLDMaskBoxFactory::initialize(const QString& xmlPath)
    {
        parseXML(xmlPath);
    }

    GLDMaskBoxFactory::GLDMaskBoxFactory(QObject* parent)
        : QObject(parent)
    {

    }

    GLDMaskBoxFactory::~GLDMaskBoxFactory()
    {

    }

    void GLDMaskBoxFactory::parseXML(const QString& xmlPath)
    {
        QFile file(xmlPath);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            return;
        }

        QString strError;
        QDomDocument document;
        int errLin = 0, errCol = 0;

        if (!document.setContent(&file, false, &strError, &errLin, &errCol))
        {
            return;
        }

        if (document.isNull())
        {
            return;
        }

        QDomElement root = document.documentElement();
        QString elementTagName = root.firstChildElement().tagName();
        QDomNodeList nodeList = root.elementsByTagName(elementTagName);

        QStringList shownBoxID;
        parseIniFile(iniPath, shownBoxID);

        const int count = nodeList.size();
        for (int i = 0; i < count; ++i)
        {
            GLDGuideInfo guideInfo;

            QString boxID;
            if (nodeList.at(i).toElement().hasAttribute("id"))
            {
                boxID = nodeList.at(i).toElement().attribute("id");
            }

            if (shownBoxID.contains(boxID))
            {
                continue;
            }

            int tipOrder;
            QString tip = nodeList.at(i).firstChildElement().tagName();
            QDomNodeList tipList = nodeList.at(i).toElement().elementsByTagName(tip);

            QList<GLDGuideInfo> guideInfoList;
            for (int i = 0; i < tipList.size(); ++i)
            {
                if (tipList.at(i).toElement().hasAttribute("order"))
                {
                    tipOrder = tipDom.attribute("order").toInt();
                }

                QDomElement tipDom = tipList.at(i).firstChildElement();

                GLDGuideInfo guideInfo;

                while(!tipDom.isNull())
                {
                    GLDGuideInfoItem guideInfoItem = parseTipNodeItem(tipDom);

                    if (tipDom.tagName() == "hint")
                    {
                        guideInfo.m_maskWidgetItem = guideInfoItem;
                    }
                    else if (tipDom.tagName() == "close")
                    {
                        guideInfo.m_closeButtonItem = guideInfoItem;
                    }
                    else if (tipDom.tagName() == "next")
                    {
                        guideInfo.m_nextButtonItem = guideInfoItem;
                    }

                    tipDom = tipDom.nextSiblingElement();
                }

                guideInfoList.append(guideInfo);
            }
        }
    }

    GlodonMask::GLDMaskBox GLDMaskBoxFactory::parseNodeItem(QDomElement &element)
    {

    }

    GlodonMask::GLDGuideInfoItem GLDMaskBoxFactory::parseTipNodeItem(QDomElement &element)
    {
        int width, height, leftXpos, leftYpos;
        QString normalImage, hoverImage, pressedImage;

        if (element.hasAttribute("width"))
        {
            width = element.attributeNode("width").value().toInt();
        }

        if (element.hasAttribute("height"))
        {
            height = element.attributeNode("height").value().toInt();
        }

        if (element.hasAttribute("normalimage"))
        {
            normalImage = element.attributeNode("normalimage").value();
            replaceAllParams(normalImage);
        }

        if (element.hasAttribute("hoverimage"))
        {
            hoverImage = element.attributeNode("hoverimage").value();
            replaceAllParams(hoverImage);
        }

        if (element.hasAttribute("pressedimage"))
        {
            pressedImage = element.attributeNode("pressedimage").value();
            replaceAllParams(pressedImage);
        }

        if (element.hasAttribute("leftXpos"))
        {
            leftXpos = element.attributeNode("leftXpos").value().toInt();
        }

        if (element.hasAttribute("leftYpos"))
        {
            leftYpos = element.attributeNode("leftYpos").value().toInt();
        }

        return GLDGuideInfoItem(width, height, leftXpos, leftYpos,
            normalImage, hoverImage, pressedImage);
    }

    void GLDMaskBoxFactory::parseIniFile(const QString& iniPath, QStringList& shownMaskBoxIDList)
    {
        QFile file(iniPath);

        if (!file.open(QFile::ReadOnly))
        {
            file.close();
            return;
        }

        shownMaskBoxIDList = QString::fromLatin1(file.readAll()).split(",");

        file.close();
    }
}

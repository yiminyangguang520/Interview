#include "GLDMaskBoxFactory.h"
#include "GLDGetSystemParams.h"

#include <assert.h>

#include <QDir>
#include <QFile>
#include <QMenu>
#include <QTextStream>
#include <QToolButton>
#include <iostream>

using namespace std;

const QString iniPath = QDir::tempPath() + "/GLDMaskBox.ini";

namespace GlodonMask
{
    static shared_ptr<GLDMaskBoxFactory> g_GLDMaskBoxFactory;

    bool WINAPI Initialize(const QString& xmlPath)
    {
        assert(!g_GLDMaskBoxFactory);
        g_GLDMaskBoxFactory.reset(new GLDMaskBoxFactory(xmlPath));
        return true;
    }

    bool WINAPI UnInitialize()
    {
        g_GLDMaskBoxFactory->writeToFile();
        return true;
    }

    int WINAPI showMasks(const QString& id, QList<QWidget*> &wgtList)
    {
        g_GLDMaskBoxFactory->showMasks(id, wgtList);
        return 1;
    }

    void GLDMaskBoxFactory::showMasks(const QString& id, QList<QWidget*> &wgtList)
    {
        menuToBtn(wgtList);

        setWidgets(id, wgtList);
    }

    void GLDMaskBoxFactory::showMasks(const QString& id, QList<QAction*> &actList)
    {
        QList<QWidget*> btnList = actionToBtn(actList);

        setWidgets(id, btnList);
    }

    GLDMaskBoxFactory::GLDMaskBoxFactory(const QString& xmlPath)
    {
        doParseXML(xmlPath);
    }

    GLDMaskBoxFactory::~GLDMaskBoxFactory()
    {

    }


    void GLDMaskBoxFactory::doParseMaskBoxTipInfos(QDomNodeList &tipList, QList<GLDTipInfo> &guideInfoList)
    {
        for (int i = 0; i < tipList.size(); ++i)
        {
            int tipOrder;
            if (tipList.at(i).toElement().hasAttribute("order"))
            {
                tipOrder = tipList.at(i).toElement().attribute("order").toInt();
            }

            GLDTipInfo guideInfo;

            QDomElement tipDom = tipList.at(i).firstChildElement();
            while (!tipDom.isNull())
            {
                GLDTipInfoItem guideInfoItem = doParseTipInfoItem(tipDom);

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

    void GLDMaskBoxFactory::doParseXML(const QString& xmlPath)
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

        for (int i = 0; i < nodeList.size(); ++i)
        {
            QString boxID;

            if (nodeList.at(i).toElement().hasAttribute("id"))
            {
                boxID = nodeList.at(i).toElement().attribute("id");
            }

            if (shownBoxID.contains(boxID))
            {
                continue;
            }

            QString tip = nodeList.at(i).firstChildElement().tagName();
            QDomNodeList tipList = nodeList.at(i).toElement().elementsByTagName(tip);

            QList<GLDTipInfo> guideInfoList;
            doParseMaskBoxTipInfos(tipList, guideInfoList);

            m_maskBoxHash.insert(boxID, new GLDMaskBox(boxID, guideInfoList));
        }
    }

    GlodonMask::GLDTipInfoItem GLDMaskBoxFactory::doParseTipInfoItem(QDomElement &element)
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

        return GLDTipInfoItem(width, height, leftXpos, leftYpos,
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

    void GLDMaskBoxFactory::writeToFile()
    {
        QFile file(iniPath);

        if (file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QTextStream in(&file);

            QHash<QString, GLDMaskBox*>::iterator iter = m_maskBoxHash.begin();

            for (; iter != m_maskBoxHash.end(); ++iter)
            {
                if (iter.value()->isMaskBoxShown())
                {
                    in << iter.key() << ",";
                }
            }

        }

        file.close();
    }

    void GLDMaskBoxFactory::menuToBtn(QList<QWidget*> & wgtList)
    {
        for (int index = 0; index < wgtList.count(); ++index)
        {
            QWidget* widget = wgtList[index];

            if (!widget)
            {
                break;
            }

            if (QMenu* pMenu = dynamic_cast<QMenu*>(widget))
            {
                foreach(QWidget* pWidget, pMenu->menuAction()->associatedWidgets())
                {
                    if (QToolButton* pToolButton = dynamic_cast<QToolButton*>(pWidget))
                    {
                        widget = pToolButton;
                    }
                }

                wgtList.removeAt(index);
                wgtList.insert(index, widget);
            }
        }
    }

    QList<QWidget*> GLDMaskBoxFactory::actionToBtn(QList<QAction*> & actList)
    {
        QList<QWidget *> wgtList;

        foreach(QAction* pAct, actList)
        {
            foreach(QWidget* pWidget, pAct->associatedWidgets())
            {
                if (QToolButton* pToolButton = dynamic_cast<QToolButton*>(pWidget))
                {
                    wgtList.append(pToolButton);
                }
            }
        }

        return wgtList;
    }

    void GLDMaskBoxFactory::setWidgets(const QString& id, QList<QWidget*> &wgtList)
    {
        QHash<QString, GLDMaskBox*>::iterator iter = m_maskBoxHash.begin();

        for (; iter != m_maskBoxHash.end(); ++iter)
        {
            if (iter.key() == id)
            {
                iter.value()->setMaskedWgts(wgtList);
            }
        }
    }
}

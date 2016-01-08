#include "GLDMaskBoxFactory.h"
#include "GLDGetSysParams.h"

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

    bool WINAPI initialize(const QString& xmlPath)
    {
        assert(!g_GLDMaskBoxFactory);
        g_GLDMaskBoxFactory.reset(new GLDMaskBoxFactory(xmlPath));
        return true;
    }

    bool WINAPI unInitialize()
    {
        g_GLDMaskBoxFactory->writeMaskBoxIDToFile();
        return true;
    }

    int WINAPI showMasks(const QString& id, QList<QWidget*> &wgtList)
    {
        g_GLDMaskBoxFactory->showMasks(id, wgtList);
        return 1;
    }

    bool WINAPI setMaskBoxColor(const QString & id, GLDMask::MASKCOLOR color)
    {
        g_GLDMaskBoxFactory->setMaskBoxColor(id, color);
        return true;
    }

    bool WINAPI setMaskBoxArrowColor(const QString& id, const QColor& color)
    {
        g_GLDMaskBoxFactory->setMaskBoxArrowColor(id, color);
        return true;
    }

    bool WINAPI setMaskArrowLineWidth(const QString& id, const int lineWidth)
    {
        g_GLDMaskBoxFactory->setMaskArrowLineWidth(id, lineWidth);
        return true;
    }

    class GLDMaskBoxFactory::InnerMaskBoxFactoryImpl
    {
    public:
        InnerMaskBoxFactoryImpl(const QString & xmlPath)
        {
            doParseXML(xmlPath);
        }

        ~InnerMaskBoxFactoryImpl()
        {

        }


        /**
        * @brief 解析XML文件
        * @param xmlPath
        */
        void doParseXML(const QString& xmlPath)
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

        /**
        * @brief 解析MaskBox中的一组提示信息
        * @param tipList
        * @param guideInfoList
        */
        void doParseMaskBoxTipInfos(QDomNodeList &tipList, QList<GLDTipInfo> &guideInfoList)
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

        /**
        * @brief 解析提示信息中的单个Item(hint、close、next)
        * @param element
        * @return
        */
        GLDTipInfoItem doParseTipInfoItem(QDomElement &element)
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

        /**
        * @brief 解析ini文件,该文件中存放已经显示过的MaskBox,对已经显示过的MaskBox进行持久化操作
        * @param iniPath
        * @param shownMaskBoxIDList
        */
        void parseIniFile(const QString& iniPath, QStringList& shownMaskBoxIDList)
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

        /**
        * @brief 将wgtList中的Menu转换为button,并添加到list中
        * @param wgtList
        */
        void menuToBtn(QList<QWidget*> & wgtList)
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

        /**
        * @brief 将action相关的ToolButton形式的widget添加到list中
        * @return
        */
        QList<QWidget*> actionToBtn(QList<QAction*> & actList)
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

        /**
        * @brief 将需要显示蒙版的widget添加到GLDMaskBox中
        * @param id
        * @param wgtList
        */
        void setWidgets(const QString& id, QList<QWidget*> &wgtList)
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

        void writeMaskBoxIDToFile()
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

        QHash<QString, GLDMaskBox*> m_maskBoxHash;

    private:
        Q_DISABLE_COPY(InnerMaskBoxFactoryImpl)
    };


    GLDMaskBoxFactory::GLDMaskBoxFactory(const QString& xmlPath)
        : d(new InnerMaskBoxFactoryImpl(xmlPath))
    {

    }

    GLDMaskBoxFactory::~GLDMaskBoxFactory()
    {

    }

    void GLDMaskBoxFactory::showMasks(const QString& id, QList<QWidget*> &wgtList)
    {
        d->menuToBtn(wgtList);

        d->setWidgets(id, wgtList);
    }

    void GLDMaskBoxFactory::showMasks(const QString& id, QList<QAction*> &actList)
    {
        QList<QWidget*> btnList = d->actionToBtn(actList);

        d->setWidgets(id, btnList);
    }

    void GLDMaskBoxFactory::writeMaskBoxIDToFile()
    {
        d->writeMaskBoxIDToFile();
    }

    void GLDMaskBoxFactory::setMaskBoxColor(const QString& id, GLDMask::MASKCOLOR color)
    {
        if (GLDMaskBox* pMaskBox = d->m_maskBoxHash.value(id))
        {
            pMaskBox->setMaskColor(color);
        }
    }

    void GLDMaskBoxFactory::setMaskBoxArrowColor(const QString& id, const QColor& color)
    {
        if (GLDMaskBox* pMaskBox = d->m_maskBoxHash.value(id))
        {
            pMaskBox->setMaskArrowColor(color);
        }
    }

    void GLDMaskBoxFactory::setMaskArrowLineWidth(const QString& id, const int lineWidth)
    {
        if (GLDMaskBox* pMaskBox = d->m_maskBoxHash.value(id))
        {
            pMaskBox->setMaskArrowLineWidth(lineWidth);
        }
    }

}

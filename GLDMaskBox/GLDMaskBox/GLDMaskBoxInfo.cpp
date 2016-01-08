#include "GLDMaskBoxInfo.h"
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
    static shared_ptr<GLDMaskBoxInfo> g_GLDMaskBoxFactory;

    bool WINAPI initialize(const QString& xmlPath)
    {
        assert(!g_GLDMaskBoxFactory);
        g_GLDMaskBoxFactory.reset(new GLDMaskBoxInfo(xmlPath));
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

    class GLDMaskBoxInfo::InnerMaskBoxFactoryImpl
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
        * @brief ����XML�ļ�
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
        * @brief ����MaskBox�е�һ����ʾ��Ϣ
        * @param tipList
        * @param guideInfoList
        */
        void doParseMaskBoxTipInfos(QDomNodeList &tipList, QList<GLDTipInfo> &guideInfoList)
        {
            for (int i = 0; i < tipList.size(); ++i)
            {
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
        * @brief ������ʾ��Ϣ�еĵ���Item(hint��close��next)
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
        * @brief ����ini�ļ�,���ļ��д���Ѿ���ʾ����MaskBox,���Ѿ���ʾ����MaskBox���г־û�����
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
        * @brief ��wgtList�е�Menuת��Ϊbutton,����ӵ�list��
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
        * @brief ��action��ص�ToolButton��ʽ��widget��ӵ�list��
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
        * @brief ����Ҫ��ʾ�ɰ��widget��ӵ�GLDMaskBox��
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


    GLDMaskBoxInfo::GLDMaskBoxInfo(const QString& xmlPath)
        : d(new InnerMaskBoxFactoryImpl(xmlPath))
    {

    }

    GLDMaskBoxInfo::~GLDMaskBoxInfo()
    {

    }

    void GLDMaskBoxInfo::showMasks(const QString& id, QList<QWidget*> &wgtList)
    {
        d->menuToBtn(wgtList);

        d->setWidgets(id, wgtList);
    }

    void GLDMaskBoxInfo::showMasks(const QString& id, QList<QAction*> &actList)
    {
        QList<QWidget*> btnList = d->actionToBtn(actList);

        d->setWidgets(id, btnList);
    }

    void GLDMaskBoxInfo::writeMaskBoxIDToFile()
    {
        d->writeMaskBoxIDToFile();
    }

    void GLDMaskBoxInfo::setMaskBoxColor(const QString& id, GLDMask::MASKCOLOR color)
    {
        if (GLDMaskBox* pMaskBox = d->m_maskBoxHash.value(id))
        {
            pMaskBox->setMaskColor(color);
        }
    }

    void GLDMaskBoxInfo::setMaskBoxArrowColor(const QString& id, const QColor& color)
    {
        if (GLDMaskBox* pMaskBox = d->m_maskBoxHash.value(id))
        {
            pMaskBox->setMaskArrowColor(color);
        }
    }

    void GLDMaskBoxInfo::setMaskArrowLineWidth(const QString& id, const int lineWidth)
    {
        if (GLDMaskBox* pMaskBox = d->m_maskBoxHash.value(id))
        {
            pMaskBox->setMaskArrowLineWidth(lineWidth);
        }
    }

}

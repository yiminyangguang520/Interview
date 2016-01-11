#include "GLDMaskBoxInfo.h"
#include "GLDGetSysParams.h"

#include <QDir>
#include <QFile>
#include <QMenu>
#include <QTextStream>
#include <QToolButton>

const QString g_iniPath = QDir::tempPath() + "/GLDMaskBox.ini";

namespace GlodonMask
{
    class GLDMaskBoxInfo::InnerMaskBoxInfoImpl
    {
    public:
        InnerMaskBoxInfoImpl(const QString & xmlPath)
        {
            doParseXML(xmlPath);
        }

        ~InnerMaskBoxInfoImpl()
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
            parseIniFile(g_iniPath, shownBoxID);

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
            int width, height, X, Y;
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
                normalImage = element.attributeNode("normalimage").value().replace("\\", "/");
                replaceAllParams(normalImage);
            }

            if (element.hasAttribute("hoverimage"))
            {
                hoverImage = element.attributeNode("hoverimage").value().replace("\\", "/");
                replaceAllParams(hoverImage);
            }

            if (element.hasAttribute("pressedimage"))
            {
                pressedImage = element.attributeNode("pressedimage").value().replace("\\", "/");
                replaceAllParams(pressedImage);
            }

            if (element.hasAttribute("X"))
            {
                X = element.attributeNode("X").value().toInt();
            }

            if (element.hasAttribute("Y"))
            {
                Y = element.attributeNode("Y").value().toInt();
            }

            return GLDTipInfoItem(width, height, X, Y,
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

            QString str = QString::fromLatin1(file.readAll());
            shownMaskBoxIDList = str.remove(str.length() - 1, 1).split(",");

            file.close();
        }

        /**
        * @brief 将wgtList中的Menu转换为button,并添加到list中
        * @param wgtList
        */
        // todo
        QList<QWidget*> menuToAssociatedWgt(QList<QWidget*> & wgtList)
        {
            QList<QWidget*> associatedWgtList;

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
                }

                associatedWgtList.append(widget);
            }

            return associatedWgtList;
        }

        /**
        * @brief 将action相关的ToolButton形式的widget添加到list中
        * @return
        */
        QList<QWidget*> actionToAssociatedWgt(QList<QAction*> & actList)
        {
            QList<QWidget *> wgtList;

            foreach(QAction* pAct, actList)
            {
                if (!pAct)
                {
                    break;
                }

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
        STATUS setWidgets(const QString& id, QList<QWidget*> &wgtList)
        {
            // todo
            QHash<QString, GLDMaskBox*>::iterator iter = m_maskBoxHash.find(id);

            if ((*iter))
            {
                return (*iter)->setMaskedWgts(wgtList);
            }

            return FAILURE;
        }

        /**
        * @brief 将已经显示过的Mask所在的GLDMaskBox的ID写入到文件
        */
        void writeMaskBoxIDToFile(const QString& iniPath = g_iniPath)
        {
            QFile file(iniPath);

            if (file.open(QIODevice::WriteOnly | QIODevice::Append))
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
        Q_DISABLE_COPY(InnerMaskBoxInfoImpl)
    };


    GLDMaskBoxInfo::GLDMaskBoxInfo(const QString& xmlPath)
        : d(new InnerMaskBoxInfoImpl(xmlPath))
    {

    }

    GLDMaskBoxInfo::~GLDMaskBoxInfo()
    {

    }

    STATUS GLDMaskBoxInfo::showMasks(const QString& id, QList<QWidget*> &wgtList)
    {
        // todo 返回状态码
        if (d->m_maskBoxHash.keys().contains(id))
        {
            QList<QWidget*> associatedWgtList = d->menuToAssociatedWgt(wgtList);
            return d->setWidgets(id, associatedWgtList);
        }

        return FAILURE;
    }

    STATUS GLDMaskBoxInfo::showMasks(const QString& id, QList<QAction*> &actList)
    {
        if (d->m_maskBoxHash.keys().contains(id))
        {
            QList<QWidget*> associatedWgtList = d->actionToAssociatedWgt(actList);
            return d->setWidgets(id, associatedWgtList);
        }

        return FAILURE;
    }

    void GLDMaskBoxInfo::writeMaskBoxIDToFile()
    {
        d->writeMaskBoxIDToFile();
    }

    bool GLDMaskBoxInfo::setMaskBoxColor(const QString& id, GLDMask::MASKCOLOR color)
    {
        GLDMaskBox* pMaskBox = d->m_maskBoxHash.value(id);

        if (pMaskBox && !pMaskBox->isMaskBoxShown())
        {
            pMaskBox->setMaskColor(color);

            return true;
        }

        return false;
    }

    bool GLDMaskBoxInfo::setMaskBoxArrowColor(const QString& id, const QColor& color)
    {
        // todo
        GLDMaskBox* pMaskBox = d->m_maskBoxHash.value(id);

        if (pMaskBox && !pMaskBox->isMaskBoxShown())
        {
            pMaskBox->setMaskArrowColor(color);

            return true;
        }

        return false;
    }

    bool GLDMaskBoxInfo::setMaskBoxArrowLineWidth(const QString& id, const int lineWidth)
    {
        GLDMaskBox* pMaskBox = d->m_maskBoxHash.value(id);

        if (pMaskBox && !pMaskBox->isMaskBoxShown())
        {
            pMaskBox->setMaskArrowLineWidth(lineWidth);

            return true;
        }

        return false;
    }

}

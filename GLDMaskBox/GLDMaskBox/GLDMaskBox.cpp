#include "GLDMaskBox.h"

#include <QFile>
#include <QMenu>
#include <QToolButton>

namespace GlodonMask
{
    class GLDMaskBox::InnerMaskBoxImpl
    {
    public:
        InnerMaskBoxImpl(QList<QWidget *> wgtList, const QString &xmlPath, const QString & iniPath)
            : m_step(0)
        {
            init(wgtList, xmlPath, iniPath);
        }

        InnerMaskBoxImpl(QList<QAction *> actList, const QString &xmlPath, const QString & iniPath)
            : m_step(0)
        {
            QList<QWidget*> wgtList = actToWidget(actList);

            init(wgtList, xmlPath, iniPath);
        }

        InnerMaskBoxImpl(const QString & id, const QList<GLDGuideInfo> & guideInfoList)
            : m_step(0)
            , m_maskBoxID(id)
            , m_bIsShown(false)
        {
            foreach(GLDGuideInfo guideInfo, guideInfoList)
            {
                NEXTCLICKEDCALLBACK next = std::bind(&GLDMaskBox::InnerMaskBoxImpl::onNextBtnClicked, this);
                m_tipWgtList.append(new GLDTipWidget(guideInfo, next));
            }
        }

        ~InnerMaskBoxImpl()
        {

        }


        /**
        * @brief 初始化提示信息和蒙版
        * @param wgtList
        * @param xmlPath
        * @param iniPath
        */
        void init(QList<QWidget *> wgtList, const QString &xmlPath, const QString& iniPath);

        /**
        * @brief 初始化蒙版参数列表
        * @param wgtList
        */
        void initMaskList(QList<QWidget*> & wgtList, QList<GLDTipWidget*> & tipList, const QString &iniPath);

        /**
        * @brief 初始化提示信息列表
        * @return
        */
        void initTipInfo(const QString & xmlPath, QList<GLDTipWidget *> & tipList);

        /**
        * @brief 解析xml文件
        * @return
        */
        void parseXML(const QString & xmlPath, QList<GLDGuideInfo> & guideInfoList);

        /**
        * @brief 解析xml文件中的一个节点项
        * @return
        */
        GLDGuideInfoItem parseNodeItem(QDomElement &element);

        /**
        * @brief 将action相关的ToolButton形式的widget添加到list中
        * @return
        */
        QList<QWidget* > actToWidget(QList<QAction* > actList);

        /**
        * @brief TipWidget上下一个按钮被点击触发的操作
        * @return
        */
        void onNextBtnClicked();


        int                     m_step;           // 第几个蒙版
        QList<GLDMask*>         m_maskList;       // 蒙版列表
        QString                 m_maskBoxID;
        QList<GLDTipWidget*>    m_tipWgtList;
        bool                    m_bIsShown;       // MaskBox里的Mask是否已显示过

    private:
        Q_DISABLE_COPY(InnerMaskBoxImpl)
    };


    void GLDMaskBox::InnerMaskBoxImpl::init(QList<QWidget *> wgtList,
        const QString &xmlPath,
        const QString& iniPath)
    {
        QList<GLDTipWidget*> tipWgtList;
        initTipInfo(xmlPath, tipWgtList);
        initMaskList(wgtList, tipWgtList, iniPath);
    }

    void GLDMaskBox::InnerMaskBoxImpl::initMaskList(QList<QWidget *> & wgtList,
        QList<GLDTipWidget *> &tipList,
        const QString & iniPath)
    {
        Q_ASSERT(wgtList.size() == tipList.size());

        GLDMask* pTip = nullptr;

        const int count = wgtList.size();
        for (int index = 0; index < count; ++index)
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

            QWidget* pWidget = topLevelParentWidget(widget);
            pTip = new GLDMask(widget, tipList[index], index, pWidget);
            pTip->hide();

            m_maskList.append(pTip);
        }
    }

    void GLDMaskBox::InnerMaskBoxImpl::initTipInfo(const QString & xmlPath,
        QList<GLDTipWidget*>& tipList)
    {
        QList<GLDGuideInfo> infoList;

        parseXML(xmlPath, infoList);

        GLDTipWidget* pTipWgt = nullptr;

        foreach(GLDGuideInfo guideInfo, infoList)
        {
            NEXTCLICKEDCALLBACK next = std::bind(&GLDMaskBox::InnerMaskBoxImpl::onNextBtnClicked, this);
            pTipWgt = new GLDTipWidget(guideInfo, next);
            tipList.append(pTipWgt);
        }
    }

    void GLDMaskBox::InnerMaskBoxImpl::parseXML(const QString &xmlPath, QList<GLDGuideInfo> &guideInfoList)
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

        const int count = nodeList.size();
        for (int i = 0; i < count; ++i)
        {
            GLDGuideInfo guideInfo;

            QDomElement element = nodeList.item(i).firstChildElement();

            while (!element.isNull())
            {
                GLDGuideInfoItem guideInfoItem = parseNodeItem(element);

                if (element.tagName() == "hint")
                {
                    guideInfo.m_maskWidgetItem = guideInfoItem;
                }
                else if (element.tagName() == "close")
                {
                    guideInfo.m_closeButtonItem = guideInfoItem;
                }
                else if (element.tagName() == "next")
                {
                    guideInfo.m_nextButtonItem = guideInfoItem;
                }

                element = element.nextSiblingElement();
            }

            guideInfoList.append(guideInfo);
        }
    }

    GLDGuideInfoItem GLDMaskBox::InnerMaskBoxImpl::parseNodeItem(QDomElement &element)
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
            //replaceAllParams(normalImage);
        }

        if (element.hasAttribute("hoverimage"))
        {
            hoverImage = element.attributeNode("hoverimage").value();
            //replaceAllParams(hoverImage);
        }

        if (element.hasAttribute("pressedimage"))
        {
            pressedImage = element.attributeNode("pressedimage").value();
            //replaceAllParams(pressedImage);
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

    void GLDMaskBox::InnerMaskBoxImpl::onNextBtnClicked()
    {
        if (m_step == m_maskList.count() - 1)
        {
            m_maskList[m_step]->slotClose();
            return;
        }
        else
        {
            m_maskList[m_step]->close();
        }

        m_step++;
        m_maskList[m_step]->show();
        m_maskList[m_step]->raise();
    }

    GLDMaskBox::GLDMaskBox(const QString & id, const QList<GLDGuideInfo> & guideInfoList)
        : d(new InnerMaskBoxImpl(id, guideInfoList))
    {

    }

    GLDMaskBox::~GLDMaskBox()
    {

    }

    void GLDMaskBox::setMaskedWgts(QList<QWidget*> & wgtList)
    {
        Q_ASSERT(wgtList.size() == d->m_tipWgtList.size());

        GLDMask* pMask = nullptr;

        for (int index = 0; index < wgtList.count(); ++index)
        {
            pMask = new GLDMask(wgtList[index], d->m_tipWgtList[index], index, topLevelParentWidget(wgtList[index]));
            pMask->hide();

            d->m_maskList.append(pMask);
        }

        connect(pMask, &GLDMask::alreadyShow, this, &GLDMaskBox::setAllMaskShown);
    }

    void GLDMaskBox::setMaskColor(GLDMask::MASKCOLOR color)
    {
        const int num = count();

        for (int index = 0; index < num; ++index)
        {
            d->m_maskList[index]->setMaskColor(color);
        }
    }

    void GLDMaskBox::setMaskArrowColor(const QColor &color)
    {
        const int num = count();

        for (int index = 0; index < num; ++index)
        {
            d->m_maskList[index]->setArrowColor(color);
        }
    }

    void GLDMaskBox::setMaskArrowLineWidth(const int lineWidth)
    {
        const int num = count();

        for (int index = 0; index < num; ++index)
        {
            d->m_maskList[index]->setArrowLineWidth(lineWidth);
        }
    }

    size_t GLDMaskBox::count()
    {
        return d->m_maskList.size();
    }

    QString GLDMaskBox::getID()
    {
        return d->m_maskBoxID;
    }

    bool GLDMaskBox::isMaskBoxShown()
    {
        return d->m_bIsShown;
    }

    void GLDMaskBox::setAllMaskShown()
    {
        for (int i = 0; i < d->m_maskList.size(); ++i)
        {
            d->m_maskList[i]->setIsShown(true);
            d->m_bIsShown = true;
        }
    }

}


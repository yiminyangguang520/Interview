#include "GLDMaskBox.h"

#include <QFile>

namespace GlodonMask
{
    class GLDMaskBox::InnerMaskBoxImpl
    {
    public:
        InnerMaskBoxImpl(const QString & id, const QList<GLDTipInfo> & guideInfoList)
            : m_step(0)
            , m_maskBoxID(id)
            , m_bIsShown(false)
        {
            foreach(GLDTipInfo guideInfo, guideInfoList)
            {
                NEXTCLICKEDCALLBACK next = std::bind(&GLDMaskBox::InnerMaskBoxImpl::onNextBtnClicked, this);
                m_tipHash.insert(guideInfo.m_name, new GLDTipWidget(guideInfo, next));
            }
        }

        ~InnerMaskBoxImpl()
        {

        }

        /**
        * @brief TipWidget上下一个按钮被点击触发的操作
        * @return
        */
        void onNextBtnClicked()
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


        int                              m_step;           // 第几个蒙版
        QString                          m_maskBoxID;      // maskBox的ID
        bool                             m_bIsShown;       // MaskBox里的Mask是否已显示过
        QHash<int, GLDTipWidget*>        m_tipHash;      // MaskBox里的提示信息列表
        QList<GLDMask*>                  m_maskList;       // 蒙版列表

    private:
        Q_DISABLE_COPY(InnerMaskBoxImpl)
    };




    GLDMaskBox::GLDMaskBox(const QString & id, const QList<GLDTipInfo> & guideInfoList)
        : d(new InnerMaskBoxImpl(id, guideInfoList))
    {
    
    }

    GLDMaskBox::~GLDMaskBox()
    {

    }

    STATUS GLDMaskBox::setMaskedWgts(QHash<int, QWidget*> & wgtHash)
    {
        QList<QWidget*> wgtList = wgtHash.values();

        if (wgtList.size() != d->m_tipHash.size())
        {
            return UNMATCHED;
        }

        GLDMask* pMask = nullptr;

        QHash<int, QWidget*>::iterator iter = wgtHash.begin();
        for (; iter != wgtHash.end(); ++iter)
        {
            QWidget* pWgt = wgtHash.value(iter.key());
            GLDTipWidget* pTipWgt = d->m_tipHash.value(iter.key());

            if (pWgt && pTipWgt)
            {
                pMask = new GLDMask(pWgt, pTipWgt, iter.key(), topLevelParentWidget(pWgt));
                pMask->hide();

                d->m_maskList.append(pMask);
                connect(pMask, &GLDMask::alreadyShow, this, &GLDMaskBox::setAllMaskIsShown);
            }
        }

        return SUCCESS;
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

    size_t GLDMaskBox::count() const
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

    void GLDMaskBox::setAllMaskIsShown()
    {
        for (int i = 0; i < d->m_maskList.size(); ++i)
        {
            d->m_maskList[i]->setIsShown(true);
            d->m_bIsShown = true;
        }
    }

}


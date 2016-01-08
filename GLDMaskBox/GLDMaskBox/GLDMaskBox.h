#ifndef GLDMASKBOX_H
#define GLDMASKBOX_H

#include "GLDMask.h"
#include "GLDMaskBox_Global.h"

namespace GlodonMask
{
    class GLDMASKBOX_EXPORT GLDMaskBox : public QObject
    {
    public:
        explicit GLDMaskBox(const QString & id, const QList<GLDTipInfo> & guideInfoList);

        ~GLDMaskBox();


        /**
        * @brief ������Ҫ��ʾ�ɰ��widget
        * @param color
        */
        void setMaskedWgts(QList<QWidget*> & wgtList);

        /**
        * @brief �����ɰ���ɫ
        * @param color
        */
        void setMaskColor(GLDMask::MASKCOLOR color);

        /**
        * @brief �����ɰ��ͷ��ɫ
        * @param color
        */
        void setMaskArrowColor(const QColor& color);

        /**
        * @brief �����ɰ��ͷ��ϸ
        * @param lineWidth
        */
        void setMaskArrowLineWidth(const int lineWidth);

        /**
        * @brief �����ɰ����
        * @return
        */
        size_t count() const;

        /**
        * @brief ��ȡMaskBox��ID
        * @return
        */
        QString getID();

        /**
        * @brief MaskBox�Ƿ��Ѿ���ʾ��
        * @return
        */
        bool isMaskBoxShown();

    private slots:
        void setAllMaskShown();

    private:
        class InnerMaskBoxImpl;
        QScopedPointer<InnerMaskBoxImpl> d;
    };
}

#endif // GLDMASKBOX_H

#ifndef GLDMASKBOX_H
#define GLDMASKBOX_H

#include "GLDMask.h"
#include "GLDMaskBox_Global.h"

#include <QAction>

namespace GlodonMask
{

    class GLDMASKBOX_EXPORT GLDMaskBox : public QObject
    {
    public:
        explicit GLDMaskBox(QList<QWidget*> wgtList, const QString & xmlPath,
            const QString & iniPath);

        explicit GLDMaskBox(QList<QAction*> actList, const QString & xmlPath,
            const QString & iniPath);

        ~GLDMaskBox();

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
        size_t count();

    private:
        class InnerMaskBoxImpl;
        QScopedPointer<InnerMaskBoxImpl> d;
    };
}

#endif // GLDMASKBOX_H

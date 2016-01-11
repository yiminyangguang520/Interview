#ifndef GLDMASKBOX_H
#define GLDMASKBOX_H

#include "GLDMask.h"
#include "GLDMaskBox_Global.h"

namespace GlodonMask
{
    enum STATUS
    {
        SUCCESS = 0,
        FAILURE,
        UNMATCHED,
        UNKNOWN
    };

    class GLDMASKBOX_EXPORT GLDMaskBox : public QObject
    {
    public:
        explicit GLDMaskBox(const QString & id, const QList<GLDTipInfo> & guideInfoList);

        ~GLDMaskBox();

        /**
        * @brief 设置需要显示蒙版的widget
        * @param color
        */
        STATUS setMaskedWgts(QHash<int, QWidget*> & wgtHash);

        /**
        * @brief 设置蒙版颜色
        * @param color
        */
        void setMaskColor(GLDMask::MASKCOLOR color);

        /**
        * @brief 设置蒙版箭头颜色
        * @param color
        */
        void setMaskArrowColor(const QColor& color);

        /**
        * @brief 设置蒙版箭头粗细
        * @param lineWidth
        */
        void setMaskArrowLineWidth(const int lineWidth);

        /**
        * @brief 返回蒙版个数
        * @return
        */
        size_t count() const;

        /**
        * @brief 获取MaskBox的ID
        * @return
        */
        QString getID();

        /**
        * @brief MaskBox是否已经显示过
        * @return
        */
        bool isMaskBoxShown();

    private slots:
        void setAllMaskIsShown();

    private:
        class InnerMaskBoxImpl;
        QScopedPointer<InnerMaskBoxImpl> d;
    };
}

#endif // GLDMASKBOX_H

#ifndef GLDMASKBOXFACTORY_H
#define GLDMASKBOXFACTORY_H

#include "GLDMaskBox.h"
#include "GLDMaskBox_Global.h"

#include <windows.h>

#include <QHash>
#include <QtXml/QDomElement>

namespace GlodonMask
{
    class GLDMASKBOX_EXPORT GLDMaskBoxInfo
    {
    public:
        GLDMaskBoxInfo(const QString& xmlPath);
        ~GLDMaskBoxInfo();

    public:
        /**
         * @brief ��������ʾ�ɰ�
         * @param id        GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
         * @param wgtList   ��Ҫ��ʾ�ɰ��widget
         */
        void showMasks(const QString& id, QList<QWidget*> &wgtList);

        /**
         * @brief ��������ʾ�ɰ�
         * @param id        GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
         * @param actList   ��Ҫ��ʾ�ɰ��action
         */
        void showMasks(const QString& id, QList<QAction*> &actList);

        /**
         * @brief �����ɰ���ɫ
         * @param id      GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
         * @param color   ��ɫö��ֵ
         */
        void setMaskBoxColor(const QString& id, GLDMask::MASKCOLOR color);

        /**
         * @brief �����ɰ��ͷ����ɫ
         * @param id      GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
         * @param color   ��ͷ��ɫ
         */
        void setMaskBoxArrowColor(const QString& id, const QColor& color);

        /**
         * @brief �����ɰ��ͷ���߿�
         * @param id         GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
         * @param lineWidth  �������
         */
        void setMaskArrowLineWidth(const QString& id, const int lineWidth);

        /**
         * @brief ���Ѿ���ʾ�����ɰ����ڵ�GLDMaskBox��IDд�뵽�ļ�
         */
        void writeMaskBoxIDToFile();

    private:
        class InnerMaskBoxInfoImpl;
        QScopedPointer<InnerMaskBoxInfoImpl> d;
    };
}

#endif // GLDMASKBOXFACTORY_H

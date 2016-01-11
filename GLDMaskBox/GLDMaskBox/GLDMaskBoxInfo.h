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
         * @param wgtHash   ��Ҫ��ʾ�ɰ��widget,����keyΪxml�ļ���tip�ڵ��step���Ե�ֵ,step�谴˳���1��ʼ���;valueΪQWidget*
         */
        STATUS showMasks(const QString& id, QHash<int, QWidget*> &wgtHash);

        /**
         * @brief ��������ʾ�ɰ�
         * @param id        GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
         * @param actHash   ��Ҫ��ʾ�ɰ��action,����keyΪxml�ļ���tip�ڵ��step���Ե�ֵ,step�谴˳���1��ʼ���;valueΪQAction*
         */
        STATUS showMasks(const QString& id, QHash<int, QAction*> &actHash);

        /**
        * @brief �ɰ��Ƿ��Ѿ���ʾ��
        * @param id        GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
        */
        bool isShown(const QString& id);

        /**
         * @brief �����ɰ���ɫ
         * @param id      GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
         * @param color   ��ɫö��ֵ
         */
        bool setMaskBoxColor(const QString& id, GLDMask::MASKCOLOR color);

        /**
         * @brief �����ɰ��ͷ����ɫ
         * @param id      GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
         * @param color   ��ͷ��ɫ
         */
        bool setMaskBoxArrowColor(const QString& id, const QColor& color);

        /**
         * @brief �����ɰ��ͷ���߿�
         * @param id         GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
         * @param lineWidth  �������
         */
        bool setMaskBoxArrowLineWidth(const QString& id, const int lineWidth);

        /**
         * @brief ���Ѿ���ʾ�����ɰ����ڵ�GLDMaskBox��IDд�뵽�ļ�
         */
        bool writeMaskBoxIDToFile();

    private:
        class InnerMaskBoxInfoImpl;
        QScopedPointer<InnerMaskBoxInfoImpl> d;
    };
}

#endif // GLDMASKBOXFACTORY_H

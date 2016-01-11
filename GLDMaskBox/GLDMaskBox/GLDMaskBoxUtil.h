#ifndef GLDMASKBOXUTIL_H
#define GLDMASKBOXUTIL_H

#include "GLDMaskBoxInfo.h"

#include <windows.h>

namespace GlodonMask
{
    /**
    * @brief ��ʼ������
    * @param xmlPath    xml�ļ�·��
    * @return           ��ʼ���ɹ�����true,���򷵻�false
    */
    GLDMASKBOX_EXPORT bool WINAPI initialize(const QString& xmlPath);

    /**
    * @brief ��������ʾ�ɰ�
    * @param id         GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
    * @param wgtList    ��Ҫ��ʾ�ɰ��widget,����keyΪxml�ļ���tip�ڵ��step���Ե�ֵ,step�谴˳���1��ʼ���;valueΪQWidget*
    * @return           ��ʾ�ɹ�����SUCESS;xml�ļ���IDΪid��GLDMaskBox�µ�tip�ڵ������wgtHash������һ��,����UNMATCHED;���򷵻�FAILURE
    */
    GLDMASKBOX_EXPORT STATUS WINAPI showMasks(const QString& id, QHash<int, QWidget*> &wgtHash);

    /**
    * @brief ��������ʾ�ɰ�
    * @param id         GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
    * @param wgtList    ��Ҫ��ʾ�ɰ��QAction,����keyΪxml�ļ���tip�ڵ��step���Ե�ֵ,step�谴˳���1��ʼ���;valueΪQAction*
    * @return           ��ʾ�ɹ�����SUCESS;xml�ļ���IDΪid��GLDMaskBox�µ�tip�ڵ������wgtHash������һ��,����UNMATCHED;���򷵻�FAILURE
    */
    GLDMASKBOX_EXPORT STATUS WINAPI showMasks(const QString& id, QHash<int, QAction*> &actHash);

    /**
    * @brief �ɰ��Ƿ���ʾ��
    * @param id         GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
    * @return           ��ʾ���򷵻�true,���򷵻�false
    */
    GLDMASKBOX_EXPORT bool WINAPI isShown(const QString& id);

    /**
    * @brief ΪMaskBox�е�Mask�����ɰ���ɫ
    * @param id         GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
    * @param color      ��ɫ
    * @return           ���óɹ�����true,���򷵻�false
    */
    GLDMASKBOX_EXPORT bool WINAPI setMaskBoxColor(const QString& id, GLDMask::MASKCOLOR color);

    /**
    * @brief ΪMaskBox�е�Mask���ü�ͷ��ɫ
    * @param id         GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
    * @param color      ��ɫ
    * @return           ���óɹ�����true,���򷵻�false
    */
    GLDMASKBOX_EXPORT bool WINAPI setMaskBoxArrowColor(const QString& id, const QColor& color);

    /**
    * @brief ΪMaskBox�е�Mask���ü�ͷ���
    * @param id         GLDMaskBox ID,��xml�ļ���MaskBox�ڵ�IDһ��
    * @param lineWidth  �߿�
    * @return           ���óɹ�����true,���򷵻�false
    */
    GLDMASKBOX_EXPORT bool WINAPI setMaskBoxArrowLineWidth(const QString& id, const int lineWidth);

    /**
    * @brief  ���г־û�����,��Ӧ�ó���رպ����
    * @return �ɹ�����true,���򷵻�false
    */
    GLDMASKBOX_EXPORT bool WINAPI unInitialize();
}
#endif // GLDMASKBOXUTIL_H

#ifndef GLDMASKBOXFACTORY_H
#define GLDMASKBOXFACTORY_H

#include "GLDMaskBox.h"

#include <windows.h>

#include <QHash>
#include <QStringList>
#include <QtXml/QDomElement>

namespace GlodonMask
{
    GLDMASKBOX_EXPORT bool WINAPI Initialize(const QString& xmlPath);

    GLDMASKBOX_EXPORT int WINAPI showMasks(const QString& id, QList<QWidget*> &wgtList);

    GLDMASKBOX_EXPORT bool WINAPI UnInitialize();

    class GLDMaskBoxFactory
    {
    public:
        GLDMaskBoxFactory(const QString& xmlPath);
        ~GLDMaskBoxFactory();

    public:
        void showMasks(const QString& id, QList<QWidget*> &wgtList);
        void showMasks(const QString& id, QList<QAction*> &actList);
        void writeToFile();

    private:
        /**
         * @brief ����XML�ļ�
         * @param xmlPath
         */
        void doParseXML(const QString& xmlPath);

        /**
         * @brief ����MaskBox�е�һ����ʾ��Ϣ
         * @param tipList
         * @param guideInfoList
         */
        void doParseMaskBoxTipInfos(QDomNodeList &tipList, QList<GLDTipInfo> &guideInfoList);

        /**
         * @brief ������ʾ��Ϣ�еĵ���Item(hint��close��next)
         * @param element
         * @return
         */
        GLDTipInfoItem doParseTipInfoItem(QDomElement &element);

        /**
         * @brief ����ini�ļ�,���ļ��д���Ѿ���ʾ����MaskBox,���Ѿ���ʾ����MaskBox���г־û�����
         * @param iniPath
         * @param shownMaskBoxIDList
         */
        void parseIniFile(const QString& iniPath, QStringList& shownMaskBoxIDList);

        /**
         * @brief ��wgtList�е�Menuת��Ϊbutton,����ӵ�list��
         * @param wgtList
         */
        void menuToBtn(QList<QWidget*> & wgtList);

        /**
        * @brief ��action��ص�ToolButton��ʽ��widget��ӵ�list��
        * @return
        */
        QList<QWidget*> actionToBtn(QList<QAction*> & actList);

        /**
         * @brief ����Ҫ��ʾ�ɰ��widget��ӵ�GLDMaskBox��
         * @param id
         * @param wgtList
         */
        void setWidgets(const QString& id, QList<QWidget*> &wgtList);

    private:
        QHash<QString, GLDMaskBox*> m_maskBoxHash;
    };
}

#endif // GLDMASKBOXFACTORY_H
#pragma once

#include "GLDMaskBox.h"

#include <QHash>
#include <QStringList>
#include <QtXml/QDomElement>

namespace GlodonMask
{
    class GLDMASKBOX_EXPORT GLDMaskBoxFactory : public QObject
    {
        Q_OBJECT

    public:
        void initialize(const QString& xmlPath);
        void showMasks(const QString& id, QList<QWidget*> &wgtList);
        void showMasks(const QString& id, QList<QAction*> &actList);

    public:
        GLDMaskBoxFactory(QObject* parent = nullptr);
        virtual ~GLDMaskBoxFactory();

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
        void doParseMaskBoxTipInfos(QDomNodeList &tipList, QList<GLDGuideInfo> &guideInfoList);

        /**
         * @brief ������ʾ��Ϣ�еĵ���Item(hint��close��next)
         * @param element
         * @return
         */
        GLDGuideInfoItem doParseTipInfoItem(QDomElement &element);


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

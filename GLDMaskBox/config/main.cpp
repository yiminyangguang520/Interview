
#include <QtCore/QCoreApplication>
#include <QFile>
#include <QtXml/QDomElement>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("GuideInfo.xml");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return 0;
    }

    QString strError;
    QDomDocument document;
    int errLin = 0, errCol = 0;

    if (!document.setContent(&file, false, &strError, &errLin, &errCol))
    {
        return 0;
    }

    if (document.isNull())
    {
        return 0;
    }

    QDomElement root = document.documentElement();
    QString elementTagName = root.firstChildElement().tagName();
    QDomNodeList nodeList = root.elementsByTagName(elementTagName);

    const int count = nodeList.size();
    for (int i = 0; i < count; ++i)
    {

        QString nodeName = nodeList.at(i).nodeName();
        QString id = nodeList.at(i).toElement().attribute("id");
        QDomElement tipDom = nodeList.at(i).firstChildElement();

        QString tipElement = tipDom.tagName();
        int tipSize = nodeList.at(i).toElement().elementsByTagName(tipElement).size();

        QString str = tipDom.attribute("order");

        while (!tipDom.isNull())
        {
            QString tip = tipDom.tagName();
            QDomNodeList nodeList = tipDom.elementsByTagName(tip);

            tipDom = tipDom.nextSiblingElement();
        }
    }
    return a.exec();
}

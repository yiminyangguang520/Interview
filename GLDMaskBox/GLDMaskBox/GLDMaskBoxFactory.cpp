#include "GLDMaskBoxFactory.h"

#include <QFile>

void GLDMaskBoxFactory::initialize(const QString& xmlPath)
{

}

GLDMaskBoxFactory::GLDMaskBoxFactory(QObject* parent)
    : QObject(parent)
{

}

GLDMaskBoxFactory::~GLDMaskBoxFactory()
{

}

QStringList GLDMaskBoxFactory::parseIniFile(const QString& iniPath)
{
    QFile file(iniPath);

    if (!file.open(QFile::ReadOnly))
    {
        file.close();
        return;
    }

    QString strShownMaskBoxID = QString::fromLatin1(file.readAll());
    return strShownMaskBoxID.split(",");
}

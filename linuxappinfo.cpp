#include "linuxappinfo.h"
#include <QFile>
#include <QDebug>
#include <QTextCodec>

LinuxAppInfo::LinuxAppInfo(QObject *parent)
{

}

void LinuxAppInfo::run()
{
    m_process = new QProcess();
    m_process->setReadChannel(QProcess::StandardOutput);
    m_process->moveToThread(this);

    //Generates a list of Desktop filenames for the installed software under Linux
    makeLinuxAppsFileNameList();
    //Generates a list of the Desktop file paths for the installed software under Linux
    makeAppsDesktopFilePathList();
    //Store applied information according to certain rules
    makeAppsInfoObject();
    exec();
}

void LinuxAppInfo::makeLinuxAppsFileNameList()
{
    m_process->start("ls /usr/share/applications/");
    m_process->waitForFinished();

    QString data = m_process->readAllStandardOutput();
    //Gets all the file names under /usr/share/applications
    m_AppsFileNameList = data.split("\n");
    //Remove files that do not have the.Desktop suffix from the list
    for(int i=0; i<m_AppsFileNameList.size(); i++)
    {
        data = m_AppsFileNameList.at(i);
        if(!data.contains(".desktop",Qt::CaseSensitive))
            m_AppsFileNameList.removeAt(i);
    }
}

void LinuxAppInfo::makeAppsDesktopFilePathList()
{
    foreach (QString fileName, m_AppsFileNameList) {
        m_AppsDesktopFilePathList.append(QString("/usr/share/applications/%1").arg(fileName));
    }
}

void LinuxAppInfo::makeAppsInfoObject()
{
    foreach (QString filePath, m_AppsDesktopFilePathList) {
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly))
            qDebug()<<file.errorString();

        //Read All data
        QByteArray array = file.readAll();
        QTextCodec *tc = QTextCodec::codecForName("UTF-8");
        QString buf = tc->toUnicode(array);
        if(buf.contains("NoDisplay=true"))
            continue;
        if(buf.contains("Terminal=true"))
            continue;
        if(!buf.contains("Type=Application"))
            continue;
        file.close();

        //parse
        if(!file.open(QIODevice::ReadOnly))
            qDebug()<<file.errorString();

        //init
        QJsonObject app;
        app["appName"] = "";
        app["enName"] = "";
        app["name"] = "";
        app["appPath"] = "";
        app["image"] = "";

        while(file.atEnd() == false)
        {
            QByteArray array = file.readLine();
            QTextCodec *tc = QTextCodec::codecForName("UTF-8");
            //The conversion removes white space characters on both sides of the string
            QString buf = tc->toUnicode(array).trimmed();
            QStringList content = buf.split("=");

            if(buf.contains("Name="))
            {
                app["name"] = content.last();
                app["enName"] = content.last();
                app["appName"] = content.last();
            }

            if(buf.contains("Name[en]="))
                app["enName"] = content.last();

            if(buf.contains("Name[zh_CN]="))
                app["name"] = content.last();

            if(buf.contains("Exec="))
                app["appPath"] = content.last();

            if(buf.contains("Icon="))
                app["image"] = getAppIconFilePath(content.last());
        }
        file.close();

        if(!app["name"].toString().isEmpty())
            emit findOneApp(app);
    }
}

QString LinuxAppInfo::getAppIconFilePath(QString iconStr)
{
    //First, determine whether the parameter is a path or a filename
    if(iconStr.contains("/"))
        return iconStr;
    //And then parse out the path of the photo
    m_process->start(QString("find /usr/share/icons -name %1*").arg(iconStr));
    m_process->waitForFinished();

    QString data = m_process->readAllStandardOutput();
    if(data.isEmpty())
    {
        m_process->start(QString("find /usr/share/pixmaps -name %1*").arg(iconStr));
        m_process->waitForFinished();
        data = m_process->readAllStandardOutput();
    }
    //There may be a number of different size ICONS
    QStringList iconsPath= data.split("\n");
    data = iconsPath.at(0);
    if(!data.isEmpty())
        return iconsPath.at(0);

    return "";
}



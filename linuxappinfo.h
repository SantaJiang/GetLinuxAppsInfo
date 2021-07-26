#ifndef LINUXAPPINFO_H
#define LINUXAPPINFO_H

#include <QThread>
#include <QProcess>
#include <QJsonObject>

class LinuxAppInfo : public QThread
{
    Q_OBJECT
public:
    explicit LinuxAppInfo(QObject *parent = nullptr);

signals:
    void findOneApp(QJsonObject);
protected:
    void run() override;

private:
    /**
     * @brief Generates a list of Desktop filenames for the installed software under Linux
     */
    void makeLinuxAppsFileNameList();

    /**
     * @brief Generates a list of the Desktop file paths for the installed software under Linux
     */
    void makeAppsDesktopFilePathList();

    /**
     * @brief Store applied information according to certain rules
     */
    void makeAppsInfoObject();

    /**
     * @brief Find the path to the icon and return
     * @return
     */
    QString getAppIconFilePath(QString iconStr);
private:
    QProcess* m_process;
    QStringList m_AppsFileNameList;
    QStringList m_AppsDesktopFilePathList;
};

#endif // LINUXAPPINFO_H

#pragma once
#include <QString>
#include <QObject>
#include <QStringList>
#include <QDir>
#include <QVector>
#include <QString>

class FileTool
{
public:
    static QString makeupFullPath(const QString& parent_path, const QString& sub);
    //�Ӿ���·������ȡ�ļ�����Ĭ�ϲ�������׺
    static QString getFileName(const QString& absolute_path, bool suffix = false);
    //��ȡ�ļ���׺(������.)�����û�к�׺�����ؿ�
    static QString getSuffix(const QString& absolute_path);
    //��ø�·��
    static QString getParentPath(const QString& full_path);

    static void modifyFileName(const QString& path);

    static QVector<QString> searchImageFile(const QString& search_path); //����ǳ���ϵͳ�����ϵ�ͼ���ļ�

    static bool isStandardImageFile(const QString& image_name); //�ǲ��Ǳ�׼��ͼ���ļ�
    static bool isImageFile(const QString& image_name);        //����ǳ���ϵͳ�����ϵ�ͼ���ļ�
    static bool isAviFile(const QString& file_name);
    static bool isZipFile(const QString& path);

    //���Ŀ¼�µ��ļ��б�
    static void getDirAllFile(const QString& dir_path, QStringList& file_list);
    //����ļ�Ŀ¼��������������ļ�Ŀ¼
    static bool makeFileCatalog(const QString& catalog_path);
    //���һ���ļ������������Ƴ����Ƴ�ʧ�ܷ���false
    static bool removeFile(const QString& file_path);
    //ɾ��һ��Ŀ¼�����˵��̷�Ŀ¼����֤��ȫ
    static bool removeRecursively(const QString& dirName);
    //�ж�һ��Ŀ¼�Ƿ����
    static bool exists(const QString& dir_path);
    //�ж�һ���ļ��Ƿ����
    static bool fileExists(const QString& file_path);
    //����ļ��������������ļ�
    static bool makeFile(const QString& file_path);
    //���Ŀ¼�����ھʹ���Ŀ¼
    static bool mkPath(const QString& path);
    //�����ļ�
    static bool copyFile(const QString& src_path, const QString& dest_path);
    static bool createDestPath(const QString& src_path, const QString& dest_path, const QString& src_file_path, QString& dest_file_path);
    static bool copyAllFilesToDirectory(const QString& from_dir, const QString& to_dir, bool cover_file_if_exist);
    //����Ŀ¼���ļ�������Ϊ��д
    static void setFilesPermissionsToWriteOwner(const QString& dir_path);
    //���Ŀ¼����Ŀ¼·����
    static void getSubDirList(const QString& dir_path, QStringList& file_list);

    static QString jointToNativePath(const QString& parent, const QString& sub);
    static QString jointToNativePath(const QString& parent, const QStringList& sub_list);
    static void writeTextFile(const QString& filepath, const QStringList& text);
    static bool writeDataFile(const QString& filepath, const QByteArray& data);
    static bool readTextFile(const QString& filepath, QStringList& text);
    static void addTimeTagForFile(const QString& file_name);

    static void writeTextFile(QString filename, QVector<QString> str);
};

class DirTool
{
public:
    //��λΪbyte
    static qint64 getDirSpace(const QString& path);
    // ƴ��·�����ļ�ʱ�����Բ�����ƴ�Ӵ���б������
    static QString join(const QString& front_path, const QString& end_path);
    static QString insureASlashAtTheEnd(const QString& path);

    QString getDir() { return m_dir; }
    DirTool& operator<<(const QString& path);

private:
    QString m_dir;
};

class FileSizeMethod
{
public:
    FileSizeMethod();
    ~FileSizeMethod();

    static qint64 calFileSize(const QString& file_path);
    static qint64 calFileSize(const QStringList& file_path_list);

public:
    qint64 addFile(const QString& file_path);
    qint64 addFile(const QStringList& file_path_list);
    qint64 getAllFileSize() const;
    QStringList getfileList() const;
    void clearFileList();

private:
    qint64 fileSize = 0;
    QStringList fileList;
};
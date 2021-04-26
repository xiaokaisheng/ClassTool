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
    //从绝对路径中提取文件名，默认不包括后缀
    static QString getFileName(const QString& absolute_path, bool suffix = false);
    //获取文件后缀(不包含.)，如果没有后缀，返回空
    static QString getSuffix(const QString& absolute_path);
    //获得父路径
    static QString getParentPath(const QString& full_path);

    static void modifyFileName(const QString& path);

    static QVector<QString> searchImageFile(const QString& search_path); //这个是超声系统意义上的图像文件

    static bool isStandardImageFile(const QString& image_name); //是不是标准的图像文件
    static bool isImageFile(const QString& image_name);        //这个是超声系统意义上的图像文件
    static bool isAviFile(const QString& file_name);
    static bool isZipFile(const QString& path);

    //获得目录下的文件列表
    static void getDirAllFile(const QString& dir_path, QStringList& file_list);
    //如果文件目录不存在则会生成文件目录
    static bool makeFileCatalog(const QString& catalog_path);
    //如果一个文件存在则会把它移除，移除失败返回false
    static bool removeFile(const QString& file_path);
    //删除一个目录，过滤掉盘符目录，保证安全
    static bool removeRecursively(const QString& dirName);
    //判断一个目录是否存在
    static bool exists(const QString& dir_path);
    //判断一个文件是否存在
    static bool fileExists(const QString& file_path);
    //如果文件不存在则不生产文件
    static bool makeFile(const QString& file_path);
    //如果目录不存在就创建目录
    static bool mkPath(const QString& path);
    //复制文件
    static bool copyFile(const QString& src_path, const QString& dest_path);
    static bool createDestPath(const QString& src_path, const QString& dest_path, const QString& src_file_path, QString& dest_file_path);
    static bool copyAllFilesToDirectory(const QString& from_dir, const QString& to_dir, bool cover_file_if_exist);
    //设置目录中文件的属性为可写
    static void setFilesPermissionsToWriteOwner(const QString& dir_path);
    //获得目录的子目录路径名
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
    //单位为byte
    static qint64 getDirSpace(const QString& path);
    // 拼接路径或文件时，可以不关心拼接处的斜杠问题
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
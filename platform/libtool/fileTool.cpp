#include "filetool.h"
#include "QTextStream"
#include <QDateTime>
#include <QDir>
#include <QMutex>
#include <QRect>
#include <QVector>

bool FileTool::createDestPath(const QString& src_path, const QString& dest_path, const QString& src_file_path, QString& dest_file_path)
{
    //源文件子路径
    const QString sub_path = src_file_path.right(src_file_path.length() - src_path.length());

    //目标路径
    dest_file_path = DirTool::join(dest_path, sub_path);
    //如果在路径下已经存在这个文件则删除
    bool suc = true;
    suc &= removeFile(dest_file_path);

    //目录路径
    const QString dir_path = getParentPath(dest_file_path);
    suc &= makeFileCatalog(dir_path);

    return suc;
}

bool FileTool::copyFile(const QString& src_path, const QString& dest_path)
{
    if (src_path.isEmpty() || dest_path.isEmpty()) {
        return false;
    }
    if (!fileExists(src_path)) {
        return false;
    }
    if (fileExists(dest_path)) {
        removeFile(dest_path);
    }
    else {
        QString parent_path = getParentPath(dest_path);
        makeFileCatalog(parent_path); // 确保路径完整，否则拷贝会失败
    }
    return QFile::copy(src_path, dest_path);
}

QString FileTool::makeupFullPath(const QString& parent_path, const QString& sub)
{
    QString full_path = parent_path + "/" + sub;
    return full_path;
}

//根据绝对路径得到文件名，默认不包括后缀
QString FileTool::getFileName(const QString& absolute_path, bool suffix)
{
    QString name;
    int index = absolute_path.lastIndexOf("\\");
    if (index < 0) {
        index = absolute_path.lastIndexOf("/");
    }
    if (index >= 0) {
        name = absolute_path.mid(index + 1);
    }
    else {
        name = absolute_path;
    }
    if (!suffix) {
        index = name.lastIndexOf(".");
        if (index > 0) {
            name = name.left(index);
        }
    }
    return name;
}

QString FileTool::getSuffix(const QString& absolute_path)
{
    QString file_name = getFileName(absolute_path, true);
    int index = file_name.lastIndexOf(".");
    if (index > 0) {
        return file_name.mid(index + 1);
    }
    return "";
}

QString FileTool::getParentPath(const QString& full_path)
{
    QString path = full_path;
    // 统一格式，方便查找
    path.replace('\\', '/');
    int index = path.lastIndexOf("/");
    if (index >= 0) {
        path = path.left(index);
    }
    return path;
}

void FileTool::modifyFileName(const QString& path)
{
    int num = 1;
    QString before_patient_db_path = path;
    QString after_patient_db_path = path + QString::number(num, 10);
    while (QFile::exists(after_patient_db_path)) {
        QString before_num = QString::number(num, 10);
        QString after_num = QString::number(++num, 10);
        after_patient_db_path.replace(before_num, after_num);
    }
    QFile::rename(before_patient_db_path, after_patient_db_path);
}

QVector<QString> FileTool::searchImageFile(const QString& search_path)
{
    QVector<QString> file_list;
    QDir search_dir(search_path);
    QFileInfoList file_info_list = search_dir.entryInfoList();
    foreach(const QFileInfo& file_info, file_info_list) {
        if (file_info.fileName() == "." || file_info.fileName() == "..") {
            continue;
        }
        if (file_info.isDir()) {
            QVector<QString> list = searchImageFile(file_info.absoluteFilePath());
            file_list += list;
        }
        else {
            //限定电影文件类型，文件名必须为image.data
            //文件后缀为data但是文件名不是image.data的直接跳过
            if (QString::compare(file_info.suffix(), "data", Qt::CaseInsensitive) == 0
                && QString::compare(file_info.fileName(), "image.data", Qt::CaseInsensitive) != 0) {
                continue;
            }

            //把缩略图也清理出去
            if (file_info.fileName() == "thumbnail.png") {
                continue;
            }

            if (isImageFile(file_info.suffix())) {
                file_list.push_back(file_info.absoluteFilePath());
            }
        }
    }
    return file_list;
}

bool FileTool::isImageFile(const QString& suffix)
{
    if (isStandardImageFile(suffix)
        || QString::compare(suffix, "data", Qt::CaseInsensitive) == 0 //-NO_TR
        ) {
        return true;
    }
    return false;
}

bool FileTool::isStandardImageFile(const QString& suffix)
{
    if (QString::compare(suffix, "BMP", Qt::CaseInsensitive) == 0
        || QString::compare(suffix, "jpg", Qt::CaseInsensitive) == 0
        || QString::compare(suffix, "png", Qt::CaseInsensitive) == 0) {
        return true;
    }
    else {
        return false;
    }
}

bool FileTool::isZipFile(const QString& path)
{
    QFileInfo fileinfo(path);
    QString suffix = fileinfo.suffix();
    if (QString::compare(suffix, "zip", Qt::CaseInsensitive) == 0) {
        return true;
    }
    return false;
}

bool FileTool::isAviFile(const QString& file_name)
{
    QFileInfo fileinfo(file_name);
    QString suffix = fileinfo.suffix();
    if (QString::compare(suffix, "avi", Qt::CaseInsensitive) == 0) {
        return true;
    }
    return false;
}

void FileTool::getDirAllFile(const QString& dir_path, QStringList& file_list)
{
    if (dir_path.isEmpty()) {
        return;
    }
    QFileInfo info(dir_path);
    if (info.isFile()) {
        file_list.push_back(dir_path);
        return;
    }
    QDir dir(dir_path);
    QFileInfoList file_info_list = dir.entryInfoList();
    for (int i = 0; i < file_info_list.size(); i++) {
        if (file_info_list[i].fileName() == "." || file_info_list[i].fileName() == "..") {
            continue;
        }
        if (file_info_list[i].isDir()) {
            getDirAllFile(file_info_list[i].filePath(), file_list);
        }
        else {
            file_list.push_back(file_info_list[i].filePath());
        }
    }
}

void FileTool::getSubDirList(const QString& dir_path, QStringList& file_list)
{
    if (dir_path.isEmpty()) {
        return;
    }
    QFileInfo info(dir_path);
    if (info.isFile()) {
        file_list.push_back(dir_path);
        return;
    }
    QDir dir(dir_path);
    QFileInfoList file_info_list = dir.entryInfoList();
    for (int i = 0; i < file_info_list.size(); i++) {
        if (file_info_list[i].fileName() == "." || file_info_list[i].fileName() == "..") {
            continue;
        }

        if (file_info_list[i].isDir()) {
            file_list.push_back(file_info_list[i].filePath());
        }
        else {
            continue;
        }
    }
}

bool FileTool::makeFileCatalog(const QString& catalog_path)
{
    QDir dir_path(catalog_path);
    bool suc = true;
    if (!dir_path.exists()) {
        suc = dir_path.mkpath(catalog_path);
    }
    return suc;
}

bool FileTool::removeFile(const QString& file_path)
{
    QDir dir_path(file_path);
    bool suc = true;
    if (dir_path.exists(file_path)) {
        suc = dir_path.remove(file_path);
    }
    return suc;
}

bool FileTool::removeRecursively(const QString& dirName)
{
    if (dirName.isEmpty())
        return false;

    QRegExp reg_exp("^[a-zA-Z]:[/\\\\]*$");
    if (reg_exp.exactMatch(dirName)) {
        Q_ASSERT(false);
        return false;
    }
    QDir dir(dirName);
    return dir.removeRecursively();
}

bool FileTool::exists(const QString& dir_path)
{
    if (dir_path.isEmpty()) {
        return false;
    }
    QDir dir(dir_path);
    return dir.exists();
}

bool FileTool::fileExists(const QString& file_path)
{
    return QFile::exists(file_path);
}

bool FileTool::makeFile(const QString& file_path)
{
    if (file_path.isEmpty()) {
        return false;
    }
    if (QFile::exists(file_path)) {
        return true;
    }
    QFile file(file_path);
    file.open(QIODevice::WriteOnly);
    file.close();
    return true;
}

bool FileTool::mkPath(const QString& path)
{
    QDir temp_path(path);
    if (!temp_path.exists()) {
        return temp_path.mkpath(temp_path.absolutePath());
    }
    return true;
}

bool FileTool::copyAllFilesToDirectory(const QString& from_dir, const QString& to_dir, bool cover_file_if_exist)
{
    QDir source_dir(from_dir);
    QDir target_dir(to_dir);
    if (!target_dir.exists()) {
        if (!target_dir.mkpath(target_dir.absolutePath()))
            return false;
    }

    QFileInfoList file_info_list = source_dir.entryInfoList();
    foreach(QFileInfo file_info, file_info_list) {
        if (file_info.fileName() == "." || file_info.fileName() == "..") {
            continue;
        }
        if (file_info.isDir()) { //当为目录时，递归的进行copy
            if (!copyAllFilesToDirectory(file_info.filePath(), target_dir.filePath(file_info.fileName()), cover_file_if_exist)) {
                return false;
            }
        }
        else { //当允许覆盖操作时，将旧文件进行删除操作
            if (cover_file_if_exist && target_dir.exists(file_info.fileName())) {
                target_dir.remove(file_info.fileName());
            }
            if (!QFile::copy(file_info.filePath(), target_dir.filePath(file_info.fileName()))) { //进行文件copy
                return false;
            }
        }
    }
    return true;
}

void FileTool::setFilesPermissionsToWriteOwner(const QString& dir_path)
{
    QStringList comment_file_list_execute;
    FileTool::getDirAllFile(dir_path, comment_file_list_execute);
    for (int i = 0; i < comment_file_list_execute.size(); i++) {
        QFile file(comment_file_list_execute.at(i));
#if defined(WIN32)
        file.setPermissions(QFile::WriteOwner);
#else
        QFileDevice::Permissions perm = file.permissions();
        file.setPermissions(perm | QFile::WriteOwner);
#endif
    }
}
 
QString FileTool::jointToNativePath(const QString& parent, const QString& sub)
{
    return QDir::toNativeSeparators(QDir(parent).filePath(sub));
}

QString FileTool::jointToNativePath(const QString& parent, const QStringList& sub_list)
{
    QString path = parent;
    foreach(const QString& sub, sub_list) {
        path.append(QDir::separator());
        path.append(sub);
    }
    path = QDir::toNativeSeparators(path);
    return path;
}

void FileTool::writeTextFile(const QString& filepath, const QStringList& text)
{
    QFile file(filepath);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    foreach(const QString& t, text) {
        stream << t << "\r\n"; //-NO_TR
    }
}

bool FileTool::writeDataFile(const QString& filepath, const QByteArray& data)
{
    QFile f(filepath);
    if (f.open(QFile::WriteOnly)) {
        qint64 bytes = f.write(data);
        if (bytes == data.size())
            return true;
        else
            return false;
    }
    else {
        return false;
    }
}

bool FileTool::readTextFile(const QString& filepath, QStringList& text)
{
    QFile data(filepath);
    if (data.open(QFile::ReadOnly)) {
        QTextStream stream(&data);
        QString line;
        do {
            line = stream.readLine();
            text.append(line);
        } while (!line.isNull());
        return true;
    }
    return false;
}

void FileTool::addTimeTagForFile(const QString& file_name)
{
    if (!QFile::exists(file_name)) {
        return;
    }
    QString changed_name;
    QString time_str = QDateTime::currentDateTime().toString("yyyyMMddHHmmss"); //-NO_TR
    int index = file_name.lastIndexOf(".");
    if (index >= 0) {
        changed_name = file_name.left(index);
        changed_name += "_";
        changed_name += time_str;
        changed_name += file_name.mid(index);
    }
    else {
        changed_name = file_name;
        changed_name += "_";
        changed_name += time_str;
    }
    QFile file(file_name);
    file.rename(changed_name);
}

//------------------------------------------------------------------------------------------------------------
//返回目录所占空间单位为byte
qint64 DirTool::getDirSpace(const QString& path)
{
    QStringList file_list;
    FileTool::getDirAllFile(path, file_list);

    qint64 dir_size = 0;
    foreach(const QString& file, file_list) {
        dir_size += FileSizeMethod::calFileSize(file);
    }
    return dir_size;
}

QString DirTool::join(const QString& front_path, const QString& end_path)
{
    QString front_path_tmp = front_path;
    front_path_tmp.replace('\\', '/');

    if (end_path.isEmpty()) {
        return front_path_tmp;
    }

    QString end_path_tmp = end_path;
    end_path_tmp.replace('\\', '/');

    if (front_path_tmp.endsWith("/") && end_path_tmp.startsWith("/")) {
        return front_path_tmp + end_path_tmp.remove(0, 1);
    }
    else if (!front_path_tmp.endsWith("/") && !end_path_tmp.startsWith("/")) {
        return front_path_tmp + "/" + end_path_tmp;
    }
    else {
        return front_path_tmp + end_path_tmp;
    }
}

QString DirTool::insureASlashAtTheEnd(const QString& path)
{
    if (path.endsWith('/') || path.endsWith('\\')) {
        return path;
    }
    else {
        return path + "/";
    }
}

DirTool& DirTool::operator<<(const QString& path)
{
    m_dir = m_dir.isNull() ? path : join(m_dir, path);
    return *this;
}

#include <QDir>
//FileSizeMethod
FileSizeMethod::FileSizeMethod()
{
}

FileSizeMethod::~FileSizeMethod()
{
}

qint64 FileSizeMethod::addFile(const QString& file_path)
{
    fileSize += calFileSize(file_path);
    return fileSize;
}

qint64 FileSizeMethod::getAllFileSize() const
{
    return fileSize;
}

QStringList FileSizeMethod::getfileList() const
{
    return fileList;
}

void FileSizeMethod::clearFileList()
{
    fileList.clear();
}

qint64 FileSizeMethod::calFileSize(const QString& file_path)
{
    QFile file(file_path);

    // Q_ASSERT(file.exists());

    if (file.exists()) {
        return file.size();
    }
    return 0;
}

qint64 FileSizeMethod::calFileSize(const QStringList& file_path_list)
{
    quint64 size = 0;
    foreach(const QString& file_path, file_path_list) {
        size += calFileSize(file_path);
    }
    return size;
}

qint64 FileSizeMethod::addFile(const QStringList& file_path_list)
{
    qint64 size = 0;
    foreach(const QString& file_path, file_path_list) {
        size += addFile(file_path);
    }
    return size;
}

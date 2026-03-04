#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QTextEdit>
#include <QFileDialog>
#include <QFileInfo>
#include <qglobal.h>
#include <stdlib.h>
#include <stdio.h>

// 在包含Python.h之前，先处理slots冲突
#pragma push_macro("slots")  // 保存当前slots宏的定义
#undef slots                 // 取消slots宏的定义
#include <Python.h>          // 此时包含Python.h，它看到的slots是未定义的或原始的
#pragma pop_macro("slots")   // 恢复之前保存的Qt的slots宏定义

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit->installEventFilter(this);

    // 重定向标准输出
    // TextEditStreamBuffer* buffer = new TextEditStreamBuffer(ui->textEdit);
    // std::cout.rdbuf(buffer);
    // std::cerr.rdbuf(buffer);

    // 设置无缓冲环境 确保Python的输出缓冲区被立即刷新
    #ifdef Q_OS_WIN
        // Windows环境
        _putenv("PYTHONUNBUFFERED=1");
    #elif defined(Q_OS_LINUX)
        // Linux环境
        setenv("PYTHONUNBUFFERED", "1", 1); // 第三个参数1表示覆盖已存在的值
    #endif
    // qDebug() << "PATH:\n" << qgetenv("PATH"); // getenv("PATH");
    qDebug() << "PYTHONUNBUFFERED:" << qgetenv("PYTHONUNBUFFERED");

    // 初始化 Python 解释器
    Py_Initialize();
    std::cout << "初始化python解释器" << std::endl;
}

MainWindow::~MainWindow()
{
    // 清理 Python 解释器
    Py_Finalize();
    delete ui;
}

void MainWindow::on_lineEdit_returnPressed()
{
    this->on_btnExecLine_clicked();
}

void MainWindow::on_btnExecLine_clicked()
{
    QString code = ui->lineEdit->text().trimmed();
    ui->lineEdit->setText("");
    ui->textEdit->append("> " + code);
    this->execute_python_code(code);
    // fflush(stdout); // 强制刷新标准输出缓冲区
}

///
/// \brief 直接执行代码
/// \param code
///
void MainWindow::execute_python_code(const QString code)
{
    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python interpreter" << std::endl;
        return;
    }

    try {
        PyRun_SimpleString(code.toUtf8().constData());
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown error occurred in execute_python_code" << std::endl;
    }
}

///
/// \brief 选择Python脚本文件执行, 使用PyRun_SimpleFile
///
void MainWindow::on_btnRunPyFile_clicked()
{
    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python interpreter" << std::endl;
        return;
    }

    // 执行 Python 脚本
    std::cout << "\nExecuting Python script..." << std::endl;

    try {

        // 可选： 设置打开路径R"(E:\MyDocs\Code\Qt\embed_python_test)"
        QString filePath = QFileDialog::getOpenFileName(this, tr("Open Python"),
                                                        QDir::currentPath(),
                                                        tr("Python Files (*.py)"));
        if (!filePath.isEmpty()) {
            QFileInfo fileInfo(filePath);
            QString fileName = fileInfo.fileName();

            // 使用RAII方式管理文件指针
            std::unique_ptr<FILE, decltype(&fclose)> fp(nullptr, fclose);

            // 尝试打开文件
            fp.reset(fopen(filePath.toUtf8().constData(), "rb"));

            if (fp) {
                // 运行Python脚本
                int result = PyRun_SimpleFile(fp.get(), fileName.toUtf8().constData());
                if (result == -1) {
                    if (PyErr_Occurred()) {
                        PyErr_Print();
                    }
                    throw std::runtime_error("Python script execution failed");
                }

            } else {
                throw std::runtime_error("Failed to open file: " + filePath.toStdString());
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown error occurred in file dialog" << std::endl;
    }
}

///
/// \brief 选择Python脚本文件执行, PyRun_SimpleString
///
void MainWindow::on_btnRunPyString_clicked()
{
    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python interpreter" << std::endl;
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Python"),
                                                    QDir::currentPath(),
                                                    tr("Python Files (*.py)"));
    if (filePath.isEmpty()) return;

    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.completeBaseName();   // 不含 .py 的模块名
    QString dirPath  = fileInfo.absolutePath();

    // 用 import 方式执行，子进程能正确找到 __main__ 的 __spec__
    QString code = QString(R"(
import sys, importlib

_dir = r'%1'
_mod = '%2'

if _dir not in sys.path:
    sys.path.insert(0, _dir)

# 每次重新加载，防止缓存
if _mod in sys.modules:
    del sys.modules[_mod]

import importlib.util
_spec = importlib.util.spec_from_file_location("__main__", r'%3')
_mod_obj = importlib.util.module_from_spec(_spec)
_mod_obj.__spec__ = _spec   # 关键：设置 __spec__ 让 spawn 能正确找到入口
sys.modules['__main__'] = _mod_obj
_spec.loader.exec_module(_mod_obj)
)").arg(dirPath).arg(fileName).arg(filePath);

    std::cout << "dirPath: " << dirPath.toStdString() << std::endl;
    std::cout << "fileName: " << fileName.toStdString() << std::endl;
    std::cout << "filePath: " << filePath.toStdString() << std::endl;

    try {
        PyRun_SimpleString(code.toUtf8().constData());
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
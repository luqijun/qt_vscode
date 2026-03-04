#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <streambuf>
#include <iostream>
#include <qobjectdefs.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEdit_returnPressed();
    void on_btnExecLine_clicked();
    void on_btnRunPyFile_clicked();
    void on_btnRunPyString_clicked();

private:
    Ui::MainWindow *ui;

    void execute_python_code(const QString code);
};

class TextEditStreamBuffer : public std::streambuf {
public:
    TextEditStreamBuffer(QTextEdit* textEdit) : m_textEdit(textEdit) {
        // 设置缓冲区大小（可根据需要调整）
        setp(buffer, buffer + bufferSize - 1);
    }

    ~TextEditStreamBuffer() {
        // 析构时刷新缓冲区
        sync();
    }

protected:
    int_type overflow(int_type c) override {
        if (c != EOF) {
            *pptr() = c;
            pbump(1);
        }
        if (sync() == 0) {
            return c;
        } else {
            return EOF;
        }
    }

    int sync() override {
        std::streambuf::sync();
        if (pbase() != pptr()) {
            int length = pptr() - pbase();
            QByteArray data(pbase(), length);
            // 将字节数据转换为QString（假设输入为UTF-8编码）
            QString text = QString::fromUtf8(data);

            m_textEdit->moveCursor(QTextCursor::End);
            m_textEdit->insertPlainText(text);
            m_textEdit->moveCursor(QTextCursor::End);

            // 重置缓冲区指针
            setp(buffer, buffer + bufferSize - 1);
        }
        return 0;
    }

private:
    QTextEdit* m_textEdit;
    static const int bufferSize = 1024;
    char buffer[bufferSize];
};



// class TextEditStreamBuffer : public std::streambuf {
// public:
//     TextEditStreamBuffer(QTextEdit* textEdit) : m_textEdit(textEdit) {

//         instance = this;

//         // 设置缓冲区
//         setp(buffer, buffer + bufferSize - 1);

//         // 保存原来的std::cout缓冲区
//         oldCoutBuffer = std::cout.rdbuf(this);
//         oldCerrBuffer = std::cerr.rdbuf(this);

//         // 初始化Python重定向
//         initPythonRedirect();
//     }

//     ~TextEditStreamBuffer() {
//         // 恢复原来的std::cout缓冲区
//         std::cout.rdbuf(oldCoutBuffer);
//         std::cerr.rdbuf(oldCerrBuffer);

//         // 确保析构时刷新缓冲区
//         sync();

//         // 清理Python重定向
//         cleanupPythonRedirect();
//     }

// protected:
//     int_type overflow(int_type c) override {
//         if (c != EOF) {
//             *pptr() = c;
//             pbump(1);
//         }
//         return (sync() == 0) ? c : EOF;
//     }

//     int sync() override {
//         if (pbase() != pptr()) {
//             // 获取缓冲区内容
//             int length = pptr() - pbase();
//             QByteArray data(pbase(), length);

//             // 将数据转换为QString，使用UTF-8编码防止中文乱码
//             QString text = QString::fromUtf8(data);

//             // 在主线程中更新文本控件
//             QMetaObject::invokeMethod(m_textEdit, [this, text]() {
//                 m_textEdit->moveCursor(QTextCursor::End);
//                 m_textEdit->insertPlainText(text);
//                 m_textEdit->moveCursor(QTextCursor::End);
//             });

//             // 重置缓冲区指针
//             setp(buffer, buffer + bufferSize - 1);
//         }
//         return 0;
//     }

// private:
//     QTextEdit* m_textEdit;
//     static const int bufferSize = 1024;
//     char buffer[bufferSize];
//     std::streambuf* oldCoutBuffer;
//     std::streambuf* oldCerrBuffer;

//     // 静态成员变量，用于在静态函数中访问实例
//     static TextEditStreamBuffer* instance;

//     // Python重定向相关
//     static PyObject* stdoutRedirect(PyObject* self, PyObject* args) {
//         const char* str;
//         if (!PyArg_ParseTuple(args, "s", &str)) {
//             return NULL;
//         }

//         // 使用静态实例指针访问成员
//         if (instance && instance->m_textEdit) {
//             QTextEdit* textEdit = instance->m_textEdit;
//             QString text = QString::fromUtf8(str);

//             // 在主线程中更新文本控件
//             QMetaObject::invokeMethod(textEdit, [textEdit, text]() {
//                 textEdit->moveCursor(QTextCursor::End);
//                 textEdit->insertPlainText(text);
//                 textEdit->moveCursor(QTextCursor::End);
//             });
//         }

//         Py_RETURN_NONE;
//     }

//     void initPythonRedirect() {
//         // 保存当前实例指针
//         instance = this;

//         // 定义Python模块和方法
//         static PyMethodDef moduleMethods[] = {
//             {"stdout_write", stdoutRedirect, METH_VARARGS, "Redirect stdout"},
//             {NULL, NULL, 0, NULL}
//         };

//         // 创建模块
//         static PyModuleDef moduleDef = {
//             PyModuleDef_HEAD_INIT,
//             "qt_redirect",
//             NULL,
//             -1,
//             moduleMethods
//         };

//         // 初始化模块
//         PyObject* module = PyModule_Create(&moduleDef);
//         PySys_SetObject("stdout", module);
//         PySys_SetObject("stderr", module);
//     }

//     void cleanupPythonRedirect() {
//         // 恢复Python标准输出
//         PySys_SetObject("stdout", PySys_GetObject("__stdout__"));
//         PySys_SetObject("stderr", PySys_GetObject("__stderr__"));

//         // 清除实例指针
//         instance = nullptr;
//     }
// };
// 初始化静态成员变量
// TextEditStreamBuffer* TextEditStreamBuffer::instance = nullptr;

#endif // MAINWINDOW_H

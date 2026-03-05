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

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Browse_clicked();
    //find image properties
    QVector<int> findProperties(QVector<QVector<int>> &imgArray, unsigned int &rows, unsigned int &cols);

    int bfSearch(QVector<int> &posn, QVector<QVector<int>> &explored, unsigned int &rows, unsigned int &cols, QVector<QVector<int>> &imgArray);
    void on_applyFilter_clicked();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QImage m_image;
};
#endif // MAINWINDOW_H

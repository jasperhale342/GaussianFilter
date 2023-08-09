#ifndef GAUSSIANFILTER_H
#define GAUSSIANFILTER_H
#include <QVector>
#include <QPixmap>
#include <QMainWindow>
#include <iostream>
#include <vector>
#include <cmath>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GaussianFilter
{
private:
    QVector<QVector<double>> m_gaussianKernel;
    QImage m_originalImage;
    QImage m_filteredImage;
    double gaussian(double x, double mean, double stddev);
    void createGaussianKernel(int size, double sigma);

public:
    //load the image
    GaussianFilter(QImage input, int size, double sigma);
    void createFilter(double x, double mean, double stddev);
    //apply the filter to the image
    void applyFilterToImage();
    //display the image to the ui
    void displayFilteredImage(Ui::MainWindow* ui);
    QImage getFilteredImage(){return m_filteredImage;};



};

#endif // GAUSSIANFILTER_H

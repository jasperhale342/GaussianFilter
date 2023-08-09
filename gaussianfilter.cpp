#include "gaussianfilter.h"
#include "ui_mainwindow.h"

GaussianFilter::GaussianFilter(QImage input, int size, double sigma): m_originalImage(input)
{
    createGaussianKernel(size, sigma);



//    m_gaussianKernel = {{(float)1/(float)16, (float)1/(float)8, (float)1/(float)16},
//                         {(float)1/(float)8, (float)1/(float)4,(float)1/(float)8},
//                         {(float)1/(float)16, (float)1/(float)8, (float)1/(float)16}


//    };
}

void GaussianFilter::applyFilterToImage(){
    int pad = 1;
    m_filteredImage = m_originalImage.convertToFormat(QImage::Format_Grayscale8);
     int cols = m_filteredImage.width();
     int rows = m_filteredImage.height();


    m_filteredImage.setPixel(0, 0, 255);
    for ( int i = 0; i <rows; i++ ){
        for( int j = 0; j <cols; j++ ){

            unsigned int sum = 0;
            for(int k = 0; k< m_gaussianKernel.size(); k++){
                for(int l = 0; l < m_gaussianKernel.size(); l++){


                    if(i-pad+k < 0 ||i-pad+k >= rows || j-pad+l < 0 ||j-pad+l >= cols){
                        sum = sum +  m_gaussianKernel[k][l];
                    } else {
                        sum = sum + m_gaussianKernel[k][l]* m_originalImage.pixel(j,i);
                    }



                }
            }

            m_filteredImage.setPixel(j, i, sum);


        }
    }

}

void GaussianFilter::displayFilteredImage(Ui::MainWindow* ui){


    QPixmap pix = QPixmap::fromImage(m_filteredImage);

    //get lavel dimensions
    int w = ui->Image->width();
    int h = ui->Image->height();

    ui->Image->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));



}


void GaussianFilter::createGaussianKernel(int size, double sigma){
    QVector<QVector<double>> kernel(size, QVector<double>(size, 0.0));

    if (size % 2 == 0) {
        std::cout << "Size must be odd. Adjusting size to " << size + 1 << std::endl;
        size++;
    }

    int center = size / 2;
    double sum = 0.0;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            kernel[i][j] = gaussian(sqrt((i - center) * (i - center) + (j - center) * (j - center)), 0.0, sigma);
            sum += kernel[i][j];
        }
    }

    // Normalize the kernel
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            kernel[i][j] /= sum;
        }
    }

    m_gaussianKernel = kernel;
}


double GaussianFilter::gaussian(double x, double mean, double stddev){
     return exp(-0.5 * pow((x - mean) / stddev, 2)) / (stddev * sqrt(2 * M_PI));
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QVector>
#include <QQueue>
#include <gaussianfilter.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_image(NULL)
{
    ui->setupUi(this);
    ui->filterComboBox->addItem("Gaussin");
    ui->filterComboBox->addItem("HOG");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Browse_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Images (*.png *.xpm *.jpg)"));


    if(!file_name.isEmpty()){


        QMessageBox::information(this, "...", file_name);

        m_image = QImage(file_name);
        QPixmap pix = QPixmap::fromImage(m_image);

        //get lavel dimensions
        int w = ui->Image->width();
        int h = ui->Image->height();

        ui->Image->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));


    }

}

QVector<int> MainWindow::findProperties(QVector<QVector<int>> &imgArray, unsigned int &rows, unsigned int &cols){
    int numIslands = 0;
    int maxArea = 0;
    int area = 0;

    QVector<QVector<int>> explored(rows, QVector<int>(cols, 0));
    for (unsigned int i = 0; i <rows; i++ ){
        for(unsigned int j = 0; j <cols; j++ ){
            if(imgArray[i][j] == 1 && explored[i][j] ==0){
                QVector<int> posn{(int)i, (int)j};
                area = this->bfSearch(posn, explored, rows, cols, imgArray);
                if(area >maxArea){
                    maxArea = area;
                }
                numIslands+=1;

            }
        }

    }
    QVector<int> results{numIslands, maxArea};
    return results;
}


int MainWindow::bfSearch(QVector<int> &posn, QVector<QVector<int>> &explored, unsigned int &rows, unsigned int &cols, QVector<QVector<int>> &imgArray){
    QVector<int> u{-1,0};
    QVector<int> d{1,0};
    QVector<int> l{0,-1};
    QVector<int> r{0,1};
    int islandArea = 0;

    QQueue<QVector<int>> not_explored;
    not_explored.enqueue(posn);

    while(!not_explored.isEmpty()){
        QVector<QVector<int>> potential_moves{{posn[0]+u[0],posn[1]+u[1]},{posn[0]+d[0],posn[1]+d[1]},{posn[0]+l[0],posn[1]+l[1]},{posn[0]+r[0],posn[1]+r[1]}};
        for(QVector<int> move : potential_moves){
            if (move[0] < 0 || move[0] > (int)rows-1 || move[1] < 0 || move[1] > (int)cols-1 || explored[move[0]][move[1]] == 1 || imgArray[move[0]][move[1]] == 0){
                continue;
            }
           not_explored.enqueue(move);
        }


        explored[posn[0]][posn[1]] = 1;
        islandArea+=1;
        not_explored.dequeue();
        if (!not_explored.isEmpty()){
           posn = not_explored.head();
        }

    }

    return islandArea;
}





void MainWindow::on_applyFilter_clicked()
{

    if (m_image.isNull()){
        QMessageBox msgBox;
        msgBox.setText("Must select an image from disk");
        msgBox.exec();
        return;
    }


    GaussianFilter gFilter(m_image, 9, 2.0);
    gFilter.applyFilterToImage();
    gFilter.displayFilteredImage(ui);
    m_image = gFilter.getFilteredImage();
}




void MainWindow::on_pushButton_clicked()
{
    if (m_image.isNull()){
        QMessageBox msgBox;
        msgBox.setText("Must select an image from disk");
        msgBox.exec();
        return;
    }

    unsigned int cols = m_image.width();
    unsigned int rows = m_image.height();
    unsigned int numBlackPixels = 0;

    QVector<QVector<int>> imgArray(rows, QVector<int>(cols,0));

    //get pixel data
    for (unsigned int i = 0; i <rows; i++ ){
        for(unsigned int j = 0; j <cols; j++ ){
           QColor clrCurrent(m_image.pixel(j,i));
           int r = clrCurrent.red();
           int g = clrCurrent.green();
           int b = clrCurrent.blue();
           int a = clrCurrent.alpha();

           //basically a black pixel
           if (r+g+b < 20 && a >240){
                imgArray[i][j] = 1;
                numBlackPixels+=1;
           }
        }
    }
    //stor image array to a file
    QString filename = "C:/Users/halej/Desktop/temp.txt";
    QFile fileout(filename);
    if(fileout.open(QFile::ReadWrite | QFile::Text)){
        QTextStream out(&fileout);
        for (unsigned int i = 0; i <rows; i++ ){
           for(unsigned int j = 0; j <cols; j++ ){
                out << imgArray[i][j];
           }
           out << " " << Qt::endl;
        }
    }

    //update UI with information
    ui->DimensionsWHLabel->setText(QString::fromStdString("W: " + std::to_string(cols) + " H: " + std::to_string(rows)));
    float pD = ((float)numBlackPixels/(float)(cols*rows))*100;
    ui->pDark->setText(QString::fromStdString(std::to_string(pD)));

    QVector<int> results = this->findProperties(imgArray, rows, cols);

    ui->BlobsNumberLabel->setText(QString::fromStdString(std::to_string(results[0])));
    ui->MaxBlobAreaNumberLabel->setText(QString::fromStdString(std::to_string(results[1])));
}


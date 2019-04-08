#include "processpicture.h"
#include <QImage>
#include <QString>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

ProcessPicture::ProcessPicture(QObject *parent) : QObject(parent)
{

}

//读取图片到Mat
void ProcessPicture::ReadPicture(QString path)
{
    image = imread(path.toStdString(),IMREAD_COLOR);
    //滤波，去噪点
    GaussianBlur(image, image, Size(5,5), 0, 0, BORDER_DEFAULT);
    //备份
    back = image.clone();
}

//返回图片
QImage ProcessPicture::ReturnImage()
{

    if(image.empty()){
        image = imread("/Users/lietian/Desktop/IMG_2088.jpg",IMREAD_COLOR);

    }

    if(image.type()==CV_8UC1){//灰度图
        cvtColor(image,result,COLOR_GRAY2RGB);
    }else if (image.type()==CV_8UC3) {//彩色图
        cvtColor(image,result,COLOR_BGR2RGB);
    }
    return QImage(static_cast<uchar *>(result.data),result.cols,result.rows,QImage::Format_RGB888);
}

//找到边界
void ProcessPicture::FindBoundary()
{
    image = back.clone();
    //---------------------处理得到二值图像--------
    cvtColor(image,gray,COLOR_BGR2GRAY);
    //二值化为黑白图
    threshold(gray,gray,100,255,THRESH_BINARY);
    //滤波降噪
    medianBlur(gray,gray,21);
    //填充背景，取反得到孔洞图像
    Mat temp=gray.clone();
    floodFill(temp,Point(0,0),Scalar::all(0));
    bitwise_not(gray,gray);
    //add效果重叠白色区域，即高亮区域
    add(temp,gray,gray);

    //方法1
    //Canny(gray, gray, 22, 66);
    //--还要去除孤立点
    //----

    //方法2
    //adaptiveThreshold(gray,gray,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,3,1);
    //medianBlur(gray,gray,5);
    //---连接边缘，填充轮廓
    //--

    //----------------END--------------

    //-----------------获得边界点--------
    vector<vector<Point>> contours;//存储轮廓点
    vector<Vec4i> hierarchy;//存储不同轮廓间的信息

    findContours(gray,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);\
    drawContours(image,contours,-1,Scalar::all(255),4);
    imshow("t",image);
    //drawContours(image,contours,-1,Scalar::all(255),2);
    //namedWindow("contours");
    //imshow("contours",result);
}

/*
vector<Point> ProcessPicture::FindBiggestContour(Mat binary_image)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    int largest_area = 0;
    int largest_contour_index = 0;

    findContours(binary_image, contours, hierarchy,RETR_CCOMP,CHAIN_APPROX_SIMPLE);

        for (int i = 0; i < contours.size(); i++) // iterate through each contour.
        {
            double a = contourArea(contours[i], false);  //  Find the area of contour
            if (a > largest_area){
                largest_area = a;
                largest_contour_index = i;                //Store the index of largest contour
            }
        }

        return contours[largest_contour_index];
}
*/

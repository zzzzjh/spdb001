#include<iostream>
#include<algorithm>
#include<opencv2/opencv.hpp>
#include<map>
using namespace std;
using namespace cv;
int label = 0;// 联通域的标记
int area = 0;//连通区域
vector<Point2d>p;//保存标号
Point2d seed;
map<int, Point3d>dic;//里面保存每个联通域的随机颜色
map<int, int>label_area; //联通域面积
map<int, Rect> rect;//外接矩形
Rect l;
int row_min = 0;
int row_max = 0;
int col_min = 0;
int col_max = 0;
RNG png;
void color(Mat imm);
void seedfill(Mat im)
{
	for (int i = 0; i < im.rows; i++)
	{
		uchar *data = im.ptr<uchar>(i);
		for (int j = 0; j < im.cols; j++)
		{
			if (data[j] == 255)
			{
				area = 0;
				++label;
				++area;
				dic[label] = Point3d(png.uniform(0, 255), png.uniform(0, 255), png.uniform(0, 255));//字典将颜色和label对应起来
				data[j] = label;//开始标号
				p.push_back(Point2d(i, j));
				seed = Point2d(i, j);
				col_min = j;
				col_max = j;
				row_min = i;
				row_max = i;//定义label的外接矩形
 
				while (!p.empty())
				{
					if (im.at<uchar>(seed.x, seed.y + 1) == 255)
					{
						im.at<uchar>(seed.x, seed.y + 1) = label;
						p.push_back(Point2d(seed.x, seed.y + 1));
						++area;
						if (col_max < seed.y + 1)
						{
							col_max = seed.y + 1;
						}
					}
					if (im.at<uchar>(seed.x+1, seed.y ) == 255)
					{
						im.at<uchar>(seed.x+1,seed.y ) = label;
						p.push_back(Point2d(seed.x+1, seed.y));
						++area;
						if (row_max < seed.x + 1)
						{
							row_max = seed.x + 1;
						}
					}
					if (im.at<uchar>(seed.x, seed.y -1) == 255)
					{
						im.at<uchar>(seed.x, seed.y - 1) = label;
						p.push_back(Point2d(seed.x, seed.y - 1));
						++area;
						if (col_min > seed.y- 1)
						{
							col_min = seed.y - 1;
						}
					}
					if (im.at<uchar>(seed.x-1, seed.y ) == 255)
					{
						im.at<uchar>(seed.x-1, seed.y) = label;
						p.push_back(Point2d(seed.x-1, seed.y));
						++area;
						if (row_min > seed.x - 1)
						{
							row_min = seed.x - 1;
						}
					}
					seed = p.at(p.size() - 1);//
					p.pop_back();//删除最后一个元素
				}
				label_area[label] = area;//将联通域的面积保存起来
				//Rect l = Rect( row_min,col_min,(row_max-row_min),(col_max-col_min ));
 
				Rect l = Rect(col_min, row_min, (col_max - col_min), (row_max - row_min));
				rect[label] = l;			
			}
		}	
	}
	cout << "共有" << label << "个连通区域" << endl;
	auto s= label_area.begin();
	while (s != label_area.end())
	{
		cout << "第" << s->first << "个联通区域的面积=" << s->second << endl;
		++s;
	}
	color(im);
}
//对图像进行染色
void color(Mat imm)
{
	Mat image(imm.rows, imm.cols, CV_8UC3, Scalar(0,0, 0));
	for (int i = 0; i < imm.rows; i++)
		{
		for (int j = 0; j < imm.cols; j++)
		{
			int d = imm.at<uchar>(i, j);
			if (d != 0)
			{
				image.at<Vec3b>(i, j)[0] = dic[d].x;
				image.at<Vec3b>(i, j)[1] = dic[d].y;
				image.at<Vec3b>(i, j)[2] = dic[d].z;
			}		
		}
		}
	auto ss = rect.begin();
	while (ss != rect.end())
	{
		rectangle(image,ss->second ,Scalar(0,255,0),2);
		++ss;
	}
	imshow("2", image);
	waitKey(0);
}
int main()
{
	Mat im = imread("c:\\users\\xaut\\desktop\\test.jpg");
	Mat dst, gray;
	cvtColor(im, gray, COLOR_BGR2GRAY);
	threshold(gray, dst, 50, 255, THRESH_BINARY);
	seedfill(dst);
    return 0;
}
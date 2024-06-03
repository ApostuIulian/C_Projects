#include "stdafx.h"
#include "common.h"
#include <cmath>
#include <iostream>
#include <queue>
#include <string.h>
#include <algorithm>

using namespace std;

Mat openGrayScale() {
	char fname[MAX_PATH];
	openFileDlg(fname);
	return imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
}

Mat binarized(Mat src, int threshold)
{
	int height = src.rows, width = src.cols;
	Mat dst(height, width, CV_8UC1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			dst.at<uchar>(i, j) = src.at<uchar>(i, j) > threshold ? 255 : 0;
		}
	}
	//imshow("original", src);
	//imshow("binarized", dst);
	//waitKey();
	return dst;
}

bool isInside(int i, int j, Mat src)
{
	int height = src.rows;
	int width = src.cols;
	if ((i >= 0 && i <= height - 1) && (j >= 0 && j <= width - 1))
	{
		return true;
	}
	return false;
}

int* histogram(Mat src)
{
	int height = src.rows, width = src.cols;
	int* h = (int*)malloc(256 * sizeof(int));
	for (int i = 0; i < 256; i++)
	{
		h[i] = 0;
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			h[src.at<uchar>(i, j)]++;
		}
	}
	return h;
}


Mat automaticBinarized(Mat src)
{
	int height = src.rows;
	int width = src.cols;
	int* h = histogram(src);
	int imin = 0, imax = 0;
	for (int i = 255; i >= 0; i--)
	{
		if (h[i] > 0)
		{
			imax = i;
			break;
		}
	}
	for (int i = 0; i < 256; i++)
	{
		if (h[i] > 0)
		{
			imin = i;
			break;
		}
	}

	float T2 = (float)(imin + imax) / 2;
	float T1 = 0.0;
	float error = 0.1;
	while (!((T2 - T1) < error))
	{
		T1 = T2;
		float mean1 = 0, mean2 = 0;
		int N1 = 0, N2 = 0;
		for (int i = imin; i < T1; i++)
		{
			mean1 += i * h[i];
			N1 += h[i];
		}
		for (int i = T1 + 1; i < imax; i++)
		{
			if(T1 > 0)
			{
			mean2 += i * h[i];
				N2 += h[i];
			}
		}
		T2 = (float)((float)(mean1 / N1) + (float)(mean2 / N2)) / 2;
	}
	//std::cout << T2;
	//imshow("binarized", binarized(src, T2));
	//waitKey();
	return binarized(src, T2);
}

Mat convolutie(Mat src, std::vector<std::vector<float>>& kernel)
{
	Mat dst(src.rows, src.cols, CV_8UC1);
	dst = cv::Mat::zeros(src.size(), src.type());
	int w = kernel.size();//3
	int k = w / 2;//1
	for (int i = k; i < src.rows - k; i++)
	{
		for (int j = k; j < src.cols - k; j++)
		{
			float sum = 0.0;
			for (int u = 0; u < w; u++)
			{
				for (int v = 0; v < w; v++)
				{
					sum += kernel[u][v] * src.at<uchar>(i + u - k, j + v - k);
				}
			}
			dst.at<uchar>(i, j) = max(0, min(255, sum));
		}
	}
	return dst;
}

Mat gaussianFilter(Mat src, int w)
{
	Mat dst(src.rows, src.cols, CV_8UC1);
	//Mat gausse(w, w, CV_32FC1);
	int k = w / 2;
	float sigma = w / 6.0;
	std::vector<std::vector<float>> gausse(w, std::vector<float>(w, 0.0f));
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < w; j++) {
			gausse[i][j] = (1.0 / (2.0 * PI * pow(sigma, 2))) * exp(-(pow(i - k, 2) + pow(j - k, 2)) / (2.0 * pow(sigma, 2)));
		}
	}
	return convolutie(src, gausse);
}


bool contains(const std::vector<int>& vec, int keyToFind) {
	return std::find(vec.begin(), vec.end(), keyToFind) != vec.end();
}

Point lineIntersection(int y1, int y2, int x1, int x2, int Y1, int Y2, int X1, int X2)
{
	Point p;
	double denominator = ((x1 - x2) * (Y1 - Y2)) - ((y1 - y2) * (X1 - X2));

	if (denominator == 0) {
		return Point(-1, -1);
	}

	p.x = (((x1 * y2 - y1 * x2) * (X1 - X2)) - ((x1 - x2) * (X1 * Y2 - Y1 * X2))) / denominator;
	p.y = (((x1 * y2 - y1 * x2) * (Y1 - Y2)) - ((y1 - y2) * (X1 * Y2 - Y1 * X2))) / denominator;

	return p;
}

Mat templateImageCorrection(Mat src)
{
	int height = src.rows, width = src.cols;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i < 10 || i > height - 10) {
				src.at<uchar>(i, j) = 255;
				src.at<uchar>(i, j) = 255;
			}
			if (j < 7 || j > width - 5) {
				src.at<uchar>(i, j) = 255;
			}
		}
	}
	return src;
}

Mat prepareInputPhoto()
{
	Mat binarizedInput = automaticBinarized(openGrayScale());
	Mat resizedInput;
	resize(binarizedInput, resizedInput, Size(900, 1000), INTER_LINEAR);
	return resizedInput;
}

Mat computeContours(Mat input, std::vector<std::vector<Point>> *contours, std::vector<Vec4i> *hierarchy, std::vector<int> *cardContoursIndexes)
{
	findContours(input, *contours, *hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	input = input.clone();
	Mat contouredInput(input.rows, input.cols, CV_8UC1, Scalar(255));

	drawContours(contouredInput, *contours, -1, Scalar(0, 0, 0), 2);
	for (size_t i = 0; i < (*contours).size(); i++) {
		if ((*hierarchy)[i][3] == -1) {
			Scalar color = Scalar(0, 0, 0);
			(*cardContoursIndexes).push_back(i);
		}
	}
	return contouredInput;
}

double distance(Vec4i l)
{
	return sqrt((l[0] - l[2]) * (l[0] - l[2]) + (l[1] - l[3]) * (l[1] - l[3]));
}

double distance_v2(Point A, Point B)
{
	return sqrt((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));
}

void computeDistances(std::vector<double> *distances, Mat oneCardContourCanny, std::vector<Vec4i> lines, Mat justEdges)
{

	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		double dist = distance(l);
		(*distances).push_back(dist);
		line(justEdges, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 0), 1, LINE_AA);
		//imshow("justEdges", justEdges);
		//waitKey();
	}
	std::sort((*distances).begin(), (*distances).end(), std::greater<double>());
}

void computeCorners(std::vector<Point2f> *corners, std::vector<Vec4i> lines, std::vector<double> distances, Mat markedCorners)
{
	for (size_t i = 0; i < lines.size() - 1; i++) {
		Vec4i l = lines[i];
		double dist = distance(l);
		if (dist >= distances.at(3)) {
			for (size_t j = i + 1; j < lines.size(); j++) {
				Vec4i l2 = lines[j];
				double dist2 = distance(l2);
				if (dist2 >= distances.at(3)) {
					Point2f p = lineIntersection(l[1], l[3], l[0], l[2], l2[1], l2[3], l2[0], l2[2]);
					if (p.y != -1)
					{
						if (isInside(p.y, p.x, markedCorners))
						{
							(*corners).push_back(p);
							//std::cout << p.x << " " << p.y << "\n";
							circle(markedCorners, p, 5, Scalar(0), 2, 8, 0);
						}
					}
				}
			}
			line(markedCorners, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 0), 1, LINE_AA);
		}
	}
	//puts("");
}

Mat computePerspective(double cardBigLine, double cardShortLine, Mat binarizedInput, std::vector<Point2f> corners)
{
	std::vector<Point2f> dstPoints;

	dstPoints.push_back(Point2f(0, 0));
	dstPoints.push_back(Point2f(0, cardBigLine));
	dstPoints.push_back(Point2f(cardShortLine, 0));
	dstPoints.push_back(Point2f(cardShortLine, cardBigLine));

	Mat m = getPerspectiveTransform(corners, dstPoints);
	Mat out;

	warpPerspective(binarizedInput, out, m, Size(cardShortLine, cardBigLine));
	return out;
}

std::pair<Point, double> matchTemplateBest(Mat& img, Mat& templ) {
	Mat result;
	matchTemplate(img, templ, result, TM_CCOEFF_NORMED);
	double minVal, maxVal;
	Point minLoc, maxLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
	return std::make_pair(maxLoc, maxVal);
}

void getTemplateCards(vector<Mat> *cardTemplates, vector<string> *cardLabels, double cardBigLine, double cardShortLine)
{
	vector<string> suits = { "Hearts", "Diamonds", "Clubs", "Spades" };
	vector<string> values = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
	for (const string& suit : suits) {
		for (const string& value : values) {
			string cardLabel = value + "_of_" + suit;
			Mat templ = imread("D:/Faculta/PI/Proiect/Refactored/Template/" + cardLabel + ".jpg", IMREAD_GRAYSCALE);
			if (templ.empty()) {
				cerr << "Error loading template image: " + cardLabel << endl;
				//return -1;
			}
			resize(templ, templ, Size(cardShortLine / 2, cardBigLine / 2), INTER_LINEAR);
			(*cardTemplates).push_back(templ);
			(*cardLabels).push_back(cardLabel);
		}
	}
}
std::vector<std::vector<float>> kernel = {
{1 / 16.0, 2 / 16.0, 1 / 16.0},
{2 / 16.0, 4 / 16.0, 2 / 16.0},
{1 / 16.0, 2 / 16.0, 1 / 16.0}
};

std::vector<std::vector<float>> kernelLaplace = {
{-1, -1, -1},
{-1, 8, -1},
{-1, -1, -1}
};
int main()
{

	Mat binarizedInput = prepareInputPhoto();
	Mat preparedInput;
	Mat blurredInput;
	GaussianBlur(binarizedInput, blurredInput, Size(5, 5), 0, 0);
	Canny(blurredInput, preparedInput, 100, 200);

	//imshow("preparedInput", preparedInput);
	//waitKey();

	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	std::vector<int> cardContoursIndexes;
	Mat contouredInput = computeContours(preparedInput, &contours, &hierarchy, &cardContoursIndexes);

	//imshow("contours", contouredInput);
	//waitKey();

	//std::cout << cardContoursIndexes.size();

	for (size_t i = 0; i < contours.size(); i++) {
		if (contains(cardContoursIndexes, i)) {
			Mat oneCardContour(contouredInput.rows, contouredInput.cols, CV_8UC1, Scalar(255, 255));
			drawContours(oneCardContour, contours, (int)i, Vec3b(0, 0, 0), FILLED, LINE_8);

			Mat oneCardContourCanny;
			Canny(oneCardContour, oneCardContourCanny, 100, 200);

			//imshow("eachCard", oneCardContourCanny);
			//waitKey();

			std::vector<Vec4i> lines;
			HoughLinesP(oneCardContourCanny, lines, 3, CV_PI / 180, 20, 20, 10);

			std::vector<double> distances;
			distances.clear();
			Mat justEdges(oneCardContourCanny.rows, oneCardContourCanny.cols, CV_8UC1, Scalar(255, 255));
			computeDistances(&distances, oneCardContourCanny, lines, justEdges);

			//std::cout << distances.size();

			//imshow("justEdges", justEdges);
			//waitKey();

			std::vector<Point2f> corners;
			corners.clear();
			Mat markedCorners = justEdges.clone();

			computeCorners(&corners, lines, distances, markedCorners);

			//imshow("markedCorners", markedCorners);
			//waitKey();

			double cardBigLine = distance_v2(corners.at(0), corners.at(1));
			double cardShortLine = distance_v2(corners.at(0), corners.at(2));
			
			if(cardBigLine < cardShortLine)
			{
				double temp = cardBigLine;
				cardBigLine = cardShortLine;
				cardShortLine = temp;

				Point2f tempCorner = corners.at(1);
				corners.at(1) = corners.at(2);
				corners.at(2) = tempCorner;

			}

			Mat out = computePerspective(cardBigLine, cardShortLine, binarizedInput, corners);

			//imshow("identifiedCards", templateImageCorrection(out));
			//waitKey();

			String folderpath = "D:/Faculta/PI/Proiect/Proiect/Template/*.jpg";
			std::vector<String> filenames;
			glob(folderpath, filenames);
			out = templateImageCorrection(automaticBinarized(out));
			resize(out, out, Size(cardShortLine/2, cardBigLine/2), INTER_LINEAR);

			vector<Mat> cardTemplates;
			vector<string> cardLabels;

			getTemplateCards(&cardTemplates, &cardLabels, cardBigLine, cardShortLine);

			string bestMatchCard;
			double bestMatchCardValue = -1.0;
			Point bestMatchCardLoc;

			for (size_t i = 0; i < cardTemplates.size(); ++i) {
				pair<Point, double> result = matchTemplateBest(out, cardTemplates[i]);
				Point matchLoc = result.first;
				double matchValue = result.second;
				if (matchValue > bestMatchCardValue) {
					bestMatchCardValue = matchValue;
					bestMatchCard = cardLabels[i];
					bestMatchCardLoc = matchLoc;
				}
			}

			cout << "Best match: " << bestMatchCard << endl;

			resize(out, out, Size(cardShortLine, cardBigLine), INTER_LINEAR);

			imshow(bestMatchCard, out);
			//waitKey();
		}
	}
	waitKey();

	return 0;
}
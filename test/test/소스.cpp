#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv\cv.h>
#include <iostream>
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <mysql.h>
#include <string.h>
#include <errno.h>
#include <winsock.h>

using namespace std;
using namespace cv;

#define DB_HOST "14.63.213.137"
#define DB_USER "root"
#define DB_PASS "catchup123"
#define DB_NAME "situp"
#define BUF_SIZE 1024
int gFileCnt = 0;

String upperbody_cascade_name = "C:/Users/진욱/Desktop/opencv/sources/data/haarcascades/haarcascade_mcs_upperbody.xml";
/* const char *file1 = "C:/Users/진욱/Desktop/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
const char *file2 = "C:/Users/진욱/Desktop/opencv/sources/data/haarcascades/haarcascade_eye.xml";
const char *file3 = "C:/Users/진욱/Desktop/opencv/sources/data/haarcascades/haarcascade_mcs_nose.xml";
const char *file4 = "C:/Users/진욱/Desktop/opencv/sources/data/haarcascades/haarcascade_mcs_upperbody.xml";
CvHaarClassifierCascade *cascade_f = (CvHaarClassifierCascade*)cvLoad(file1, 0, 0, 0);;
CvHaarClassifierCascade *cascade_e = (CvHaarClassifierCascade*)cvLoad(file2, 0, 0, 0);
CvHaarClassifierCascade *cascade_n = (CvHaarClassifierCascade*)cvLoad(file3, 0, 0, 0);
CvHaarClassifierCascade *cascade_m = (CvHaarClassifierCascade*)cvLoad(file4, 0, 0, 0); */
CvMemStorage   *storage;

#define NUM_OF_FEATURES 5

// CvPoint2D32f frontal_frame_features[NUM_OF_FEATURES];
// int detectFaces(IplImage *img, CvRect *face);
// int detectFaceFeatures(IplImage *img, CvRect *face);
// void sharp(const Mat& input, Mat& output);
void detectHand(Mat frame);
// void goodFeaturesToTrack_Demo(Mat frame, Mat frame2);
int tracking();
void mysqlmove(int angle);
void measurement(void);
int socketmove(void);
int recvn(int _sockFd);
void error_handling(char* message);
// void removefiles(int filenumber);
void facedetection(Mat frame, int lx, int rx, int dy, int uy);

Mat src, src_gray, src_gray2;
int upperleftx1 = 0, upperrightx1 = 0, upperleftx2 = 0, upperrightx2 = 0;
int upperyup1 = 0, upperyup2 = 0, upperydown1 = 0, upperydown2 = 0;
static const double pi = 3.14159265358979323846;
float distance2 = 0, seta = 1.30899, pi1 = 0, pi2 = 0, d = 7.1, scale1 = 640;  //seta = 0.69813-> 40도 현재 36도 -> 0.628318 0.785398 ->45도 0.401425->23eh
float nosey1 = 0, nosey2 = 0; // 코의 사진1,2의 y축 저장하는 변수
float nosex1 = 0, nosex2 = 0;
float headleftx1 = 0, headleftx2 = 0, headrightx1 = 0, headrightx2 = 0;
float headyup1 = 0, headyup2 = 0, headydown1 = 0, headydown2 = 0;
// int h = 480; // 사진의 높이
float uppersize1, uppersize2, uppersize3; // uppersize1 : 코와 명치까지 y축거리 , uppersize2 : 얼굴과 얼굴 사이의 거리  uppersize3 : 상체 x축 거리
int serv_sock;
int clnt_sock;
int k = 0;
int nSockOpt;
WSADATA wsaData;
struct sockaddr_in serv_addr;
struct sockaddr_in clnt_addr;
int clnt_addr_size;


/* void mouseEvent(int event, int x, int y, int flags, void* param)
{
	IplImage* img = (IplImage*)param;
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		printf("%d,%d\n", x, y);
	}
} */

int main(int argc, char** argv){
	int m = 1;

	if (WSAStartup(SOCK_STREAM, &wsaData) != 0){
		printf("SWAStartup fail,errorcode:%d\n", WSAGetLastError());
		return 0;
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	if (serv_sock == -1) {
		error_handling("socket() error");
		WSACleanup();

		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi("5500"));

	nSockOpt = 1;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&nSockOpt, sizeof(nSockOpt));


	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind() error");

		return -1;
	}
	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
		return -1;
	}
	clnt_addr_size = sizeof(clnt_addr);

	// while (1){
	// if (socketmove()){

	IplImage *img1 = cvLoadImage("강희7885도.png", CV_LOAD_IMAGE_COLOR); // c1_0 열기
	IplImage *img2 = cvLoadImage("강희7885도1.png", CV_LOAD_IMAGE_COLOR); // c1_1 열기
	Mat frame = img1;
	Mat frame2 = img2;
	// Mat result = img1;
	// Mat result2 = img2;

	// cvtColor(result, src_gray, CV_BGR2GRAY);
	// cvtColor(result2, src_gray2, CV_BGR2GRAY);
	storage = cvCreateMemStorage(0);
	// 	assert(cascade_f && cascade_e && storage);

	const char* name1 = "Features-Detection";
	const char* name2 = "Features-Detection2";
	cvNamedWindow(name1, 1);
	cvNamedWindow(name2, 1);

	nosex2 = nosex1;
	nosey2 = nosey1;
	// sharp(result, frame);
	detectHand(frame);
	upperleftx1 = upperleftx2;
	upperrightx1 = upperrightx2;
	upperydown1 = upperydown2;
	upperyup1 = upperyup2;
	printf("upperleftx1 : %d\nupperrightx1 : %d\nupperydown1 : %d\nupperyup1 : %d\n", upperleftx1, upperrightx1, upperydown1, upperyup1);
	facedetection(frame, upperleftx1, upperrightx1, upperydown1, upperyup1);
	nosex1 = nosex2;
	nosey1 = nosey2;
	headleftx1 = headleftx2;
	headrightx1 = headrightx2;
	headyup1 = headyup2;
	headydown1 = headydown2
		;
	// sharp(result2, frame2);
	detectHand(frame2);
	printf("upperleftx2 : %d\nupperrightx2 : %d\nupperydown2 : %d\nupperyup2 : %d\n", upperleftx2, upperrightx2, upperydown2, upperyup2);
	facedetection(frame2, upperleftx2, upperrightx2, upperydown2, upperyup2);

	measurement();

	imshow(name1, frame);
	imshow(name2, frame2);

	// cvSetMouseCallback(name1, mouseEvent, &frame);
	// cvSetMouseCallback(name2, mouseEvent, &frame2);

	cvWaitKey(0);

	// }
	// m++;
	// }
	// cvClearMemStorage(storage);
	return 0;
}

/* void sharp(const Mat& input, Mat& output){

	output.create(input.size(), input.type());

	for (int j = 1; j<input.rows - 1; j++){
		if (input.channels() == 1){
			const uchar* prevRow = input.ptr<const uchar>(j - 1); //이전 행
			const uchar* currRow = input.ptr<const uchar>(j); //현재 행
			const uchar* nextRow = input.ptr<const uchar>(j + 1); //다음 행
			uchar* resultRow = output.ptr<uchar>(j); //결과 행

			for (int i = 1; i < input.cols - 1; i++){
				*resultRow++ = saturate_cast<uchar>(5 * currRow[i] - currRow[i - 1] - currRow[i + 1] - prevRow[i] - nextRow[i]);
			}
		}
		else if (input.channels() == 3){
			const Vec3b* prevRow = input.ptr<const Vec3b>(j - 1); //이전 행
			const Vec3b* currRow = input.ptr<const Vec3b>(j); //현재 행
			const Vec3b* nextRow = input.ptr<const Vec3b>(j + 1); //다음 행

			Vec3b* resultRow = output.ptr<Vec3b>(j); //결과 행

			for (int i = 1; i<input.cols - 1; i++){

				Vec3b newPixel(saturate_cast<uchar>(5 * currRow[i][0] - currRow[i - 1][0] - currRow[i + 1][0] - prevRow[i][0] - nextRow[i][0]),
					resultRow[i][1] = saturate_cast<uchar>(5 * currRow[i][1] - currRow[i - 1][1] - currRow[i + 1][1] - prevRow[i][1] - nextRow[i][1]),
					resultRow[i][2] = saturate_cast<uchar>(5 * currRow[i][2] - currRow[i - 1][2] - currRow[i + 1][2] - prevRow[i][2] - nextRow[i][2]));
				*resultRow++ = newPixel;
			}
		}
	}
	output.row(0).setTo(Scalar(0));
	output.row(output.rows - 1).setTo(Scalar(0));
	output.col(0).setTo(Scalar(0));
	output.col(output.cols - 1).setTo(Scalar(0));
} */

void detectHand(Mat frame){

	CascadeClassifier upperbody_cascade;
	vector<Rect> hands;
	int i;
	Mat frameGray, edge;
	if (!upperbody_cascade.load(upperbody_cascade_name)) { cout << "Error loading upper body cascade file\n" << endl; return; };
	cvtColor(frame, frameGray, CV_BGR2GRAY);
	equalizeHist(frameGray, frameGray);
	upperbody_cascade.detectMultiScale(frameGray, hands, 1.1, 1, 0 | CV_HAAR_SCALE_IMAGE - 60, Size(100, 100));

	for (i = 0; i < hands.size(); i++){
		rectangle(frame, hands[i], Scalar(255, 255, 0), 2);
		upperleftx2 = hands[i].x;
		upperrightx2 = hands[i].x + hands[i].width;
		upperydown2 = hands[i].y + hands[i].height;
		upperyup2 = hands[i].y;
	}
	uppersize1 = upperydown2 - nosey1;
	uppersize3 = upperrightx2 - upperleftx1;
}

/*int detectFaces(IplImage *img, CvRect *face) {

CvSeq *faces = cvHaarDetectObjects(
img, cascade_f, storage,
1.1, 1, CV_HAAR_DO_CANNY_PRUNING, cvSize(80, 80));

if (faces->total == 0) {
return 0;
}

CvRect* _face = (CvRect*)cvGetSeqElem(faces, 0);
face->x = _face->x;
face->y = _face->y;
face->width = _face->width;
face->height = _face->height;
headleftx2 = face->x;
headrightx2 = face->x + face->width;
heady2 = face->y + face->height;
uppersize2 = face->width;


return 1;
} */

/* int detectFaceFeatures(IplImage *img, CvRect *face)
{
int i, a, b;
bool hasEyes = false;
bool hasNose = false;
cvRectangle(img,
cvPoint(face->x, face->y),
cvPoint(face->x + face->width, face->y + face->height),
CV_RGB(255, 0, 0), 1, 8, 0);


CvRect eyeROI = cvRect(face->x, face->y + (face->height / 5.5), face->width, face->height / 3.0);
CvRect noseROI = cvRect(face->x, face->y + (face->height / 2.2), face->width, face->height / 3);


CvRect *r;

cvSetImageROI(img, eyeROI);
CvSeq* eyes = cvHaarDetectObjects(
img, cascade_e, storage,
1.15, 2, 0, cvSize(25, 15));
cvResetImageROI(img);

cvSetImageROI(img, noseROI);
CvSeq* noses = cvHaarDetectObjects(
img, cascade_n, storage,
1.1, 3, 0, cvSize(25, 15));
cvResetImageROI(img);

for (i = 0; i < (eyes ? eyes->total : 0); i++) {
r = (CvRect*)cvGetSeqElem(eyes, i);
int x1 = r->x + eyeROI.x;
int y1 = r->y + eyeROI.y;
int x2 = x1 + r->width;
int y2 = y1 + r->height;
int xc = (x1 + x2) / 2;
int yc = (y1 + y2) / 2;
a = x1 + r->width / 2;
b = y1 + r->height / 2;
cvRectangle(img,
cvPoint(x1, y1),
cvPoint(x2, y2),
CV_RGB(255, 0, 0), 1, 8, 0);
cvCircle(img, cvPoint(xc, yc), 2, CV_RGB(255, 255, 0), 2, 8, 0);
float fX = xc;
float fY = yc;
frontal_frame_features[i] = cvPoint2D32f(fX, fY);
if (i == 1) {
hasEyes = true;

}
// printf("눈의 중심 좌표 : %d,%d\n", a, b);
}

for (i = 0; i < (noses ? noses->total : 0); i++) {
r = (CvRect*)cvGetSeqElem(noses, i);
int x1 = r->x + noseROI.x;
int y1 = r->y + noseROI.y;
int x2 = x1 + r->width;
int y2 = y1 + r->height;
int xc = (x1 + x2) / 2;
int yc = (y1 + y2) / 2;
front = x1 + r->width / 2;
b = y1 + r->height / 2;
cvRectangle(img,
cvPoint(x1, y1),
cvPoint(x2, y2),
CV_RGB(0, 255, 0), 1, 8, 0);
cvCircle(img, cvPoint(xc, yc), 2, CV_RGB(255, 255, 0), 2, 8, 0);
hasNose = true;
float fX = xc;
float fY = yc;
frontal_frame_features[2] = cvPoint2D32f(fX, fY);
// printf("코의 중심좌표 : %d,%d\n", front, b);
first = xc;
nosey1 = b;
nosex1 = front;
}
// cvLine(img, cvPoint(front, 0), cvPoint(front, h), Scalar(255, 55, 255), 1, 8, 0);
return (hasEyes && hasNose) ? 1 : 0;
} */

/* void goodFeaturesToTrack_Demo(Mat frame, Mat frame2)
{

RNG rng(12345);
if (maxCorners < 1) { maxCorners = 1; }
// Mat frame_gray;
cvtColor(frame, src_gray, CV_BGR2GRAY);
cvtColor(frame2, src_gray2, CV_BGR2GRAY);
/// Parameters for Shi-Tomasi algorithm
vector<Point2f> corners;
double qualityLevel = 0.02;
double minDistance = 20;
int blockSize = 2;
bool useHarrisDetector = false;
double k = 0.04;

int averageleftx1 = 0, averagerightx1 = 0, averagey1 = 0;
int averageleftx2 = 0, averagerightx2 = 0, averagey2 = 0;
int coordinate1lx = 0, coordinate1rx = 0, coordinate2lx = 0, coordinate2rx = 0;
int avercoordinate1 = 0, avercoordinate2 = 0;
int count1 = 0, count2 = 0, count3 = 0, count4 = 0;
/// Apply corner detection
goodFeaturesToTrack(src_gray,
corners,
maxCorners,
qualityLevel,
minDistance,
Mat(),
blockSize,
useHarrisDetector,
k);
averageleftx1 = (headleftx1 - upperleftx1) / 10;
averagerightx1 = (upperrightx1 - headrightx1) / 10;
averagey1 = (uppery1 - heady1) / 10;

averageleftx2 = (headleftx2 - upperleftx2) / 10;
averagerightx2 = (upperrightx2 - headrightx2) / 10;
averagey2 = (uppery2 - heady2) / 10;

/// Draw corners detected
cout << "** Number of corners detected: " << corners.size() << endl;
int r = 4;
// printf("첫번째 사진 : upper왼우y축 : %d, %d, %d\nhead왼우y축 : %d, %d, %d\n", upperleftx1, upperrightx1, uppery1, headleftx1, headrightx1, heady1);
// printf("두번째 사진 : upper왼우y축 : %d, %d, %d\nhead왼우y축 : %d, %d, %d", upperleftx2, upperrightx2, uppery2, headleftx2, headrightx2, heady2);
for (int i = 0; i < corners.size(); i++)
{
if (((upperleftx1 + averageleftx1 * 2 < corners[i].x && corners[i].x < headleftx1 - averageleftx1 * 2))
|| ((headrightx1 + averagerightx1 * 2 < corners[i].x && corners[i].x < upperrightx1 - averagerightx1))) {
if (heady1 + averagey1 * 2 < corners[i].y && corners[i].y < uppery1 - averagey1){
circle(frame, corners[i], r, Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
rng.uniform(0, 255)), -1, 8, 0);
cout << " -- Refined Corner [" << i << "]  (" << corners[i].x << "," << corners[i].y << ")" << endl;
if (upperleftx1 + averageleftx1 * 2 < corners[i].x && corners[i].x < headleftx1 - averageleftx1 * 2)
{
coordinate1lx += corners[i].x;
count1++;
}
else{
coordinate1rx += corners[i].x;
count2++;
}
}
}
}
printf("count3 : %d\ncount4 : %d\n", count1, count2);
coordinate1lx = coordinate1lx / count1;
coordinate1rx = coordinate1rx / count2;
avercoordinate1 = (coordinate1lx + coordinate1rx) / 2;
printf("%d\n", avercoordinate1);


goodFeaturesToTrack(src_gray2,
corners,
maxCorners,
qualityLevel,
minDistance,
Mat(),
blockSize,
useHarrisDetector,
k);

for (int j = 0; j < corners.size(); j++)
{

if (((upperleftx2 + averageleftx2 * 2 < corners[j].x && corners[j].x < headleftx2 - averageleftx2 * 2))
|| ((headrightx2 + averagerightx2 * 2 < corners[j].x && corners[j].x < upperrightx2 - averagerightx2))){
if (heady2 + averagey1 * 2 < corners[j].y && corners[j].y < uppery2 - averagey1){
circle(frame2, corners[j], r, Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
rng.uniform(0, 255)), -1, 8, 0);
cout << " -- Refined Corner2 [" << j << "]  (" << corners[j].x << "," << corners[j].y << ")" << endl;
if (upperleftx2 + averageleftx2 * 2 < corners[j].x && corners[j].x < headleftx2 - averageleftx2 * 2){
coordinate2lx += corners[j].x;
count3++;

}
else{
coordinate2rx += corners[j].x;
count4++;
}
}
}
}
printf("count3 : %d\ncount4 : %d\n", count3, count4);
coordinate2lx = coordinate2lx / count3;
coordinate2rx = coordinate2rx / count4;
avercoordinate2 = (coordinate2lx + coordinate2rx) / 2;
printf("%d\n", avercoordinate2);

pi3 = seta*(640 - avercoordinate1) / scale1;
pi4 = seta*avercoordinate2 / scale1;
distance3 = d * tanf((3.14159 - seta) / 2 + pi3)*tanf((3.14159 - seta) / 2 + pi4) / (tanf((3.14159 - seta) / 2 + pi3) + tanf((3.14159 - seta) / 2 + pi4));
printf("명치와의 중심 거리 : %f\n", distance3);
printf("명치 중심 좌표 : %d %d\n", avercoordinate1, avercoordinate2);



} */

void mysqlmove(int angle){
	MYSQL *conn = mysql_init(NULL);
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	char p[100] = "insert into angle values(default,000)";
	p[33] = 48 + angle / 100;
	p[34] = 48 + (angle / 10) % 10;
	p[35] = 48 + angle % 10;
	if (conn == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(conn));
		exit(1);
	}
	if (p[33] == 48 && p[34] == 48 && p[35] == 48)
		return;
	conn = mysql_real_connect(conn, DB_HOST,
		DB_USER, DB_PASS,
		DB_NAME, 3306,
		NULL, 0);
	
	mysql_query(conn, p);
	mysql_close(conn);

}

void measurement(void){
	float ratio;
	float a, b;
	int angle = 0;

	pi1 = seta*(640 - nosex1) / scale1;
	pi2 = seta*nosex2 / scale1;
	distance2 = d * (tan(0.916297 + pi1)*tan(0.916297 + pi2)) / (tan(0.916297 + pi1) + tan(0.916297 + pi2));
	a = 52.5 + tan(0.916297 + pi1);
	b = 52.5 + tan(0.916297 + pi2);
	printf("각의 합 : %f\n", a + b);
	// anticardium = uppersize1 * 17.5 / uppersize2;   //진욱 16 권재 18.5 강희 17.5
	ratio = distance2 / uppersize2 * 5000;
	printf("uppersize2 : %f\n", uppersize2);

	if (ratio <720 && ratio > 0){
		// printf("앞으로 기울었습니다.\n");
		angle = 70;
	}
	else if (ratio <800 && ratio >720){
		// printf("앞으로 기울었습니다.\n");dkdk
		angle = 75;
	}
	else if (ratio > 800 && ratio < 920){
		// printf("앞으로 기울었습니다.\n");
		angle = 80;
	}
	else if (ratio > 920 && ratio < 1070) {
		// printf("올바른 자세입니다.\n");
		angle = 85;
	}
	else if (ratio >1070 && ratio < 1200){
		// printf("올바른 자세입니다.\n");
		angle = 90;
	}
	else if (ratio >1200 && ratio <1400)
	{
		// printf("올바른 자세입니다.\n");
		angle = 95;
	}
	else if (ratio >1400 && ratio < 1550){
		// printf("뒤로 기울었습니다.\n");
		angle = 100;
	}
	else if (ratio >1550 && ratio <1650){
		// printf("뒤로 기울었습니다.\n");
		angle = 105;
	}
	if (!headleftx1){
		angle = 70;
		printf("앞으로 기울었습니다.\n");
	}

	if (!nosex1 && (headleftx1 != 0)){
		angle = 120;
		printf("뒤로 많이 기울었습니다.\n");
	}

	mysqlmove(angle);
}

void error_handling(char* message){

	fprintf(stderr, "%s", message);
	exit(1);
}

int recvn(int _sockFd) {
	FILE * image;
	int recvSize;
	int received;
	if (gFileCnt == 2)
		gFileCnt = 0;
	recv(_sockFd, (char*)&recvSize, sizeof(int), 0);

	// fprintf(stderr, "recvSize : %d\n", recvSize);
	char buf[150000];
	char *ptr = buf;
	int  left = recvSize;

	while (left > 0) {
		if ((received = recv(_sockFd, ptr, left, 0)) <= 0) {
			fprintf(stderr, "recv error : [%02d] %s\n", errno, strerror(errno));
			return -1;

		}
		else if (received == 0) {
			fprintf(stderr, "recv end\n");
			break;
		}

		left -= received;
		ptr += received;
	}

	char pathImage[1024];
	sprintf(pathImage, "C:/Temp/test/test/c1_%d.png", gFileCnt);
	gFileCnt++;

	// fprintf(stderr, "Open file : %s\n", pathImage);
	image = fopen(pathImage, "wb");
	fwrite(buf, 1, recvSize, image);
	fclose(image);

	return 1;
}

int socketmove(void){

	// printf("ok1\n");
	while (k<2) {

		if ((clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size)) == -1) {
			error_handling("accept() error");
		}
		if (recvn(clnt_sock) == -1) {
			error_handling("recv() error");
		}
		k++;
		// closesocket(clnt_sock);
	}
	k = 0;
	// closesocket(clnt_sock);

	return 1;
}

/* void removefiles(int filenumber){
char o[100] = "C:/Temp/test/test/c1_0000.png";
o[21] = 48 + filenumber / 1000;
o[22] = 48 + (filenumber % 1000) / 100;
o[23] = 48 + ((filenumber % 1000) % 100) / 10;
o[24] = 48 + filenumber % 10;
// printf("%s\n", o);
remove(o);
}*/

void facedetection(Mat frame, int lx, int rx, int dy, int uy){
	Mat gray;
	String face_cascade = "C:/Users/진욱/Desktop/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
	String eye_cascade = "C:/Users/진욱/Desktop/opencv/sources/data/haarcascades/haarcascade_mcs_nose.xml";
	CascadeClassifier face;
	CascadeClassifier eye;
	face.load(face_cascade);
	eye.load(eye_cascade);
	cvtColor(frame, gray, CV_RGB2GRAY);
	vector<Rect> face_pos;  // 얼굴위치저장
	face.detectMultiScale(gray, face_pos, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE - 50, Size(10, 10));  //얼굴검출

	//얼굴영역표시
	for (int i = 0; i < (int)face_pos.size(); i++){
		if ((face_pos[i].x + face_pos[i].width) < rx && face_pos[i].x >lx
			&& (face_pos[i].y + face_pos[i].height) < dy && face_pos[i].y > uy)
		{
			rectangle(frame, face_pos[i], Scalar(0, 255, 0), 2);
			headleftx2 = face_pos[i].x;
			headrightx2 = face_pos[i].x + face_pos[i].width;
			headyup2 = face_pos[i].y;
			headydown2 = face_pos[i].y + face_pos[i].height;
			vector<Rect> nose_pos;

			Mat roi = gray(face_pos[i]);
			eye.detectMultiScale(roi, nose_pos, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));

			//눈영역표시
			for (int j = 0; j < (int)nose_pos.size(); j++){
				Point center(face_pos[i].x + nose_pos[j].x + (nose_pos[j].width) / 2, face_pos[i].y + nose_pos[j].y + (nose_pos[j].height / 2));

				int radius = cvRound((nose_pos[j].width + nose_pos[j].height)* 0.2);
				circle(frame, center, radius, Scalar(0, 0, 225), 2);
				nosex2 = face_pos[i].x + nose_pos[j].x + (nose_pos[j].width) / 2;
				nosey2 = face_pos[i].y + nose_pos[j].y + (nose_pos[j].height) / 2;
			}
		}
		uppersize2 = face_pos[i].width;
	}
}
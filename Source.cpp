#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "Header.h"

using namespace cv;
using namespace std;

//namespace eye{
	int main()
	{
		int flag = 1000000;
		char *cascade_name="C:/opencv2.4.9/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
		CvHaarClassifierCascade *cascade=(CvHaarClassifierCascade *)cvLoad(cascade_name);
		char *cascade_eye_name="C:/opencv2.4.9/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
		CvHaarClassifierCascade *cascade_eye=(CvHaarClassifierCascade *)cvLoad(cascade_eye_name);
		CvMemStorage * storage = cvCreateMemStorage(0);
		Mat frame;
		static int done=1;
		VideoCapture cap(0); 
		abc:
		if (!cap.isOpened())  
		{
			cout << "Cannot open the video cam" << endl;
			return 0;
		}
		while (1)
		{
			cap.retrieve(frame);
			bool bSuccess = cap.read(frame); 
			if (!bSuccess) 
			{
				cout << "Cannot read a frame from video stream" << endl;
				done=0;
				cap.release();
				return done;
			}
			IplImage* frame2 = cvCloneImage(&(IplImage)frame);
			CvSeq *faces=cvHaarDetectObjects(frame2,cascade,storage,1.15,8,CV_HAAR_DO_CANNY_PRUNING,cvSize(30,30));

			if(faces)
			{
				IplImage* frame2 = cvCloneImage(&(IplImage)frame);
				IplImage* tframe;
				int i;
				i=2;
				tframe = frame2;

				CvSeq *eyes=cvHaarDetectObjects(tframe,cascade_eye,storage,1.04,8,CV_HAAR_DO_CANNY_PRUNING,cvSize(2,2));
				if (eyes)
				{
					CvRect *r=(CvRect *)cvGetSeqElem(eyes,i);

					if(r==NULL)
					{
						printf("Error");
						if(flag>0){
							flag--;
							goto abc;
						}
						else
						{
							done=0;
							flag=15;
							cap.release();
							return done;
						}
					}
					else
					{
						cvRectangle(tframe,cvPoint(r->x,r->y),cvPoint(r->x + r->width,r->y + r->height),CV_RGB(0,0,255),1,8,0);


						Mat crop(tframe, Rect(r->x,r->y,r->width,r->height));
						Mat output1 = crop.clone();
						tframe = cvCloneImage(&(IplImage)output1);
						//cvShowImage("FACE DETECTION",tframe);
						cvSaveImage("az.jpg",tframe);
#define MAX_CLUSTERS 3
						CvScalar color_tab[MAX_CLUSTERS];
						//IplImage* img = cvCreateImage( cvSize( 500, 500 ), 8, 3 );
						IplImage* img = cvLoadImage("az.jpg");
						CvRNG rng = cvRNG(0xffffffff);
						const char *name="clusters";

						color_tab[0] = CV_RGB(255,0,0);
						color_tab[1] = CV_RGB(0,255,0);
						//color_tab[2] = CV_RGB(100,100,255);
						//color_tab[3] = CV_RGB(255,0,255);
						//color_tab[4] = CV_RGB(255,255,0);
						//cvNamedWindow( name, 1 );

						int r1=0,g=0;
						float a,b;
						a=0;
						b=0;
						int s=(img->width)/2;
						int e=(img->height)/2;


						for(int j=0;j<9;j++)
						{
							a=0,b=0,g=0,r1=0; 
							int k, cluster_count = cvRandInt(&rng)%MAX_CLUSTERS + 1;
							int i, sample_count = cvRandInt(&rng)%1000 + 1;
							CvMat* points = cvCreateMat( sample_count, 1, CV_32FC2 );
							CvMat* clusters = cvCreateMat( sample_count, 1, CV_32SC1 );
							for( k = 0; k < cluster_count; k++ )
							{

								CvPoint center;
								CvMat point_chunk;
								center.x = cvRandInt(&rng)%img->width;
								center.y = cvRandInt(&rng)%img->height;

								//printf("Image Dimensions: %d %d \n\n\n",img->width,img->height);
								//printf("center %d %d \n",(img->width)/2,(img->height)/2);

								cvGetRows( points, &point_chunk,
									k*sample_count/cluster_count,
									k == cluster_count - 1 ? sample_count :
									(k+1)*sample_count/cluster_count );
								cvRandArr( &rng, &point_chunk, CV_RAND_NORMAL,
									cvScalar(center.x,center.y,0,0),
									cvScalar(img->width/6, img->height/6,0,0) );
							}

							for( i = 0; i < sample_count/2; i++ )
							{
								CvPoint2D32f* pta[100] ;
								CvPoint2D32f* pt1 = (CvPoint2D32f*)points->data.fl + cvRandInt(&rng)%sample_count;
								CvPoint2D32f* pt2 = (CvPoint2D32f*)points->data.fl +
									cvRandInt(&rng)%sample_count;
								CvPoint2D32f temp;
								CV_SWAP( *pt1, *pt2, temp );
							}

							cvKMeans2( points, cluster_count, clusters,
								cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,
								10, 1.0 ));
							cvZero( img );
							CvPoint2D32f pt,pt1; 

							for( i = 0; i < sample_count; i++)
							{
								pt = ((CvPoint2D32f*)points->data.fl)[i];
								if(sample_count/2==i )
									pt1 = ((CvPoint2D32f*)points->data.fl)[i];
								int cluster_idx = clusters->data.i[i];
								if(cluster_idx==0)
								{
									a+=pt.x;
									b+=pt.y;
									r1++;
								}
								else if(cluster_idx==1)
								{
									g++;
								}
								else
								{
									//b++;
								}

								cvCircle( img, cvPointFrom32f(pt), 2,color_tab[cluster_idx], CV_FILLED );

							}
							float temp=(r1-g)/2.0;
							if(j==8)
							{
								flag=15;
								if(pt1.x>s && pt1.y<e  ){
									printf("you are looking lower right");
									done=1;
									flag=15;
									cap.release();
									return done;
								}
								else if(pt1.x<s && pt1.y<e){
									printf("you are looking lower left");
									done=1;
									flag=15;
									cap.release();
									return done;
								}
								else if(pt1.x>s && pt1.y>e){
									printf("you are looking upper left");
									done=1;
									flag=15;
									cap.release();
									return done;
								}
								else if(pt1.x<s && pt1.y>e){
									printf("you are looking upper right");
									done=1;
									flag=15;
									cap.release();
									return done;
								}
								else if(pt1.x==s && pt1.y==e){
									printf("you are looking stright");
									done=1;
									flag=15;
									cap.release();
									return done;
								}
								else{
									printf("Wrong input");
									done=1;
									flag=15;
									cap.release();
									return done;
								}
							}
							cvReleaseMat( &points );
							cvReleaseMat( &clusters );
						}
					}
				}
				else
				{
					printf("Error");
					if(flag>0){
						flag--;
						goto abc;
					}
					else
					{
						done=0;
						flag=15;
						cap.release();
						return done;
					}
				}
			}
			else{
				printf("Error");
				if(flag>0){
					flag--;
					goto abc;
				}
				else
				{
					done=0;
					flag=15;
					cap.release();
					return done;
				}					
			}
			
		}





		//_CrtDumpMemoryLeaks();
		return done;
	}
//}
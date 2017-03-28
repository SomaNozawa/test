#include <cv.h>
#include <highgui.h>

int
main (int argc, char **argv)
{
    IplImage *src_img = 0;
    
    //画像を読み込む
    if (argc >= 2)
        src_img = cvLoadImage (argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
    if (src_img == 0)
        return -1;
    
    //画像のコピー,グレースケール変換
    IplImage *dst_img =cvCreateImage( cvGetSize(src_img),IPL_DEPTH_8U,1);
    cvCvtColor(src_img, dst_img,CV_RGB2GRAY);
    
    //画像の切り出し
    cvSetImageROI(dst_img, cvRect(0, dst_img->height * 2/3, dst_img->width, dst_img->height/3));
    
    //白線検出
    int x, y, val;
    int cnt = 0;
    CvPoint pt[cnt];
    unsigned char bright;
    
    for(y = dst_img->height*2/3; y < dst_img->height; y += dst_img->height/15){
        for(x = 0; x < dst_img->width; x += 3){
            bright = dst_img->imageData[dst_img->widthStep * y + x ];
            val = bright;
            if(val >= 250){
                pt[cnt].x = x;
                pt[cnt].y = y;
                cvCircle(src_img, pt[cnt], 2, CV_RGB(255,0,0),-1);
                if(cnt > 0){
                    cvLine (src_img, pt[cnt-1], pt[cnt], CV_RGB(255,0,0), 1);
                }
                cnt++;
                break;
            }
        }
    }
    
    //結果を表示する
    cvNamedWindow ("src", CV_WINDOW_AUTOSIZE);
    cvNamedWindow ("dst", CV_WINDOW_AUTOSIZE);
    cvShowImage ("src", src_img);
    cvShowImage ("dst", dst_img);
    cvWaitKey (0);
    
    cvReleaseImage (&src_img);
    cvReleaseImage (&dst_img);
    cvDestroyWindow ("src");
    cvDestroyWindow ("dst");
    
    return 0;
}

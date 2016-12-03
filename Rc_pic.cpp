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
    cvSetImageROI(dst_img, cvRect(0, dst_img->height*2/3, dst_img->width, dst_img->height/3));
    
    //二値化
    cvThreshold (dst_img, dst_img, 240, 255, CV_THRESH_BINARY);
    
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
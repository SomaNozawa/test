#include <cv.h>
#include <highgui.h>
#include <ctype.h>

int
main (int argc, char **argv)
{
    CvCapture *capture = 0;
    IplImage *frame = 0;
    double w = 320*2, h = 240*2;    //capturesize
    int c;
    
    // 指定された番号のカメラに対するキャプチャ構造体を作成する
    capture = cvCreateCameraCapture (0);
    
    /* この設定は，利用するカメラに依存する */
    // キャプチャサイズを設定する．
    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, w);
    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, h);
    
    cvNamedWindow ("Capture", CV_WINDOW_AUTOSIZE);
    
    // カメラから画像をキャプチャする
    while (1) {
        frame = cvQueryFrame (capture);
        
        //画像のコピー,グレースケール変換
        IplImage *dst_img =cvCreateImage( cvGetSize(frame),IPL_DEPTH_8U,1);
        cvCvtColor(frame, dst_img,CV_RGB2GRAY);
        
        //画像の切り出し
        cvSetImageROI(dst_img, cvRect(0, dst_img->height*2/3, dst_img->width, dst_img->height/3));
        
        //白線検出
        int x, y, val;
        CvPoint pt;
        unsigned char bright;
        
        for(y = dst_img->height*2/3; y < dst_img->height; y += dst_img->height/15){
            for(x = 0; x < dst_img->width; x += 3){
                bright = dst_img->imageData[dst_img->widthStep * y + x ];
                val = bright;
                if(val >= 200){
                    pt.x = x;
                    pt.y = y;
                    cvCircle(frame, pt, 3, CV_RGB(255,0,0),-1);
                    //break;
                }
            }
        }
        
        //cvShowImage ("Capture", dst_img);
        cvShowImage ("Capture", frame);
        c = cvWaitKey (2);
        if (c == '\x1b')
            break;
    }
    
    cvReleaseCapture (&capture);
    cvDestroyWindow ("Capture");
    
    return 0;
}
#include <cv.h>
#include <highgui.h>
#include <ctype.h>

int
main (int argc, char **argv)
{
    CvCapture *capture = 0;
    IplImage *frame = 0;
    int x, y, val, c;
    int cnt;
    CvPoint pt[cnt];
    unsigned char bright;
    
    
    if((capture = cvCaptureFromFile("/Users/k14094kk/Desktop/clab/pic/video.mp4")) == NULL){
        //ファイルが見つからない時
        return 0;
    }
    
    
    cvNamedWindow("Capture");
    
    
    while (1) {
        // カメラから画像をキャプチャする
        frame = cvQueryFrame (capture);
        
        //画像のコピー,グレースケール変換
        IplImage *dst_img = 0;
        
        dst_img = cvCreateImage( cvGetSize(frame),IPL_DEPTH_8U,1);
        cvCvtColor(frame, dst_img,CV_RGB2GRAY);
        
        
        //白線検出
        cnt = 0;
        
        for(y = dst_img->height*3/5; y < dst_img->height; y += dst_img->height/15){
            for(x = dst_img->width/4; x < dst_img->width/2; x += 3){
                bright = dst_img->imageData[dst_img->widthStep * y + x ];
                val = bright;
                if(val >= 125){
                    pt[cnt].x = x;
                    pt[cnt].y = y;
                    cvCircle(frame, pt[cnt], 2, CV_RGB(255,0,0),-1);
                    if(cnt > 0){
                        cvLine (frame, pt[cnt-1], pt[cnt], CV_RGB(255,0,0), 1);
                    }
                    cnt++;
                    break;
                }
            }
        }
        
        //cvShowImage ("Capture", dst_img); //カット、グレースケール
        cvShowImage ("Capture", frame);     //フルサイズ、カラー
        c = cvWaitKey (2);
        if (c == '\x1b')
            break;
    }
    
    cvReleaseCapture (&capture);
    cvDestroyWindow ("Capture");
    
    return 0;
}
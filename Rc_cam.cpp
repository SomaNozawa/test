#include <cv.h>
#include <highgui.h>
#include <ctype.h>

int
main (int argc, char **argv)
{
    CvCapture *capture = 0;
    IplImage *frame = 0;
    double w = 320*2, h = 240*2;    //capturesize
    int x, y, val, c;
    int cnt;
    CvPoint pt[cnt];
    unsigned char bright;
    
    //物体検出用変数
    CvHaarClassifierCascade* cvHCC = (CvHaarClassifierCascade*)cvLoad("/Users/k14094kk/Desktop/haarcascades/haarcascade_fullbody.xml");
    CvMemStorage* cvMStr = cvCreateMemStorage(0);
    CvSeq* face;
    
    // 指定された番号のカメラに対するキャプチャ構造体を作成する
    capture = cvCreateCameraCapture (0);
    
    /* この設定は，利用するカメラに依存する */
    // キャプチャサイズを設定する．
    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, w);
    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, h);
    
    cvNamedWindow ("Capture", CV_WINDOW_AUTOSIZE);
    
    
    while (1) {
        // カメラから画像をキャプチャする
        frame = cvQueryFrame (capture);
        
        //画像のコピー,グレースケール変換
        IplImage *dst_img =cvCreateImage( cvGetSize(frame),IPL_DEPTH_8U,1);
        cvCvtColor(frame, dst_img,CV_RGB2GRAY);
        
        //画像の切り出し
        cvSetImageROI(dst_img, cvRect(0, dst_img->height*2/3, dst_img->width, dst_img->height/3));
        
        //白線検出
        cnt = 0;
        
        for(y = dst_img->height*2/3; y < dst_img->height; y += dst_img->height/15){
            for(x = 0; x < dst_img->width; x += 3){
                bright = dst_img->imageData[dst_img->widthStep * y + x ];
                val = bright;
                if(val >= 250){
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
        
        // 画像中から検出対象の情報を取得する
        face = cvHaarDetectObjects(frame, cvHCC, cvMStr);
        
        for (int i = 0; i < face->total; i++) {
            //検出情報から顔の位置情報を取得
            CvRect* faceRect = (CvRect*)cvGetSeqElem(face, i);
            
            // 取得した顔の位置情報に基づき、矩形描画を行う
            cvRectangle(frame,
                        cvPoint(faceRect->x, faceRect->y),
                        cvPoint(faceRect->x + faceRect->width, faceRect->y + faceRect->height),
                        CV_RGB(255, 0 ,0),
                        3, CV_AA);
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
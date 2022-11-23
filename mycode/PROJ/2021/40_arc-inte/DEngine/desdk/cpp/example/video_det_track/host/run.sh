export LD_LIBRARY_PATH=../lib:LD_LIBRARY_PATH
cmd_help() {
    echo -e "\nCommand help:"
    echo "$0 -c chip num, default 1"
    echo "$0 -s stream num, default 1"
    echo "$0 -t stream type, rtsp/file/usbcamera, default rtsp"
    echo "$0 -f stream fps, default 25"
    echo "$0 -w stream pixel width, default 1920"
    echo "$0 -h stream pixel height, default 1080"
    echo "$0 -m model path, default host:/DEngine/model/dp1000/caffe_yolov3_608"
    echo "$0 -l dev libs path, default host:/DEngine/desdk/cpp/example/nodebase/dev/lib/libdevnodes.so"
    echo "$0 -u urls path, default empty, multiple URLs should be enclosed in quotation marks"
    echo -e "\nExample:"
    echo "    $0 -t rtsp -w 1920 -h 1080 -u rtsp://admin:introcks1234@192.168.33.124"
    echo "    $0 -c 1 -s 2 -t file -w 1920 -h 1080 -u "file://480p_1.h264 file://480p_2.h264""
    echo "    $0 -t usbcamera -w 640 -h 480 -f 30 v4l2:///dev/video0"
    exit
}

chip_number=1
stream_number=1
stream_type=31
stream_fps=25
stream_width=1920
stream_height=1080
model_path=host:/DEngine/model/dp1000/caffe_yolov3_416
libs_path=host:/DEngine/desdk/cpp/example/nodebase/dev/lib/libdevnodes.so
urls="rtsp://admin:introcks1234@192.168.33.124 "
#Get options.
while getopts 'c:s:t:f:w:h:m:l:u:' OPT; do
    case $OPT in
        c)
            chip_number=$OPTARG
            ;;
        s)
            stream_number=$OPTARG
            ;;
        t)
            echo $OPTARG
            if [ "$OPTARG" = "usbcamera" ]; then
                stream_type=33
            fi
      echo $stream_type
            ;;
        f)
            stream_fps=$OPTARG
            ;;
        w)
            stream_width=$OPTARG
            ;;
        h)
            stream_height=$OPTARG
            ;;
        m)
            model_path=$OPTARG
            ;;
        l)
            libs_path=$OPTARG
            ;;
        u)
            urls=$OPTARG
            ;;
        ?)
            cmd_help
            ;;
    esac
done

pidof display | xargs kill -9
pidof video_det_track | xargs kill -9

echo "/DEngine/run.sh ./display $stream_width $stream_height $stream_number 9002&"
echo "/DEngine/run.sh ./video_det_track $chip_number $stream_number $libs_path $model_path 0 $stream_type $stream_width $stream_height $stream_fps $urls"

/DEngine/run.sh ./display $stream_width $stream_height $stream_number 9002&

/DEngine/run.sh ./video_det_track $chip_number $stream_number $libs_path $model_path 0 $stream_type $stream_width $stream_height $stream_fps $urls

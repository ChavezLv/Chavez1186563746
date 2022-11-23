set -e

WORK_DIR=$(cd $(dirname $0); pwd)

wget ftp://113.100.143.90:821/release/models/dp1000 --ftp-user=ftp_guest --ftp-password=intellif --directory-prefix=$WORK_DIR/dp1000/public -nH --cut-dirs=3 -r

echo "download models from ftp successfully."
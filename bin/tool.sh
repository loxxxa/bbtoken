#! /bin/sh
prog="WatchSAFE.jar"
BIN_PATH=/usr/lib/watchdata/bbtoken/tools/
RUNJAR=`ps -ef | grep ${prog} | grep -v grep | awk '{print $2}'`
if [ "x${RUNJAR}" != "x" ]; then
    echo "Program is already running."
    exit 0
fi

cd "${BIN_PATH}"
java -Djava.library.path="${BIN_PATH}" -jar ${prog}



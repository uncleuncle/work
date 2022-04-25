log_path=log

ulimit -c 10000

export LD_LIBRARY_PATH=../lib/

#判断是否有log目录
if [ ! -d $log_path ];then
       mkdir -p $log_path
fi

./process config.ini 

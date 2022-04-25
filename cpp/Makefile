cc=g++

cc_flags=\
	 -std=c++11 \
	 -I../include \
	 -MMD

ln_flags=\
	 -L../lib -lprotobuf \
	 -L../lib -lmysqlclient \
	 -lpthread \
	 -lm \
	 -ldl \

obj=\
    ../proto/src/User.pb.o \
    XInclude/XConfig/XConfig.o \
    XInclude/XMysql/XMysql.o \
    XInclude/XThread/XThread.o \
    XInclude/XLog/XLog.o \
    XInclude/XLog/XLogFun.o \
    XProcess/XProcess.o \
    Process.o \
    main.o \

#开放打印的类型 ERROR_P WARN_P DEBUG_P
print_def= \
	  -D ERROR_P \
	  -D WARN_P \
	  -D DEBUG_P \

#开放日志类型 ERROR_L WARN_L DEBUG_L
log_def= \
	 -D ERROR_L \
	 -D WARN_L \
	 -D DEBUG_L \
    
target=process

$(target) : $(obj)
	$(cc) $(ln_flags) $(obj) -o $(target)

%.o : %.cpp
	$(cc) $(print_def) $(log_def) $(cc_flags) -c $< -o $@

%.o: %.cc
	$(cc)  $(cc_flags) -c $< -o $@


-include $(obj:.o=.d)

clean:
	rm -f $(obj) $(obj:.o=.d) $(obj:.o=.d*) $(target) core.* log/*

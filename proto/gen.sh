proto_dir=./

proto_src=./src

target=mode

#删掉生成文件，保证每次执行都是最新的
rm -rf src

#创建一个目录，用来保存pb.h pb.cc pb.o 让代码看起来更干净一些
mkdir src

#以c++的格式生成pb文件，*.pb.h *.pb.cc
protoc -I=$proto_dir --cpp_out=$proto_src $proto_dir/*.proto


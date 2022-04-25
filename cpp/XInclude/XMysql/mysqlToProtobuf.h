/**********************************************************
 * Author        : 谢名聪
 * Email         : 869408604@qq.com
 * Last modified : 2022-04-21 11:45
 * Filename      : mysqlToProtobuf.h
 * Description   : 
 * *******************************************************/

#ifndef FIRE_PROCESS_PROCESS_MYSQLER_MYSQL_PROTOBUF_H
#define FIRE_PROCESS_PROCESS_MYSQLER_MYSQL_PROTOBUF_H
#include "/usr/local/mysql/include/mysql.h"
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>

#include <vector>
#include <memory>
#include <iostream>

/*
   把mysql查询出来的结果MYSQL_RES* 根据定义的protobuf去解析,
   这样可以把结果转换成protobuf的格式，可以省去解析的流程。
*/

// 表行数据转为对应protobuf
template<typename PROTO_TYPE>
bool mysqlToProtobuf(MYSQL_RES* res, std::vector<std::shared_ptr<PROTO_TYPE>>& msgVec)
{

    //查询失败
    if (!res) {
        return false;
  }
    //sql查询结果行数
    int rowNum = mysql_num_rows(res);
    //sql查询结果字段数量
    int fcount = mysql_num_fields(res);
    
    for (int i = 0; i < rowNum; ++i) {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (!row) {
            return false;
        }

        //申请空间保存结果
        auto message = std::make_shared<PROTO_TYPE>();
        const google::protobuf::Reflection* reflection = message->GetReflection();
        const google::protobuf::Descriptor* descriptor = message->GetDescriptor();
        if (!reflection || !descriptor) {
            return false;
        }

        //判断是否匹配
        if (fcount != descriptor->field_count()) {
            return false;
        }

        //遍历proto所有的字段
        for (int i = 0; i < descriptor->field_count(); ++i) {
            //得到对应proto的描述
            const google::protobuf::FieldDescriptor* desField = descriptor->field(i);
            // 剔除repeated类型
            if (desField->label() == google::protobuf::FieldDescriptor::LABEL_REPEATED) {
                return false;
            }
            //根据类型进行string转换
            switch (desField->type()) {
                // int32_t
                case google::protobuf::FieldDescriptor::TYPE_INT32:
                    {
                        reflection->SetInt32(message.get(), desField, std::stoi(row[i]));
                        break;
                    }
                    // int64_t
                case google::protobuf::FieldDescriptor::TYPE_INT64:
                    {
                        reflection->SetInt64(message.get(), desField, std::stoll(row[i]));
                        break;
                    }
                    // uint32_t
                case google::protobuf::FieldDescriptor::TYPE_UINT32:
                case google::protobuf::FieldDescriptor::TYPE_FIXED32:
                    {
                        reflection->SetUInt32(message.get(), desField, std::stoul(row[i]));
                        break;
                    }
                    // int64_t
                case google::protobuf::FieldDescriptor::TYPE_UINT64:
                case google::protobuf::FieldDescriptor::TYPE_FIXED64:
                    {
                        reflection->SetUInt64(message.get(), desField, std::stoull(row[i]));
                        break;
                    }
                    //string bytes
                case google::protobuf::FieldDescriptor::TYPE_STRING:
                    {
                        reflection->SetString(message.get(), desField, row[i]);
                        break;
                    }
                case google::protobuf::FieldDescriptor::TYPE_BYTES:
                    {
                        //获取结果的长度
                        auto len = mysql_fetch_lengths(res);
                        reflection->SetString(message.get(), desField, std::string(row[i], len[i]));
                        break;
                    }
                    // double 
                case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
                    {
                        reflection->SetDouble(message.get(), desField, std::stod(row[i]));
                        break;
                    }
                    // bytes
                case google::protobuf::FieldDescriptor::TYPE_FLOAT:
                    {
                        reflection->SetFloat(message.get(), desField, std::stof(row[i]));
                        break;
                    }
                    // bool
                case google::protobuf::FieldDescriptor::TYPE_BOOL:
                    {
                        reflection->SetBool(message.get(), desField, std::stoi(row[i]));
                        break;
                    }
                    //暂时只写比较基础的类型，后面的再丰富
                default:
                    {
                    }
            }
        }
        msgVec.push_back(message);
    }
    return true;
}

#endif

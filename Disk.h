#ifndef _DISK_H_
#define _DISK_H_
#include<iostream>
#include<string>
#include<istream>
#include<vector>
#include<fstream>
#include<sstream>
#include <strstream>
#include<set>
#include <stdlib.h>
using namespace std;
enum Type{
    Leaf = 1,
    InterMedia,
    Root
};

// 本头文件要实现对磁盘中的数据库的设计
/* 
 * Disk中分为三类
 * DataBase
 * KeyList
 * ValueList
 * Key
 * Value
 *
*/


/*
 * DataBase构成：meteData,keyFile,valueFile
 * meteData: name,B+Tree的阶，文件末尾(注：暂时就想到这些)
 * keyFile: 存储Key的结构
 * 
*/
class MeteData {
    private:
        string name; // 数据库的名字
        int m;       // B+Tree的阶
        int keySize; // 约束key的长度
        int ValueSize; // 约束value的长度
        int klastOffSet; // Key文件的最末尾
        int vlastOffSet; // Value文件最末尾
    public:
        string getName(){
            return this->name;
        }
        void setName(string name){
            this->name = name;
        }
        int getM(){
            return this->m;
        }
        void setM(int m){
            this->m = m;
        }
        int getKLastOffSet(){
            return this->klastOffSet;
        }
        void setKLastOffSet(int lastOffSet){
            this->klastOffSet = lastOffSet;
        }
        int getVLastOffSet(){
            return this->vlastOffSet;
        }
        void setVLastOffSet(int lastOffSet){
            this->vlastOffSet = lastOffSet;
        }
};


// 此为 key在磁盘中的版本
/* 在文件中的表示方法，采用定长记录(type keySize maxKey m offsetlist[0] ....... offsetlist[m-1] ) 
 * type  maxKey offsetlist[0] ..... offsetlist[m-1] 
 * 1     keySize ...                20         )  长度为3+keySize+m*(21)
*/

class Key {
    private:
        int type; // 判断是否是叶子结点
        int maxKey; // 最大的key
        vector<int> offsetlist;
    public:

        int getType(){
            return this->type;
        }
        void setType(Type type){
            this->type = type;
        }
        int getMaxKey(){
            return this->maxKey;
        }
        void setMaxKey(int maxKey){
            this->maxKey = maxKey;
        }
        vector<int> getOffsetlist(){
            return this->offsetlist;
        }
        void setOffsetlist(vector<int> temp){
            this->offsetlist = temp;
        }
        Key(vector<int> tempInt){
            this->type = tempInt[0];
            this->maxKey = tempInt[1];
            vector<int> result;
            for(int i=2;i<tempInt.size();i++){
                result.push_back(tempInt[i]);
            }
            this->offsetlist = result;
        }
        Key(){

        }
};
class KeyFile {
    private:
        int m; // B+Tree的阶
        int keySize; // B+Tree中key的长度
        int lastOffset; //KeyFile文件的最后一个字符的偏移量
        int objectLength;
        vector<Key*>keyList;// 在内存中key的列表
        set<int> freeList; // 在内存中维护的空闲链表的位置
        string keyFileName; // keyFile的名字


        // 辅助函数帮助将int转成string
        string int_to_string(int n){
            strstream ss;
            string s;
            ss << n;
            ss >> s;
            return s;
        }
        // 辅助函数将string转成int
        int string_to_int(string s){
            stringstream sStream;
            int n;
            sStream<<s;
            sStream>>n;
            return n;
        }
        // 辅助函数将从磁盘中读取到的数字还原成一连串的数字
        vector<int> splitString(string cmdLine){
            stringstream ss(cmdLine);  // 字符流ss 
            string buf;
            vector<int> tokens;  // vector
            while (ss >> buf){
                int n = this->string_to_int(buf);  
                tokens.push_back(n);
            }
            return tokens;
        }
    public:
        // 分配Key的位置
        /*
         * 具体做法就是首先从空闲链表中寻找，找不到再append进去
         * 将key的内容写入到磁盘中
        */
        int allocKey(Key* key){
            int result = 0;
            if(freeList.size()==0){
                int result = this->lastOffset;
                this->lastOffset = this->lastOffset+this->objectLength;
            }
            else{
                int result = *(this->freeList.begin());
                this->freeList.erase(this->freeList.begin());
            }
            // 文件操作
            fstream file(this->keyFileName);
            file.seekp(result,ios::beg);
            string keyValue = ""+this->int_to_string(key->getType())+" "+this->int_to_string(key->getMaxKey())+" ";
            vector<int> offsetList = key->getOffsetlist();
            for(int i=0;i<offsetList.size();i++){
                keyValue += (this->int_to_string(offsetList[i])+" ");  
            }
            file<<keyValue;
            return result;
        }
        //将存储在磁盘上的内容转移到内存中来
        /**
         * 根据offset找到key对应的值
         * 
        */
        Key* findKey(int offset){
            
           
            // 文件操作
            char buf[this->objectLength+1];
            fstream file(this->keyFileName);
            file.seekg(offset,ios::beg);
            file.read(buf,this->objectLength);
            string temp(buf);
            vector<int> tempInt = this->splitString(temp);

            
            Key* key = new Key(tempInt);

            return key; 
        }

        /* 
         * 首先是判断offset的合法性
         * 其次是 将offset上的内容给他清除干净
         * 将offset的值放入到空闲列表中
         * 
         * 
        */

        bool deleteKey(int offset){
            if(offset>=this->lastOffset){
                return false;
            }
            else{

                // 文件操作
                fstream file(this->keyFileName);
                file.seekp(offset,ios::beg);
                string tempValue(this->objectLength,' ');
                file<<tempValue;
                this->freeList.insert(offset);
                return true;
            }
        }
        KeyFile(){
            this->objectLength = 3+this->keySize+this->m*21;
        }

};


class ValueFile {
    private:
        int valueSize; // B+Tree中value的长度
        int lastOffset; //KeyFile文件的最后一个字符的偏移量
        int objectLength;
        vector<Key*>valueList;// 在内存中key的列表
        set<int> freeList; // 在内存中维护的空闲链表的位置
        string valueFileName; // keyFile的名字


        // 辅助函数帮助将int转成string
        string int_to_string(int n){
            strstream ss;
            string s;
            ss << n;
            ss >> s;
            return s;
        }
        // 辅助函数将string转成int
        int string_to_int(string s){
            stringstream sStream;
            int n;
            sStream<<s;
            sStream>>n;
            return n;
        }
        // 辅助函数将从磁盘中读取到的数字还原成一连串的数字
        vector<int> splitString(string cmdLine){
            stringstream ss(cmdLine);  // 字符流ss 
            string buf;
            vector<int> tokens;  // vector
            while (ss >> buf){
                int n = this->string_to_int(buf);  
                tokens.push_back(n);
            }
            return tokens;
        }
    public:
        // 分配value的位置
        /*
         * 具体做法就是首先从空闲链表中寻找，找不到再append进去
         * 将value的内容写入到磁盘中
        */
        int allocValue(string value){
            int result = 0;
            if(freeList.size()==0){
                int result = this->lastOffset;
                this->lastOffset = this->lastOffset+this->objectLength;
            }
            else{
                int result = *(this->freeList.begin());
                this->freeList.erase(this->freeList.begin());
            }
            // 文件操作
            fstream file(this->valueFileName);
            file.seekp(result,ios::beg);
            string tempValue = ""+value+" ";
            file<<tempValue;
            return result;
        }
        //将存储在磁盘上的内容转移到内存中来
        /**
         * 根据offset找到key对应的值
         * 
        */
        string findValue(int offset){
            
           
            // 文件操作
            char buf[this->objectLength+1];
            fstream file(this->valueFileName);
            file.seekg(offset,ios::beg);
            file.read(buf,this->objectLength);
            string value(buf);
            return value;
        }

        /* 
         * 首先是判断offset的合法性
         * 其次是 将offset上的内容给他清除干净
         * 将offset的值放入到空闲列表中
         * 
         * 
        */

        bool deleteValue(int offset){
            if(offset>=this->lastOffset){
                return false;
            }
            else{

                // 文件操作
                fstream file(this->valueFileName);
                file.seekp(offset,ios::beg);
                string tempValue(this->objectLength,' ');
                file<<tempValue;
                this->freeList.insert(offset);
                return true;
            }
        }
        ValueFile(){
            this->objectLength = this->valueSize+1;
        }

};

class DataBase {
    private:
        MeteData meteData;
        KeyFile keyFile;
        ValueFile ValueFile;
};
#endif 

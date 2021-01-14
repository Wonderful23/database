#ifndef _CACHE_H_
#define _CACHE_H_
#include<iostream>
#include<string>
#include<istream>
#include<vector>
#include<sstream>
#include<algorithm>
#include <stdlib.h>
using namespace std;
// 用于Cache设计，提高检索速度，采用LRU策略

// 在Cache的数据单元
class KeyValue {
    private:
        int key; // key
        string value; // value
        long time;// 对key访问相关哦都可以，比如find or update
    public:
        int getKey(){
            return this->key;
        }
        void setKey(int key){
            this->key = key;
        }
        string getValue(){
            return this->value;
        }
        void setValue(string value){
            this->value = value;
        }
        long getTime(){
            return this->time;
        }
        void setTime(long time){
            this->time = time;
        }
};


class Cache {
    private:
        vector<KeyValue*> keyValueList;// 按照key进行升序排列
        int nums; // cache中能够容纳的(key,value)的数量
        long count;//计数器(有点问题，但是不太管了，解决方法就是每多少次操作就给他清一清)
    public:
        // 根据key进行查找
        /*
         * 二分查找法找到相应的值，修改time
         * 未找到返回NULL
         * 计数器加一
         * 
        */
        KeyValue* findKey(int key){
            this->count += 1; 
            
            int low = 0;
            int high = this->keyValueList.size()-1;
            while (low <= high){
                
                int middle = (low + high) / 2;
                int currentKey = this->keyValueList[middle]->getKey();
                if ( key == currentKey){

                    this->keyValueList[middle]->setTime(this->count);
                    return this->keyValueList[middle];
                }
                else if ( key > currentKey)
                {
                    low = middle + 1;
                }
                else if (key < currentKey)
                {
                    high = middle - 1;
                }
            }
            return NULL;
        }
        // 插入(key,value)
        /* 计数器加一
         * 若cache中还没满
         *     插入keyValueList重新排序
         * 若满了
         *     删除Time最小的，然后再插入，之后再重新排序
         * 
         * 
        */
        void insert(KeyValue* keyValue){
            this->count ++;
            keyValue->setTime(this->count);
            
            if(this->keyValueList.size()<this->nums){
                this->keyValueList.push_back(keyValue);
                
            }
            else{
                vector<KeyValue*>::iterator minPos = min_element(this->keyValueList.begin(),this->keyValueList.end(),[](KeyValue* k1,KeyValue*k2){
                    return k1->getTime() < k2->getTime();
                });
                this->keyValueList.erase(minPos);
                this->keyValueList.push_back(keyValue);
            }
            sort(this->keyValueList.begin(),this->keyValueList.end(),[](KeyValue* k1,KeyValue* k2){
                return k1->getTime()< k2->getTime();
            });
        }


};

#endif 
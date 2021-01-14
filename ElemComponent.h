#ifndef _ElemComponent_H_
#define _ElemComponent_H_
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

const int M = 5; // B+Tree的阶数

// 说明结点的类型
enum Type{
    Leaf = 1,
    InterMedia,
    Root
};

// 存储数据的元组
class Tuple
{
    private:
        
        // 数值关系
        int key; // key的值
        string value; // value的值
        
    public:
        //  获得key的值
        int getKey(){
            return this->key;
        }
        // 设置key的值
        void setKey(int key){
            this->key = key;
        }
        // 获得value的值
        string getValue(){
            return this->value;
        }
        // 设置value的值
        void setValue(string value){
            this->value = value;
        }


};
// 用于中间结点和根结点
class Node{
    private:
        
        // 通用
        int m; // 阶数
        Type type;
        int maxIndex; // 在indexes中最大的值帮助加快操作速度
        vector<Node*> indexes; // 结点中的索引的数组

    public:
        // 获得结点的类型
        Type getType(){
            return this->type;
        }
        // 设置结点的类型
        void setType(Type type){
            this->type = type;
        }
        // 获得结点中最大的索引
        int getMaxIndex(){
            return this->maxIndex;
        }
        // 设置结点中最大的索引
        void setMaxIndex(int maxIndex){
            this->maxIndex = maxIndex;
        }
        // 获得整个索引列表
        vector<Node*> getIndexes(){
            return this->indexes;
        }
        /*
         * 在索引列表中加入结点
         * 当满足数值关系直接添加成功，并重新按照重小到大排序,并修改最大值
         * 当不满足数值关系不能直接添加成功，交给上级树结构中处理
         * 适用于中间结点与根结点，叶子结点应对values进行操作
         * 
        */
        bool addIndex(Node* node){
            if(indexes.size()<=m-1){

                indexes.push_back(node);
                sort(indexes.begin(),indexes.end(),[](Node* node1,Node* node2){
                    return node1->getMaxIndex()<node2->getMaxIndex();
                });
                this->maxIndex = indexes[indexes.size()-1];
                return true;
            }
            else{
                return false;
            }
        }

        /*
         * 在索引列表中删除结点
         * 当满足数值关系直接删除成功,并修改最大值
         * 当不满足数值关系，交给上级树结构中处理
         * 注意要重点考虑根结点和中间结点的差异
         * 
         * 
         * 
         */
        bool deleteIndex(int i){

            // i符合条件
            if(i>=0 && i<this->indexes.size()){

                // 中间结点操作
                if(this->type == Type::InterMedia){
                    if(indexes.size()>=m/2+1){
                        this->indexes.erase(this->indexes.begin()+i);
                        this->maxIndex = this->indexes[indexes.size()-1];
                    }
                    else{
                        return false;
                    }
                }
                // 根结点
                else{
                    if(indexes.size()>=3){
                        this->indexes.erase(this->indexes.begin()+i);
                        this->maxIndex = this->indexes[indexes.size()-1];
                    }
                    else{
                        return false;
                    }
                }
            }
            else
            {
                return false;
            }
            
        }

};
// 用于叶子结点
class LeafNode:public Node{
    private:
        vector<Tuple*> values; // 只有叶子结点才能保证该数组有值
        LeafNode* rightNode; // 只有叶子结点才会有这个链表
    public:
        vector<Tuple*> 
};


class BPlusTree
{
    private:
	    double real, imag;
    public:
	    BPlusTree();//空的构造函数
	    bool Insert();
};

#endif 
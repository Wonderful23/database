#ifndef _CACHE_H_
#define _CACHE_H_
#include<iostream>
#include<string>
#include<istream>
#include<vector>
#include<sstream>
#include<algorithm>
#include <stdlib.h>
#include<math.h>
using namespace std;
// 用于Memery中的B+Tree的设计
enum Type{
    ROOT = 0,
    INTERMEDIA
};

// 辅助类，帮助简化Node结点中映射的
class INT_TO_Node {
    private:
        int value;
        Node* node;
    public:
        int getValue(){
            return this->value;
        }
        void setValue(int value){
            this->value = value;
        }
        Node* getNode(){
            return this->node;
        }
        void setNode(Node* node){
            this->node = node;
        }
        INT_TO_Node(int value,Node* node){
            this->value = value;
            this->node = node;
        }
};
// 借数值的辅助函数
class BorrowHepler{
    private:
        int type;// 0:借不到，1:向左边借，2:向右边借
        Node* borrower; // 被借的结点
        Node* borrowConcat;// 被借出的结点的内容
        bool flag; // 是否能够成功借
    public:
        int getType(){
            return this->type;
        }
        void setType(int type){
            this->type = type;
        }
        Node* getBorrower(){
            return this->borrower;
        }
        void setBorrower(Node* node){
            this->borrower = node;
        }
        bool getFlag(){
            return this->flag;
        }
        void setFlag(bool flag){
            this->flag = flag;
        }
        Node* getBorrowConcat(){
            return this->borrowConcat;
        }
        void setBorrowConcat(Node* node){
            this->borrowConcat = node;
        }
        BorrowHepler(int type,Node* borrower,Node* borrowConcat,bool flag){
            this->type = type;
            this->borrower = borrower;
            this->borrowConcat = borrower;
            this->flag = flag;
        }
};
// 
class Node{
    private:
        Type type; // 结点的类型
        int maxValue; // 最大索引值
        int m;// B+Tree的阶数
        vector<INT_TO_Node*> nodes;// 结点中含有的内容
        Node* parent; //写的时候发现必须要知道父结点
    public:
        vector<INT_TO_Node*> getNodes(){
            return this->nodes;
        }
        Node* getParent(){
            return this->parent;
        }
        int getMaxValue(){
            return this->maxValue;
        }
        Node(int m,vector<INT_TO_Node*>nodes,Type type,int maxValue,Node* parent){
            this->m = m;
            this->nodes = nodes;
            this->type = type;
            this->maxValue = maxValue;
            this->parent = parent;
        }
        // 初始化根结点
        Node(int m,int value){
            this->m = m;
            this->maxValue = value;
            this->type = Type::ROOT;
            this->nodes.push_back(new INT_TO_Node(value,NULL));
            this->parent = NULL;
        }

        // 范围查找返回满足ki-1<value<=ki
        Node* findRangeValue(int value){
            //起始结点因为无法[-1,0]所以单独考虑
            if(this->nodes[0]->getValue()== value){
                return this->nodes[0]->getNode();
            }
            for(int i=1;i<this->nodes.size();i++){
                if(this->nodes[i-1]->getValue()<value && this->nodes[i]->getValue()>=value){
                    return this->nodes[i]->getNode();
                }
            }
        }
        // 寻找这个结点左右兄弟
        /*
         * 分情况讨论，
         * 情况一: 这个结点在最左边，显然只能往右边去借
         * 情况二: 这个结点在最右边, 显然只能往左边去借
         * 情况三: 这个结点在中间，显然往两边都可以借
         * 返回值就是可以操作的兄弟结点的集合
        
        */
        vector<BorrowHepler*> findSibling(){
            int index = this->parent->findValue(this->maxValue);
            vector<BorrowHepler*> result;
            if(index == 0){
                BorrowHepler* borrowHelper = new BorrowHepler(2,this->parent->getNodes()[1],NULL,false);
                result.push_back(borrowHelper);
                return result;
            }
            else if(index == this->parent->getNodes().size()-1){
                BorrowHepler* borrowHelper = new BorrowHepler(1,this->parent->getNodes()[index-1],NULL,false);
                result.push_back(borrowHelper);
                return result; 
            }
            else{
                result.push_back(new BorrowHepler(1,this->parent->getNodes()[index-1],NULL,false));
                result.push_back(new BorrowHepler(2,this->parent->getNodes()[index+1],NULL,false));
                return result;
            }
        }
        BorrowHepler* borrowFromSibling(){
            vector<BorrowHelper*> possibleSibling  = this->findSibling();
            BorrowHelper* result = NULL;
            for(int i=0;i<possibleSibling.size();i++){
                Node* targetNode = possibleSibling[i]->getBorrower();
                if(targetNode->getNodes().size()>=ceil(this->m/2)+1){
                    if(possibleSibling[i]->getType() == 1){

                    }
                    else if(possibleSibling)
                }
            }
        }
        /*
         * 将超过m的结点一分为二，前(M+1)/2,后M-(M+1)/2
         * 
        */
        Node* splitNode(){
            if(this->nodes.size() != this->m+1)return NULL;
            else{
                int splitIndex = this->m/2;
                int firstMaxValue = this->nodes[splitIndex]->getValue();
                vector<INT_TO_Node*> firstNodes,secondeNodes;
                for(int i=0;i<=splitIndex;i++){
                    firstNodes.push_back(this->nodes[i]);
                }
                for(int i=splitIndex+1;i<this->nodes.size();i++){
                    secondeNodes.push_back(this->nodes[i]);
                }
                // 后面的
                this->nodes = secondeNodes;
                // 前面的
                return new Node(this->m,firstNodes,this->type,firstMaxValue,this->parent);
            }
        }
        /*
         * 判断目前node是否是合法的，从nodes的大小来看
         * 参数解释: total 表示的是BplusTree的总的key的数量，如果total<=m说明root 为rootLEAF
         * 
        */
        bool isValid(int total){
            if(this->type == Type::ROOT && total <= this->m){
                return this->nodes.size() > 0 && this->nodes.size() <= this->m;
            }
            else if(this->type == Type::ROOT){
                return this->nodes.size() > 1 && this->nodes.size() <= this->m;
            }
            else{
                return this->nodes.size() >= ceil(m/2) && this->nodes.size() <= this->m;
            }
        }
        /*
         * 整理nodes,maxValue，使其满足条件
        */
        void reorganize(){
            sort(this->nodes.begin(),this->nodes.end(),[](INT_TO_Node* n1,INT_TO_Node* n2){
                return n1->getValue() <n2->getValue();
            });
            this->maxValue = this->nodes[this->nodes.size()-1]->getValue();
        }
        /*
         * 完成更新操作
         * 将beforeValue的值改成currentValue的值，并更新maxValue,排序，返回旧的maxValue
         */
        int update(int currentValue,int beforeValue){
            int result = this->maxValue;
            for(int i=0;i<this->nodes.size();i++){
                if(this->nodes[i]->getValue() == beforeValue){
                    this->nodes[i]->setValue(currentValue);
                    break;
                }
            }
            this->reorganize();
            return result;
        }
        // 寻找value的位置，若没找到返回-1
        int findValue(int value){
            for(int i=0;i<this->nodes.size();i++){
                if(this->nodes[i]->getValue() == value){
                    return i;
                }
            }
            return -1;
        }
        // 向该结点中插入value值
        /*
         * 直接插入,排序，修改maxValue
         * 返回原来maxValue;
         * (注:破坏分层结构想办法解决)
         * 
         * 
         */
        int addValue(int value,Node* tempNode){
            int result = this->getMaxValue();
            INT_TO_Node* temp = new INT_TO_Node(value,tempNode);
            this->nodes.push_back(temp);
            this->reorganize();
            return result;
        }
        // 向该结点删除值
        /*
         * 找到值直接删除掉，修改maxValue,返回原来的maxValue
         * (注：破坏了分层结构和函数的原本意思，要非常小心)
         *
         * 
         * 
        */
        int deleteValue(int value){
            
            int result = this->maxValue;
            int index = this->findValue(value);
            this->nodes.erase(this->nodes.begin()+index);
            this->reorganize();
            return result;
        }
};
class BPlusTree{
    private:
        Node* root;//根结点
        int m;//阶数
        int total;//key的总数
        // 辅助函数求B+Tree的高度
        int Height(){
            int height =0;
            Node* currentNode = this->root;
            while(root != NULL){
                height++;
                currentNode = this->root->getNodes()[0]->getNode();
            }
            return NULL;
        }
        /*
         * 预处理的关键步骤，需要利用递归操作完成
         * 
         * 算法过程
         * 1.首先比较currentValue值是否是最大，若不是直接返回
         * 2.如果是最大的，需要修改node->parent中beforeValue值为currentValue,然后排序，之后修改maxValue
         * 3.重复1.2步骤，知道node == NULL
         * 
         * 
         * 
         * 
        */
        void preTreatmentKeyStep(int currentValue,int beforeValue,Node* node){
            if(node == NULL){
                return;
            }
            else{
                int before = node->update(currentValue,beforeValue);
                if(currentValue != node->getMaxValue())
                    return;
                else{
                    preTreatmentKeyStep(currentValue,before,node->getParent());
                }
            }
        }
        /* 
         * 辅助函数对插入操作进行预处理使得符合数值关系
         *
        */
        void preInsert(int currentValue,int beforeValue,Node* node){
            // 第一步单独处理一下
            if(node == NULL) 
                return;
            else{
                if(currentValue != node->getMaxValue()) 
                    return;
                else{
                    preTreatmentKeyStep(currentValue,beforeValue,node->getParent());
                }
            }
            // 剩下的通过递归操作完成
        }


        /*
         * 辅助函数在删除操作彻底完成之前先调整数值关系
         * 如果 deleteValue != beforeValue，不需要调整
         * 如果 deleteValue == beforealue, 按照以下规则调整
         * 沿着parent链将沿途中的beforeValue改成currentValue，如果改的不是最大值就结束吧，否则就一直改到根结点
        
        */
        void preDelete(int currentValue,int beforeValue,int deleteValue,Node*node){
            if(node == NULL)
                return;
            else{
                if(deleteValue != beforeValue)
                    return;
                else{
                    return this->preTreatmentKeyStep(currentValue,beforeValue,node->getParent());
                }
            }
        }
    public:

        // 初始化一棵B+Tree
        BPlusTree(int m,int value){
            this->m = m;
            root = new Node(m,value);
        }
        // 寻找
        /*
         * 寻找的方法是，由于Node的索引结构是
         * k1<k2<k3<....<kn-1<kn
         * 找到 ki<value<=ki+1 再从nodes[i+1]出发集训寻找
         * 
         * 重复上述过程，直到找到nodes[i+1]==NULL,就返回nodes[i+1]
         * 
        */
        Node* findValue(int value){
           Node* currentNode =  root;
           while(currentNode->findRangeValue(value)){
               currentNode = currentNode->findRangeValue(value);
           }
           return currentNode;
        }
        // 遍历整棵树
        void Visit(){

        }

        /*
         * 插入一个值完成插入的递归操作
         * 以下递归操作
         * 步骤二(情况1 结束递归的条件): 若能直接插入成功，返回
         * 步骤三(中间步骤 实现结点分裂): 将结点分裂成 前(M+1)/2,后M-(M+1)/2,
         * 步骤三(情况2 分裂的结点不是根结点的情况): 将前(M+1)/2的maxIndex往上插入
         * 步骤四(情况3 分裂的结点是根结点的情况 结束递归的条件): 生成新的根的结点，孩子有两个
         * 
         * 
        */
        bool insertValuekeyStep(Node* frontNode,Node* endNode,Node* targetNode){
            // 需要生成新的根结点
            if(targetNode == NULL){
                vector<INT_TO_Node*> nodes;
                nodes.push_back(new INT_TO_Node(frontNode->getMaxValue(),frontNode));
                nodes.push_back(new INT_TO_Node(endNode->getMaxValue(),endNode));
                Node* newNode = new Node(this->m,nodes,Type::ROOT,endNode->getMaxValue(),NULL);
                this->root = newNode;
                return true;
            }
            else{
                targetNode->addValue(frontNode->getMaxValue(),frontNode);
                if(targetNode->isValid(this->total)) return true;
                else{
                    Node* newNode = targetNode->splitNode();

                    return this->insertValuekeyStep(newNode,targetNode,targetNode->getParent());
                }
            }
        }
        // 插入一个数值
        /*
         * 
         * 步骤一(预处理): 寻找到合适的位置，判断插入的值是不是最大的，如果是的话，沿着parent链进行更新
         * 由于起始实在叶子结点，情况比较不一样，需要单独处理，剩余的通过递归进行处理
         * 
        */
        bool insertValue(int value){
            this->total ++;
            // 判断位置，而且看是否已经有value了，有了直接返回
            Node* target = this->findValue(value);
            if(target->findValue(value) != -1) return false;

            // 先插入，再处理
            int  before = target->addValue(value,NULL);
            // 预处理，调整数值关系
            preInsert(value,before,target);
            if(target->isValid(this->total))
                return true;
            else{
                Node* newNode = target->splitNode();

                return this->insertValuekeyStep(newNode,target,target->getParent());
            }
        }
        /*
         * 预处理: 寻找到合适的位置
         * 以下为递归操作
         * 步骤一(数值关系调整): 删除掉该值，判断删除的值是不是最大的，如果是的话，先沿着parent链向上进行更新(数值关系)
         * 步骤二(情况一 递归结束的条件一): 判断删除的结点的位置是不是满足数量关系，如果满足递归结束
         * 步骤三(情况二): 不满足的情况下，从兄弟结点借，能借的话，最左边的往右边借，其他向左边借，借结点就转成删除兄弟某结点的操作，以此来实现递归
         * 步骤四(情况三): 如果兄弟处借不到，需要将这两个兄弟结点进行合并，转成对父亲结点的删除操作
         * 步骤五(情况四 递归结束的条件二): 递归操作不断回溯到根结点，如果根结点不满足条件就删除根结点，完成操作。
        */
        bool deleteValueKeyStep(int value,Node* targetNode){
            
            if(targetNode == NULL){
                return true;
            }
            else{
                int before = targetNode->deleteValue(value);
                this->preDelete(targetNode->getMaxValue(),before,value,targetNode);
                // 情况一
                if(targetNode->isValid(this->total)) 
                    return true;
                else{

                }
            }
            
        }
        // 删除一个数值
        /*
         * 步骤一(预处理): 寻找到合适的位置，删除掉该值，判断删除的值是不是最大的，如果是的话，先沿着parent链向上进行更新(数值关系)
         * 步骤二(情况一 递归结束的条件一): 判断删除的结点的位置是不是满足数量关系，如果满足递归结束
         * 步骤三(情况二): 不满足的情况下，从兄弟结点借，能借的话，最左边的往右边借，其他向左边借，借结点就转成删除兄弟某结点的操作，以此来实现递归
         * 步骤四(情况三): 如果兄弟处借不到，需要将这两个兄弟结点进行合并，转成对父亲结点的删除操作
         * 步骤五(情况四 递归结束的条件二): 递归操作不断回溯到根结点，如果根结点不满足条件就删除根结点，完成操作。
        */
        bool deleteValue(int value){
            this->total --;
            Node* target = this->findValue(value);
            if(target->findValue(value) == -1) return false;

        }


};


#endif 
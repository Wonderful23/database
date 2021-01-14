#ifndef _CMD_H_
#define _CMD_H_
#include<iostream>
#include<string>
#include<istream>
#include<vector>
#include<sstream>
#include <stdlib.h>
using namespace std;



// 命令行处理模块

/* 
 * quit:退出命令行
 * clear database: 清除数据库数据 
 * initial database: 初始化一个数据库
 * update key value:修改(key,value)
 * delete key: 删除(key,value)
 * find key: 获得key对应的value的值
 * insert key value:插入(key,value)
 * 
 * 
 */
class CMD {
    private:
       // 将输入命令进行切分
        vector<string> splitString(string cmdLine){
            stringstream ss(cmdLine);  // 字符流ss 
            string buf;
            vector<string> tokens;  // vector
            while (ss >> buf)  
                tokens.push_back(buf);
            return tokens;
        }
        // 处理每一条命令
        void keyStep(string cmdLine){
            
            vector<string> interMedia = this->splitString(cmdLine);
            if(interMedia.size()!=0){
                string oper = interMedia[0];
                if(oper == "update"){
                    if(interMedia.size() == 3){
                        cout<<"You "<<oper<<" key="<<interMedia[1]<<" value="<<interMedia[2]<<endl;
                    }
                    else{
                        cout<<"Please input valid command line!"<<endl;
                    }
                }
                else if(oper == "delete") {
                    if(interMedia.size() == 2){
                        cout<<"You "<<oper<<" key="<<interMedia[1]<<endl;
                    }
                    else{
                        cout<<"Please input valid command line!"<<endl;
                    }
                }
                else if(oper == "insert"){
                    if(interMedia.size() == 3){
                        cout<<"You "<<oper<<" key="<<interMedia[1]<<" value="<<interMedia[2]<<endl;
                    }
                    else{
                        cout<<"Please input valid command line!"<<endl;
                    }
                }
                else if(oper == "find") {
                    if(interMedia.size() == 2){
                        cout<<"You "<<oper<<" key="<<interMedia[1]<<endl;
                    }
                    else{
                        cout<<"Please input valid command line!"<<endl;
                    }
                }
                else if(oper == "clear") {
                    if(interMedia.size() == 2){
                        cout<<"You "<<oper<<" dataBase "<<interMedia[1]<<endl;
                    }
                    else{
                        cout<<"Please input valid command line!"<<endl;
                    }
                }
                else if(oper == "quit"){
                    if(interMedia.size() == 1){
                        cout<<"You exit database"<<endl;
                        exit(0);
                    }
                    else{
                        cout<<"Please input valid command line!"<<endl;
                    }
                }
                else if(oper == "initial"){
                    if(interMedia.size() == 2){
                        cout<<"You "<<oper<<" database "<<interMedia[1]<<endl;
                    }
                    else{
                        cout<<"Please input valid command line!"<<endl;
                    }
                }
                else{                        
                    cout<<"Please input valid command line!"<<endl;
                }
            }
            else{
                cout<<"Please input valid command line!"<<endl;
            }
        }
    public:
        void run(){
            string cmdLine;
            while(true){
                std::getline(cin,cmdLine);
                this->keyStep(cmdLine);
            }
        }
};

#endif 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

class Node{
  public:
    Node* next;
    Node* son;
    Node* father;
    int label;
    int fq;
    int depth;
    Node(){
      depth = 0;
      son = NULL;
      next = NULL;
      father = NULL;
      label = -1;
      fq = 0;
    }
    Node(int label,int fq,int depth){
      son = NULL;
      next = NULL;
      this->label = label;
      this->fq = fq;
      this->depth = depth;
    }
    Node(int label,int fq,int depth,Node* father){
      son = NULL;
      next = NULL;
      this->father = father;
      this->label = label;
      this->fq = fq;
      this->depth = depth;
    }
};

class Tries{
  private:
    int minsupp;
    Node* root;
    int depth;
    ifstream in;
    ofstream out;
    set<int> term;
  public:
    Tries(){
      minsupp = 0;
      root = new Node;
      depth = 0;
    }
    void setms(int minsupp){
      this->minsupp = minsupp;
    }
    void setifof(char* fin,char* fout){
      in.open(fin,ifstream::in);
      out.open(fout,ofstream::out);
    }
    void init();
    void dfs(Node* now,set<int>::iterator it,set<int>::iterator end);
    void search();
    void print();
    void grow();
    void check();
};

void Tries::grow(){
  Node* now = root->son;
  Node* old = root;
  while(now != NULL || old != root){
    if(now == NULL){
      now = old->next;
      old = old->father;
    }else if(now->depth < depth){
      old = now;
      now=now->son;
    }else{
      set<int>::iterator it = ++term.find(now->label);
      if(it != term.end()){
        now->son = new Node(*it,0,depth+1,now);
        it++;
      }
      Node* tmp = now->son;
      for(;it != term.end();++it){
        tmp->next = new Node(*it,0,depth+1,now);
        tmp = tmp->next;
      }
      now = now->next;
    }
  }
  depth++;
}

void Tries::init(){
  set<int> words;
  map<int,int> fq;
  string tmp;
  while ( !in.eof() ) {
    getline(in,tmp);
    char cstr[tmp.length() + 1];
    strcpy(cstr, tmp.c_str());
    for (char *token=strtok(cstr," ");token;token=strtok(0," ")){
      int wordid = atoi(token);
      if(wordid > 0){
        words.insert(wordid);
        if(fq.find(wordid) == fq.end()){
          fq[wordid] = 1;
        }else{
          fq[wordid] = fq[wordid] + 1;
        }

      }
    }
  }
  set<int>::iterator it = words.begin();
  while(it != words.end() && fq[*it] < minsupp){
    it++;
  }
  if(it != words.end()){ 
    depth++;
    root->son = new Node(*it,fq[*it],1,root);
    term.insert(*it);
    it++;
  }
  Node* now = root->son;
  for (; it!=words.end(); ++it){
    if(fq[*it] >= minsupp){
      now->next = new Node(*it,fq[*it],1,root);
      term.insert(*it);
      now = now->next;
    }
  }

}
void Tries::dfs(Node* now,set<int>::iterator it,set<int>::iterator end){
  while(it != end){
    if(now == NULL) return;
    if(*it < now->label){
      it++;
    }else if(*it == now->label){
      if(now->depth == this->depth){
        now->fq++;
        now = now->next;
      }else{
        dfs(now->son,it,end);
        now = now->next;
        it++;
      }
    }else{
      now = now->next;
    }
  }
}

void Tries::search(){
  in.clear();
  in.seekg(0,ios::beg);
  string tmp;
  while ( !in.eof() ) {
    getline(in,tmp);
    char cstr[tmp.length() + 1];
    strcpy(cstr, tmp.c_str());
    set<int> line;
    for (char *token=strtok(cstr," ");token;token=strtok(0," ")){
      line.insert(atoi(token));
    }
    this->dfs(this->root->son,line.begin(),line.end());
  }
  return;
}

void Tries::print(){
  cout<<"depth:"<<depth<<endl;
  Node* now = root->son;
  Node* old = root;
  vector<int> tmp;
  while(now != NULL || old != root){
    if(now == NULL){
      while(old->next == NULL){
        if(old == root){
          cout<<"print over"<<endl;
          return;
        } 
        tmp.pop_back();
        old = old -> father;
      }
      tmp.pop_back();
      now = old->next;
      old = old->father;
    }else if(now->depth < depth){
      tmp.push_back(now->label);
      old = now;
      now=now->son;
    }else{
      while(now != NULL){
        if(now->fq <= minsupp){
          now = now->next;
          continue;
        }
        for (vector<int>::iterator it=tmp.begin();it!=tmp.end();++it)
          out<<*it<<" ";
        out<<now->label<<":"<<now->fq<<endl;
        now = now->next;
      }
    }
  }
  cout<<"print over"<<endl;
}

void Tries::check(){
  Node* now = root->son;
  Node* old = root;
  while(now != NULL || old != root){
    if(now == NULL){
      now = old->next;
      old = old->father;
    }else if(now->depth < depth){
      old = now;
      now=now->son;
    }else{
      set<int>::iterator it = ++term.find(now->label);
      if(it != term.end()){
        now->son = new Node(*it,0,depth+1,now);
        it++;
      }
      Node* tmp = now->son;
      for(;it != term.end();++it){
        tmp->next = new Node(*it,0,depth+1);
        tmp = tmp->next;
      }
      now = now->next;
    }
  }
}


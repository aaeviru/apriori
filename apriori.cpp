#include "apriori.h"

int main(int argc,char** argv){
  if(argc != 4){
     printf("input:input-filename output-filename min-sup\n");
     return -1;
  }
  printf("min-sup:%s\n",argv[3]);
  Tries test;
  test.setifof(argv[1],argv[2]);
  test.setms(atoi(argv[3]));
  test.init();
  test.print();
  test.grow();
  test.search();
  test.print();
  return 0;
}



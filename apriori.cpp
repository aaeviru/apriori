#include "apriori.h"

int main(int argc,char** argv){
  if(argc < 4) return -1;
  Tries test;
  test.setifof(argv[1],argv[2]);
  test.setms(atoi(argv[3]));
  test.init();
  test.print();
  test.grow();
  test.search();
  test.grow();
  test.search();

  test.print();
  return 0;
}



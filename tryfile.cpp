#include "filesystem.hpp"
#include <iostream>
int main()
{
    std::string path = "./aa";
    char a[6];
    
    a[0] = 'a';
    a[1] = 'b';
    a[2] = 'c';
    a[3] = '\0';
    a[4] = 'd';
    a[5] = '\0';
    std::string str(a,6) ;
    std::cout <<
    FileSystem fs =  FileSystem(path,WRITE,".txt");
    fs.open();
    fs.Write_chunk(str,6);
}
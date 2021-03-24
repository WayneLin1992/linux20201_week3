#include <iostream>
#include <string>
#include <fstream>
#include <sys/time.h>

using namespace std;
int main(){
    struct timespec start, end;
//    FILE * fp;
//    fp = fopen ("test.txt","r");
//
    ifstream ifs("testbench1.txt");
/*    for (int i=0; i<50; i++){
        clock_gettime(CLOCK_MONOTONIC, &start);
        getline(, str)
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_taken = end.tv_nsec - start.tv_nsec;
        cout << time_taken <<endl;
    }*/
    ofstream myfile;
    myfile.open ("cppstringcd.txt");
    string str;
    //string file_contents;
    int i = 0;
    while (getline(ifs, str))
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        str = "\0";
        //delete file_contents;
        //file_contents.push_back('\n');
        clock_gettime(CLOCK_MONOTONIC, &end);
        long time_taken = end.tv_nsec - start.tv_nsec;
        myfile << i <<"\t"<< time_taken;
        myfile << "\n";
        i++;
    }
    myfile.close ( );
    ifs.close();  
//    string content( (istreambuf_iterator<char>(ifs) ),
//                       (istreambuf_iterator<char>()    ) );
//    clock_gettime(CLOCK_MONOTONIC, &end);
//    double time_taken = end.tv_nsec - start.tv_nsec;

    //string string1 = "hellow ";
    //string string2 = string1 + "world";
    //cout << file_contents <<endl;
    //std::cout << "Hello World" <<std::endl;
    return  0;
    }

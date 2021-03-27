#include <iostream>
#include <string>
#include <fstream>
#include <sys/time.h>

using namespace std;

std::string& ltrim(std::string& str, const std::string& chars = "b")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}
 
std::string& rtrim(std::string& str, const std::string& chars = "b")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}
 
std::string& trim(std::string& str, const std::string& chars = "b")
{
    return ltrim(rtrim(str, chars), chars);
}

int main(){
    struct timespec start, end;
//    FILE * fp;
//    fp = fopen ("test.txt","r");
//
    ifstream ifs("testbench1.txt");
    ifstream fp1("testbench2.txt");
    ifstream fp2("testbench3.txt");    
/*    for (int i=0; i<50; i++){
        clock_gettime(CLOCK_MONOTONIC, &start);
        getline(, str)
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_taken = end.tv_nsec - start.tv_nsec;
        cout << time_taken <<endl;
    }*/
    ofstream myfile;
    myfile.open ("cppcow.txt");
    string str, prefix, suffix;
    string file_contents, file_prefix, file_suffix;
    int i = 0;
    while (getline(ifs, str) &&  getline(fp1,prefix) && getline(fp2,suffix))
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        //str = "\0";
        file_contents = str;
        file_prefix = prefix;
        file_suffix = suffix;
        file_contents = file_prefix + file_contents +file_suffix;
        trim(file_contents);
        //cout << file_contents << endl;
        string file_trim_contents = file_contents;
        file_contents = str;
        //file_contents.push_back('\n');
        clock_gettime(CLOCK_MONOTONIC, &end);
        long time_taken = end.tv_nsec - start.tv_nsec;
        myfile << i <<"\t"<< time_taken;
        myfile << "\n";
        i++;
    }
    myfile.close ( );
    ifs.close();
    fp1.close();
    fp2.close();
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

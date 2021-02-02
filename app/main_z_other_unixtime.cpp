#include <iostream>
#include <ctime>

int main(int argc, char *argv[])
{
    std::cout << "Hello!!" << std::endl;
    //
    unsigned long int sec = time(NULL);
    std::cout << sec << std::endl;
    return 0;
}
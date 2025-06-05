#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

struct Options
{
    bool lines = false;
    bool bytes = false;
    bool chars = false;
    bool words = false;
};

int main(int argc, char* argv[])
{
    Options options;
    std::vector<std::string> files;
    for (int i = 1; i < argc; i++)
    {
        //figure out with options
        if (argv[i][0] == '-' and argv[i][1] != '-')
        {
            int j = 1;
            while (argv[i][j]!='\0')
            {
                if (argv[i][j]=='l') options.lines = 1;
                else if (argv[i][j]=='c') options.bytes = 1;
                else if (argv[i][j]=='w') options.words = 1;
                else if (argv[i][j]=='m') options.chars = 1;
                else std::cout<<"Unknown option: " << argv[i][j] << "\n";

                j++;
            }
        }
        else if (argv[i][0] == '-' and argv[i][1] == '-')
        {
            if (strcmp(argv[i],"--bytes")==0)  options.bytes = 1;
            else if (strcmp(argv[i],"--lines")==0)  options.lines = 1;
            else if (strcmp(argv[i],"--words")==0)  options.words = 1;
            else if (strcmp(argv[i],"--chars")==0)  options.chars = 1;
            else std::cout<<"Unknown option: " << argv[i] << "\n";
        }

        //figure out with files
        else files.push_back(argv[i]);

    }

    for (auto file: files)
    {
        std::ifstream f;
        f.open(file);
        if (!f.is_open()) {
            std::cerr << "File can't be opened: " << file << std::endl;
            continue;
        }
        if (f.fail()) {
            std::cerr << "Error while opening file: " << file << std::endl;
            continue;
        }

        char t;
        uint64_t l = 1;
        uint64_t w = 0;
        uint64_t m = 0;
        uint64_t c = 0;
        bool is_prechar_space = true;
        f.seekg(0, std::ios::end);    // move  file pointer to the end of the file
        c = f.tellg(); //absolute position of file pointer = bytes
        f.seekg(0, std::ios::beg); //move  file pointer to the end of the file for reading
        while(!f.eof())
        {
            f.get(t);
            if (not(isspace(t)))
            {
                m++; //chars
                if (is_prechar_space == true)
                {
                    w++;//words
                    is_prechar_space = false;
                }
            }
            else
            {
                is_prechar_space = true;
                if (t==' ') m++;
            }
            if (t == '\n') l++; //lines

        }
        if(!isspace(t)) m--;
        if (options.lines) std::cout << l << " "; //lines
        if (options.words) std::cout << w << " "; //words
        if (options.bytes) std::cout << c << " "; //bytes
        if (options.chars) std::cout << m << " "; //chars
        std::cout << file << std::endl;
    }

    return 0;
}

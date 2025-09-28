#include "bfcc.hpp"

void initglobalvars()
{
    global_vars::entryname = "main";
    global_vars::platform = "win64";
    global_vars::inputfilename = "nullptr";
    global_vars::outputfilename = "a.obj";
    global_vars::memory_size = 30000;   //brainfuck standart
    global_vars::start = false;
    global_vars::isbin = false;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("BFCC error: not enough argumets. use [bfcc.exe -h] or [bfcc.exe --help]\n");
        exit(1);
    }

    initglobalvars();
    for(int i = 1; i < argc; i++)
    {
        str arg = argv[i];
        if(arg == "-h" || arg == "--help")
        {
            //printf help stuff
            printf("BFCC help: use bfcc.exe [argumets]\n -h - information about argumets parameters\n -p - platform for compile\n -f - input file\n -o - output file\n -s - execute file (requires -f)\n-m - .data memory in bytes\n-e - set entry function name (optional)\n");
            system("pause");
            exit(0);
        }
        else if(arg == "-p" || arg == "--platform")
        {
            if(i + 1 < argc)
            {
                global_vars::platform = argv[++i];
            }
            else
            {
                printf("BFCC error: --platform requires a platform: windows or linux (win64, elf64)\n");
                exit(1);
            }
        }
        else if(arg == "-f" || arg == "--file")
        {
            if(i + 1 < argc)
            {
                global_vars::inputfilename = argv[++i];
            }
            else
            {
                printf("BFCC error: --file requires a platform\n");
                exit(1);
            }
        }
        else if(arg == "-b" || arg == "--binary")
        {
            global_vars::isbin = true;
        }
        else if(arg == "-e" || arg == "--entry")
        {
            if(i + 1 < argc)
            {
                global_vars::entryname = argv[++i];
            }
            else
            {
                printf("BFCC error: --entry requires a entry function name\n");
                exit(1);
            }
        }
        else if(arg == "-s" || arg == "--start")
        {
            global_vars::start = true;
        }
        else if(arg == "-o" || arg == "--output")
        {
            if(i + 1 < argc)
            {
                global_vars::outputfilename = argv[++i];
            }
            else
            {
                printf("BFCC error: --output requires a output file name\n");
                exit(1);
            }
        }
        else if(arg == "-m" || arg == "--memory")
        {
            if(i + 1 < argc)
            {
                global_vars::memory_size = stoi(argv[++i]);
            }
            else
            {
                printf("BFCC error: --memory requires a memory size (30000 int brainfuck standart)\n");
                exit(1);
            }
        }
    }

    if(global_vars::start)
    {
        ifstream f(global_vars::inputfilename.c_str());

        if(!f.is_open())
        {
            printf("BFCC error: failed to open file %s\n", global_vars::inputfilename.c_str());
            exit(1);
        }

        stringstream buffer;

        buffer << f.rdbuf();

        f.close();

        interpreter inter(buffer.str().c_str());
        buffer.clear();
        inter.interpret();

        exit(0);
    }
    else if(global_vars::isbin)
    {
        global_vars::outputfilename = filename_ret_extension(global_vars::inputfilename, "bin");
        ifstream f(global_vars::inputfilename.c_str());
        if(!f.is_open())
        {
            printf("BFCC error: failed to open file %s\n", global_vars::inputfilename.c_str());
            exit(1);
        }
        stringstream buffer;

        buffer << f.rdbuf();

        f.close();

        bfCOFF bf(global_vars::inputfilename, buffer.str().c_str());
        buffer.clear();
        bf.Generate();
    }
    else
    {
        
        if(global_vars::outputfilename == "a.obj")
        {
            global_vars::outputfilename = filename_ret_extension(global_vars::inputfilename, "obj");
        }

        ifstream f(global_vars::inputfilename.c_str());

        if(!f.is_open())
        {
            printf("BFCC error: failed to open file %s\n", global_vars::inputfilename.c_str());
            exit(1);
        }

        stringstream buffer;

        buffer << f.rdbuf();

        f.close();

        bfCOFF bf(global_vars::inputfilename, buffer.str().c_str());
        buffer.clear();
        bf.Generate();
    }


    return 0;
}
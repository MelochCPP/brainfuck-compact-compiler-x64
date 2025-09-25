#pragma once

inline str filename_ret_extension(str inname, str extension)
{
    return inname.substr(0, inname.find_last_of(".") + 1) + extension;
}
//THIS SHIT DOESN'T WORKS
class bfFile
{
    FILE* f;

    const char* file;
    const char* args;
public:
    bfFile(str input, str arg)  //use filename_ret_extension
    {
        f = fopen(input.c_str(), arg.c_str());
        file = input.c_str();
        args = arg.c_str();
    }

    void bfread(void *ptr, size_t size)
    {
        size_t n = fread(ptr, 1, size, f);
        if(n != size)
        {
            printf("FILE API error: failed to read file\n");
            exit(1);
        }
    }

    void bfwrite(void* ptr, size_t size)
    {
        size_t n = fwrite(ptr, 1, size, f);
        if(n != size || ferror(f) || feof(f))
        {
            printf("FILE API error: failed to write file\n");
            exit(1);
        }
    }

    FILE* getfile()
    {
        return f;
    }

    void fwrite8(uint8_t data)
    {
        bfwrite(&data, 1);
    }

    void fwrite16(uint16_t data)
    {
        bfwrite(&data, 2);
    }

    void fwrite32(uint32_t data)
    {
        bfwrite(&data, 4);
    }

    void fwrite64(uint64_t data)
    {
        bfwrite(&data, 8);
    }

    template<typename T>
    void fwriteU(T data)
    {
        bfwrite(&data, sizeof(T));
    }

    void bfopen(const char* input, const char* arg)
    {
        f = fopen(input, arg);

        //perror("FILE API error: failed to open");
        if(!f || ferror(f) || f == nullptr)
        {
            printf("FILE API error: failed to open %s\n", input);
            exit(1);
        }
    }

    void bfexit()
    {
        fclose(f);
        f = nullptr;
    }
};
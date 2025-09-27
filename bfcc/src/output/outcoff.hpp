#pragma once

#define IMAGE_FILE_MACHINE_AMD64 0x8664 //COFF HEADER MACHINE

struct COFFHeader
{
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
};

struct SectionHeader
{
    char Name[8];
    uint32_t VirtualSize;   //null
    uint32_t VirtualAddress;    //null
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
};

struct Relocation 
{
    uint32_t VirtualAddress;
    uint32_t SymbolTableIndex;
    uint16_t Type;
};

struct Symbol 
{
    char Name[8];
    uint32_t Value;
    int16_t SectionNumber;
    uint16_t Type;
    uint8_t StorageClass;
    uint8_t NumberOfAuxSymbols;
};

struct Character
{
    char value;
    unsigned int repeat;
};

class bfCOFF
{
    bfFile* file;
    FILE* f;

    str source;
    char current;
    size_t limit;
    int cursor;

    vector<COFFHeader*> coffheaderlist;
    vector<SectionHeader*> sectionheaderlist;
    vector<Relocation*> relocationlist;
    vector<Symbol*> symbollist;

    map<int, const char*> reloclistmap;
    map<const char*, int> jmpmap;   //

    stack<size_t> loop;

    int rpa;    //reloc num

    vector<const char*> sections;     //idk that is this

    int textsize;
    int datasize;

    int copen;  //[ counter
    int cclose; //] counter

    vector<int> rewritepos;

    template<typename T>
    void writeLE(vector<uint8_t>& vec, T value)
    {
        /*vec.push_back(static_cast<uint8_t>(value));
        vec.push_back(static_cast<uint8_t>(value >> 8)); uint16_t*/
        
        for(int i = 0; i < sizeof(T); ++i)
        {
            vec.push_back(static_cast<uint8_t>((value >> i*8) & 0xFF));
        }
    }

    int FindSymbolIndex(const char* Name)
    {
        int i = 0;
        for(auto a : symbollist)
        {
            if(strcmp(Name, a->Name) == 0)
            {
                return i;
            }
            ++i;
        }
        return -1;
    }

    COFFHeader* CreateCOFFHeader(uint16_t, uint16_t, uint32_t, uint32_t, uint32_t, uint16_t, uint16_t);
    SectionHeader* CreateSectionHeader(char[8], uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint16_t, uint16_t, uint32_t);
    Relocation* CreateRelocation(uint32_t, uint32_t, uint16_t);
    Symbol* CreateSymbol(char[8], uint32_t, int16_t, uint16_t, uint8_t, uint8_t);

    /*
    СИР-СТОУН-ЛАРЛ-БКБ-КОЛААААААААААААААААААПС
    */

    vector<uint8_t> GenerateCOFFHeader();
    vector<uint8_t> GenerateSymbolTable();
    vector<uint8_t> GenerateSectionHeader();
    vector<uint8_t> GenearteRelocation();

    vector<uint8_t> GenerateDataBuffer();
    vector<uint8_t> GenerateCall();
    vector<uint8_t> GenerateExit();
    vector<uint8_t> GenerateMOVZX();
    void InsertVector(vector<uint8_t>&, const vector<uint8_t>&);
    vector<uint8_t> GenerateCode(vector<Character*>);

    uint32_t UnixTimeStamp();
    char advance();
    Character* CreateCharacter(char);
    vector<Character*> Analyse();
public:
    bfCOFF(str filename, str input)
    {
        //file->bfopen(filename_ret_extension(filename,  "obj").c_str(), "wb");
        f = fopen(filename_ret_extension(filename,  "obj").c_str(), "wb");

        source = input;
        limit = source.length();
        cursor = 0;
        current = source.at(cursor);
    }

    void Generate();
};
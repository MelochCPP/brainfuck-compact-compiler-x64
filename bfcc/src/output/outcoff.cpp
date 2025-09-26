#include "..\bfcc.hpp"

uint32_t bfCOFF::UnixTimeStamp()
{
    return uint32_t(chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count());
}

COFFHeader* bfCOFF::CreateCOFFHeader(uint16_t Machine, uint16_t NumberOfSections, uint32_t TimeDateStamp, uint32_t PointerToSymbolTable, uint32_t NumberOfSymbols, uint16_t SizeOfOptionalHeader, uint16_t Characteristics)
{
    COFFHeader* coffheader = new COFFHeader();

    coffheader->Machine = Machine;
    coffheader->NumberOfSections = NumberOfSections;
    coffheader->TimeDateStamp = TimeDateStamp;
    coffheader->PointerToSymbolTable = PointerToSymbolTable;
    coffheader->NumberOfSymbols = NumberOfSymbols;
    coffheader->SizeOfOptionalHeader = SizeOfOptionalHeader;
    coffheader->Characteristics = Characteristics;

    coffheaderlist.push_back(coffheader);

    return coffheader;
}

SectionHeader* bfCOFF::CreateSectionHeader(char Name[8], uint32_t VirtualSize, uint32_t VirtualAddress, uint32_t SizeOfRawData, uint32_t PointerToRawData, uint32_t PointerToRelocations, uint32_t PointerToLinenumbers, uint16_t NumberOfRelocations, uint16_t NumberOfLinenumbers, uint32_t Characteristics)
{
    SectionHeader* sectheader = new SectionHeader();

    memcpy(sectheader->Name, Name, 8);
    sectheader->VirtualSize = VirtualSize;
    sectheader->VirtualAddress = VirtualAddress;
    sectheader->SizeOfRawData = SizeOfRawData;
    sectheader->PointerToRawData = PointerToRawData;
    sectheader->PointerToRelocations = PointerToRelocations;
    sectheader->PointerToLinenumbers = PointerToLinenumbers;
    sectheader->NumberOfRelocations = NumberOfRelocations;
    sectheader->PointerToRelocations = PointerToRelocations;
    sectheader->Characteristics = Characteristics;

    sectionheaderlist.push_back(sectheader);

    return sectheader;
}

Relocation* bfCOFF::CreateRelocation(uint32_t VirtualAddress, uint32_t SymbolTableIndex, uint16_t Type)
{
    Relocation* reloc = new Relocation();

    reloc->VirtualAddress = VirtualAddress;
    reloc->SymbolTableIndex = SymbolTableIndex;
    reloc->Type = Type;

    //relocationlist.push_back(reloc);

    return reloc;
}

Symbol* bfCOFF::CreateSymbol(char Name[8], uint32_t Value, int16_t SectionNumber, uint16_t Type, uint8_t StorageClass, uint8_t NumberOfAuxSymbols)
{
    Symbol* symb = new Symbol();
    Symbol* buffer = new Symbol();
    bool t = false;

    memcpy(symb->Name, Name, 8);

    if(strcmp(Name, ".data") == 0)
    {
        memcpy(buffer->Name, &datasize, sizeof(datasize));
        t = true;
    }
    else if(strcmp(Name, ".text") == 0)
    {
        memcpy(buffer->Name, &textsize, sizeof(textsize));
        memcpy(buffer->Name + 4, &rpa, sizeof(rpa));
        t = true;
    }   //map <int, string>
    else if(strcmp(Name, ".file") == 0)
    {
        const char* pad = global_vars::inputfilename.c_str();
        memcpy(buffer->Name, &pad, sizeof(pad));
        t = true;
    }
    symb->Value = Value;
    symb->SectionNumber = SectionNumber;
    symb->Type = Type;
    symb->StorageClass = StorageClass;
    symb->NumberOfAuxSymbols = NumberOfAuxSymbols;

    symbollist.push_back(symb);
    if(t)
    {
        symbollist.push_back(buffer);
    }

    return symb;
}

char bfCOFF::advance()
{
	if (cursor < limit)
	{
		char tmp = current;
		cursor++;
		current = source[cursor];
		return tmp;
	}
	else
		return '\0';
}


Character* bfCOFF::CreateCharacter(char input)
{
    Character* charac = new Character();    //why in Create**** u use make unique, but there u use default pointer? dude u so stupid

    charac->value = current;
    int i = 0;
    while(current == input)
    {
        i++;
        advance();
    }

    charac->repeat = i;

    return charac;
}

vector<Character*> bfCOFF::Analyse()
{
    vector<Character*> ret;

    copen = 0;  //[ counter
    cclose = 0; //] counter

    rpa = 1;    //reloc num
    while(cursor < limit)
    {
        switch (current)
        {
            case '+':
                ret.push_back(CreateCharacter('+'));
                break;
            case '-':
                ret.push_back(CreateCharacter('-'));
                break;
            case '>':
                ret.push_back(CreateCharacter('>'));
                break;
            case '<':
                ret.push_back(CreateCharacter('<'));
                break;
            case '[':
                copen++;
                ret.push_back(CreateCharacter('['));
                break;
            case ']':
                cclose++;
                ret.push_back(CreateCharacter(']'));
                break;
            case '.':
                ret.push_back(CreateCharacter('.'));
                rpa++;
                break;
            case ',':
                ret.push_back(CreateCharacter(','));
                rpa++;
                break;
            default:
                advance();
                break;
        }
    }

    if(cclose != copen)
    {
        printf("Analyser error: [ count does't equals ] count\n");
        exit(1);
    }

    return ret;
}

vector<uint8_t> bfCOFF::GenerateCall()
{
    vector<uint8_t> call;

    call.push_back(0xE8);

    call.push_back(0x00);
    call.push_back(0x00);
    call.push_back(0x00);
    call.push_back(0x00);

    return call;
}

vector<uint8_t> bfCOFF::GenerateMOVZX()
{
    vector<uint8_t> movzx;

    movzx.push_back(0x48);
    movzx.push_back(0xBE);

    movzx.push_back(0x00);
    movzx.push_back(0x00);
    movzx.push_back(0x00);
    movzx.push_back(0x00);
    movzx.push_back(0x00);
    movzx.push_back(0x00);
    movzx.push_back(0x00);
    movzx.push_back(0x00);

    return movzx;
}

vector<uint8_t> bfCOFF::GenerateExit()
{
    vector<uint8_t> exit;

    /*exit.push_back(0x48);
    exit.push_back(0x31);
    exit.push_back(0xC0);

    exit.push_back(0xC3);*/

    return exit;
}

void bfCOFF::InsertVector(vector<uint8_t>& to, const vector<uint8_t>& dot)
{
    to.insert(to.end(), dot.begin(), dot.end());
}

vector<uint8_t> bfCOFF::GenerateCode(vector<Character*> input)
{
    vector<uint8_t> code;

    int cco = 0;
    int ccc = 0;

    //prologue
    //InsertVector(code, GenerateMOVZX());

    code.push_back(0x48);
    code.push_back(0xBE);
    //CreateRelocation(code.size(), FindSymbolIndex(".data"), 1);
    reloclistmap[code.size()] = ".data";
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);

    //

    //main
    for(auto a : input)
    {
        switch (a->value)
        {
            case '+':
            {    if(a->repeat > 1)
                {
                    code.push_back(0x80);
                    code.push_back(0x06);
                    code.push_back((uint8_t)a->repeat);
                }
                else
                {
                    code.push_back(0xFE);
                    code.push_back(0x06);
                }
                break;
            }
            case '-':
            {    if(a->repeat > 1)
                {
                    code.push_back(0x80);
                    code.push_back(0x2E);
                    code.push_back((uint8_t)a->repeat);
                }
                else
                {
                    code.push_back(0xFE);
                    code.push_back(0x0E);
                }
                break;
            }
            case '>':
            {    if(a->repeat > 1)
                {
                    code.push_back(0x48);
                    code.push_back(0x83);
                    code.push_back(0xC6);
                    code.push_back((uint8_t)a->repeat);
                }
                else
                {
                    code.push_back(0x48);
                    code.push_back(0xFF);
                    code.push_back(0xC6);
                }
                break;
            }
            case '<':
            {
                if(a->repeat > 1)
                {
                    code.push_back(0x48);
                    code.push_back(0x83);
                    code.push_back(0xEE);
                    code.push_back((uint8_t)a->repeat);
                }
                else
                {
                    code.push_back(0x48);
                    code.push_back(0xFF);
                    code.push_back(0xCE);
                }
                break;
            }
            case '[':
            {
                //str kam = "jmp" + to_string(cco);
                //printf("jmp%i = %i\n", cco, code.size());
                //jmpmap[kam.c_str()] = code.size();
                loop.push(code.size());
                code.push_back(0x80);
                code.push_back(0x3E);
                code.push_back(0x00);


                code.push_back(0x0F);
                code.push_back(0x84);
                
                code.push_back(0x00);
                code.push_back(0x00);
                code.push_back(0x00);
                code.push_back(0x00);
                
                //cco++;
                break;
            }
            case ']':
            {
                /*str kbm = "jmp" + to_string(cco);

                auto it = jmpmap.find(kbm.c_str());

                if(it == jmpmap.end())
                {
                    exit(1);
                }


                int k = it->second - code.size() - 2;
                if(k < -127)
                {
                    code.push_back(0xE9);
                    writeLE(code, k);
                }
                else
                {
                    code.push_back(0xEB);
                    code.push_back(k);
                }

                int cds = code.size() - it->second - 9;
                memcpy(&code[it->second + 5], &cds, sizeof(cds));
                cco--;*/
                if(loop.empty())
                    exit(1);

                size_t jzoffset = loop.top();

                loop.pop();

                int k = jzoffset - code.size() - 5;

                code.push_back(0xE9);
                writeLE(code, k);

                int cds = code.size() - jzoffset - 9;
                memcpy(&code[jzoffset + 5], &cds, sizeof(cds));

                break;
            }
            case '.':
            {
                //InsertVector(code, GenerateCall());
                code.push_back(0x48);
                code.push_back(0x0F);
                code.push_back(0xB6);
                code.push_back(0x0E);

                code.push_back(0xE8);
                //CreateRelocation(code.size(), FindSymbolIndex("putchar"), 4);
                reloclistmap[code.size()] = "putchar";
                code.push_back(0x00);
                code.push_back(0x00);
                code.push_back(0x00);
                code.push_back(0x00);
                break;
            }
            case ',':
            {
                //InsertVector(code, GenerateCall());
                code.push_back(0xE8);
                //CreateRelocation(code.size(), FindSymbolIndex("getchar"), 4);
                reloclistmap[code.size()] = "getchar";
                code.push_back(0x00);
                code.push_back(0x00);
                code.push_back(0x00);
                code.push_back(0x00);

                code.push_back(0x88);
                code.push_back(0x06);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    //

    code.push_back(0x48);
    code.push_back(0x31);
    code.push_back(0xC9);

    code.push_back(0xE8);
    reloclistmap[code.size()] = "exit";
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);

    //epilogue
    InsertVector(code, GenerateExit());
    //

    return code;
}

vector<uint8_t> bfCOFF::GenerateDataBuffer()
{
    vector<uint8_t> data;

    for(int i = 0; i != global_vars::memory_size; ++i)
    {
        data.push_back(0x00);
    }

    return data;
}

vector<uint8_t> bfCOFF::GenerateCOFFHeader()
{
    vector<uint8_t> coffcode;

    int pointertosymbols = sizeof(COFFHeader) + sizeof(SectionHeader) * sectionheaderlist.size() + datasize + textsize + 10 * relocationlist.size();

    CreateCOFFHeader(IMAGE_FILE_MACHINE_AMD64, sections.size(), UnixTimeStamp(), pointertosymbols, symbollist.size(), 0, 0);

    for(auto a : coffheaderlist)
    {
        writeLE(coffcode, a->Machine);
        writeLE(coffcode, a->NumberOfSections);
        writeLE(coffcode, a->TimeDateStamp);
        writeLE(coffcode, a->PointerToSymbolTable);
        writeLE(coffcode, a->NumberOfSymbols);
        writeLE(coffcode, a->SizeOfOptionalHeader);
        writeLE(coffcode, a->Characteristics);
    }

    return coffcode;
}

vector<uint8_t> bfCOFF::GenerateSectionHeader()
{
    vector<uint8_t> seccode;

    int pointertorawdata = 0;
    for(auto a : sections)
    {
        if(strcmp(a, ".data") == 0)
        {
            pointertorawdata = sizeof(COFFHeader) + 40 * sections.size();
            char padnamedata[8] = ".data";  //0x400030C0
            CreateSectionHeader(padnamedata, 0, 0, datasize, pointertorawdata, pointertorawdata + datasize, 0, 0, 0, 0xC0300040);
        }
        else if(strcmp(a, ".text") == 0)
        {
            pointertorawdata = sizeof(COFFHeader) + 40 * sections.size() + datasize;
            char padnametext[8] = ".text";  //0x20005060
            CreateSectionHeader(padnametext, 0, 0, textsize, pointertorawdata, pointertorawdata + textsize, 0, relocationlist.size(), 0, 0x60500020);
        }
    }

    for(auto b : sectionheaderlist)
    {
        for(size_t i = 0; i < 8; ++i)
        {
            seccode.push_back(b->Name[i]);
        }
        writeLE(seccode, b->VirtualSize);
        writeLE(seccode, b->VirtualAddress);
        writeLE(seccode, b->SizeOfRawData);
        writeLE(seccode, b->PointerToRawData);
        writeLE(seccode, b->PointerToRelocations);
        writeLE(seccode, b->PointerToLinenumbers);
        writeLE(seccode, b->NumberOfRelocations);
        writeLE(seccode, b->NumberOfLinenumbers);
        writeLE(seccode, b->Characteristics);
    }

    return seccode;
}

vector<uint8_t> bfCOFF::GenearteRelocation()
{
    vector<uint8_t> relcode;

    int type = 0;

    for(auto b : reloclistmap)
    {
        if(strcmp(b.second, ".data") == 0)
        {
            type = 1;
        }
        else
        {
            type = 4;
        }
        relocationlist.push_back(CreateRelocation(b.first, FindSymbolIndex(b.second), type));
    }

    for(auto a : relocationlist)
    {
        writeLE(relcode, a->VirtualAddress);
        writeLE(relcode, a->SymbolTableIndex);
        writeLE(relcode, a->Type);
    }

    return relcode;
}

vector<uint8_t> bfCOFF::GenerateSymbolTable()
{
    vector<uint8_t> symbolcode;

    char padnameentry[8] = "main";
    char padnamedata[8] = ".data";
    char padnametext[8] = ".text";
    char padnameabsolut[8] = {};
    char padnameput[8] = "putchar";
    char padnameget[8] = "getchar";
    char padnamefile[8] = ".file";
    char padnametape[8] = "tape";
    char padnameexit[8] = "exit";
    memcpy(padnameabsolut, ".absolut", 8);

    //CreateSymbol(padnamefile, 0, -2, 0, 0x67, 1);
    CreateSymbol(padnamedata, 0, 1, 0, 3, 1);
    sections.push_back(".data");
    CreateSymbol(padnametext, 0, 2, 0, 3, 1);
    sections.push_back(".text");
    CreateSymbol(padnameabsolut, 0, -1, 0, 3, 0);
    //CreateSymbol(padnametape, 0, 1, 0, 3, 0);
    CreateSymbol(padnameput, 0, 0, 0, 2, 0);
    CreateSymbol(padnameget, 0, 0, 0, 2, 0);
    CreateSymbol(padnameexit, 0, 0, 0, 2, 0);
    CreateSymbol(padnameentry, 0, 2, 0, 2, 0);

    for(auto a : symbollist)
    {
        for(size_t i = 0; i < 8; ++i)
        {
            symbolcode.push_back(a->Name[i]);
        }
        writeLE(symbolcode, a->Value);
        writeLE(symbolcode, a->SectionNumber);
        writeLE(symbolcode, a->Type);
        writeLE(symbolcode, a->StorageClass);
        writeLE(symbolcode, a->NumberOfAuxSymbols);
    }

    return symbolcode;
}

void bfCOFF::Generate()
{
    vector<Character*> tokens = Analyse();
    vector<uint8_t> datacode = GenerateDataBuffer();
    datasize = datacode.size();
    vector<uint8_t> textcode = GenerateCode(tokens);
    textsize = textcode.size();
    vector<uint8_t> symbcode = GenerateSymbolTable();
    vector<uint8_t> relcode = GenearteRelocation();
    vector<uint8_t> seccode = GenerateSectionHeader();
    vector<uint8_t> coffcode = GenerateCOFFHeader();

    vector<uint8_t> code;

    InsertVector(code, coffcode);
    InsertVector(code, seccode);
    InsertVector(code, datacode);
    InsertVector(code, textcode);
    InsertVector(code, relcode);
    InsertVector(code, symbcode);

    //printf("%i\n", code.size());
    for(auto a : code)
    {
        //printf("%02x ", a);
        fwrite(&a, 1, sizeof(a), f);
        //file->fwrite8(a);
    }
    //file->fwrite32(4);	//string table XD
    int stringtable = 4;
    fwrite(&stringtable, 1, sizeof(int), f);
    //file->bfexit();
    fclose(f);
}
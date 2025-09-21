#include "bfcc.hpp"

char bf::advance()
{
    if(cursor < limit)
    {
        char tmp = current;
        cursor++;
        current = source[cursor];
        return tmp;
    }
    else
    {
        return '\0';
    }
}

Character* bf::CreateCharacter(char tocreate)
{
    Character* T = new Character();

    T->value = current;

    int i = 0;
    while(current == tocreate)
    {
        i++;
        advance();
    }

    T->times = i;

    return T;
}

vector<Character*> bf::Analyse()
{
    vector<Character*> tokens;

    while(cursor < limit)
    {
        switch (current)
        {
        case '+':
            tokens.push_back(CreateCharacter('+'));
            break;

        case '-':
            tokens.push_back(CreateCharacter('-'));
            break;

        case '>':
            tokens.push_back(CreateCharacter('>'));
            break;

        case '<':
            tokens.push_back(CreateCharacter('<'));
            break;
        
        case '[':
            tokens.push_back(CreateCharacter('['));
            break;

        case ']':
            tokens.push_back(CreateCharacter(']'));
            break;
        
        case '.':
            tokens.push_back(CreateCharacter('.'));
            break;

        case ',':
            tokens.push_back(CreateCharacter(','));
            break;
        
        default:
            advance();
            break;
        }
    }

    return tokens;
}

static Relocation* push_rel(uint32_t VirtualAddress, uint32_t SymbolTableIndex, uint16_t type, uint8_t RelocType)
{
    Relocation* rel = new Relocation();

    rel->VirtualAddress = VirtualAddress;
    rel->Type = type;
    rel->SymbolTableIndex = SymbolTableIndex;
    rel->RelocType = RelocType;

    return rel;
}

vector<uint8_t> bf::GenerateCode(vector<Character*> input)
{
    vector<uint8_t> code;

    code.push_back(0x48);
    code.push_back(0xBE);
    coff_vars::rel.push_back(push_rel(code.size(), 0, 1, 0));
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);
    code.push_back(0x00);


    for(auto a : input)
    {
        switch (a->value)
        {
        case '+':
            if(a->times > 1)
            {
                code.push_back(0x80);
                code.push_back(0x06);
                code.push_back((uint8_t)a->times);
            }
            else
            {
                code.push_back(0xFE);
                code.push_back(0x06);
            }
            break;

        case '-':
            if(a->times > 1)
            {
                code.push_back(0x80);
                code.push_back(0x2E);
                code.push_back((uint8_t)a->times);
            }
            else
            {
                code.push_back(0xFE);
                code.push_back(0x0E);
            }
            break;

        case '>':
            if(a->times > 1)
            {
                code.push_back(0x48);
                code.push_back(0x83);
                code.push_back(0xC6);
                code.push_back((uint8_t)a->times);
            }
            else
            {
                code.push_back(0x48);
                code.push_back(0xFF);
                code.push_back(0xC6);
            }
            break;

        case '<':
            if(a->times > 1)
            {
                code.push_back(0x48);
                code.push_back(0x83);
                code.push_back(0xEE);
                code.push_back((uint8_t)a->times);
            }
            else
            {
                code.push_back(0x48);
                code.push_back(0xFF);
                code.push_back(0xCE);
            }
            break;

        case '.':
            code.push_back(0x48);
            code.push_back(0x0F);
            code.push_back(0xB6);
            code.push_back(0x0E);

            code.push_back(0xE8);
            coff_vars::rel.push_back(push_rel(code.size(), 0, 4, 1));
            code.push_back(0x00);
            code.push_back(0x00);
            code.push_back(0x00);
            code.push_back(0x00);
            coff_vars::defputchar = true;
            break;
        case ',':
            code.push_back(0xE8);
            coff_vars::rel.push_back(push_rel(code.size(), 0, 4, 2));
            code.push_back(0x00);
            code.push_back(0x00);
            code.push_back(0x00);
            code.push_back(0x00);

            code.push_back(0x88);
            code.push_back(0x06);

            coff_vars::defgetchar = true;
            break;
        
        default:
            break;
        }
    }

    code.push_back(0xC3);
    return code;
}

void bf::WriteCode(vector<uint8_t> input, FILE* f)
{
    for(auto a : input)
    {
        bf_write8(a, f);
    }
}

void bf::Generate()
{
    vector<Character*> tokens = Analyse();
    vector<uint8_t> code = GenerateCode(tokens);
    size_t code_size = code.size();

    for(auto a : tokens)
    {
        printf("%c - times: %i\n", a->value, a->times);
    }

    for(auto a : code)
    {
        printf("%x", a);
    }

    FILE* f = bf_open_write(filename_ret_extension(global_vars::inputfilename, "obj").c_str());

    coff_vars::sympos = 100 + global_vars::memory_size + code_size + 10*coff_vars::rel.size();
    int aput = (coff_vars::defputchar) ? 0 : 1;
    int aget = (coff_vars::defgetchar) ? 0 : 1;
    coff_vars::coff_nsyms = 6 + aput + aget + 3;

    //COFFHEADER
    bf_write16(IMAGE_FILE_MACHINE_AMD64, f);
    bf_write16(2, f);
    bf_write32(UnixTimestamp(), f);
    bf_write32(coff_vars::sympos, f);   //PointerToSymbolTable
    bf_write32(coff_vars::coff_nsyms + 2, f);   //NumberOfSymbols
    bf_write32(0, f);
    //

    //section .data
    char padnamedata[8] = ".data";
    bf_write(padnamedata, 8, f);    //section name
    bf_write32(0, f);   //virt size ignore
    bf_write32(0, f);   //virt addr ignore 
    bf_write32(global_vars::memory_size, f);   //datalen
    bf_write32(100, f);   //datapos
    bf_write32(100 + global_vars::memory_size, f);   //relpos
    bf_write32(0, f);   //line pos ignore
    bf_write16(0, f);   //relnum ignore
    bf_write16(0, f);   //linenum ignore
    bf_write32(0xC0300040, f);
    //

    //section .text
    char padnametext[8] = ".text";
    bf_write(padnametext, 8, f);
    bf_write32(0, f);   //virt size ignore
    bf_write32(0, f);   //virt addr ignore 
    bf_write32(code_size, f);   //datalen
    bf_write32(100 + global_vars::memory_size, f);   //datapos
    bf_write32(100 + code_size + global_vars::memory_size, f);   //relpos = datapos + datalen
    bf_write32(0, f);   //line pos ignore
    bf_write16(coff_vars::rel.size(), f);   //relnum
    bf_write16(0, f);   //linenum ignore
    bf_write32(0x60500020, f);
    //

    //tape db
    for(int k = 0; k < global_vars::memory_size; ++k)
    {
        bf_write8(0, f);
    }
    //

    //code
    WriteCode(code, f);
    //

    //relocations
    for(auto a : coff_vars::rel)
    {
        bf_write32(a->VirtualAddress, f);
        if(a->RelocType == 0)
        {
            bf_write32(2, f);
        }
        else if(a->RelocType == 1)
        {
            bf_write32(8, f);
        }
        else if(a->RelocType == 2)
        {
            bf_write32(9, f);
        }
        
        bf_write16(a->Type, f);
    }
    //

    //symbol table file
    char spaces[12] = "";
    char padnamefile[8] = ".file";
    char* filename = global_vars::inputfilename.data();
    bf_write(padnamefile, 8, f);    //name symbol
    bf_write32(0, f);   //value
    bf_write16(-2, f);  //section
    bf_write16(0, f);   //type
    bf_write8(103, f);  //storage class
    bf_write8(1, f);    //aux symbols
    bf_write(filename, 18, f);
    //
    bf_write(padnamedata, 8, f);
    bf_write32(0, f);
    bf_write16(1, f);
    bf_write16(0, f);
    bf_write8(3, f);
    bf_write8(1, f);
    bf_write32(global_vars::memory_size, f);
    bf_write16(0, f);
    bf_write(spaces, 12, f);
    //
    bf_write(padnametext, 8, f);
    bf_write32(0, f);
    bf_write16(2, f);
    bf_write16(0, f);
    bf_write8(3, f);
    bf_write8(1, f);
    bf_write32(code_size, f);
    bf_write16(coff_vars::rel.size(), f);
    bf_write(spaces, 12, f);
    //
    char padnameabs[] = ".absolut";
    bf_write(padnameabs, 8, f);    //name symbol
    bf_write32(0, f);   //value
    bf_write16(-1, f);   //type
    bf_write16(0, f);  //storage class
    bf_write16(3, f);    //aux symbols
    //
    char padnametape[] = "tape";
    bf_write(padnametape, 8, f);    //name symbol
    bf_write32(0, f);   //value
    bf_write16(0, f);   //type
    bf_write16(1, f);  //storage class
    bf_write16(3, f);    //aux symbols
    //
    if(coff_vars::defputchar)
    {
        char padnameputchar[] = "putchar";
        bf_write(padnameputchar, 8, f);    //name symbol
        bf_write32(0, f);   //value
        bf_write16(0, f);   //type
        bf_write16(0, f);  //storage class
        bf_write16(2, f);    //aux symbols
    }
    if(coff_vars::defgetchar)
    {
        char padnamegetchar[] = "getchar";
        bf_write(padnamegetchar, 8, f);    //name symbol
        bf_write32(0, f);   //value
        bf_write16(0, f);   //type
        bf_write16(0, f);  //storage class
        bf_write16(2, f);    //aux symbols
    }

    char padnamemain[] = "main";
    bf_write(padnamemain, 8, f);    //name symbol
    bf_write32(0, f);   //value
    bf_write16(2, f);   //type
    bf_write16(0, f);  //storage class
    bf_write16(2, f);    //aux symbols


    bf_write32(4,f);
    //ПОФИКСИ РЕЛОКАЦИИ
    bf_close(f);
}
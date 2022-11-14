#ifndef STRUCT_DEF_H
#define STRUCT_DEF_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

using namespace std;

#define DATA_DIR "./data/"
#define ST_MEMBER_SET(x)                      \
    {                                         \
        x, sizeof(x) / sizeof(stParserStruct) \
    }

#define MAJOR_VER (5)
#define MINOR_VER (1)
#define BUILD_VER (1234)

enum eTypeDef
{
    eCHAR,
    eBYTE,
    eSHORT,
    eWORD,
    eINT,
    eDWORD,
};

typedef struct
{
    eTypeDef type;
    string member;
    bool isvariable;
    bool isprint;
} stParserStruct;

typedef struct
{
    const stParserStruct *st;
    size_t size;
} stDefinedStruct;

typedef struct
{
    BYTE major;
    BYTE minor;
    WORD pad;
    DWORD chksum;
} structHead;

const stParserStruct stHead[] = {
    {eBYTE,  "major_ver", false, true},
    {eBYTE,  "minor_ver", false, true},
    {eWORD,  "pad",       false, true},
    {eDWORD, "check_sum", false, true},
};

typedef struct
{
    WORD setting_no;
    WORD str_len;
    char moji[1];
} structData;

const stParserStruct stData[] = {
    {eWORD, "data_no", false, true},
    {eWORD, "date_len", false, true},
    {eCHAR, "data", true, true},
};

typedef struct
{
    DWORD fix_data;
} stTail;

const stParserStruct stEnd[] = {
    {eDWORD, "check_sum", false, true},
};
typedef struct
{
    structHead header;
    structData setting;
    stTail dummy;
} stAllData;


const stDefinedStruct AllSettingKas4[] = {
    ST_MEMBER_SET(stHead),
    ST_MEMBER_SET(stData),
    ST_MEMBER_SET(stEnd),
};

static char buf[1024];

class BiParser
{
public:
    BiParser(std::string);
    int write(std::string, WORD, const char *);
    int read(std::string);
    std::vector<std::string> get_vec(void);
    void print();
    int parse(string path){
        ifstream fp(path, ios::in | ios::binary);

        if (!fp)
        {
            cout << path << "can not be opend";
            return 1;
        }else{
            cout << path << endl;
        }

        size_t struct_def_num = sizeof(AllSettingKas4) / sizeof(stDefinedStruct);

        for (int i = 0; i < struct_def_num; i++)
        {
            const stDefinedStruct p = AllSettingKas4[i];
            for (int k = 0; k < p.size; k++)
            {
                cout << "<" << p.st[k].member << ">" << endl;

                if (p.st[k].isvariable)
                {
                    // 可変長データなら，自分の前のメンバーを読み出してデータに変換
                    auto d = *(size_t*)local_read(fp, p.st[k - 1].type, false);
                    cout << "[" << d << "]" << endl;
                    local_read(fp, d, p.st[k].isprint);
                }else{
                    local_read(fp, p.st[k].type, p.st[k].isprint);
                }
            }
        }
        return 0;
    };
private:
    std::vector<std::string> vec_path;

    void dumpByType(void* buf, eTypeDef type){
        switch (type)
        {
        case eCHAR:
            cout << *(char*)buf << endl;
            break;
        case eBYTE:
            cout << *(BYTE*)buf << endl;
            break;
        case eSHORT:
            cout << *(short*)buf << endl;
            break;
        case eWORD:
            cout << *(WORD*)buf << endl;
            break;
        case eINT:
            cout << *(int*)buf << endl;
            break;
        case eDWORD:
            cout << *(DWORD*)buf << endl;
            break;
        default:
            break;
        }        
    }

    size_t type2size(eTypeDef type)
    {
        size_t sz = 0;

        switch (type)
        {
        case eCHAR:
            sz = sizeof(char);
            break;
        case eBYTE:
            sz = sizeof(BYTE);
            break;
        case eSHORT:
            sz = sizeof(short);
            break;
        case eWORD:
            sz = sizeof(WORD);
            break;
        case eINT:
            sz = sizeof(int);
            break;
        case eDWORD:
            sz = sizeof(DWORD);
            break;
        default:
            break;
        }
        return sz;
    };

    void *local_read(ifstream &fin, size_t sz, bool b_print)
    {
        memset(buf, 0, 1024);
        fin.read(buf, sz);
        if (b_print)
        {
            cout << buf << endl;
        }
        return buf;
    };

    void *local_read(ifstream &fin, eTypeDef type, bool b_print)
    {
        memset(buf, 0, 1024);

        size_t sz = type2size(type);
        fin.read(buf, sz);

        if (b_print)
        {
            dumpByType(buf, type);
        }
        return static_cast<void *>(buf);
    };
};

#endif // STRUCT_DEF_H
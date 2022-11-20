#ifndef STRUCT_DEF_H
#define STRUCT_DEF_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <dirent.h>

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

#define SIZE_WORK_BUF (1024)

#define member_size_of(type, member)    sizeof(type::member)

/*
もとのソースのヘッダをそのままインクルードする

*/

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
    {eBYTE, "major_ver", false, true},
    {eBYTE, "minor_ver", false, true},
    {eWORD, "pad", false, true},
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

static char buf[SIZE_WORK_BUF];

class BiParser
{
public:
    BiParser(std::string work_path)
    {
        const char *data_path = work_path.c_str();
        DIR *dp;
        dp = opendir(data_path);

        dirent *entry = readdir(dp);
        while (entry != NULL)
        {
            if (entry != NULL)
            {
                if ((string(".").compare(string(entry->d_name)) != 0) &&
                    (string("..").compare(string(entry->d_name)) != 0))
                {
                    string s_path = work_path + string(entry->d_name);
                    this->vec_path.push_back(s_path);
                }
            }
            entry = readdir(dp);
        }
    };
    int write(string path, WORD no, const char *moji)
    {
        ofstream fout;
        structHead head = {MAJOR_VER, MINOR_VER, 0, BUILD_VER};
        stTail tail = {1};

        WORD len = strlen(moji);

        fout.open(path, ios::out | ios::binary | ios::app);

        if (!fout)
        {
            cout << path << "can not be opend";
            return 1;
        }

        {
            fout.write((char *)&head, sizeof(structHead));

            {
                fout.write((char *)&no, member_size_of(structData, setting_no));
                fout.write((char *)&len, member_size_of(structData, str_len));
                fout.write((char *)moji, len); // no NULL end
            }

            fout.write((char *)&tail, sizeof(stTail));
        }

        fout.close();
        return 0;
    };
    void print();
    int parse(string path)
    {
        ifstream fp(path, ios::in | ios::binary);

        if (!fp)
        {
            cout << path << "can not be opend";
            return 1;
        }
        else
        {
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
                    fp.seekg(-type2size(p.st[k - 1].type), ios_base::cur);  // shift pointer
                    auto d = *(size_t *)impl_read(fp, p.st[k - 1].type, false);
                    impl_read(fp, d, p.st[k].isprint);
                }
                else
                {
                    impl_read(fp, p.st[k].type, p.st[k].isprint);
                }
            }
        }
        fp.close();
        return 0;
    };

private:
    std::vector<std::string> vec_path;

    void dumpByType(void *buf, eTypeDef type)
    {
        switch (type)
        {
        case eCHAR:
            printf("%d\n", *(char*)buf);
            break;
        case eBYTE:
            printf("%d\n", *(BYTE*)buf);
            break;
        case eSHORT:
            printf("%d\n", *(short*)buf);
            break;
        case eWORD:
            printf("%d\n", *(WORD*)buf);
            break;
        case eINT:
            printf("%d\n", *(int*)buf);
            break;
        case eDWORD:
            printf("%d\n", *(DWORD*)buf);
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

    void *impl_read(ifstream &fin, size_t sz, bool b_print)
    {
        memset(buf, 0, SIZE_WORK_BUF);
        fin.read(buf, sz);
        if (b_print)
        {
            cout << buf << endl;
        }
        return buf;
    };

    void *impl_read(ifstream &fin, eTypeDef type, bool b_print)
    {
        memset(buf, 0, SIZE_WORK_BUF);

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
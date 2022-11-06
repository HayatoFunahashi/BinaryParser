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

#define DATA_DIR "./data/"

#define MAJOR_VER  (5)
#define MINOR_VER0 (1)
#define MINOR_VER1 (2)
#define BUILD_VER  (1234)

typedef struct
{
    BYTE major;
    BYTE minor0;
    BYTE minor1;
    BYTE pad0;
    DWORD build_ver;
} stHeader;

typedef struct
{
    WORD setting_no;
    WORD str_len;
    char moji[1];
} stSetting;

typedef struct
{
    BYTE fix_data1;
    BYTE fix_data2;
    BYTE fix_data3;
    BYTE fix_data4;
} stTail;

typedef struct
{
    stHeader header;
    stSetting setting;
    stTail dummy;
} stAllData;

class BiParser{
private:
    std::vector<std::string> vec_path;
public:
    BiParser(std::string);
    int write(std::string, WORD, const char*);
    int read(std::string);
    std::vector<std::string> get_vec(void);
    void print();
};

#endif // STRUCT_DEF_H
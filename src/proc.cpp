#include "struct_def.h"
#include <dirent.h>

using namespace std;

BiParser::BiParser(string work_path)
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
}

int BiParser::write(string path, WORD no, const char *moji)
{
    ofstream fout;
    stHeader head = {MAJOR_VER, MINOR_VER0, MINOR_VER1, 0, BUILD_VER};
    stTail tail = {1, 0, 1, 0};

    WORD len = strlen(moji);

    fout.open(path, ios::out | ios::binary | ios::app);

    if (!fout)
    {
        cout << path << "can not be opend";
        return 1;
    }

    {
        fout.write((char *)&head, sizeof(stHeader));
        fout.write((char *)&no, sizeof(WORD));
        fout.write((char *)&len, sizeof(WORD));
        fout.write((char *)moji, (len)); // no NULL end
        fout.write((char *)&tail, sizeof(stTail));
    }

    fout.close();
    return 0;
}

int BiParser::read(string path)
{
    ifstream fin(path, ios::in | ios::binary);

    if (!fin)
    {
        cout << path << "can not be opend";
        return 1;
    }else{
        cout << path << endl;
    }

    // skip header
    fin.seekg(sizeof(stHeader), ios_base::cur);

    WORD no, len;
    char *pstr = new char[1024];
    memset(pstr, 0, 1024);

    {
        fin.read((char *)&no, sizeof(WORD));
        fin.read((char *)&len, sizeof(WORD));
        fin.read(pstr, len);
        printf("setting no:%d\nword_len:%d\nmoji:%s\n", no, len, pstr);
    }

    // while(!fin.eof()){  //ファイルの最後まで続ける
    // }

    fin.close(); //ファイルを閉じる
    delete[](pstr);
    return 0;
}

void BiParser::print()
{
    for (int i = 0; i < this->vec_path.size(); i++)
    {
        cout << i << ":" << this->vec_path[i] << endl;
    }
};

vector<string> BiParser::get_vec(void)
{
    return this->vec_path;
}
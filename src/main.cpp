#include "struct_def.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    BiParser bp(DATA_DIR);

    if (strcmp(argv[1], "w") == 0)
    {
        string s[3] = {
            (DATA_DIR "1.txt"),
            (DATA_DIR "2.txt"),
            (DATA_DIR "3.txt")
        };
        for (int i = 0; i < 3; i++){
            bp.write(s[i], 10, "hogehoge");
        }   
    }
    else if (strcmp(argv[1], "r") == 0)
    {
        auto v = bp.get_vec();
        for(const auto& e : v){
            bp.read(e);
        }
    }
    else if (strcmp(argv[1], "p") == 0)
    {
        bp.print();
    }

    return 0;
};
#include "struct_def.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    BiParser bp(DATA_DIR);

    if (strcmp(argv[1], "w") == 0)
    {
        bp.write((DATA_DIR "1.bin"), 1, "hogehoge");
        bp.write((DATA_DIR "2.bin"), 2, "toma");
        bp.write((DATA_DIR "3.bin"), 99, "Osaka");
    }
    else if (strcmp(argv[1], "r") == 0)
    {
        bp.parse(DATA_DIR "1.bin");
    }
    else if (strcmp(argv[1], "p") == 0)
    {
        bp.print();
    }

    return 0;
};
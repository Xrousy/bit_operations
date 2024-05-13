#include "command.h"

int main(int argc, char *argv[]) 
{
    if (argc < 4) 
    {
        printf("Usage:\n");
        printf("coder encode <in-file-name> <out-file-name>\n");
        printf("coder decode <in-file-name> <out-file-name>\n");
        return 1;
    }

    if (argc == 4 && strcmp(argv[1], "encode") == 0) 
    {
        return encode_file(argv[2], argv[3]);
    } 
    else if (argc == 4 && strcmp(argv[1], "decode") == 0) 
    {
        return decode_file(argv[2], argv[3]);
    } 
    else 
    {
        printf("Unknown command: %s\n", argv[1]);
        printf("Usage:\n");
        printf("coder encode <in-file-name> <out-file-name>\n");
        printf("coder decode <in-file-name> <out-file-name>\n");
        return 1;
    }

    return 0;
}
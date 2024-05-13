#include "command.h"
#include "coder.h"

int encode_file(const char *in_file_name, const char *out_file_name) 
{
    FILE *in_file = fopen(in_file_name, "r");
    FILE *out_file = fopen(out_file_name, "wb");

    if (!in_file || !out_file) 
    {
        fprintf(stderr, "Error opening file\n");
        return -1;
    }

    uint32_t code_point;
    CodeUnits code_units;

    while (fscanf(in_file, "%" SCNx32, &code_point) == 1) 
    {
        if (encode(code_point, &code_units) == -1) 
        {
            fprintf(stderr, "Number encoding error\n");
            fclose(in_file);
            fclose(out_file);
            return -1;
        }
        if (write_code_unit(out_file, &code_units) == -1) 
        {
            fprintf(stderr, "Error writing encoded number\n");
            fclose(in_file);
            fclose(out_file);
            return -1;
        }
    }

    fclose(in_file);
    fclose(out_file);
    return 0;
}

int decode_file(const char *in_file_name, const char *out_file_name) 
{
    FILE *in_file = fopen(in_file_name, "rb");
    FILE *out_file = fopen(out_file_name, "w");

    if (!in_file || !out_file) 
    {
        fprintf(stderr, "Error opening file\n");
        return -1;
    }

    CodeUnits code_units;
    uint32_t code_point;

    while (read_next_code_unit(in_file, &code_units) != EOF) 
    {
        code_point = decode(&code_units);
        fprintf(out_file, "%" PRIx32 "\n", code_point);
    }

    fclose(in_file);
    fclose(out_file);
    return 0;
}

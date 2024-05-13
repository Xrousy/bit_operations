#include "coder.h"

int encode(uint32_t code_point, CodeUnits *code_units) 
{
    if (code_point <= 0x7f) // <= 1 byte
    {
        code_units->code[0] =  (uint8_t)code_point;
        code_units->length = 1;
    } 
    else if (code_point <= 0x7ff) // <= 2 byte
    {
        code_units->code[0] = (uint8_t)((code_point >> 6)| 0xC0);
        code_units->code[1] = (uint8_t)((code_point & 0x3f)| 0x80);
        code_units->length = 2;
    } 
    else if (code_point <= 0xffff) // <= 3 byte
    {
        code_units->code[0] = (uint8_t)((code_point >> 12)| 0xE0);
        code_units->code[1] = (uint8_t)((code_point >> 6 & 0x3f)| 0x80);
        code_units->code[2] = (uint8_t)((code_point & 0x3f)| 0x80);
        code_units->length = 3;
    } 
    else if (code_point <=  0x1fffff) // <= 4 byte
    {
        code_units->code[0] = (uint8_t)((code_point >> 18) | 0xF0);
        code_units->code[1] = (uint8_t)((code_point >> 12 & 0x3f)| 0x80);
        code_units->code[2] = (uint8_t)((code_point >> 6 & 0x3f)| 0x80);
        code_units->code[3] = (uint8_t)((code_point & 0x3f)| 0x80);
        code_units->length = 4;
    } 
    else // > 4 byte
    {
        return -1;
    }
    return 0;
}

uint32_t decode(const CodeUnits *code_unit) 
{
    uint32_t code_point = 0;
    if(code_unit->length == 1)
    {
        code_point |= code_unit->code[0];
    }
    else if(code_unit->length == 2)
    {
        code_point |= ((code_unit->code[0] & 0x1F) << 6);
        code_point |= (code_unit->code[1] & 0x3F); 
    }
    else if(code_unit->length == 3)
    {
        code_point |= ((code_unit->code[0] & 0xF) << 12);
        code_point |= ((code_unit->code[1] & 0x3F) << 6);
        code_point |= (code_unit->code[2] & 0x3F);
    }
    else
    {
        code_point |= ((code_unit->code[0] & 0x7) << 18);
        code_point |= ((code_unit->code[1] & 0x3F) << 12);
        code_point |= ((code_unit->code[2] & 0x3F) << 6);
        code_point |= (code_unit->code[3] & 0x3F);
    }
    return code_point;
}

int read_next_code_unit(FILE *in, CodeUnits *code_units) 
{
    uint8_t byte;
    if(fread(&byte, sizeof(uint8_t), 1, in) == 1)
    {
        code_units->code[0] = byte;
        if(byte <= 127)
        {
            code_units->length = 1;
        }
        else if(byte >= 191 && byte <= 223)
        {
            code_units->length = 2;
        }
        else if(byte >= 224 && byte <= 239)
        {
            code_units->length = 3;
        }
        else
        {
            code_units->length = 4;
        }
    }
    for(size_t len = 1; len < code_units->length; len++)
    {
        fread(&byte, sizeof(uint8_t), 1, in);
        code_units->code[len] = byte;
    }
    if (feof(in)) 
    {
        return EOF;
    }
    return 0; 
}

int write_code_unit(FILE *out, const CodeUnits *code_unit) 
{
    if (fwrite(code_unit->code, sizeof(uint8_t), code_unit->length, out) != code_unit->length) 
    {
        return -1;
    }
    return 0;
}
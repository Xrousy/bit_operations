#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


size_t encode_varint(uint32_t value, uint8_t* buf) // функция для кодированя числа по принципу varint
{
    assert(buf != NULL);
    uint8_t* cur = buf;
    while (value >= 0x80) 
    {
        const uint8_t byte = (value & 0x7f) | 0x80;
        *cur = byte;
        value >>= 7;
        ++cur;
    }
    *cur = value;
    ++cur;
    return cur - buf;
}

uint32_t decode_varint(uint8_t** bufp) //декодирует
{
    uint8_t* cur = *bufp;
    uint8_t byte = *cur++;
    uint32_t value = byte & 0x7f;
    size_t shift = 7;
    while (byte >= 0x80) 
    {
        byte = *cur++;
        value += (byte & 0x7f) << shift;
        shift += 7;
    }
    *bufp = cur;
    return value;
}

uint32_t generate_number()
{
    const int r = rand();
    const int p = r % 100;
    if (p < 90)
    {
        return r % 128;
    }
    if (p < 95)
    {
        return r % 16384;
    }
    if (p < 99)
    {
        return r % 2097152;
    }
    return r % 268435455;
}

int main()
{

    FILE *uncompressed = fopen("uncompressed.dat", "wb");
    FILE *compressed = fopen("compressed.dat", "wb");
    int num_numbers = 1000000;
    
    if(uncompressed == NULL || compressed == NULL)
    {
        puts("Невозможно открыть файл");
        return 0;
    }

    uint8_t *buffer = malloc(sizeof(uint8_t) * 5);
    for (int num_number = 0; num_number < num_numbers; num_number++)
    {
        uint32_t number = generate_number();
        fwrite(&number, sizeof(uint32_t), 1, uncompressed);
        encode_varint(number, buffer);

        if (buffer[0] == 0) // проверяется буфер является закадираванное число 0;
        {
            fwrite(&buffer[0], sizeof(uint8_t), 1, compressed);
        }
        for (int numb_var = 0; numb_var < 5; numb_var++) // здесь проверяется является ли текущий байт не нулевым;
        {
            if (buffer[numb_var] != 0)
            {
                fwrite(&buffer[numb_var], sizeof(uint8_t), 1, compressed);
            }
            buffer[numb_var] = 0;
        }
    }

    fclose(uncompressed);
    fclose(compressed);

    uncompressed = fopen("uncompressed.dat", "rb");
    compressed = fopen("compressed.dat", "rb");

        if (uncompressed == NULL || compressed == NULL) 
    {
        perror("Невозможно открыть файл");
        return 0;
    }
    fseek(uncompressed, 0, SEEK_END); //перемещает указатель на конец файла
    int END_FILE = ftell(uncompressed) / 4; // хранится количество чисел записанных в файле
    rewind(uncompressed);
    for(int numb_kol = 0; numb_kol < END_FILE; numb_kol++)
    {
        uint32_t uncompressed_number, compressed_number;
        uint8_t *decode_buffer = malloc(sizeof(uint8_t) * 5);
        fread(&uncompressed_number, sizeof(uint32_t), 1, uncompressed); // считываем одно 32битное число
        fread(&decode_buffer[0], sizeof(uint8_t), 1, compressed); //считываем 1 байт
        int numb_var = 0;
        for(; decode_buffer[numb_var] >= 128 && numb_var < 5; numb_var++) //будет считывать байты из файла, пока он не будет либо меньше 128 либо же не будет прочитанно 5 байт
        {
            fread(&decode_buffer[numb_var + 1], sizeof(uint8_t), 1, compressed);
        }
        compressed_number = decode_varint(&decode_buffer);
        if (uncompressed_number != compressed_number) 
        {
            printf("%d %d\n", uncompressed_number, compressed_number);
            printf("Последовательность цифр не совпадает!\n");
            return 0;
        }
    }

    fseek(uncompressed, 0, SEEK_END);
    long uncompressed_size = ftell(uncompressed);

    fseek(compressed, 0, SEEK_END);
    long compressed_size = ftell(compressed);

    printf("Размер несжатого файла: %ld байт\n", uncompressed_size);
    printf("Размер сжатого файла: %ld байт\n", compressed_size);

    fclose(uncompressed);
    fclose(compressed);

    printf("Числовые последовательности совпадают.\n");

    return 0;
}


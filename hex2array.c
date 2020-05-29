// 把二进制数据转成一个c数组

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

typedef unsigned char u8;
typedef unsigned int  u32;


void read_bin(char *file_name, u8 *buf, u8 size)
{
    FILE *fp;

    if(NULL == (fp=fopen(file_name, "rb")))
    {
        printf( "\nCan not open the file: %s \n", file_name);
        exit(-1);
    }
    fread(buf, sizeof(u8), size, fp);
    fclose(fp);
}

u32 get_file_length(char *file_name , char * mode)
{
    u32  size = 0;
    FILE  *fp = fopen(file_name, mode);
    if(NULL != fp)
    {
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fclose(fp);
    }
    return size;
}

void hex2array(char *out_file, u8 *buf, u32 size)
{
    FILE *fp;
    int i , k;
    char pbuf[10] = {0};
    char HEX_BIN_LENGTH[50];
    char *array = "static const u8 hex_bin[HEX_BIN_LENGTH] = {\n";

    if(NULL == (fp=fopen(out_file, "wa+")))
    {
        printf( "\nCan not open the file: %s \n", out_file);
        exit(-1);
    }

    snprintf(HEX_BIN_LENGTH, 50, "\n\n#define HEX_BIN_LENGTH  (%d)\n\n", size);
    fwrite(HEX_BIN_LENGTH, strlen(HEX_BIN_LENGTH), 1, fp);
    fwrite(array, strlen(array), 1, fp);

    k = 0;
    for(i = 0; i < size; i++)
    {
            k++;
		    sprintf(pbuf,"0x%02x", buf[i]);
            fwrite(pbuf, strlen(pbuf), 1, fp);
            if(16 != k)
               fwrite(", ", strlen(", "), 1, fp);
            else
               fwrite(",", strlen(","), 1, fp);
            if(16 == k)
            {
                k = 0;
                fwrite("\n", strlen("\n"), 1, fp);
            }
    }
    fseek(fp, 0, SEEK_END);
    if(0 == k)
        fwrite("};", strlen("};"), 1, fp);
    else
        fwrite("\n};", strlen("\n};"), 1, fp);

    fclose(fp);
}

int main(int argc, char* argv[])
{
	u8 *buf = NULL;
	u32 size;
	char *in_file_name = NULL;
	char *out_file_name = NULL;

    if(argc < 3)
    {
        printf("./hex2array in_bin_file out_file \n");
        exit(-1);
    }

    in_file_name = argv[1];
    out_file_name = argv[2];

	size = get_file_length(in_file_name, "rb");
	buf = (u8 *) malloc(size);
    if(NULL == buf)
    {
        printf("mallc failed \n");
        exit(-1);
    }

	read_bin(in_file_name, buf, size);
	hex2array(out_file_name, buf, size);
    printf("hex2array success: out -> %s\n", out_file_name);
    free(buf);
    buf = NULL;

    exit(0);
}

#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"

#define SOURCE_FILE "source.txt"
#define OUTPUT_FILE "output.txt"

#define MAX_BUFSIZE 1024

static int generate_source(FILE *fp);
static int check_validation(FILE *in_fp, FILE *out_fp);

int main(int argc, char* argv[]) {
    Al_Buffer* buffer;
    FILE *in_fp;
    FILE *out_fp;
    int in_fp_size;
    int i = 0;
    char* data;
    in_fp = fopen(SOURCE_FILE, "w+");
    if (in_fp == NULL) {
        printf("failed to open source file:%s\n", SOURCE_FILE); return -1;
    }
    out_fp = fopen(OUTPUT_FILE, "w+");
    if (out_fp == NULL) {
        printf("failed to open output file:%s\n", OUTPUT_FILE);
        return -2;
    }
    in_fp_size = generate_source(in_fp);
    printf("size of input file:%d\n", in_fp_size);
    buffer = (Al_Buffer*) malloc(sizeof(Al_Buffer));
    buf_create(buffer, MAX_BUFSIZE);
    while (1) {
        /* make random data */
        int ret_size = 0;
        int size = random() % 100;
        if (size == 0) continue;
        data = (char*) malloc(size);
        ret_size = fread(data, 1, size, in_fp);
        //printf("READ|size:%d, ret_size:%d\n", size, ret_size);
        if (ret_size == 0) {
            printf("end of file, break;\n");
            break;
        }
        buf_write(buffer, data, ret_size);
        free(data);
        /* read from buffer and write to output file */
        {
            char* out = (char*) malloc(size);
            buf_read(buffer, out, ret_size);
            if (fwrite(out, 1, ret_size, out_fp) != ret_size) {
                printf("failed to write data to output file\n");
            }
            free(out);
        }
    } 
    buf_destroy(buffer);
    free(buffer);
    if (check_validation(in_fp, out_fp) > -1) {
        printf("SUCCESS|Circular buffer is working well.\n");
    } else {
        printf("FAIL|There is an error in circular buffer implementation.\n");
    }
    close(in_fp);
    close(out_fp);
    return 0;
}

/** generate source file. 
  This file is readed by user then inputed into buffer */
static int generate_source(FILE *fp) 
{
    int i = 0;
    int result = 0;
    while (i++ < (1024 * 1024)) {
        int size = random() % 26;
        char c = (0x61 + size);
        //fwrite(fp, &c, 1);
        fputc(c, fp);
    }
    result = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return result;
}

/** check validatation between source file and output file */
static int check_validation(FILE *in_fp, FILE *out_fp) 
{
    int result = -1;
    fseek(in_fp, 0, SEEK_SET);
    fseek(out_fp, 0, SEEK_SET);
    while (1) {
        int ret_size;
        char *in_buf;
        char *out_buf;
        int size = 1024;
        in_buf = (char *) malloc (size);
        out_buf = (char *) malloc (size);
        ret_size = fread(in_buf, 1, size, in_fp);
        if (ret_size == 0) {
            break;
        }
        if (fread(out_buf, 1, ret_size, out_fp) != ret_size) {
            result = -1;
            break;
        }
        if (strncmp(in_buf, out_buf, ret_size) != 0) {
            result = -3;
        } else {
            result = 0;
        }
    }
    return result;  
}

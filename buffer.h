#include <stdio.h>

typedef struct _Al_Buffer {
    char* buf;
    int head;
    int tail;
    int size;
}  Al_Buffer;

int buf_write(Al_Buffer* buffer, const char* data, int size);
int buf_read(Al_Buffer* buffer, char* out, int size);
int buf_get_current_size(Al_Buffer* buffer);
int buf_create(Al_Buffer* buffer, int buf_size);
int buf_destroy(Al_Buffer* buffer);

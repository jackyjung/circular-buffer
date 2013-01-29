#include <stdlib.h>
#include <string.h>

#include "buffer.h"

int buf_write(Al_Buffer* buffer, const char* data, int size)
{
    if (buffer->tail + size < buffer->size) {
        memcpy(buffer->buf + buffer->tail, data, size);
        buffer->tail += size;
    } else {
        int recorded_size;
        int remained_size;
        recorded_size = buffer->size - buffer->tail;
        memcpy(buffer->buf + buffer->tail, data, recorded_size);
        remained_size = size - recorded_size;
        if (remained_size < buffer->head) {
            memcpy(buffer->buf, data + recorded_size, remained_size);
            buffer->tail = remained_size;
        } else {
            /* buffer full */
            printf("buffer full during writing\n");
            return -1;
        }
    }
    printf("after buf_write:buf_size:%d\n",
                buf_get_current_size(buffer));
    return size;
}

int buf_read(Al_Buffer* buffer, char* out, int size)
{
    if (buffer->head + size <= buffer->size) {
        memcpy(out, buffer->buf + buffer->head, size);
        buffer->head += size;
    } else {
        int readable_size = buffer->size - buffer->head;
        int remained_size = size - readable_size;
        memcpy(out, buffer->buf + buffer->head, readable_size);
        if (remained_size < buffer->head) {
            memcpy(out + readable_size, buffer->buf, remained_size);
            buffer->head = remained_size;
        } else {
            /* buffer is empty */
            printf("buffer empty during reading\n");
            return -1;
        }
    }
    printf("after buf_read:buf_size:%d\n", buf_get_current_size(buffer));
    return size;
}

int buf_get_current_size(Al_Buffer* buffer)
{
    int size = 0;
    if (buffer->tail > buffer->head) {
        size = buffer->tail - buffer->head;
    } else {
        size = buffer->size - (buffer->head - buffer->tail);
    }
    return size;
}

int buf_create(Al_Buffer* buffer, int buf_size)
{
    buffer->buf = (char*) malloc(buf_size);
    buffer->size = buf_size;
    buffer->tail = 0;
    buffer->head = 0;
}

int buf_destroy(Al_Buffer* buffer)
{
    if (buffer->buf) {
        free(buffer->buf);
    }
    memset(buffer, 0, sizeof(Al_Buffer));
    buffer->head = 0;
    buffer->tail = 0;
    buffer->size = 0;
}

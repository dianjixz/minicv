#ifndef __VFS_WRAPPER_H
#define __VFS_WRAPPER_H

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define file_open fopen
#define file_close fclose
#define file_seek fseek
#define file_read_data fread

/*
#define file_obj FILE
int file_write_open_raise(file_obj* fp, const char *path);
int file_read_open_raise(file_obj* fp, const char *path);
int file_write_open(file_obj* fp, const char *path);
int file_read_open(file_obj* fp, const char *path);
int file_close(file_obj fp);
int file_seek(file_obj fp, int offset, uint8_t whence);
bool file_eof(file_obj fp);
int file_seek_raise(file_obj fp, int offset, uint8_t whence);
size_t file_save_data(const char* path, uint8_t* data, size_t length, int* error_code);
size_t file_size(file_obj fp);
void file_buffer_on(file_obj fp);
void file_buffer_off(file_obj fp);

void fs_unsupported_format(file_obj fp);
void fs_file_corrupted(file_obj fp);
void fs_not_equal(file_obj fp);
void fs_no_intersection(file_obj *fp);



int file_corrupted_raise(file_obj fp);
const char *ffs_strerror(int res);


int read_byte(file_obj fp, uint8_t* value);
int read_byte_raise(file_obj fp, uint8_t* value);
int read_byte_expect(file_obj fp, uint8_t value);
int read_byte_ignore(file_obj fp);
int write_byte(file_obj fp, uint8_t value);
int write_byte_raise(file_obj fp, uint8_t value);

int read_word(file_obj fp, uint16_t* value);
int read_word_raise(file_obj fp, uint16_t* value);
int read_word_expect(file_obj fp, uint16_t value);
int read_word_ignore(file_obj fp);
int write_word(file_obj fp, uint16_t value);
int write_word_raise(file_obj fp, uint16_t value);

int read_long(file_obj fp, uint32_t* value);
int read_long_raise(file_obj fp, uint32_t* value);
int read_long_expect(file_obj fp, uint32_t value);
int read_long_ignore(file_obj fp);
int write_long(file_obj fp, uint32_t value);
int write_long_raise(file_obj fp, uint32_t value);

int read_data(file_obj fp, void *data, size_t size);
int read_data_raise(file_obj fp, void *data, size_t size);
int write_data(file_obj fp, const void *data, size_t size);
int write_data_raise(file_obj fp, const void *data, size_t size);
int file_write(file_obj fp, void *data, size_t size, size_t* size_out);
int file_read(file_obj fp, void *data, size_t size, size_t* size_out);
int read_data_raise(file_obj fp, void *data, size_t size);
*/


#ifdef __cplusplus
}
#endif
#endif


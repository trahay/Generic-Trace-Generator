
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <zlib.h>
#include "GTGCompress.h"

#define BUF_SIZE_MAX (1024*1024*32)
/* block size used  */
#define BLOCK_SIZE (128*1024)

int gtg_compress_init(z_stream *z, int compression_ratio)
{
  static int first_init = 1;
  int status = Z_OK;
  if(first_init) {
    int level = compression_ratio;

    z->zalloc = NULL;
    z->zfree = NULL;
    z->opaque = NULL;
    z->avail_in = 0;
    z->next_in = Z_NULL;

    status = deflateInit(z, level);
    if (status != Z_OK) {
      fprintf(stderr, "error at init: %d\n", status);
    }
    first_init=0;
  }
  return status;
}

int gtg_compress_m2m(z_stream *z, void* in_buf, uint32_t len, void* out_buf, uint32_t out_max_len)
{
  int status;
  int offset_out = 0;		/* offset in the dest buffer */

  z->avail_out = out_max_len;
  z->avail_in = len;
  z->next_out = out_buf;
  z->next_in = in_buf;

  /* compress */
  status = deflate( z, Z_SYNC_FLUSH );
  if ( Z_OK != status ) {
    fprintf( stderr, "error %d in compressing\n", status);
    return -1;
  }

  /* compute output len */
  offset_out = out_max_len - z->avail_out;

  return offset_out;
}

int gtg_compress_m2f(z_stream *z, void* in_buf, uint32_t len, FILE* file_out)
{
  int status;

  int offset_in = 0;
  int offset_out = 0;
  int total_written = 0;
  int cur_input_size = 0;
  uint8_t out_buf[BLOCK_SIZE];
  int written = 0;

  /* compress each block of data */
  while(offset_in < len) {
    z->next_out = out_buf;
    z->avail_out = BLOCK_SIZE;

    /* compute input buffer address */
    if(offset_in + BLOCK_SIZE > len)
      z->avail_in = len - offset_in;
    else
      z->avail_in = BLOCK_SIZE;

    cur_input_size = z->avail_in;
    z->next_in = &((uint8_t*)in_buf)[offset_in];

    /* compress */
    status = deflate( z, Z_SYNC_FLUSH );
    if ( Z_OK != status ) {
      fprintf( stderr, "error %d in compressing\n", status);
      return -1;
    }

    /* compute new offset */
    offset_out = BLOCK_SIZE - z->avail_out;
    offset_in += cur_input_size;

    /* write to disk */
    written = fwrite(out_buf, sizeof(uint8_t), offset_out, file_out);

    if(written < offset_out) {
      fprintf(stderr, "warning: %d bytes out of %d were written to disk\n", written, offset_out);
    }
    total_written += written;
  }

  return total_written;
}

int gtg_compress_f2m(z_stream *z, FILE* file_in, void* out_buf, uint32_t out_max_len)
{
  int status;
  int offset = 0;		/* offset in the dest buffer */
  uint8_t in_buf[BLOCK_SIZE];
  int in_len = 0;

  while(!feof(file_in)) {
    /* read a block of data */
    in_len = fread(in_buf, sizeof(uint8_t), BLOCK_SIZE, file_in);

    z->avail_in = in_len;
    z->next_in = in_buf;

    z->avail_out = out_max_len - offset;
    z->next_out = &((uint8_t*)out_buf)[offset];

    /* compress the block */
    status = deflate( z, Z_SYNC_FLUSH );
    if ( Z_OK != status ) {
      fprintf( stderr, "error %d in compressing\n", status);
      return -1;
    }

    /* compute output len */
    offset = out_max_len - z->avail_out;
  }

  return offset;
}

int gtg_compress_f2f(z_stream *z, FILE* file_in, FILE* file_out)
{
  int status;
  int offset = 0;		/* offset in the dest buffer */
  uint8_t in_buf[BLOCK_SIZE];
  uint8_t out_buf[BLOCK_SIZE];
  int in_len = 0;
  int total_written = 0;
  int written = 0;

  while(!feof(file_in)) {
    /* read a block of data */
    in_len = fread(in_buf, sizeof(uint8_t), BLOCK_SIZE, file_in);

    z->avail_in = in_len;
    z->next_in = in_buf;

    z->avail_out = BLOCK_SIZE;
    z->next_out = out_buf;

    /* compress the block */
    status = deflate( z, Z_SYNC_FLUSH );
    if ( Z_OK != status ) {
      fprintf( stderr, "error %d in compressing\n", status);
      return -1;
    }

    /* compute output len */
    offset = BLOCK_SIZE - z->avail_out;

    /* write the output block to disk */
    written = fwrite(out_buf, sizeof(uint8_t), offset, file_out);
    if(written < offset) {
      fprintf(stderr, "warning: %d bytes out of %d were written to disk\n", written, offset);
    }
    total_written += written;
  }

  return total_written;
}

/* decompression routines */

int gtg_decompress_init(z_stream *z)
{
  int status;
  z->zalloc = NULL;
  z->zfree = NULL;
  z->opaque = NULL;
  z->avail_in = 0;
  z->next_in = Z_NULL;

  status = inflateInit(z);
  if (status != Z_OK) {
    fprintf(stderr, "error at init: %d\n", status);
  }
    return status;
}

int gtg_decompress_m2m(z_stream *z, void* in_buf, uint32_t len, void* out_buf, uint32_t out_max_len)
{
  int status;

  z->next_in= in_buf;
  z->avail_in = len;

  z->next_out = out_buf;
  z->avail_out = out_max_len;

  /* decompress */
  status = inflate( z, Z_SYNC_FLUSH );
  if ( Z_OK != status ) {
    fprintf( stderr, "error %d in compressing\n", status);
    return -1;
  }

  return out_max_len - z->avail_out;
}

int gtg_decompress_m2f(z_stream *z, void* in_buf, uint32_t len, FILE* file_out)
{
  int status;
  uint8_t out_buf[BLOCK_SIZE];
  int offset_out = 0;
  int offset_in = 0;
  int total_written = 0;
  int written = 0;

  /* decompress one block at a time */
  while(offset_in < len) {
    z->next_in= &((uint8_t*)in_buf)[offset_in];
    z->avail_in = len - offset_in;

    z->next_out = out_buf;
    z->avail_out = BLOCK_SIZE;

    /* decompress */
    status = inflate( z, Z_SYNC_FLUSH );
    if ( Z_OK != status ) {
      fprintf( stderr, "error %d in compressing\n", status);
      return -1;
    }

    /* compute new offsets */
    offset_out = BLOCK_SIZE - z->avail_out;
    offset_in = len - z->avail_in;
    written = fwrite(out_buf, 1, offset_out, file_out);

    if(written < offset_out){
      fprintf(stderr, "warning: %d bytes out of %d written\n", written, offset_out);
      return -1;
    }
    total_written += written;
  }

  return total_written;
}

int gtg_decompress_f2m(z_stream *z, FILE* file_in, void* out_buf, uint32_t out_max_len)
{
  int status;
  uint8_t in_buf[BLOCK_SIZE];
  uint32_t in_len = 0;
  int offset_out = 0;

  while(! feof(file_in)) {
    in_len = fread(in_buf, BLOCK_SIZE, sizeof(uint8_t), file_in);

    z->avail_in = in_len;
    z->next_in = in_buf;

    z->avail_out = out_max_len - offset_out;
    z->next_out = &((uint8_t*)out_buf)[offset_out];

    /* decompress */
    status = inflate( z, Z_SYNC_FLUSH );
    if ( Z_OK != status ) {
      fprintf( stderr, "error %d in compressing\n", status);
      return -1;
    }

    /* compute output len */
    offset_out = out_max_len - z->avail_out;
  }

  return offset_out;
}

int gtg_decompress_f2f(z_stream *z, FILE* file_in, FILE* file_out)
{
  int status;
  int offset = 0;		/* offset in the dest buffer */
  uint8_t in_buf[BLOCK_SIZE];
  uint8_t out_buf[BLOCK_SIZE];
  int in_len = 0;
  int total_written = 0;
  int written = 0;

  while(!feof(file_in)) {
    in_len = fread(in_buf, sizeof(uint8_t), BLOCK_SIZE, file_in);

    z->avail_in = in_len;
    z->next_in = in_buf;

    z->avail_out = BLOCK_SIZE;
    z->next_out = out_buf;

    /* decompress */
    status = inflate( z, Z_SYNC_FLUSH );
    if ( Z_OK != status ) {
      fprintf( stderr, "error %d in compressing\n", status);
      return -1;
    }

    /* compute output len */
    offset = BLOCK_SIZE - z->avail_out;
    written = fwrite(out_buf, sizeof(uint8_t), offset, file_out);
    if(written < offset) {
      fprintf(stderr, "warning: %d bytes out of %d were written to disk\n", written, offset);
    }
    total_written += written;
  }

  return total_written;
}

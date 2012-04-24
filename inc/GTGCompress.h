#ifndef GTG_COMPRESS_H
#define GTG_COMPRESS_H

#include <stdint.h>
#include <stdio.h>
#include <zlib.h>

/* compress a buffer into another buffer */
int gtg_compress_m2m(z_stream *z, void* in_buf, uint32_t len, void* out_buf, uint32_t out_max_len);
/* compress a buffer and save the result in a file */
int gtg_compress_m2f(z_stream *z, void* in_buf, uint32_t len, FILE* file_out);
/* compress a file into a buffer */
int gtg_compress_f2m(z_stream *z, FILE* file_in, void* out_buf, uint32_t out_max_len);
/* compress a file into another file */
int gtg_compress_f2f(z_stream *z, FILE* file_in, FILE* file_out);

/* save functions for decompression */
int gtg_decompress_m2m(z_stream *z, void* in_buf, uint32_t len, void* out_buf, uint32_t out_max_len);
int gtg_decompress_m2f(z_stream *z, void* in_buf, uint32_t len, FILE* file_out);
int gtg_decompress_f2m(z_stream *z, FILE* file_in, void* out_buf, uint32_t out_max_len);
int gtg_decompress_f2f(z_stream *z, FILE* file_in, FILE* file_out);

int gtg_compress_init(z_stream *z, int compression_ratio);
int gtg_decompress_init(z_stream *z);

#endif	/* GTG_COMPRESS_H */

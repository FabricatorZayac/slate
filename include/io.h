#ifndef SLATE_IO_H_
#define SLATE_IO_H_

#include <stddef.h>
#include <stdio.h>

#ifdef __has_c_attribute
#define NODISCARD(MSG) [[nodiscard(MSG)]]
#else
#define NODISCARD(MSG)
#endif

typedef struct {
    void *context;
    size_t (*write_fn)(void *ctx, char **data);
} writer_t;

typedef struct {
    void *context;
    size_t (*read_fn)(void *ctx, char **buf);
} reader_t;

extern const struct io_mod {
    writer_t (*file_writer)(FILE *self);
    reader_t (*file_reader)(FILE *self);

    size_t (*write)(writer_t, char **bytes);

    size_t (*read)(reader_t, char **buf);
    size_t (*read_all)(reader_t, char **buf);
    size_t (*read_at_least)(reader_t, char **buf, size_t len);
    void (*read_all_vec)(reader_t, char **vec, size_t max_append_size);
} io;

#endif // !SLATE_IO_H_

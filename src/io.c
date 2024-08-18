#include <assert.h>
#include <stdint.h>

#include "io.h"
#include "slice.h"
#include "vec.h"

#define MIN(a,b) (((a)<(b))?(a):(b))

static size_t write(writer_t self, char **bytes) {
    return self.write_fn(self.context, bytes);
}

static size_t read(reader_t self, char **buf) {
    return self.read_fn(self.context, buf);
}
static size_t read_at_least(reader_t self, char **buf, size_t len) {
    assert(len <= len(buf));
    size_t index = 0;
    while (index < len) {
        size_t amt = io.read(self, buf);
        if (amt == 0) break;
        index += amt;
    }
    return index;
}
static size_t read_all(reader_t self, char **buf) {
    return read_at_least(self, buf, len(buf));
}
static void read_all_vec(reader_t self, char **vec, size_t max_append_size) {
    Vec.ensure_capacity(vec, MIN(max_append_size, 4096));
    size_t original_len = len(vec);
    size_t start_index = original_len;
    while (true) {
        Vec.expand_to_cap(vec);
        char **dest_slice = SLICE(*vec, start_index, len(vec));
        size_t bytes_read = io.read_all(self, dest_slice);
        start_index += bytes_read;

        // TODO:
    }
}

static size_t file_write_fn(void *ctx, char **data) {
    return fwrite(*data, sizeof(char), len(data), ctx);
}
static writer_t file_writer(FILE *self) {
    return (writer_t) {
        .context = self,
        .write_fn = file_write_fn,
    };
}
static size_t file_read_fn(void *ctx, char **data) {
    if (fread(*data, sizeof(char), len(data), ctx) != len(data)) {
        if (feof(ctx)) return 0;
        else return SIZE_MAX;
    }
    return len(data);
}
static reader_t file_reader(FILE *self) {
    return (reader_t) {
        .context = self,
        .read_fn = file_read_fn,
    };
}

const struct io_mod io = {
    .file_writer = file_writer,
    .file_reader = file_reader,

    .write = write,

    .read = read,
    .read_all = read_all,
    .read_at_least = read_at_least,
};

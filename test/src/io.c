#include "io.h"
#include  "slice.h"

int main() {
    writer_t out = io.file_writer(stdout);
    io.write(out, str("Hello, World!\n"));
}

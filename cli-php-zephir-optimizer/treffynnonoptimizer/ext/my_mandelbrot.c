#include "my_mandelbrot.h"

/*
 * Function adapted from example in The Computer
 * Languages Benchmark Game
 *
 * ASCII switch added by Simon Holywell and inspired
 * by code from Glenn Rhoads
 * (http://docs.parrot.org/parrot/0.9.1/html/examples/pir/mandel.pir.html)
 */
bool write_mandelbrot_to_stream(int w, int h, FILE *stream, bool bitmap) {
    int bit_num = 0;
    char byte_acc = 0;
    int i, iter = 50;
    double x, y, limit = 2.0;
    double Zr, Zi, Cr, Ci, Tr, Ti;
    const char* ochars = " .:-;!/>)|&IH%*#";

    if(bitmap)
        fprintf(stream, "P4\n%d %d\n", w, h);

    for(y=0;y<h;++y) 
    {
        for(x=0;x<w;++x)
        {
            Zr = Zi = Tr = Ti = 0.0;
            Cr = (2.0*x/w - 1.5); Ci=(2.0*y/h - 1.0);

            for (i=0;i<iter && (Tr+Ti <= limit*limit);++i)
            {
                Zi = 2.0*Zr*Zi + Ci;
                Zr = Tr - Ti + Cr;
                Tr = Zr * Zr;
                Ti = Zi * Zi;
            }

            if(bitmap) {
                byte_acc <<= 1; 
                if(Tr+Ti <= limit*limit) byte_acc |= 0x01;

                ++bit_num; 

                if(bit_num == 8)
                {
                    putc(byte_acc, stream);
                    byte_acc = 0;
                    bit_num = 0;
                }
                else if(x == w-1)
                {
                    byte_acc <<= (8-w%8);
                    putc(byte_acc, stream);
                    byte_acc = 0;
                    bit_num = 0;
                }
            } else {
                if(iter == i) {
                    putc(ochars[0], stream);
                } else {
                    putc(ochars[i & 15], stream);
                }
            }
        }
        if(!bitmap)
            putc('\n', stream);
    }
    return true;
}

bool mandelbrot_to_file(const char *filename, const int w, const int h, bool binary_output) {
    FILE *stream;
    char *file_open_type = "w";

    if(binary_output) {
        file_open_type = "wb";
    }

    stream = fopen(filename, file_open_type);
    if(stream == NULL)
        return false;

    write_mandelbrot_to_stream(w, h, stream, binary_output);
    fclose(stream);
    return true;
}

char* mandelbrot_to_mem(const int w, const int h, bool binary_output) {
    FILE *stream;
    char *char_buffer;
    size_t buffer_size = 0;

    stream = open_memstream(&char_buffer, &buffer_size);
    if(stream == NULL)
        return "";

    write_mandelbrot_to_stream(w, h, stream, binary_output);
    fclose(stream);
    return char_buffer;
}

char* my_mandelbrot_to_mem(zval *w, zval *h, zval *binary_output) {
    return mandelbrot_to_mem((int) Z_LVAL_P(w), (int) Z_LVAL_P(h), Z_BVAL_P(binary_output));
}

bool my_mandelbrot_to_file(zval *filename, zval *w, zval *h, zval *binary_output) {
    return mandelbrot_to_file(Z_STRVAL_P(filename), (int) Z_LVAL_P(w), (int) Z_LVAL_P(h), Z_BVAL_P(binary_output));
}

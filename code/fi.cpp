// fi.cpp

internal void print_buffer(Renderer_Backbuffer *backbuffer, char *buffer)
{
    v2i pos = {};
    for (char *c = buffer; *c; ++c) {
        sw_draw_char(backbuffer, pos, *c, 0xD9DBD6);
        pos.x += 8;
    }
}

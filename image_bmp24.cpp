//
// Created by grubchan on 26.09.2021.
//

#include "image_bmp24.h"

#include <utility>

constexpr uint8_t get8bit(const char buffer[]) {
    return
            static_cast<uint8_t>(static_cast<uint8_t>(buffer[0]));
}
constexpr uint16_t get16bit(const char buffer[]) {
    return
            static_cast<uint16_t>(static_cast<uint8_t>(buffer[0])) |
            static_cast<uint16_t>(static_cast<uint8_t>(buffer[1])) << 8;
}
constexpr uint32_t get32bit(const char buffer[]) {
    return
            static_cast<uint32_t>(static_cast<uint8_t>(buffer[0])) |
            static_cast<uint32_t>(static_cast<uint8_t>(buffer[1])) << 8 |
            static_cast<uint32_t>(static_cast<uint8_t>(buffer[2])) << 16 |
            static_cast<uint32_t>(static_cast<uint8_t>(buffer[3])) << 24;
}

constexpr void set8bit(uint8_t data, char buffer[]) {
    buffer[0] = static_cast<char>(data);
}
constexpr void set16bit(uint16_t data, char buffer[]) {
    buffer[0] = static_cast<char>(data);
    buffer[1] = static_cast<char>(data >> 8);
}
constexpr void set32bit(uint32_t data, char buffer[]) {
    buffer[0] = static_cast<char>(data);
    buffer[1] = static_cast<char>(data >> 8);
    buffer[2] = static_cast<char>(data >> 16);
    buffer[3] = static_cast<char>(data >> 24);
}

image_bmp24::image_bmp24(const std::string& way) {
    try {
        image_name = way;
        std::ifstream fin(way, std::ios::binary | std::ios::app);
        if (!fin) { throw 1; }
        char buffer[4];

        // offset 0, size 2 - BM
        if (!fin.read(buffer, 2)) { throw 2; }
        if (buffer[0] != 'B' && buffer[1] != 'M') { throw 3; }

        // offset 2, size 4 - size of file
        if (!fin.read(buffer, 4)) { throw 2; }
        file_size = get32bit(buffer);

        // offset 6, size 4 - reserved (ignoring)
        if (!fin.read(buffer, 4)) { throw 2; }

        // offset 10, 4 - location of data
        if (!fin.read(buffer, 4)) { throw 2; }
        data_of_set = get32bit(buffer);

        // offset 14, size 4 - size of DIB header
        if (!fin.read(buffer, 4)) { throw 2; }
        dib_header_size = get32bit(buffer);
        if (dib_header_size != 40) { throw 3; }

        // offset 18, size 4 - pixel width
        if (!fin.read(buffer, 4)) { throw 2; }
        pixel_width = get32bit(buffer);

        // offset 22, size 4 - pixel height
        if (!fin.read(buffer, 4)) { throw 2; }
        pixel_height = get32bit(buffer);

        // offset 26, size 2 - colour planes
        if (!fin.read(buffer, 2)) { throw 2; }
        colour_planes = get16bit(buffer);
        if (colour_planes != 1) { throw 4; }

        // offset 28, size 2 - bits per pixel
        if (!fin.read(buffer, 2)) { throw 2; }
        bits_per_pixel = get16bit(buffer);
        if (bits_per_pixel != 24) { throw 6; }

        // offset 30, size 4 - compression type
        if (!fin.read(buffer, 4)) { throw 2; }
        compression_type = get32bit(buffer);
        if (compression_type != 0) { throw 5; }

        // offset 34, size 4 - image size
        if (!fin.read(buffer, 4)) { throw 2; }
        image_size = get32bit(buffer);

        // offset 38, size 4 - horizontal resolution
        if (!fin.read(buffer, 4)) { throw 2; }
        horizontal_res = get32bit(buffer);

        // offset 42, size 4 - vertical resolution
        if (!fin.read(buffer, 4)) { throw 2; }
        vertical_res = get32bit(buffer);

        // offset 46, size 4 - number of colours
        if (!fin.read(buffer, 4)) { throw 2; }
        number_of_colours = std::max<uint32_t>(get32bit(buffer), static_cast<uint32_t>(1) << bits_per_pixel);

        // offset 50, size 4 - number of important colours
        if (!fin.read(buffer, 4)) { throw 2; }
        number_of_important_colours = get32bit(buffer);

        buffer[0] = 0;  buffer[1] = 0; buffer[2] = 0; buffer[3] = 0;


        for (int i = 0; i < pixel_height; i++) {
            std::vector<COLOUR> tmp;
            COLOUR col = {0, 0, 0};
            for(int k = 0; k < pixel_width; k++){
                tmp.push_back(col);
            }
            data_pixel.push_back(tmp);
            for (int j = 0; j < pixel_width; j++) {
                if (!fin.read(buffer, 1)) { throw 2; }
                data_pixel[i][j].BLUE = get8bit(buffer);

                if (!fin.read(buffer, 1)) { throw 2; }
                data_pixel[i][j].GREEN = get8bit(buffer);

                if (!fin.read(buffer, 1)) { throw 2; }
                data_pixel[i][j].RED = get8bit(buffer);
            }
        }
        fin.close();
    }
    catch (const int& e) {
        std::cout << "Exception opening/reading file";
    }
}

void image_bmp24::rewriting() {
    try {
        std::ofstream fout(image_name, std::ios::binary);
        char* buffer = new char[4];

        // offset 0, size 2 - BM
        buffer[0] = 'B'; buffer[1] = 'M';
        if (!fout.write(buffer, 2)) { throw 2; }

        // offset 2, size 4 - size of file
        set32bit(get_file_size(), buffer);
        if (!fout.write(buffer, 4)) { throw 2; }

        // offset 6, size 4 - reserved (ignoring)
        set32bit(0, buffer);
        if (!fout.write(buffer, 4)) { throw 2; }

        // offset 10, 4 - location of data
        set32bit(get_data_of_set(), buffer);
        if (!fout.write(buffer, 4)) { throw 2; }

        // offset 14, size 4 - size of DIB header
        set32bit(get_dib_header_size(), buffer);
        if (!fout.write(buffer, 4)) { throw 2; }

        // offset 18, size 4 - pixel width
        set32bit(get_pixel_width(), buffer);
        if (!fout.write(buffer, 4)) { throw 2; }

        // offset 22, size 4 - pixel height
        set32bit(get_pixel_height(), buffer);
        if (!fout.write(buffer, 4)) { throw 2; }

        // offset 26, size 2 - colour planes
        set16bit(get_colour_planes(), buffer);
        if (!fout.write(buffer, 2)) { throw 2; }

        // offset 28, size 2 - bits per pixel
        set16bit(get_bits_per_pixel(), buffer);
        if (!fout.write(buffer, 2)) { throw 2; }

        // offset 30, size 4 - compression type
        set32bit(get_compression_type(), buffer);
        if (!fout.write(buffer, 4)) { throw 2; }

        // offset 34, size 4 - image size
        set32bit(get_image_size(), buffer);
        if (!fout.write(buffer, 4)) { throw 2; }

        // offset 38, size 4 - horizontal resolution
        set32bit(get_horizontal_res(), buffer);
        if (!fout.write(buffer, 4)) { throw 2; }

        // offset 42, size 4 - vertical resolution
        set32bit(get_vertical_res(), buffer);
        if (!fout.write(buffer, 4)) { throw 2; }

        // offset 46, size 4 - number of colours
        set32bit(get_number_of_colours(), buffer);
        if (!fout.write(buffer, 4)) { throw 2; }

        // offset 50, size 4 - number of important colours
        set32bit(get_number_of_important_colours(), buffer);
        if (!fout.write(buffer, 4)) { throw 2; }

        buffer[0] = 0;  buffer[1] = 0; buffer[2] = 0; buffer[3] = 0;

        for (int i = 0; i < pixel_height; i++) {
            for (int j = 0; j < pixel_width; j++) {
                set8bit(data_pixel[i][j].BLUE, buffer);
                if (!fout.write(buffer, 1)) { throw 2; }

                set8bit(data_pixel[i][j].GREEN, buffer);
                if (!fout.write(buffer, 1)) { throw 2; }

                set8bit(data_pixel[i][j].RED, buffer);
                if (!fout.write(buffer, 1)) { throw 2; }
            }
        }
        fout.close();
    }
    catch (const int& e) {
        std::cout << "Exception opening/reading file";
    }
}

void image_bmp24::print() const {
    std::cout << "File size: " << get_file_size() << std::endl
              << "Data of set: " << get_data_of_set() << std::endl
              << "Dib header size: " << get_dib_header_size() << std::endl
              << "Pixel width: " << get_pixel_width() << std::endl
              << "Pixel height: " << get_pixel_height() << std::endl
              << "Colour planes: " << get_colour_planes() << std::endl
              << "Bits per pixel: " << get_bits_per_pixel() << std::endl
              << "Compression type: " << get_compression_type() << std::endl
              << "Image_size: " << get_image_size() << std::endl
              << "Horizontal resolution: " << get_horizontal_res() << std::endl
              << "Vertical resolution: " << get_vertical_res() << std::endl
              << "Number of colours: " << get_number_of_colours() << std::endl
              << "Number of important colours: " << get_number_of_important_colours() << std::endl;
}

void image_bmp24::set_data_pixel(uint32_t x, uint32_t y, COLOUR colour) {
    if (x >= 0 && x < pixel_height && y >= 0 && y < pixel_width) {
        data_pixel[x][y].RED = colour.RED;
        data_pixel[x][y].BLUE = colour.BLUE;
        data_pixel[x][y].GREEN = colour.GREEN;
    }
}
void image_bmp24::set_data_pixel_time(uint32_t t, COLOUR colour) {
    if (t >= 0 && t < image_size) {
        uint32_t count = 0;
        for(int i = 0; i < get_pixel_height(); i++){
            for(int j = 0 ; j < get_pixel_width(); j++){
                if(count == t){
                    data_pixel[i][j].RED = colour.RED;
                    data_pixel[i][j].BLUE = colour.BLUE;
                    data_pixel[i][j].GREEN = colour.GREEN;
                }
                count++;
            }
        }
    }
}
void image_bmp24::set_new_data_pixel(std::vector<std::vector<COLOUR>> new_data) { data_pixel = std::move(new_data); }
void image_bmp24::set_file_size(uint32_t new_file_size) { file_size = new_file_size; }
void image_bmp24::set_data_of_set(uint32_t new_data_of_set) { data_of_set = new_data_of_set; }
void image_bmp24::set_dib_header_size(uint32_t new_dib_header_size) { dib_header_size = new_dib_header_size; }
void image_bmp24::set_pixel_width(uint32_t new_pixel_width) { pixel_width = new_pixel_width; }
void image_bmp24::set_pixel_height(uint32_t new_pixel_height) { pixel_height = new_pixel_height; }
void image_bmp24::set_colour_planes(uint16_t new_colour_planes) { colour_planes = new_colour_planes; }
void image_bmp24::set_bits_per_pixel(uint16_t new_bits_per_pixel) { bits_per_pixel = new_bits_per_pixel; }
void image_bmp24::set_compression_type(uint32_t new_compression_type) { compression_type = new_compression_type; }
void image_bmp24::set_image_size(uint32_t new_image_size) { image_size = new_image_size; }
void image_bmp24::set_horizontal_res(uint32_t new_horizontal_res) { horizontal_res = new_horizontal_res; }
void image_bmp24::set_vertical_res(uint32_t new_vertical_res) { vertical_res = new_vertical_res; }
void image_bmp24::set_number_of_colours(uint32_t new_number_of_colours) { number_of_colours = new_number_of_colours; }
void image_bmp24::set_number_of_important_colours(uint32_t new_number_of_important_colours) { number_of_important_colours = new_number_of_important_colours; }

std::vector<std::vector<COLOUR>> image_bmp24::get_data_pixel() { return data_pixel; }
uint32_t image_bmp24::get_file_size() const { return file_size; }
uint32_t image_bmp24::get_data_of_set() const { return data_of_set; }
uint32_t image_bmp24::get_dib_header_size() const { return dib_header_size; }
uint32_t image_bmp24::get_pixel_width() const { return pixel_width; }
uint32_t image_bmp24::get_pixel_height() const { return pixel_height; }
uint16_t image_bmp24::get_colour_planes() const { return colour_planes; }
uint16_t image_bmp24::get_bits_per_pixel() const { return bits_per_pixel; }
uint32_t image_bmp24::get_compression_type() const { return compression_type; }
uint32_t image_bmp24::get_image_size() const { return image_size; }
uint32_t image_bmp24::get_horizontal_res() const { return horizontal_res; }
uint32_t image_bmp24::get_vertical_res() const { return vertical_res; }
uint32_t image_bmp24::get_number_of_colours() const { return number_of_colours; }
uint32_t image_bmp24::get_number_of_important_colours() const { return number_of_important_colours; }

COLOUR image_bmp24::get_colour_pixel(uint32_t x, uint32_t y) {
    if (x >= 0 && x < pixel_height && y >= 0 && y < pixel_width) {
        return data_pixel[x][y];
    }
    return COLOUR();
}

COLOUR image_bmp24::get_colour_pixel_time(uint32_t t) {
    if (t >= 0 && t < image_size) {
        uint32_t count = 0;
        for(int i = 0; i < get_pixel_height(); i++){
            for(int j = 0 ; j < get_pixel_width(); j++){
                if(count == t){
                    return data_pixel[i][j];
                }
                count++;
            }
        }
    }
    return COLOUR();
}

image_bmp24::~image_bmp24() {

}
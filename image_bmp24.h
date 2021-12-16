//
// Created by MIKHAIL GRUBOV on 26.09.2021.
//

#ifndef IMAGE_RESAMPLING_1_4_IMAGE_BMP24_H
#define IMAGE_RESAMPLING_1_4_IMAGE_BMP24_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

constexpr uint8_t get8bit(const char buffer[]);
constexpr uint16_t get16bit(const char buffer[]);
constexpr uint32_t get32bit(const char buffer[]);
constexpr void set8bit(uint8_t data, char buffer[]);
constexpr void set16bit(uint16_t data, char buffer[]);
constexpr void set32bit(uint32_t data, char buffer[]);

struct COLOUR {
    uint8_t RED;
    uint8_t GREEN;
    uint8_t BLUE;
};

class image_bmp24 {
private:
    std::string image_name;
    std::vector<std::vector<COLOUR>> data_pixel;
    uint32_t file_size;
    uint32_t data_of_set;
    uint32_t dib_header_size;
    uint32_t pixel_width;
    uint32_t pixel_height;
    uint16_t colour_planes;
    uint16_t bits_per_pixel;
    uint32_t compression_type;
    uint32_t image_size;
    uint32_t horizontal_res;
    uint32_t vertical_res;
    uint32_t number_of_colours;
    uint32_t number_of_important_colours;
public:
    image_bmp24(const std::string& way);
    void rewriting();
    void print() const;

    void set_new_data_pixel(std::vector<std::vector<COLOUR>> new_data);
    void set_data_pixel(uint32_t x, uint32_t y, COLOUR colour);
    void set_data_pixel_time(uint32_t t, COLOUR colour);
    void set_file_size(uint32_t new_file_size);
    void set_data_of_set(uint32_t new_data_of_set);
    void set_dib_header_size(uint32_t new_dib_header_size);
    void set_pixel_width(uint32_t new_pixel_width);
    void set_pixel_height(uint32_t new_pixel_height);
    void set_colour_planes(uint16_t new_colour_planes);
    void set_bits_per_pixel(uint16_t new_bits_per_pixel);
    void set_compression_type(uint32_t new_compression_type);
    void set_image_size(uint32_t new_image_size);
    void set_horizontal_res(uint32_t new_horizontal_res);
    void set_vertical_res(uint32_t new_vertical_res);
    void set_number_of_colours(uint32_t new_number_of_colours);
    void set_number_of_important_colours(uint32_t new_number_of_important_colours);

    std::vector<std::vector<COLOUR>> get_data_pixel();
    COLOUR get_colour_pixel(uint32_t x, uint32_t y);
    COLOUR get_colour_pixel_time(uint32_t t);
    uint32_t get_file_size() const;
    uint32_t get_data_of_set() const;
    uint32_t get_dib_header_size() const;
    uint32_t get_pixel_width() const;
    uint32_t get_pixel_height() const;
    uint16_t get_colour_planes() const;
    uint16_t get_bits_per_pixel() const;
    uint32_t get_compression_type() const;
    uint32_t get_image_size() const;
    uint32_t get_horizontal_res() const;
    uint32_t get_vertical_res() const;
    uint32_t get_number_of_colours() const;
    uint32_t get_number_of_important_colours() const;

    ~image_bmp24();
};


#endif //IMAGE_RESAMPLING_1_4_IMAGE_BMP24_H

//
// Created by grubchan on 03.11.2021.
//

#include "CAST_128.h"

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

constexpr uint64_t get64bit(const char buffer[]) {
    return
            static_cast<uint64_t>(static_cast<uint8_t>(buffer[0])) |
            static_cast<uint64_t>(static_cast<uint8_t>(buffer[1])) << 8 |
            static_cast<uint64_t>(static_cast<uint8_t>(buffer[2])) << 16 |
            static_cast<uint64_t>(static_cast<uint8_t>(buffer[3])) << 24 |
            static_cast<uint64_t>(static_cast<uint8_t>(buffer[4])) << 32 |
            static_cast<uint64_t>(static_cast<uint8_t>(buffer[5])) << 40 |
            static_cast<uint64_t>(static_cast<uint8_t>(buffer[6])) << 48 |
            static_cast<uint64_t>(static_cast<uint8_t>(buffer[7])) << 56;
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

constexpr void set64bit(uint64_t data, char buffer[]) {
    buffer[0] = static_cast<char>(data);
    buffer[1] = static_cast<char>(data >> 8);
    buffer[2] = static_cast<char>(data >> 16);
    buffer[3] = static_cast<char>(data >> 24);
    buffer[4] = static_cast<char>(data >> 32);
    buffer[5] = static_cast<char>(data >> 40);
    buffer[6] = static_cast<char>(data >> 48);
    buffer[7] = static_cast<char>(data >> 56);
}


Key_128::Key_128(uint64_t L, uint64_t R, int size) {
    this->size = size;
    this->Left = L;
    this->Right = R;
    generation_of_subkeys();
}

void Key_128::generation_of_subkeys() {
    auto *subkeys = new uint32_t[32];
    char *Z = new char[16];
    char *X = new char[16];

    for (int i = 0; i < 16; i++) { X[i] = this->get_index(i); }

    char *buffer = new char[4];

    buffer[0] = X[3];
    buffer[1] = X[2];
    buffer[2] = X[1];
    buffer[3] = X[0];

    uint32_t z_1 = get32bit(buffer) ^ S[4][X[13]] ^ S[5][X[15]] ^ S[6][X[12]] ^ S[7][X[14]] ^ S[6][X[8]];

    set32bit(z_1, buffer);

    Z[0] = buffer[3];
    Z[1] = buffer[2];
    Z[2] = buffer[1];
    Z[3] = buffer[0];

    buffer[0] = X[11];
    buffer[1] = X[10];
    buffer[2] = X[9];
    buffer[3] = X[8];

    uint32_t z_2 = get32bit(buffer) ^ S[4][Z[0]] ^ S[5][Z[1]] ^ S[6][Z[2]] ^ S[7][Z[3]] ^ S[7][X[10]];

    set32bit(z_2, buffer);

    Z[4] = buffer[3];
    Z[5] = buffer[2];
    Z[6] = buffer[1];
    Z[7] = buffer[0];

    buffer[0] = X[15];
    buffer[1] = X[14];
    buffer[2] = X[13];
    buffer[3] = X[12];

    uint32_t z_3 = get32bit(buffer) ^ S[4][Z[7]] ^ S[5][Z[6]] ^ S[6][Z[5]] ^ S[7][Z[4]] ^ S[4][X[9]];

    set32bit(z_3, buffer);

    Z[8] = buffer[3];
    Z[9] = buffer[2];
    Z[10] = buffer[1];
    Z[11] = buffer[0];

    buffer[0] = X[7];
    buffer[1] = X[6];
    buffer[2] = X[5];
    buffer[3] = X[4];

    uint32_t z_4 = get32bit(buffer) ^ S[4][Z[10]] ^ S[5][Z[9]] ^ S[6][Z[11]] ^ S[7][Z[8]] ^ S[5][X[11]];

    set32bit(z_4, buffer);

    Z[12] = buffer[3];
    Z[13] = buffer[2];
    Z[14] = buffer[1];
    Z[15] = buffer[0];

    subkeys[0] = S[4][Z[8]] ^ S[5][Z[9]] ^ S[6][Z[7]] ^ S[7][Z[6]] ^ S[5][Z[2]];
    subkeys[1] = S[4][Z[10]] ^ S[5][Z[11]] ^ S[6][Z[5]] ^ S[7][Z[4]] ^ S[5][Z[6]];
    subkeys[2] = S[4][Z[12]] ^ S[5][Z[13]] ^ S[6][Z[3]] ^ S[7][Z[2]] ^ S[5][Z[9]];
    subkeys[3] = S[4][Z[14]] ^ S[5][Z[15]] ^ S[6][Z[1]] ^ S[7][Z[0]] ^ S[5][Z[12]];

    buffer[0] = Z[11];
    buffer[1] = Z[10];
    buffer[2] = Z[9];
    buffer[3] = Z[8];

    uint32_t k_1 = get32bit(buffer) ^ S[4][Z[5]] ^ S[5][Z[7]] ^ S[6][Z[4]] ^ S[7][Z[6]] ^ S[6][Z[0]];

    set32bit(k_1, buffer);

    X[0] = buffer[3];
    X[1] = buffer[2];
    X[2] = buffer[1];
    X[3] = buffer[0];

    buffer[0] = Z[3];
    buffer[1] = Z[2];
    buffer[2] = Z[1];
    buffer[3] = Z[0];

    uint32_t k_2 = get32bit(buffer) ^ S[4][X[0]] ^ S[5][X[2]] ^ S[6][X[1]] ^ S[7][X[3]] ^ S[7][Z[2]];

    set32bit(k_2, buffer);

    X[4] = buffer[3];
    X[5] = buffer[2];
    X[6] = buffer[1];
    X[7] = buffer[0];

    buffer[0] = Z[7];
    buffer[1] = Z[6];
    buffer[2] = Z[5];
    buffer[3] = Z[4];

    uint32_t k_3 = get32bit(buffer) ^ S[4][X[7]] ^ S[5][X[6]] ^ S[6][X[5]] ^ S[7][X[4]] ^ S[4][Z[1]];

    set32bit(k_3, buffer);

    X[8] = buffer[3];
    X[9] = buffer[2];
    X[10] = buffer[1];
    X[11] = buffer[0];

    buffer[0] = Z[15];
    buffer[1] = Z[14];
    buffer[2] = Z[13];
    buffer[3] = Z[12];

    uint32_t k_4 = get32bit(buffer) ^ S[4][X[10]] ^ S[5][X[9]] ^ S[6][X[11]] ^ S[7][X[8]] ^ S[5][Z[3]];

    set32bit(k_4, buffer);

    X[12] = buffer[3];
    X[13] = buffer[2];
    X[14] = buffer[1];
    X[15] = buffer[0];

    subkeys[4] = S[4][X[3]] ^ S[5][X[2]] ^ S[6][X[12]] ^ S[7][X[13]] ^ S[5][X[8]];
    subkeys[5] = S[4][X[1]] ^ S[5][X[0]] ^ S[6][X[14]] ^ S[7][X[15]] ^ S[5][X[13]];
    subkeys[6] = S[4][X[7]] ^ S[5][X[6]] ^ S[6][X[8]] ^ S[7][X[9]] ^ S[5][X[3]];
    subkeys[7] = S[4][X[5]] ^ S[5][X[4]] ^ S[6][X[10]] ^ S[7][X[11]] ^ S[5][X[7]];

    buffer[0] = X[3];
    buffer[1] = X[2];
    buffer[2] = X[1];
    buffer[3] = X[0];

    uint32_t z_5 = get32bit(buffer) ^ S[4][X[13]] ^ S[5][X[15]] ^ S[6][X[12]] ^ S[7][X[14]] ^ S[6][X[8]];

    set32bit(z_5, buffer);

    Z[0] = buffer[3];
    Z[1] = buffer[2];
    Z[2] = buffer[1];
    Z[3] = buffer[0];

    buffer[0] = X[11];
    buffer[1] = X[10];
    buffer[2] = X[9];
    buffer[3] = X[8];

    uint32_t z_6 = get32bit(buffer) ^ S[4][Z[0]] ^ S[5][Z[1]] ^ S[6][Z[2]] ^ S[7][Z[3]] ^ S[7][X[10]];

    set32bit(z_6, buffer);

    Z[4] = buffer[3];
    Z[5] = buffer[2];
    Z[6] = buffer[1];
    Z[7] = buffer[0];

    buffer[0] = X[15];
    buffer[1] = X[14];
    buffer[2] = X[13];
    buffer[3] = X[12];

    uint32_t z_7 = get32bit(buffer) ^ S[4][Z[7]] ^ S[5][Z[6]] ^ S[6][Z[5]] ^ S[7][Z[4]] ^ S[4][X[9]];

    set32bit(z_7, buffer);

    Z[8] = buffer[3];
    Z[9] = buffer[2];
    Z[10] = buffer[1];
    Z[11] = buffer[0];

    buffer[0] = X[7];
    buffer[1] = X[6];
    buffer[2] = X[5];
    buffer[3] = X[4];

    uint32_t z_8 = get32bit(buffer) ^ S[4][Z[10]] ^ S[5][Z[9]] ^ S[6][Z[11]] ^ S[7][Z[8]] ^ S[5][X[11]];

    set32bit(z_8, buffer);

    Z[12] = buffer[3];
    Z[13] = buffer[2];
    Z[14] = buffer[1];
    Z[15] = buffer[0];

    subkeys[8] = S[4][Z[3]] ^ S[5][Z[2]] ^ S[6][Z[12]] ^ S[7][Z[13]] ^ S[5][Z[9]];
    subkeys[9] = S[4][Z[1]] ^ S[5][Z[0]] ^ S[6][Z[14]] ^ S[7][Z[15]] ^ S[5][Z[12]];
    subkeys[10] = S[4][Z[7]] ^ S[5][Z[6]] ^ S[6][Z[8]] ^ S[7][Z[9]] ^ S[5][Z[2]];
    subkeys[11] = S[4][Z[5]] ^ S[5][Z[4]] ^ S[6][Z[10]] ^ S[7][Z[11]] ^ S[5][Z[6]];

    buffer[0] = Z[11];
    buffer[1] = Z[10];
    buffer[2] = Z[9];
    buffer[3] = Z[8];

    uint32_t k_5 = get32bit(buffer) ^ S[4][Z[5]] ^ S[5][Z[7]] ^ S[6][Z[4]] ^ S[7][Z[6]] ^ S[6][Z[0]];

    set32bit(k_5, buffer);

    X[0] = buffer[3];
    X[1] = buffer[2];
    X[2] = buffer[1];
    X[3] = buffer[0];

    buffer[0] = Z[3];
    buffer[1] = Z[2];
    buffer[2] = Z[1];
    buffer[3] = Z[0];

    uint32_t k_6 = get32bit(buffer) ^ S[4][X[0]] ^ S[5][X[2]] ^ S[6][X[1]] ^ S[7][X[3]] ^ S[7][Z[2]];

    set32bit(k_6, buffer);

    X[4] = buffer[3];
    X[5] = buffer[2];
    X[6] = buffer[1];
    X[7] = buffer[0];

    buffer[0] = Z[7];
    buffer[1] = Z[6];
    buffer[2] = Z[5];
    buffer[3] = Z[4];

    uint32_t k_7 = get32bit(buffer) ^ S[4][X[7]] ^ S[5][X[6]] ^ S[6][X[5]] ^ S[7][X[4]] ^ S[4][Z[1]];

    set32bit(k_7, buffer);

    X[8] = buffer[3];
    X[9] = buffer[2];
    X[10] = buffer[1];
    X[11] = buffer[0];

    buffer[0] = Z[15];
    buffer[1] = Z[14];
    buffer[2] = Z[13];
    buffer[3] = Z[12];

    uint32_t k_8 = get32bit(buffer) ^ S[4][X[10]] ^ S[5][X[9]] ^ S[6][X[11]] ^ S[7][X[8]] ^ S[5][Z[3]];

    set32bit(k_8, buffer);

    X[12] = buffer[3];
    X[13] = buffer[2];
    X[14] = buffer[1];
    X[15] = buffer[0];

    subkeys[12] = S[4][X[8]] ^ S[5][X[9]] ^ S[6][X[7]] ^ S[7][X[6]] ^ S[5][X[3]];
    subkeys[13] = S[4][X[10]] ^ S[5][X[11]] ^ S[6][X[5]] ^ S[7][X[4]] ^ S[5][X[7]];
    subkeys[14] = S[4][X[12]] ^ S[5][X[13]] ^ S[6][X[3]] ^ S[7][X[2]] ^ S[5][X[8]];
    subkeys[15] = S[4][X[14]] ^ S[5][X[15]] ^ S[6][X[1]] ^ S[7][X[0]] ^ S[5][X[9]];

    buffer[0] = X[3];
    buffer[1] = X[2];
    buffer[2] = X[1];
    buffer[3] = X[0];

    uint32_t z_9 = get32bit(buffer) ^ S[4][X[13]] ^ S[5][X[15]] ^ S[6][X[12]] ^ S[7][X[14]] ^ S[6][X[8]];

    set32bit(z_9, buffer);

    Z[0] = buffer[3];
    Z[1] = buffer[2];
    Z[2] = buffer[1];
    Z[3] = buffer[0];

    buffer[0] = X[11];
    buffer[1] = X[10];
    buffer[2] = X[9];
    buffer[3] = X[8];

    uint32_t z_10 = get32bit(buffer) ^ S[4][Z[0]] ^ S[5][Z[1]] ^ S[6][Z[2]] ^ S[7][Z[3]] ^ S[7][X[10]];

    set32bit(z_10, buffer);

    Z[4] = buffer[3];
    Z[5] = buffer[2];
    Z[6] = buffer[1];
    Z[7] = buffer[0];

    buffer[0] = X[15];
    buffer[1] = X[14];
    buffer[2] = X[13];
    buffer[3] = X[12];

    uint32_t z_11 = get32bit(buffer) ^ S[4][Z[7]] ^ S[5][Z[6]] ^ S[6][Z[5]] ^ S[7][Z[4]] ^ S[4][X[9]];

    set32bit(z_11, buffer);

    Z[8] = buffer[3];
    Z[9] = buffer[2];
    Z[10] = buffer[1];
    Z[11] = buffer[0];

    buffer[0] = X[7];
    buffer[1] = X[6];
    buffer[2] = X[5];
    buffer[3] = X[4];

    uint32_t z_12 = get32bit(buffer) ^ S[4][Z[10]] ^ S[5][Z[9]] ^ S[6][Z[11]] ^ S[7][Z[8]] ^ S[5][X[11]];

    set32bit(z_12, buffer);

    Z[12] = buffer[3];
    Z[13] = buffer[2];
    Z[14] = buffer[1];
    Z[15] = buffer[0];

    subkeys[16] = S[4][Z[8]] ^ S[5][Z[9]] ^ S[6][Z[7]] ^ S[7][Z[6]] ^ S[5][Z[2]];
    subkeys[17] = S[4][Z[10]] ^ S[5][Z[11]] ^ S[6][Z[5]] ^ S[7][Z[4]] ^ S[5][Z[6]];
    subkeys[18] = S[4][Z[12]] ^ S[5][Z[13]] ^ S[6][Z[3]] ^ S[7][Z[2]] ^ S[5][Z[9]];
    subkeys[19] = S[4][Z[14]] ^ S[5][Z[15]] ^ S[6][Z[1]] ^ S[7][Z[0]] ^ S[5][Z[12]];

    buffer[0] = Z[11];
    buffer[1] = Z[10];
    buffer[2] = Z[9];
    buffer[3] = Z[8];

    uint32_t k_9 = get32bit(buffer) ^ S[4][Z[5]] ^ S[5][Z[7]] ^ S[6][Z[4]] ^ S[7][Z[6]] ^ S[6][Z[0]];

    set32bit(k_9, buffer);

    X[0] = buffer[3];
    X[1] = buffer[2];
    X[2] = buffer[1];
    X[3] = buffer[0];

    buffer[0] = Z[3];
    buffer[1] = Z[2];
    buffer[2] = Z[1];
    buffer[3] = Z[0];

    uint32_t k_10 = get32bit(buffer) ^ S[4][X[0]] ^ S[5][X[2]] ^ S[6][X[1]] ^ S[7][X[3]] ^ S[7][Z[2]];

    set32bit(k_10, buffer);

    X[4] = buffer[3];
    X[5] = buffer[2];
    X[6] = buffer[1];
    X[7] = buffer[0];

    buffer[0] = Z[7];
    buffer[1] = Z[6];
    buffer[2] = Z[5];
    buffer[3] = Z[4];

    uint32_t k_11 = get32bit(buffer) ^ S[4][X[7]] ^ S[5][X[6]] ^ S[6][X[5]] ^ S[7][X[4]] ^ S[4][Z[1]];

    set32bit(k_11, buffer);

    X[8] = buffer[3];
    X[9] = buffer[2];
    X[10] = buffer[1];
    X[11] = buffer[0];

    buffer[0] = Z[15];
    buffer[1] = Z[14];
    buffer[2] = Z[13];
    buffer[3] = Z[12];

    uint32_t k_12 = get32bit(buffer) ^ S[4][X[10]] ^ S[5][X[9]] ^ S[6][X[11]] ^ S[7][X[8]] ^ S[5][Z[3]];

    set32bit(k_12, buffer);

    X[12] = buffer[3];
    X[13] = buffer[2];
    X[14] = buffer[1];
    X[15] = buffer[0];

    subkeys[20] = S[4][X[3]] ^ S[5][X[2]] ^ S[6][X[12]] ^ S[7][X[13]] ^ S[5][X[8]];
    subkeys[21] = S[4][X[1]] ^ S[5][X[0]] ^ S[6][X[14]] ^ S[7][X[15]] ^ S[5][X[13]];
    subkeys[22] = S[4][X[7]] ^ S[5][X[6]] ^ S[6][X[8]] ^ S[7][X[9]] ^ S[5][X[3]];
    subkeys[23] = S[4][X[5]] ^ S[5][X[4]] ^ S[6][X[10]] ^ S[7][X[11]] ^ S[5][X[7]];

    buffer[0] = X[3];
    buffer[1] = X[2];
    buffer[2] = X[1];
    buffer[3] = X[0];

    uint32_t z_13 = get32bit(buffer) ^ S[4][X[13]] ^ S[5][X[15]] ^ S[6][X[12]] ^ S[7][X[14]] ^ S[6][X[8]];

    set32bit(z_13, buffer);

    Z[0] = buffer[3];
    Z[1] = buffer[2];
    Z[2] = buffer[1];
    Z[3] = buffer[0];

    buffer[0] = X[11];
    buffer[1] = X[10];
    buffer[2] = X[9];
    buffer[3] = X[8];

    uint32_t z_14 = get32bit(buffer) ^ S[4][Z[0]] ^ S[5][Z[1]] ^ S[6][Z[2]] ^ S[7][Z[3]] ^ S[7][X[10]];

    set32bit(z_14, buffer);

    Z[4] = buffer[3];
    Z[5] = buffer[2];
    Z[6] = buffer[1];
    Z[7] = buffer[0];

    buffer[0] = X[15];
    buffer[1] = X[14];
    buffer[2] = X[13];
    buffer[3] = X[12];

    uint32_t z_15 = get32bit(buffer) ^ S[4][Z[7]] ^ S[5][Z[6]] ^ S[6][Z[5]] ^ S[7][Z[4]] ^ S[4][X[9]];

    set32bit(z_15, buffer);

    Z[8] = buffer[3];
    Z[9] = buffer[2];
    Z[10] = buffer[1];
    Z[11] = buffer[0];

    buffer[0] = X[7];
    buffer[1] = X[6];
    buffer[2] = X[5];
    buffer[3] = X[4];

    uint32_t z_16 = get32bit(buffer) ^ S[4][Z[10]] ^ S[5][Z[9]] ^ S[6][Z[11]] ^ S[7][Z[8]] ^ S[5][X[11]];

    set32bit(z_16, buffer);

    Z[12] = buffer[3];
    Z[13] = buffer[2];
    Z[14] = buffer[1];
    Z[15] = buffer[0];

    subkeys[24] = S[4][Z[3]] ^ S[5][Z[2]] ^ S[6][Z[12]] ^ S[7][Z[13]] ^ S[5][Z[9]];
    subkeys[25] = S[4][Z[1]] ^ S[5][Z[0]] ^ S[6][Z[14]] ^ S[7][Z[15]] ^ S[5][Z[12]];
    subkeys[26] = S[4][Z[7]] ^ S[5][Z[6]] ^ S[6][Z[8]] ^ S[7][Z[9]] ^ S[5][Z[2]];
    subkeys[27] = S[4][Z[5]] ^ S[5][Z[4]] ^ S[6][Z[10]] ^ S[7][Z[11]] ^ S[5][Z[6]];

    buffer[0] = Z[11];
    buffer[1] = Z[10];
    buffer[2] = Z[9];
    buffer[3] = Z[8];

    uint32_t k_13 = get32bit(buffer) ^ S[4][Z[5]] ^ S[5][Z[7]] ^ S[6][Z[4]] ^ S[7][Z[6]] ^ S[6][Z[0]];

    set32bit(k_13, buffer);

    X[0] = buffer[3];
    X[1] = buffer[2];
    X[2] = buffer[1];
    X[3] = buffer[0];

    buffer[0] = Z[3];
    buffer[1] = Z[2];
    buffer[2] = Z[1];
    buffer[3] = Z[0];

    uint32_t k_14 = get32bit(buffer) ^ S[4][X[0]] ^ S[5][X[2]] ^ S[6][X[1]] ^ S[7][X[3]] ^ S[7][Z[2]];

    set32bit(k_14, buffer);

    X[4] = buffer[3];
    X[5] = buffer[2];
    X[6] = buffer[1];
    X[7] = buffer[0];

    buffer[0] = Z[7];
    buffer[1] = Z[6];
    buffer[2] = Z[5];
    buffer[3] = Z[4];

    uint32_t k_15 = get32bit(buffer) ^ S[4][X[7]] ^ S[5][X[6]] ^ S[6][X[5]] ^ S[7][X[4]] ^ S[4][Z[1]];

    set32bit(k_15, buffer);

    X[8] = buffer[3];
    X[9] = buffer[2];
    X[10] = buffer[1];
    X[11] = buffer[0];

    buffer[0] = Z[15];
    buffer[1] = Z[14];
    buffer[2] = Z[13];
    buffer[3] = Z[12];

    uint32_t k_16 = get32bit(buffer) ^ S[4][X[10]] ^ S[5][X[9]] ^ S[6][X[11]] ^ S[7][X[8]] ^ S[5][Z[3]];

    set32bit(k_16, buffer);

    X[12] = buffer[3];
    X[13] = buffer[2];
    X[14] = buffer[1];
    X[15] = buffer[0];

    subkeys[28] = S[4][X[8]] ^ S[5][X[9]] ^ S[6][X[7]] ^ S[7][X[6]] ^ S[5][X[3]];
    subkeys[29] = S[4][X[10]] ^ S[5][X[11]] ^ S[6][X[5]] ^ S[7][X[4]] ^ S[5][X[7]];
    subkeys[30] = S[4][X[12]] ^ S[5][X[13]] ^ S[6][X[3]] ^ S[7][X[2]] ^ S[5][X[8]];
    subkeys[31] = S[4][X[14]] ^ S[5][X[15]] ^ S[6][X[1]] ^ S[7][X[0]] ^ S[5][X[9]];

    for (int i = 0; i < 16; i++) {
        K_m[i] = subkeys[i];
        K_r[i] = subkeys[16 + i];
    }

    delete[] subkeys, Z, X, buffer;
}

char Key_128::get_index(int index) const {
    char *buffer = new char[8];
    if (index >= 0 && index < size) {
        if (index < 8) {
            set64bit(Left, buffer);
            return buffer[7 - index];
        } else {
            set64bit(Right, buffer);
            return buffer[7 - (index - 8)];
        }
    } else {
        set64bit(Left, buffer);
        return buffer[0];
    }
}

uint32_t Key_128::get_index_S(int index_i, int index_j) {
    return S[index_i][index_j];
}

uint32_t Key_128::get_index_K_m(int index) {
    return K_m[index];
}

uint32_t Key_128::get_index_K_r(int index) {
    return K_r[index];
}


CAST_128::CAST_128() {
    std::cout << "--------CAST-128--------\n";
    std::cout << "Введите название файла: ";
    std::cin >> file_name;
    start();
}

void CAST_128::start() {
    std::cout << "Выберите режим работы алгоритма\n1 - ECB;\n2 - CBC;\n>";
    int work_mode;
    std::cin >> work_mode;
    std::cout << '\n';

    std::cout << "Ввдедите одну из предложеных команд:\n"
                 "1 - зашифровать\n"
                 "2 - расшифровать\n"
                 "3 - изменить название файла\n"
                 "4 - изменить режим шифрования\n"
                 "5 - вывести файл на экран\n"
                 "6 - справочная информация\n"
                 "7 - выход\n";
    bool check = false;
    bool stop = false;

    while (!stop) {
        std::cout << ">";
        int mode;
        std::cin >> mode;
        std::string str;
        switch (mode) {
            case (1):
                if (!check) {
                    if (work_mode == 1) {
                        ECB_ENCRYPT();
                    } else if (work_mode == 2) {
                        CBC_ENCRYPT();
                    }
                    check = true;
                } else {
                    std::cout << "Сообщение уже зашифровано!\n";
                }
                break;
            case (2):
                if (check) {
                    if (work_mode == 1) {
                        ECB_DECIPTER();
                    } else if (work_mode == 2) {
                        CBC_DECIPTER();
                    }
                    check = false;
                } else {
                    std::cout << "Сообщение Не зашифровано!\n";
                }
                break;
            case (3):
                if (!check) {
                    std::cout << "Введите название файла: ";
                    std::cin >> file_name;
                } else {
                    std::cout << "Нельзя менять название файла если сообщение уже зашифровано!\n";
                }
                break;
            case (4):
                if (!check) {
                    while (true) {
                        std::cout << "Выберите режим работы алгоритма\n1 - ECB;\n2 - CBC;\n>";
                        std::cin >> work_mode;
                        if (work_mode == 1 || work_mode == 2) break;
                    }
                } else {
                    std::cout << "Нельзя менять режим работы если сообщение уже зашифровано!\n";
                }
                break;
            case (5):
                str = "";
                str += "xdg-open ";
                str += file_name;
                std::system(str.c_str());
                break;

            case (6):
                std::cout << "Ввдедите одну из предложеных команд:\n"
                             "1 - зашифровать\n"
                             "2 - расшифровать\n"
                             "3 - изменить название файла\n"
                             "4 - изменить режим шифрования\n"
                             "5 - вывести файл на экран\n"
                             "6 - справочная информация\n"
                             "7 - выход\n";
                break;
            case (7):
                stop = true;
                break;
            default:
                std::cout << "Такой команды не существует!\n";
                break;
        }
    }
}

uint32_t CAST_128::F(uint32_t data, int index) {
    char *buffer = new char[4];
    if (index == 0 || index == 3 || index == 6 || index == 9 || index == 12 || index == 15) {
        uint32_t I = ((K->get_index_K_m(index) + data) % static_cast<uint32_t>(pow(2, 32)));
        uint32_t K_r = K->get_index_K_r(index) & 0x0000001f;
        uint32_t s = K_r >= 0 ? K_r % 32 : -((-K_r) % 32);
        I = (I << s) | (I >> (32 - s));
        set32bit(I, buffer);
        return ((((K->get_index_S(0, buffer[3]) ^ K->get_index_S(1, buffer[2])) - K->get_index_S(2, buffer[1])) %
                 static_cast<uint32_t>(pow(2, 32))) + K->get_index_S(3, buffer[0])) % static_cast<uint32_t>(pow(2, 32));
    } else if (index == 1 || index == 4 || index == 7 || index == 10 || index == 13) {
        uint32_t I = (K->get_index_K_m(index) ^ data);
        uint32_t K_r = K->get_index_K_r(index) & 0x0000001f;
        uint32_t s = K_r >= 0 ? K_r % 32 : -((-K_r) % 32);
        I = (I << s) | (I >> (32 - s));
        set32bit(I, buffer);
        return ((((K->get_index_S(0, buffer[3]) - K->get_index_S(1, buffer[2])) % static_cast<uint32_t>(pow(2, 32))) +
                 K->get_index_S(2, buffer[1]))) % static_cast<uint32_t>(pow(2, 32)) ^ K->get_index_S(3, buffer[0]);
    } else if (index == 2 || index == 5 || index == 8 || index == 11 || index == 14) {
        uint32_t I = ((K->get_index_K_m(index) - data) % static_cast<uint32_t>(pow(2, 32)));
        uint32_t K_r = K->get_index_K_r(index) & 0x0000001f;
        uint32_t s = K_r >= 0 ? K_r % 32 : -((-K_r) % 32);
        I = (I << s) | (I >> (32 - s));
        set32bit(I, buffer);
        return ((((K->get_index_S(0, buffer[3]) + K->get_index_S(1, buffer[2])) % static_cast<uint32_t>(pow(2, 32))) ^
                 K->get_index_S(2, buffer[1])) - K->get_index_S(3, buffer[0])) % static_cast<uint32_t>(pow(2, 32));
    } else { return 0; }
}

uint64_t CAST_128::encrypt(uint64_t data) {
    char *buffer_8 = new char[8];
    char *buffer_4 = new char[4];
    auto *L = new uint32_t[17];
    auto *R = new uint32_t[17];

    set64bit(data, buffer_8);

    buffer_4[0] = buffer_8[0];
    buffer_4[1] = buffer_8[1];
    buffer_4[2] = buffer_8[2];
    buffer_4[3] = buffer_8[3];

    L[0] = get32bit(buffer_4);

    buffer_4[0] = buffer_8[4];
    buffer_4[1] = buffer_8[5];
    buffer_4[2] = buffer_8[6];
    buffer_4[3] = buffer_8[7];

    R[0] = get32bit(buffer_4);

    for (int i = 1; i <= 16; i++) {
        L[i] = R[i - 1];
        R[i] = L[i - 1] ^ F(R[i - 1], i - 1);
    }

    set32bit(L[16], buffer_4);

    buffer_8[0] = buffer_4[0];
    buffer_8[1] = buffer_4[1];
    buffer_8[2] = buffer_4[2];
    buffer_8[3] = buffer_4[3];

    set32bit(R[16], buffer_4);

    buffer_8[4] = buffer_4[0];
    buffer_8[5] = buffer_4[1];
    buffer_8[6] = buffer_4[2];
    buffer_8[7] = buffer_4[3];

    uint64_t C = get64bit(buffer_8);

    delete[] buffer_8;
    delete[] buffer_4;

    return C;
}

uint64_t CAST_128::decrypt(uint64_t data) {
    char *buffer_8 = new char[8];
    char *buffer_4 = new char[4];
    auto *L = new uint32_t[17];
    auto *R = new uint32_t[17];

    set64bit(data, buffer_8);

    buffer_4[0] = buffer_8[0];
    buffer_4[1] = buffer_8[1];
    buffer_4[2] = buffer_8[2];
    buffer_4[3] = buffer_8[3];

    L[0] = get32bit(buffer_4);

    buffer_4[0] = buffer_8[4];
    buffer_4[1] = buffer_8[5];
    buffer_4[2] = buffer_8[6];
    buffer_4[3] = buffer_8[7];

    R[0] = get32bit(buffer_4);

    for (int i = 1; i <= 16; i++) {
        R[i] = L[i - 1];
        L[i] = R[i - 1] ^ F(L[i - 1], 15 - (i - 1));
    }

    set32bit(L[16], buffer_4);

    buffer_8[0] = buffer_4[0];
    buffer_8[1] = buffer_4[1];
    buffer_8[2] = buffer_4[2];
    buffer_8[3] = buffer_4[3];

    set32bit(R[16], buffer_4);

    buffer_8[4] = buffer_4[0];
    buffer_8[5] = buffer_4[1];
    buffer_8[6] = buffer_4[2];
    buffer_8[7] = buffer_4[3];

    uint64_t C = get64bit(buffer_8);

    delete[] buffer_8;
    delete[] buffer_4;

    return C;
}

void CAST_128::ECB_ENCRYPT() {
    std::cout << "Выберите один из двух вариантов:\n1 - Ввести ключ шифрования\n2 - Сгенерировать его автоматически.\n> ";
    int ch;
    bool stop = false;
    while (!stop) {
        std::cin >> ch;
        switch (ch) {
            case (1):
                uint64_t L, R;
                std::cout << "Введите ключ: \nL: ";
                std::cin >> L;
                std::cout << "R: ";
                std::cin >> R;
                K = new Key_128(L, R, 16);
                stop = true;
                break;
            case (2):
                srand(std::time(nullptr));
                uint64_t L_t, R_t;
                L_t = static_cast<uint64_t>(rand());
                R_t = static_cast<uint64_t>(rand());
                K = new Key_128(L_t, R_t, 16);
                stop = true;
                break;
            default:
                std::cout << "Такой команды несуществует!\n";
                break;
        }
    }
    std::vector<uint64_t> data_do, data;
    file_rider(data);
    data_do = data;
    for(auto& i : data){
        i = encrypt(i);
    }
    raspredelenie_0_1(data);
    korelation_kofizient(data_do ,data);
    file_writer(data);
}

void CAST_128::ECB_DECIPTER() {
    std::vector<uint64_t> data;
    file_rider(data);
    for(auto& i : data){
        i = decrypt(i);
    }
    file_writer(data);
}

void CAST_128::CBC_ENCRYPT() {
    K = new Key_128(0x0123456712345678, 0x234567893456789A, 16);
    std::vector<uint64_t> data_do, data;
    file_rider(data);
    data_do = data;
    C.push_back(0x8a3c61b3901e5b27);
    for(int i = 0; i < data.size(); i++){
        data[i] ^= C[i];
        data[i] = encrypt(data[i]);
        C.push_back(i);
    }
    raspredelenie_0_1(data);
    korelation_kofizient(data_do ,data);
    file_writer(data);
}

void CAST_128::CBC_DECIPTER() {
    std::vector<uint64_t> data;
    file_rider(data);
    for(int i = 0; i < data.size(); i++){
        data[i] = decrypt(data[i]);
        data[i] ^= C[i];
    }
    file_writer(data);
}

void CAST_128::file_rider(std::vector<uint64_t>& data) {
    try {
        char* buffer_8 = new char[8];
        char* buffer_1 = new char[1];
        bool is_image = false;

        for(int i = 0; i < file_name.size(); i++){
            if(file_name[i] == '.'){
                if(file_name[i+1] == 'b'){
                    if(file_name[i+2] == 'm'){
                        if(file_name[i+3] == 'p'){
                            is_image = true;
                        }
                    }
                }
            }
        }
        if(is_image){
            auto* image = new image_bmp24(file_name);
            std::vector<std::vector<COLOUR>> image_data = image->get_data_pixel();
            int index = 0;
            for(auto & i : image_data){
                for(auto & j : i){
                    set8bit(j.BLUE, buffer_1);
                    buffer_8[index++] = buffer_1[0];
                    if(index == 8){
                        index = 0;
                        data.push_back(get64bit(buffer_8));
                    }
                    set8bit(j.GREEN, buffer_1);
                    buffer_8[index++] = buffer_1[0];
                    if(index == 8){
                        index = 0;
                        data.push_back(get64bit(buffer_8));
                    }
                    set8bit(j.RED, buffer_1);
                    buffer_8[index++] = buffer_1[0];
                    if(index == 8){
                        index = 0;
                        data.push_back(get64bit(buffer_8));
                    }
                }
            }
        }
        else {
            std::ifstream fin(file_name, std::ios::binary | std::ios::app);
            if (!fin) { throw 1; }
            int index = 0;
            while (fin.read(buffer_1, 1)) {
                buffer_8[index++] = buffer_1[0];
                if (index == 8) {
                    index = 0;
                    data.push_back(get64bit(buffer_8));
                }
            }
        }
    }
    catch (const int &e) {
        std::cout << "Exception opening/reading file";
    }
}

void CAST_128::file_writer(std::vector<uint64_t>& data) {
    try {
        char* buffer_8 = new char[8];
        char* buffer_1 = new char[1];
        bool is_image = false;

        for(int i = 0; i < file_name.size(); i++){
            if(file_name[i] == '.'){
                if(file_name[i+1] == 'b'){
                    if(file_name[i+2] == 'm'){
                        if(file_name[i+3] == 'p'){
                            is_image = true;
                        }
                    }
                }
            }
        }

        if(is_image){
            auto* image = new image_bmp24(file_name);
            std::vector<std::vector<COLOUR>> image_data = image->get_data_pixel();
            int index_data = 0;
            int index = -1;
            for(auto & i: image_data){
                for(auto & j : i){
                    if(index == -1){
                        set64bit(data[index_data++], buffer_8);
                        index = 0;
                    }
                    buffer_1[0] = buffer_8[index++];
                    j.BLUE = get8bit(buffer_1);
                    if(index == 8){
                        set64bit(data[index_data++], buffer_8);
                        index = 0;
                    }
                    buffer_1[0] = buffer_8[index++];
                    j.GREEN = get8bit(buffer_1);
                    if(index == 8){
                        set64bit(data[index_data++], buffer_8);
                        index = 0;
                    }
                    buffer_1[0] = buffer_8[index++];
                    j.RED = get8bit(buffer_1);
                    if(index == 8){
                        set64bit(data[index_data++], buffer_8);
                        index = 0;
                    }

                }
            }
            image->set_new_data_pixel(image_data);
            image->rewriting();
            delete image;
        }
        else {
            std::ofstream fout(file_name, std::ios::binary);
            if (!fout) { throw 1; }

            for (auto &i: data) {
                set64bit(i, buffer_8);
                if (!fout.write(buffer_8, 8)) { throw 2; }
            }
        }
    }
    catch (const int& e) {
        std::cout << "Exception opening/reading file";
    }
}

void CAST_128::korelation_kofizient(std::vector<uint64_t> A, std::vector<uint64_t> B) {
    double r = 0;
    double value = 0;
    double a = 0;
    double b = 0;
    std::bitset<64> t_1, t_2;
    int count = 0;
    if(A.size() == B.size()) {
        size_t size= A.size();
        int de = 0;
        for (int i = 0; i < size; i++){
            t_1 = A[i];
            t_2 = B[i];
            for(int j = 0; j < t_1.size(); j++){
                de += t_1[j] * t_2[j];
                count++;
            }
        }
        r = (double) ( (double) de / (double) count );
        r = sqrt(r);
        de = 0;
        for (int i = 0; i < size; i++){
            t_1 = A[i];
            for(int j = 0; j < t_1.size(); j++){
                de += t_1[j] * t_1[j];
            }
        }
        a = (double) ((double) de / (double) size);
        a = sqrt(a);
        for (int i = 0; i < size; i++){
            t_1 = B[i];
            for(int j = 0; j < t_1.size(); j++){
                de += t_1[j] * t_1[j];
            }
        }
        b = (double) ((double) de / (double) size);
        b = sqrt(b);
        value = r / (a * b);
    }
    std::cout << "коэфециент кореляции = " << value << std::endl << std::endl;
}

void CAST_128::raspredelenie_0_1(const std::vector<uint64_t>& data) {
    std::bitset<64> t;
    int count_0 = 0;
    int count_1 = 0;

    for(auto & i : data){
        t = i;
        for(int j = 0; j < t.size(); j++){
            if(t[0] == 0){
                count_0++;
            }
            else if(t[0] == 1){
                count_1++;
            }
        }
    }
    std::cout << "количество 0 бит = " << count_0 << std::endl;
    std::cout << "количество 1 бит = " << count_1 << std::endl;
}



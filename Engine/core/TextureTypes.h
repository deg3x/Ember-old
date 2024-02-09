#pragma once

enum TextureFormat : uint16_t
{
    R                = 0x1903,
    RGB              = 0x1907,
    RGBA             = 0x1908,
    RGB16F           = 0x881B,
    DEPTH24          = 0x81A6,
    DEPTH24_STENCIL8 = 0x88F0,
    
    NONE   = 0x00
};

enum class TextureType : uint8_t
{
    DIFFUSE,
    CUBE_MAP,
    HDR
};

enum TextureDataType : uint16_t
{
    UNSIGNED_BYTE = 0x1401,
    FLOAT         = 0x1406
};

enum TextureTarget : uint16_t
{
    TEXTURE_2D                  = 0x0DE1,
    TEXTURE_CUBE_MAP            = 0x8513,
    TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515,
    TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516,
    TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517,
    TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518,
    TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519,
    TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A,
    
    AUTO = 0x0000
};

enum TextureUnit : uint16_t
{
    TEX_0  = 0x84C0,
    TEX_1  = 0x84C1,
    TEX_2  = 0x84C2,
    TEX_3  = 0x84C3,
    TEX_4  = 0x84C4,
    TEX_5  = 0x84C5,
    TEX_6  = 0x84C6,
    TEX_7  = 0x84C7,
    TEX_8  = 0x84C8,
    TEX_9  = 0x84C9,
    TEX_10 = 0x84CA,
    TEX_11 = 0x84CB,
    TEX_12 = 0x84CC,
    TEX_13 = 0x84CD,
    TEX_14 = 0x84CE,
    TEX_15 = 0x84CF,
    TEX_16 = 0x84D0,
    TEX_17 = 0x84D1,
    TEX_18 = 0x84D2,
    TEX_19 = 0x84D3,
    TEX_20 = 0x84D4,
    TEX_21 = 0x84D5,
    TEX_22 = 0x84D6,
    TEX_23 = 0x84D7,
    TEX_24 = 0x84D8,
    TEX_25 = 0x84D9,
    TEX_26 = 0x84DA,
    TEX_27 = 0x84DB,
    TEX_28 = 0x84DC,
    TEX_29 = 0x84DD,
    TEX_30 = 0x84DE,
    TEX_31 = 0x84DF
};
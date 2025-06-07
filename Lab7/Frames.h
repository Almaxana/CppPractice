#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

uint64_t KnowSize(std::vector<char>& vec, uint64_t count, uint64_t start);
uint64_t KnowSize(char* first, uint64_t count);

class Frame {
public:
    char* name;
    uint32_t frame_size;

    Frame(char* new_name, uint32_t new_frame_size);
    virtual ~Frame();

    virtual void print();
};

class WebFrame : public Frame {
public:
    std::vector<char> data;

    WebFrame(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class TextFrame : public WebFrame {
public:
    char text_encoding;

    TextFrame(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class T_W_XXX : public TextFrame {
public:
    std::vector<char> descr;

    T_W_XXX(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class USER : public TextFrame {
public:
    std::vector<char> language;

    USER(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class USLT_COMM : public USER {
public:
    std::vector<char> content_descr;

    USLT_COMM(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class POPM : public Frame {
public:
    std::vector<char> email;
    char rating;
    uint64_t counter;

    POPM(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class UFID : public Frame {
public:
    std::vector<char> owner_id;
    std::vector<char> id;

    UFID(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class PCNT : public Frame {
public:
    uint64_t counter;

    PCNT(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class LINK : public Frame {
public:
    uint32_t frame_id;
    std::vector<char> url;
    std::vector<char> add;

    LINK(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class OWNE : public Frame {
public:
    char text_encoding;
    std::vector<char> price;
    std::vector<char> date;
    std::vector<char> seller;

    OWNE(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class RBUF : public Frame {
public:
    uint32_t buff_size;
    bool flag;
    uint32_t offset_to_next_tag;

    RBUF(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class POSS : public Frame {
public:
    char time_stamp_form;
    uint32_t position;

    POSS(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class SIGN : public Frame {
public:
    char symbol;
    std::vector<char> bin_data;

    SIGN(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class GRID : public SIGN {
public:
    std::vector<char> own_id;

    GRID(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class PRIV : public Frame {
public:
    std::vector<char> own_id;
    std::vector<char> priv_data;

    PRIV(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class SEEK : public Frame {
public:
    uint32_t offset;

    SEEK(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class COMR : public OWNE {
public:
    std::vector<char> URL;
    char received_as;
    std::vector<char> descr;
    char pict_type;
    std::vector<char> logo;

    COMR(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class ENCR : public Frame {
public:
    std::vector<char> own_id;
    char symbol;
    std::vector<char> encr_data;

    ENCR(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

class SYLT : public Frame {
public:
    char text_encoding;
    std::vector<char> lang;
    char time_st_form;
    char cont_type;
    std::vector<char> content_descr;
    std::vector<char> content;

    SYLT(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

struct point {
    uint16_t freq;
    uint16_t vol_adj;
};

class EQU2 : public Frame {
public:
    char interp_m;
    std::vector<char> id;
    std::vector<point> points;

    EQU2(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

struct event {
    char type;
    uint32_t time_stamp;
};

class ETCO : public Frame {
public:
    char time_st_form;
    std::vector<event> events;

    ETCO(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

struct channel {
    char type;
    uint16_t vol_adj;
    char bit_repr_pick;
    uint32_t pick_vol;
};

class RVA2 : public Frame {
public:
    std::vector<char> id;
    std::vector<channel> channels;

    RVA2(char* new_name, uint32_t new_frame_size);
    virtual void print();
};

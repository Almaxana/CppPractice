#include "frames.h"
#include <cmath>
#include <cstdlib>


uint64_t KnowSize(std::vector<char>& vec, uint64_t count, uint64_t start) {
    uint64_t size = 0;
    for (uint64_t j = 0; j < count; j++) {
        size += static_cast<uint64_t>(vec[start + j]) * pow(128, count - j - 1);
    }
    return size;
}

uint64_t KnowSize(char* first, uint64_t count) {
    uint64_t size = 0;
    for (uint64_t j = 0; j < count; j++) {
        size += static_cast<uint64_t>(first[j]) * pow(128, count - j - 1);
    }
    return size;
}


Frame::Frame(char* new_name, uint32_t new_frame_size) {
    name = new_name;
    frame_size = new_frame_size;
}

Frame::~Frame() {
    free(name);
}

void Frame::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size << "\n\n\n";
}

// --- WebFrame ---

WebFrame::WebFrame(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void WebFrame::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size << "\n";
    for (char c : data) std::cout << c;
    std::cout << "\n\n\n";
}

// --- TextFrame ---

TextFrame::TextFrame(char* new_name, uint32_t new_frame_size)
        : WebFrame(new_name, new_frame_size) {}

void TextFrame::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\ntext encoding   " << static_cast<int>(text_encoding)
              << "\ndata   ";
    for (char c : data) std::cout << c;
    std::cout << "\n\n\n";
}

// --- T_W_XXX ---

T_W_XXX::T_W_XXX(char* new_name, uint32_t new_frame_size)
        : TextFrame(new_name, new_frame_size) {}

void T_W_XXX::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\ntext encoding   " << static_cast<int>(text_encoding)
              << "\ndescription   ";
    for (char c : descr) std::cout << c;
    std::cout << "\ndata   ";
    for (char c : data) std::cout << c;
    std::cout << "\n\n\n";
}

// --- USER ---

USER::USER(char* new_name, uint32_t new_frame_size)
        : TextFrame(new_name, new_frame_size) {}

void USER::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\ntext encoding   " << static_cast<int>(text_encoding)
              << "\nlanguage   ";
    for (char c : language) std::cout << c;
    std::cout << "\ndata   ";
    for (char c : data) std::cout << c;
    std::cout << "\n\n\n";
}

// --- USLT_COMM ---

USLT_COMM::USLT_COMM(char* new_name, uint32_t new_frame_size)
        : USER(new_name, new_frame_size) {}

void USLT_COMM::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\ntext encoding   " << static_cast<int>(text_encoding)
              << "\nlanguage   ";
    for (char c : language) std::cout << c;
    std::cout << "\ncontent description   ";
    for (char c : content_descr) std::cout << c;
    std::cout << "\nlyrics   ";
    for (char c : data) std::cout << c;
    std::cout << "\n\n\n";
}

// --- POPM ---

POPM::POPM(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void POPM::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\nemail   ";
    for (char c : email) std::cout << c;
    std::cout << "\nrating   " << static_cast<int>((unsigned char)rating)
              << "\ncounter   " << counter;
    std::cout << "\n\n\n";
}

// --- UFID ---

UFID::UFID(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void UFID::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\nowner id   ";
    for (char c : owner_id) std::cout << c;
    std::cout << "\nid   ";
    for (char c : id) std::cout << c;
    std::cout << "\n\n\n";
}

// --- PCNT ---

PCNT::PCNT(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void PCNT::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\ncounter   " << counter;
    std::cout << "\n\n\n";
}

// --- LINK ---

LINK::LINK(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void LINK::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\nframe id   " << frame_id
              << "\nurl   ";
    for (char c : url) std::cout << c;
    std::cout << "\nadditional   ";
    for (char c : add) std::cout << c;
    std::cout << "\n\n\n";
}

// --- OWNE ---

OWNE::OWNE(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void OWNE::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\ntext encoding   " << static_cast<int>(text_encoding)
              << "\nprice   ";
    for (char c : price) std::cout << c;
    std::cout << "\ndate  ";
    for (char c : date) std::cout << c;
    std::cout << "\nseller   ";
    for (char c : seller) std::cout << c;
    std::cout << "\n\n\n";
}

// --- RBUF ---

RBUF::RBUF(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void RBUF::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\nbuffer size   " << buff_size
              << "\nflag   " << (flag ? "true" : "false")
              << "\noffset to next tag   " << offset_to_next_tag;
    std::cout << "\n\n\n";
}

// --- POSS ---

POSS::POSS(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void POSS::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\ntime stamp format   " << static_cast<int>(time_stamp_form)
              << "\nposition   " << position;
    std::cout << "\n\n\n";
}

// --- SIGN ---

SIGN::SIGN(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void SIGN::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\nsymbol   " << static_cast<int>(symbol)
              << "\nbinary data   ";
    for (char c : bin_data) std::cout << c;
    std::cout << "\n\n\n";
}

// --- GRID ---

GRID::GRID(char* new_name, uint32_t new_frame_size)
        : SIGN(new_name, new_frame_size) {}

void GRID::print() {
    SIGN::print();
    std::cout << "own id   ";
    for (char c : own_id) std::cout << c;
    std::cout << "\n\n\n";
}

// --- PRIV ---

PRIV::PRIV(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void PRIV::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\nown id   ";
    for (char c : own_id) std::cout << c;
    std::cout << "\nprivate data   ";
    for (char c : priv_data) std::cout << c;
    std::cout << "\n\n\n";
}

// --- SEEK ---

SEEK::SEEK(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void SEEK::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\noffset   " << offset;
    std::cout << "\n\n\n";
}

// --- COMR ---

COMR::COMR(char* new_name, uint32_t new_frame_size)
        : OWNE(new_name, new_frame_size) {}

void COMR::print() {
    OWNE::print();
    std::cout << "URL   ";
    for (char c : URL) std::cout << c;
    std::cout << "\nreceived as   " << static_cast<int>(received_as)
              << "\ndescription   ";
    for (char c : descr) std::cout << c;
    std::cout << "\npicture type   " << static_cast<int>(pict_type)
              << "\nlogo   ";
    for (char c : logo) std::cout << c;
    std::cout << "\n\n\n";
}

// --- ENCR ---

ENCR::ENCR(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void ENCR::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\nown id   ";
    for (char c : own_id) std::cout << c;
    std::cout << "\nsymbol   " << static_cast<int>(symbol)
              << "\nencrypted data   ";
    for (char c : encr_data) std::cout << c;
    std::cout << "\n\n\n";
}

// --- SYLT ---

SYLT::SYLT(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void SYLT::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\ntext encoding   " << static_cast<int>(text_encoding)
              << "\nlanguage   ";
    for (char c : lang) std::cout << c;
    std::cout << "\ntime stamp format   " << static_cast<int>(time_st_form)
              << "\ncontent type   " << static_cast<int>(cont_type)
              << "\ncontent description   ";
    for (char c : content_descr) std::cout << c;
    std::cout << "\ncontent   ";
    for (char c : content) std::cout << c;
    std::cout << "\n\n\n";
}

// --- EQU2 ---

EQU2::EQU2(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void EQU2::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\ninterpolation method   " << static_cast<int>(interp_m)
              << "\nid   ";
    for (char c : id) std::cout << c;
    std::cout << "\npoints   \n";
    for (auto& p : points) {
        std::cout << "freq: " << p.freq << " vol_adj: " << p.vol_adj << "\n";
    }
    std::cout << "\n\n\n";
}

// --- ETCO ---

ETCO::ETCO(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void ETCO::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\ntime stamp format   " << static_cast<int>(time_st_form)
              << "\nevents   \n";
    for (auto& e : events) {
        std::cout << "type: " << static_cast<int>(e.type)
                  << " time_stamp: " << e.time_stamp << "\n";
    }
    std::cout << "\n\n\n";
}

// --- RVA2 ---

RVA2::RVA2(char* new_name, uint32_t new_frame_size)
        : Frame(new_name, new_frame_size) {}

void RVA2::print() {
    std::cout << "name  ";
    for (int i = 0; i < 4; i++) std::cout << name[i];
    std::cout << "\nsize  " << frame_size
              << "\nid   ";
    for (char c : id) std::cout << c;
    std::cout << "\nchannels   \n";
    for (auto& ch : channels) {
        std::cout << "type: " << static_cast<int>(ch.type)
                  << " vol_adj: " << ch.vol_adj
                  << " bit_repr_pick: " << static_cast<int>(ch.bit_repr_pick)
                  << " pick_vol: " << ch.pick_vol << "\n";
    }
    std::cout << "\n\n\n";
}

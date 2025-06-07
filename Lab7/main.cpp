#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>
#include <fstream>

#include "Frames.h"


int main(int argc, char** argv) {

    std::ifstream is(argv[1], std::ios::binary);
    if (!is) {
        std::cerr << "Error: can't open the file " << argv[1] << "\n";
        return 0;
    }

    std::vector<Frame*> store;

    std::vector<char> main_header (10);
    bool ext_header = false;
    is.read(&main_header[0], 10);


    std::cout<<"\nfile identifier    "<<main_header[0]<<main_header[1]<<main_header[2]<<"\n";
    std::cout<<"version            "<<static_cast<int>(main_header[3])<<"\n";
    std::cout<<"subversion         "<<static_cast<int>(main_header[4])<<"\n";


    if (main_header[5] & 0b01000000) {
        std::cout << "Extended Header exists\n";
        ext_header = true;
    }

    int frames_size = KnowSize(main_header, 4, 6);
    std::cout << "frames size        " << frames_size<<"\n\n\n";

    if (ext_header) {
        std::vector<char> ext_header (6);
        is.read(&ext_header[0], 6);
        uint32_t ext_header_size = KnowSize(ext_header, 4, 0);
        is.seekg(ext_header_size - 6, std::ios::cur);
    }


    while (is.tellg() < frames_size + 10) {


        char* new_name = (char*) malloc(4);///frame name
        is.read(new_name,  4);

        char* buf_size = (char*) malloc(4);///frame size
        is.read(buf_size,  4);
        uint32_t new_size = KnowSize(buf_size, 4);

        is.seekg( 2, std::ios::cur);///flags

        if (new_name[0] == 'T' and new_name[1] != 'X') {

            TextFrame* textFrame = new TextFrame(new_name, new_size);

            is.read(&textFrame->text_encoding, 1);///text encoding

            textFrame->data.resize(new_size-1);///data
            is.read(&textFrame->data[0],  new_size-1);

            store.push_back(textFrame);


        } else if (std::strncmp(new_name, "WXXX", 4)==0 or std::strncmp(new_name, "TXXX", 4)==0){

            T_W_XXX* t_w_xxx = new T_W_XXX(new_name, new_size);

            is.read(&t_w_xxx->text_encoding, 1);///text encoding

            t_w_xxx->descr.resize(1);///descr
            is.read(&t_w_xxx->descr[0], 1);
            while (static_cast<int>(t_w_xxx->descr[t_w_xxx->descr.size() - 1]) != 0) {
                t_w_xxx->descr.resize(t_w_xxx->descr.size() + 1);
                is.read(&t_w_xxx->descr[t_w_xxx->descr.size() - 1], 1);
            }

            t_w_xxx->data.resize(new_size-1 - t_w_xxx->descr.size());///data
            is.read(&t_w_xxx->data[0],  new_size - 1 - t_w_xxx->descr.size());

            store.push_back(t_w_xxx);

        } else if (std::strncmp(new_name, "USER", 4)==0) {

            USER* user = new USER(new_name, new_size);
            is.read(&user->text_encoding, 1);///text encoding

            user->language.resize(3);
            is.read(&user->language[0], 3);///language


            user->data.resize(new_size - 1 - 3);///data
            is.read(&user->data[0], new_size - 1 - 3);


            store.push_back(user);

        } else if (std::strncmp(new_name, "USLT", 4)==0 or std::strncmp(new_name, "COMM", 4)==0) {

            USLT_COMM* uslt_comm = new USLT_COMM(new_name, new_size);

            is.read(&uslt_comm->text_encoding, 1);///text encoding

            uslt_comm->language.resize(3);
            is.read(&uslt_comm->language[0], 3);///language

            uslt_comm->content_descr.resize(1);///content descriotion
            is.read(&uslt_comm->content_descr[0], 1);
            while (static_cast<int>(uslt_comm->content_descr[uslt_comm->content_descr.size() - 1]) != 0) {
                uslt_comm->content_descr.resize(uslt_comm->content_descr.size() + 1);
                is.read(&uslt_comm->content_descr[uslt_comm->content_descr.size() - 1], 1);
            }

            uslt_comm->data.resize(new_size - 1 - 3 - uslt_comm->content_descr.size());///data
            is.read(&uslt_comm->data[0], new_size - 1 - 3 - uslt_comm->content_descr.size());

            store.push_back(uslt_comm);

        } else if (new_name[0]=='W') {

            WebFrame* web_frame = new WebFrame(new_name, new_size);

            web_frame->data.resize(new_size);///data
            is.read(&web_frame->data[0], new_size);

            store.push_back(web_frame);

        } else if (std::strncmp(new_name, "POPM", 4)==0) {

            POPM* popm = new POPM(new_name, new_size);

            popm->email.resize(1);///email
            is.read(&popm->email[0], 1);
            while (static_cast<int>(popm->email[popm->email.size() - 1]) != 0) {
                popm->email.resize(popm->email.size() + 1);
                is.read(&popm->email[popm->email.size() - 1], 1);
            }

            is.read(&popm->rating, 1); ///rating

            char* new_counter = (char*) malloc (sizeof (char) * (new_size - popm->email.size() - 1));///counter
            is.read(new_counter, new_size - popm->email.size() - 1);
            popm->counter = KnowSize(new_counter, new_size - popm->email.size() - 1);
            free(new_counter);

            store.push_back(popm);

        } else if (std::strncmp(new_name, "UFID", 4) == 0) {

            UFID* ufid = new UFID(new_name, new_size);

            ufid->owner_id.resize(1);///own id
            is.read(&ufid->owner_id[0], 1);
            while (static_cast<int>(ufid->owner_id[ufid->owner_id.size() - 1]) != 0) {
                ufid->owner_id.resize(ufid->owner_id.size() + 1);
                is.read(&ufid->owner_id[ufid->owner_id.size() - 1], 1);
            }

            ufid->id.resize(new_size - ufid->owner_id.size());///id
            is.read(&ufid->id[0], new_size - ufid->owner_id.size());

            store.push_back(ufid);

        } else if (std::strncmp(new_name, "PCNT", 4) == 0) {

            PCNT* pcnt = new PCNT(new_name, new_size);

            char* new_counter = (char*) malloc (new_size);///counter
            is.read(new_counter, new_size);
            pcnt->counter = KnowSize(new_counter, new_size);
            free(new_counter);

            store.push_back(pcnt);

        } else if (std::strncmp(new_name, "LINK", 4) == 0) {

            LINK* link = new LINK(new_name, new_size);

            char* new_frame_id = (char*)malloc(4);
            is.read(new_frame_id, 4);///frame identifier
            link->frame_id = KnowSize(new_frame_id, 4);
            free(new_frame_id);


            link->url.resize(1);///url
            is.read(&link->url[0], 1);
            while (static_cast<int>(link->url[link->url.size() - 1]) != 0) {
                link->url.resize(link->url.size() + 1);
                is.read(&link->url[link->url.size() - 1], 1);
            }

            link->url.resize(new_size - link->url.size() - 4);///add
            is.read(&link->url[0], new_size - link->url.size() - 4);

            store.push_back(link);

        } else if(std::strncmp(new_name, "OWNE", 4) == 0) {

            OWNE* owne = new OWNE(new_name, new_size);

            is.read(&owne->text_encoding, 1);///text encoding

            owne->price.resize(1);///price
            is.read(&owne->price[0], 1);
            while (static_cast<int>(owne->price[owne->price.size() - 1]) != 0) {
                owne->price.resize(owne->price.size() + 1);
                is.read(&owne->price[owne->price.size() - 1], 1);
            }

            owne->date.resize(8);///date
            is.read(&owne->date[0], 8);

            owne->seller.resize(new_size - 1 - owne->price.size() - 8);///seller
            is.read(&owne->seller[0], new_size - 1 - owne->price.size() - 8);

            store.push_back(owne);

        } else if(std::strncmp(new_name, "RBUF", 4) == 0) {

            RBUF* rbuf = new RBUF(new_name, new_size);

            char* new_buff_size = (char*) malloc (sizeof(char) * 3);///buffer size
            is.read(new_buff_size, 3);
            rbuf->buff_size = KnowSize(new_buff_size, 3);
            free(new_buff_size);

            char new_flag;///flag
            is.read(&new_flag, 1);
            rbuf->flag = bool(new_flag);

            char* new_offset = (char*) malloc (sizeof(char) * 3);///offset
            is.read(new_offset, 3);
            rbuf->offset_to_next_tag = KnowSize(new_offset, 3);
            free(new_offset);

            store.push_back(rbuf);

        } else if(std::strncmp(new_name, "POSS", 4) == 0) {

            POSS* poss = new POSS(new_name, new_size);

            is.read(&poss->time_stamp_form, 1);///time stamp format

            char* new_position = (char*) malloc(sizeof (char) * (new_size - 1));///position
            is.read(new_position, new_size - 1);
            poss->position = KnowSize(new_position, new_size - 1);
            free(new_position);

            store.push_back(poss);

        } else if(std::strncmp(new_name, "GRID", 4) == 0) {

            GRID* grid = new GRID(new_name, new_size);

            grid->own_id.resize(1);///own id
            is.read(&grid->own_id[0], 1);
            while (static_cast<int>(grid->own_id[grid->own_id.size() - 1]) != 0) {
                grid->own_id.resize(grid->own_id.size() + 1);
                is.read(&grid->own_id[grid->own_id.size() - 1], 1);
            }

            is.read(&grid->symbol, 1);///group symbol

            grid->bin_data.resize(new_size - grid->own_id.size() - 1);///group data
            is.read(&grid->bin_data[0], new_size - grid->own_id.size() - 1);

            store.push_back(grid);

        } else if (std::strncmp(new_name, "PRIV", 4) == 0) {

            PRIV* priv = new PRIV(new_name, new_size);

            priv->own_id.resize(1);///own id
            is.read(&priv->own_id[0], 1);
            while (static_cast<int>(priv->own_id[priv->own_id.size() - 1]) != 0) {
                priv->own_id.resize(priv->own_id.size() + 1);
                is.read(&priv->own_id[priv->own_id.size() - 1], 1);
            }

            priv->priv_data.resize(new_size - priv->own_id.size());///private data
            is.read(&priv->priv_data[0],  new_size - priv->own_id.size());

            store.push_back(priv);

        } else if (std::strncmp(new_name, "SIGN", 4) == 0) {

            SIGN* sign = new SIGN(new_name, new_size);

            is.read(&sign->symbol, 1);///symbol


            sign->bin_data.resize(new_size - 1);///bin data
            is.read(&sign->bin_data[0], new_size - 1);

            store.push_back(sign);

        } else if (std::strncmp(new_name, "SEEK", 4) == 0) {

            SEEK* seek = new SEEK(new_name, new_size);

            char* new_offset = (char*) malloc (sizeof (char) * 4);///offset
            is.read(new_offset,4);
            seek->offset = KnowSize(new_offset, 4);
            free(new_offset);

            store.push_back(seek);

        } else if (std::strncmp(new_name, "COMR", 4) == 0) {

            COMR* comr = new COMR(new_name, new_size);

            is.read(&comr->text_encoding, 1);///text encoding

            comr->price.resize(1);///price
            is.read(&comr->price[0], 1);
            while (static_cast<int>(comr->price[comr->price.size() - 1]) != 0) {
                comr->price.resize(comr->price.size() + 1);
                is.read(&comr->price[comr->price.size() - 1], 1);
            }

            comr->date.resize(8);///valid until
            is.read(&comr->date[0], 8);


            comr->URL.resize(1);///url
            is.read(&comr->URL[0], 1);
            while (static_cast<int>(comr->URL[comr->URL.size() - 1]) != 0) {
                comr->URL.resize(comr->URL.size() + 1);
                is.read(&comr->URL[comr->URL.size() - 1], 1);
            }

            is.read(&comr->received_as, 1);///received as

            comr->seller.resize(1);///seeler
            is.read(&comr->seller[0], 1);
            while (static_cast<int>(comr->seller[comr->seller.size() - 1]) != 0) {
                comr->seller.resize(comr->seller.size() + 1);
                is.read(&comr->seller[comr->seller.size() - 1], 1);
            }

            comr->descr.resize(1);///descr
            is.read(&comr->descr[0], 1);
            while (static_cast<int>(comr->descr[comr->descr.size() - 1]) != 0) {
                comr->descr.resize(comr->descr.size() + 1);
                is.read(&comr->descr[comr->descr.size() - 1], 1);
            }

            is.read(&comr->pict_type, 1);

            comr->logo.resize(new_size - comr->price.size() - comr->seller.size() - comr->descr.size() - comr->URL.size() - 1 - 1 - 1 - 8);
            is.read(&comr->logo[0], new_size - comr->price.size() - comr->seller.size() - comr->descr.size() - comr->URL.size() - 1 - 1 - 1 - 8);

            store.push_back(comr);

        } else if(std::strncmp(new_name, "ENCR", 4) == 0) {

            ENCR* encr = new ENCR(new_name, new_size);

            encr->own_id.resize(1);///own id
            is.read(&encr->own_id[0], 1);
            while (static_cast<int>(encr->own_id[encr->own_id.size() - 1]) != 0) {
                encr->own_id.resize(encr->own_id.size() + 1);
                is.read(&encr->own_id[encr->own_id.size() - 1], 1);
            }

            is.read(&encr->symbol, 1);///symbol

            encr->encr_data.resize(new_size - encr->own_id.size() - 1);///data
            is.read(&encr->encr_data[0], new_size - encr->own_id.size() - 1);

            store.push_back(encr);

        } else if(std::strncmp(new_name, "SYLT", 4) == 0) {

            SYLT* sylt = new SYLT(new_name, new_size);

            is.read(&sylt->text_encoding, 1);///text encoding

            sylt->lang.resize(3);
            is.read(&sylt->lang[0], 3);///language

            is.read(&sylt->time_st_form, 1);///time stamp format

            is.read(&sylt->cont_type, 1);///content type

            sylt->content_descr.resize(1);///content descr
            is.read(&sylt->content_descr[0], 1);
            while (static_cast<int>(sylt->content_descr[sylt->content_descr.size() - 1]) != 0) {
                sylt->content_descr.resize(sylt->content_descr.size() + 1);
                is.read(&sylt->content_descr[sylt->content_descr.size() - 1], 1);
            }

            sylt->content.resize(new_size - 1 - 3 - 1 - 1 -sylt->content_descr.size());
            is.read(&sylt->content[0], new_size - 1 - 3 - 1 - 1 -sylt->content_descr.size());

            store.push_back(sylt);

        } else if(std::strncmp(new_name, "ETCO", 4) == 0) {

            ETCO* etco = new ETCO(new_name, new_size);

            is.read(&etco->time_st_form, 1);///time stamp format

            for(size_t i = 0; i < (new_size - 1)/5; i++) {///events

                event ev;

                is.read(&ev.type, 1);

                char* new_time_st = (char*) malloc (4);
                is.read(new_time_st, 4);
                ev.time_stamp = KnowSize(new_time_st, 4);
                free(new_time_st);

                etco->events.emplace_back(ev);
            }

            store.push_back(etco);

        } else if(std::strncmp(new_name, "RVA2", 4) == 0) {

            RVA2* rva2 = new RVA2(new_name, new_size);

            rva2->id.resize(1);///id
            is.read(&rva2->id[0], 1);
            while (static_cast<int>(rva2->id[rva2->id.size() - 1]) != 0) {
                rva2->id.resize(rva2->id.size() + 1);
                is.read(&rva2->id[rva2->id.size() - 1], 1);
            }

            for(size_t i = 0; i < (new_size - rva2->id.size())/8; i++) {///channels

                channel ch;

                is.read(&ch.type, 1);

                char* new_vol_adj = (char*) malloc (2);
                is.read(new_vol_adj, 2);
                ch.vol_adj = KnowSize(new_vol_adj, 2);
                free(new_vol_adj);

                is.read(&ch.bit_repr_pick, 1);

                char* new_pick_vol = (char*) malloc (4);
                is.read(new_pick_vol, 4);
                ch.pick_vol = KnowSize(new_pick_vol, 4);
                free(new_pick_vol);

                rva2->channels.emplace_back(ch);
            }

            store.push_back(rva2);

        } else if(std::strncmp(new_name, "EQU2", 4) == 0) {

            EQU2* equ2 = new EQU2(new_name, new_size);

            is.read(&equ2->interp_m, 1);///interpol method

            equ2->id.resize(1);///id
            is.read(&equ2->id[0], 1);
            while (static_cast<int>(equ2->id[equ2->id.size() - 1]) != 0) {
                equ2->id.resize(equ2->id.size() + 1);
                is.read(&equ2->id[equ2->id.size() - 1], 1);
            }

            for(size_t i = 0; i < (new_size - 1 - equ2->id.size())/4; i++) {///points

                point p;

                char* new_freq = (char*) malloc(2);
                is.read(new_freq, 2);
                p.freq = KnowSize(new_freq, 2);
                free(new_freq);

                char* new_vol_adj = (char*) malloc(2);
                is.read(new_vol_adj, 2);
                p.vol_adj = KnowSize(new_vol_adj, 2);
                free(new_vol_adj);

                equ2->points.emplace_back(p);

            }

            store.push_back(equ2);

        } else {
            is.seekg(new_size, std::ios::cur);
        }
    }


    for(auto it : store) {
        it->print();
    }

    for(auto it : store) {
        delete it;
    }


    return 0;
}
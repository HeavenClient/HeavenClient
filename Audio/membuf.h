//
// Created by rich on 3/7/20.
//

#ifndef HEAVENCLIENT_MEMBUF_H
#define HEAVENCLIENT_MEMBUF_H

#include <streambuf>
#include <sstream>

class membuf : public std::streambuf
{
public:
    membuf(const char *data, unsigned int len);

private:
    int_type underflow();
    int_type uflow();
    int_type pbackfail(int_type ch);
    std::streamsize showmanyc();
    pos_type seekoff(off_type off,
                     std::ios_base::seekdir dir,
                     std::ios_base::openmode which);
    pos_type seekpos(pos_type sp_, std::ios_base::openmode which_);

    const char * const begin_;
    const char * const end_;
    const char * current_;
};


#endif //HEAVENCLIENT_MEMBUF_H

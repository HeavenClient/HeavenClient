//
// Created by rich on 3/7/20.
//

#include "membuf.h"

membuf::membuf(const char *data, unsigned int len)
        : begin_(data), end_(data + len), current_(data) {
    this->setg(const_cast<char_type*>(begin_), const_cast<char_type*>(begin_),
               const_cast<char_type*>(end_));
}

membuf::pos_type membuf::seekoff(off_type off,
                                 std::ios_base::seekdir dir,
                                 std::ios_base::openmode which = std::ios_base::in) {
    if (dir == std::ios_base::cur)
        gbump(off);
    else if (dir == std::ios_base::end)
        setg(eback(), egptr() + off, egptr());
    else if (dir == std::ios_base::beg)
        setg(eback(), eback() + off, egptr());
    return gptr() - eback();
}

membuf::pos_type membuf::seekpos(membuf::pos_type sp_, std::ios_base::openmode which_)
{
    return seekoff(off_type(sp_), std::ios_base::beg, which_);
}

membuf::int_type membuf::underflow() {
    if (current_ == end_) {
        return traits_type::eof();
    }
    return traits_type::to_int_type(*current_);     // HERE!
}

membuf::int_type membuf::uflow() {
    if (current_ == end_) {
        return traits_type::eof();
    }
    return traits_type::to_int_type(*current_++);   // HERE!
}

membuf::int_type membuf::pbackfail(int_type ch) {
    if (current_ == begin_ || (ch != traits_type::eof() && ch != current_[-1])) {
        return traits_type::eof();
    }
    return traits_type::to_int_type(*--current_);   // HERE!
}

std::streamsize membuf::showmanyc() {
    return end_ - current_;
}
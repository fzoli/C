/* 
 * File:   seqbuf.h
 * Author: zoli
 *
 * Created on 2013. szeptember 4., 13:42
 */

#ifndef SEQBUF_H
#define	SEQBUF_H

#include <streambuf>
#include <vector>

template<typename Ch, typename Traits = std::char_traits<Ch>,
         typename Sequence = std::vector<Ch> >
struct basic_seqbuf : std::basic_streambuf<Ch, Traits> {
     typedef std::basic_streambuf<Ch, Traits> base_type;
     typedef typename base_type::int_type int_type;
     typedef typename base_type::traits_type traits_type;

     virtual int_type overflow(int_type ch) {
         if(traits_type::eq_int_type(ch, traits_type::eof()))
             return traits_type::eof();
         c.push_back(traits_type::to_char_type(ch));
         return ch;
     }

    Sequence const& get_sequence() const {
        return c;
    }
    
protected:
    Sequence c;
};

// convenient typedefs
typedef basic_seqbuf<char> seqbuf;
typedef basic_seqbuf<wchar_t> wseqbuf;

#endif	/* SEQBUF_H */


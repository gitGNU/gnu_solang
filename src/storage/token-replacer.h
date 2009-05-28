/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 *
 * Solang is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solang is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_TOKEN_REPLACER_H
#define SOLANG_TOKEN_REPLACER_H

namespace Solang
{

template<size_t numEntries_>
class TokenReplacer
{
    public:
        struct ReplacementEntry
        {
            Glib::ustring token_;
            Glib::ustring replacement_;
        };
    public:

        TokenReplacer(
            const ReplacementEntry (&replacementEntries)[numEntries_]);

        inline Glib::ustring
        extract( const Glib::ustring &str );

    private:
        inline void
        replaceString( Glib::ustring &str,
                        const Glib::ustring &token,
                        const Glib::ustring &replacement,
                        Glib::ustring::size_type pos = 0);

        const ReplacementEntry  (&replacementEntries_)[ numEntries_ ];

};

template<size_t numEntries_>
TokenReplacer<numEntries_>::TokenReplacer(
            const ReplacementEntry (&replacementEntries)[numEntries_])
    :replacementEntries_( replacementEntries )
{
}

template<size_t numEntries_>
inline Glib::ustring
TokenReplacer<numEntries_>::extract( const Glib::ustring &str )
{
    Glib::ustring tmp = str;
    for( size_t i = 0; i < numEntries_; i++ )
    {
        const ReplacementEntry &entry = replacementEntries_[ i ];
        replaceString( tmp, entry.token_, entry.replacement_ );
    }
    return tmp;
}

template<size_t numEntries_>
inline void
TokenReplacer<numEntries_>::replaceString( Glib::ustring &str,
                const Glib::ustring &token,
                const Glib::ustring &replacement,
                Glib::ustring::size_type pos)
{
    size_t spos = str.find(token, pos);
    if( spos != Glib::ustring::npos )
    {
        str.replace( spos, token.length(), replacement);
        replaceString( str, token, replacement, spos );
    }
    return;
}


} //  namespace Solang

#endif //SOLANG_TOKEN_REPLACER_H

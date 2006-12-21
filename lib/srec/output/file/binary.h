//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998, 1999, 2001, 2002, 2005, 2006 Peter Miller
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: interface definition for lib/srec/output/file/binary.cc
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_BINARY_H
#define INCLUDE_SREC_OUTPUT_FILE_BINARY_H


#include <lib/srec/output/file.h>

/**
  * The srec_output_file_binary class is used to represetn an output
  * file in raw binary format.
  */
class srec_output_file_binary:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_binary();

    /**
      * The defualrt constructor.
      * Ouput will be written to the standard output.
      */
    srec_output_file_binary();

    /**
      * The constructor.
      *
      * @param filename
      *     The file name to open and write output to.
      */
    srec_output_file_binary(const char *filename);

    // See base class for documentation.
    virtual void write(const srec_record &);

    // See base class for documentation.
    virtual void line_length_set(int);

    // See base class for documentation.
    virtual void address_length_set(int);

    // See base class for documentation.
    virtual int preferred_block_size_get() const;

private:
    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_binary(const srec_output_file_binary &);

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_binary &operator=(const srec_output_file_binary &);

    // See base class for documentation.
    const char *mode() const;
};

#endif // INCLUDE_SREC_OUTPUT_FILE_BINARY_H
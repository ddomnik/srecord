/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998 Peter Miller;
 *	All rights reserved.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * MANIFEST: functions to impliment the srec_input_filter_minimum class
 */

#pragma implementation

#include <srec/input/filter/minimum.h>
#include <srec/record.h>


srec_input_filter_minimum::srec_input_filter_minimum()
	: srec_input_filter(), minimum_address(0), minimum_length(0),
		minimum_order(0), minimum(0), minimum_set(false), data(0)
{
}


srec_input_filter_minimum::srec_input_filter_minimum(srec_input *a1, int a2,
		int a3, int a4)
	: srec_input_filter(a1), minimum_address(a2), minimum_length(a3),
		minimum_order(a4), minimum(0), minimum_set(false), data(0)
{
	if (minimum_length < 0)
		minimum_length = 0;
	else if (minimum_length > 8)
		minimum_length = 8;
	minimum = minimum_address;
	minimum_set = true;
}


srec_input_filter_minimum::srec_input_filter_minimum(
		const srec_input_filter_minimum &arg)
	: srec_input_filter(arg), minimum_address(arg.minimum_address),
		minimum_length(arg.minimum_length),
		minimum_order(arg.minimum_order), minimum(0),
		minimum_set(false), data(0)
{
	if (minimum_length < 0)
		minimum_length = 0;
	else if (minimum_length > 8)
		minimum_length = 8;
	minimum = minimum_address;
	minimum_set = true;
}


srec_input_filter_minimum &
srec_input_filter_minimum::operator=(const srec_input_filter_minimum &arg)
{
	srec_input_filter::operator=(arg);
	minimum_address = arg.minimum_address;
	minimum_length = arg.minimum_length;
	minimum_order = arg.minimum_order;

	if (minimum_length < 0)
		minimum_length = 0;
	else if (minimum_length > 8)
		minimum_length = 8;
	minimum = minimum_address;
	minimum_set = true;
	return *this;
}


srec_input_filter_minimum::~srec_input_filter_minimum()
{
}


int
srec_input_filter_minimum::read(srec_record &record)
{
	if (data)
	{
		record = *data;
		delete data;
		data = 0;
	}
	else if (!srec_input_filter::read(record))
	{
		if (minimum_length > 0)
			goto generate;
		return 0;
	}
	switch (record.get_type())
	{
	default:
		break;

	case srec_record::type_data:
		if (!minimum_set || record.get_address() < minimum)
		{
			minimum = record.get_address();
			minimum_set = true;
		}
		break;

	case srec_record::type_termination:
		if (minimum_length <= 0)
			break;
		data = new srec_record(record);
		generate:
		if (minimum_length > 8)
			minimum_length = 8;
		unsigned char chunk[8];
		if (minimum_order)
			srec_record::encode_little_endian(chunk, minimum, minimum_length);
		else
			srec_record::encode_big_endian(chunk, minimum, minimum_length);
		record =
			srec_record
			(
				srec_record::type_data,
				minimum_address,
				chunk,
				minimum_length
			);
		minimum_length = 0;
		break;
	}
	return 1;
}
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
 * MANIFEST: functions to impliment the srec_input_filter_maximum class
 */

#pragma implementation

#include <interval.h>
#include <srec/input/filter/maximum.h>
#include <srec/record.h>


srec_input_filter_maximum::srec_input_filter_maximum()
	: srec_input_filter(), maximum_address(0), maximum_length(0),
		maximum_order(0), maximum(0), maximum_set(false), data(0)
{
}


srec_input_filter_maximum::srec_input_filter_maximum(srec_input *a1, int a2,
		int a3, int a4)
	: srec_input_filter(a1), maximum_address(a2), maximum_length(a3),
		maximum_order(a4), maximum(0), maximum_set(false), data(0)
{
	if (maximum_length < 0)
		maximum_length = 0;
	else if (maximum_length > 8)
		maximum_length = 8;
	maximum = maximum_address + maximum_length;
	maximum_set = true;
}


srec_input_filter_maximum::srec_input_filter_maximum(
		const srec_input_filter_maximum &arg)
	: srec_input_filter(arg), maximum_address(arg.maximum_address),
		maximum_length(arg.maximum_length),
		maximum_order(arg.maximum_order), maximum(0),
		maximum_set(false), data(0)
{
	if (maximum_length < 0)
		maximum_length = 0;
	else if (maximum_length > 8)
		maximum_length = 8;
	maximum = maximum_address + maximum_length;
	maximum_set = true;
}


srec_input_filter_maximum &
srec_input_filter_maximum::operator=(const srec_input_filter_maximum &arg)
{
	srec_input_filter::operator=(arg);
	maximum_address = arg.maximum_address;
	maximum_length = arg.maximum_length;
	maximum_order = arg.maximum_order;

	if (maximum_length < 0)
		maximum_length = 0;
	else if (maximum_length > 8)
		maximum_length = 8;
	maximum = maximum_address + maximum_length;
	maximum_set = true;
	return *this;
}


srec_input_filter_maximum::~srec_input_filter_maximum()
{
}


int
srec_input_filter_maximum::read(srec_record &record)
{
	if (data)
	{
		record = *data;
		delete data;
		data = 0;
	}
	else if (!srec_input_filter::read(record))
	{
		if (maximum_length > 0)
			goto generate;
		return 0;
	}
	switch (record.get_type())
	{
	default:
		break;

	case srec_record::type_data:
		if (!maximum_set || maximum < record.get_address_end())
		{
			maximum = record.get_address_end();
			maximum_set = true;
		}
		break;

	case srec_record::type_termination:
		if (maximum_length <= 0)
			break;
		data = new srec_record(record);
		generate:
		if (maximum_length > 8)
			maximum_length = 8;
		unsigned char chunk[8];
		if (maximum_order)
			srec_record::encode_little_endian(chunk, maximum, maximum_length);
		else
			srec_record::encode_big_endian(chunk, maximum, maximum_length);
		record =
			srec_record
			(
				srec_record::type_data,
				maximum_address,
				chunk,
				maximum_length
			);
		maximum_length = 0;
		break;
	}
	return 1;
}

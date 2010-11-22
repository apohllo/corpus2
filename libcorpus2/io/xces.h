/*
    Copyright (C) 2010 Tomasz Śniatowski, Adam Radziszewski
    Part of the libcorpus2 project

    This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.

    This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. 

    See the LICENSE and COPYING files for more details.
*/

#ifndef LIBCORPUS2_IO_XCES_H
#define LIBCORPUS2_IO_XCES_H

#include <libcorpus2/tagset.h>
#include <libcorpus2/tokensource.h>

namespace Corpus2 {

class XcesError : public Corpus2Error
{
public:
	/// Constructor
	XcesError(const std::string &what);

	/// Destructor
	~XcesError() throw();
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_XCES_H

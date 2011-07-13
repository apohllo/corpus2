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

#ifndef LIBCORPUS2_TAGGING_H
#define LIBCORPUS2_TAGGING_H

#include <libcorpus2/tag.h>
#include <libcorpus2/tagset.h>

namespace Corpus2 {

/**
  * Creates a mask having all values of the given attribute set.
  * Pass an empty string to get a mask for all POS values.
  * The resulting object should only be used for masking as it won't be
  * a valid tag.
  *
  * @arg tagset_name tagset to use
  * @arg attr_name attrinbute as defined in tagset or empty string for POS
  * @return mask for given attribute
  */
Tag get_attribute_mask(const Tagset& tagset,
					   const std::string attr_name);

} /* end ns Corpus2 */

#endif // LIBCORPUS2_TAGGING_H

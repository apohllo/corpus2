#ifndef LIBCORPUS2_TAG_H
#define LIBCORPUS2_TAG_H

#include <string>
#include <vector>
#include <cassert>
#include <boost/cstdint.hpp>
#include <boost/strong_typedef.hpp>
#include <boost/operators.hpp>

namespace Corpus2 {

class Tagset;

/// Typedefs for the string -> index mappings
typedef boost::int8_t idx_t;
typedef boost::uint64_t mask_t;
BOOST_STRONG_TYPEDEF(boost::uint32_t, tagset_idx_t);

/**
 * This is an extended version of a POS tag, which also includes a number
 * of attributes which may have values set.
 *
 * The tag is stored in a binary firmat, with POS names etc. mapped to
 * numbers. A Tagset object is needed to create tags or print their string
 * representation.
 *
 * While not strictly necessary, the tagset ID of a tag is stored within.
 * This allows more sanity checking, esp. during tagset conversion.
 */
class Tag
//	: boost::equality_comparable<Tag>, boost::less_than_comparable<Tag>
{
public:
	/// Empty tag constructor
	Tag()
		: pos_(0), values_(0)
	{
	}

	/// Tagset-and-POS (no values) constructor
	explicit Tag(mask_t pos)
		: pos_(pos), values_(0)
	{
	}

	/// Tagset-POS-values constructor
	Tag(mask_t pos, mask_t values)
		: pos_(pos), values_(values)
	{
	}

	bool is_null() const {
		return pos_ == 0 && values_ == 0;
	}

	int pos_count() const;

	int get_pos_index() const;

	/// POS (part-of-speech) accesor
	mask_t get_pos() const {
		return pos_;
	}

	/// POS setter
	void set_pos(mask_t v) {
		pos_ = v;
	}

	void add_pos(mask_t v) {
		pos_ |= v;
	}

	/// values accesor
	mask_t get_values() const {
		return values_;
	}

	mask_t get_values_for(mask_t mask) const {
		return values_ & mask;
	}

	/// values accesor -- nonconst reference
	void set_values(mask_t v) {
		values_ = v;
	}

	void add_values(mask_t v) {
		values_ |= v;
	}

	void add_values_masked(mask_t value, mask_t mask)
	{
		assert(mask & value);
		values_ = (values_ & ~mask) | value;
	}

	Tag& combine_with(const Tag& other) {
		pos_ |= other.pos_;
		values_ |= other.values_;
		return *this;
	}

	Tag get_combined(const Tag& other) const {
		Tag t(*this);
		return t.combine_with(other);
	}

	Tag& mask_with(const Tag& other) {
		pos_ &= other.pos_;
		values_ &= other.values_;
		return *this;
	}

	Tag get_masked(const Tag& other) const {
		Tag t(*this);
		return t.get_masked(other);
	}

	/// debug aid, dump the tag's internal numeric representation
	std::string raw_dump() const;

	/**
	 * Tag comparison.
	 */
	bool operator<(const Tag& other) const;

	/**
	 * Tag equality. Non-equality is provided by Boost template magic.
	 */
	bool operator==(const Tag& other) const;

private:
	/// the POS id
	mask_t pos_;

	/// the values
	mask_t values_;

};

size_t hash_value(const Tag &tag);

} /* end ns Corpus2 */


#endif // LIBCORPUS2_TAG_H

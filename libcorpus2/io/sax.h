#ifndef LIBCORPUS2_IO_SAX_H
#define LIBCORPUS2_IO_SAX_H

#include <libcorpus2/io/xcescommon.h>
#include <libcorpus2/util/tokentimer.h>
#include <libpwrutils/foreach.h>
#include <libxml++/parsers/saxparser.h>
#include <vector>

namespace Corpus2 {

/**
 * Sax parser base class, templated on the character buffer class, which
 * may be:
 * * a stringstream,
 * * or a std::string,
 * * or a std::vector<char>.
 */
template<typename BufT>
class BasicSaxParserT : public xmlpp::SaxParser
{
public:
	BasicSaxParserT()
		: xmlpp::SaxParser(), grab_characters_(false), buf_()
	{
	}

protected:

	void on_characters(const Glib::ustring &text)
	{
		if (grab_characters_) {
			buf_ += text.raw();
		}
	}
	void on_error(const Glib::ustring &text) {
		std::cerr << "XML Error: " << (std::string)text << "\n";
	}

	void on_fatal_error(const Glib::ustring &text) {
		std::cerr << "XML Fatal Error: " << (std::string)text << "\n";
	}

	void clear_buf() {
		buf_.clear();
	}

	std::string get_buf() {
		return buf_;
	}

	bool grab_characters_;

	BufT buf_;
};

template<> inline
void BasicSaxParserT< std::stringstream >::on_characters(const Glib::ustring &text)
{
	buf_ << text.raw();
}

template<> inline
void BasicSaxParserT< std::stringstream >::clear_buf()
{
	buf_.str("");
	buf_.clear();
}

template<> inline
std::string BasicSaxParserT< std::stringstream >::get_buf()
{
	return buf_.str();
}

template<> inline
void BasicSaxParserT< std::vector<char> >::on_characters(const Glib::ustring &text)
{
	//size_t bsize = buf_.size();
	//size_t need = bsize + text.size();
	//buf_.resize(need);
	std::copy(text.raw().begin(), text.raw().end(), std::back_inserter(buf_));
	//memcpy(&buf_[bsize], text.raw().c_str(), text.size());
}

template<> inline
std::string BasicSaxParserT< std::vector<char> >::get_buf()
{
	return std::string(&buf_[0], buf_.size());
}

typedef BasicSaxParserT< std::string > BasicSaxParser;

} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_SAX_H

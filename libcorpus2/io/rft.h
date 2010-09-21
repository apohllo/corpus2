#ifndef LIBCORPUS2_IO_RFT_H
#define LIBCORPUS2_IO_RFT_H

#include <libcorpus2/io/reader.h>
#include <libcorpus2/io/writer.h>

namespace Corpus2 {

/**
 * Simple writer class to output token in RFTagger-compatible corpora form.
 *
 * One token per line, token line consists of the orth, followed by the
 * tag, followed by newline (one tag per token only). Each sentence
 * is followed by a blank line.
 *
 * The first lexeme is used. No-lexeme tokens trigger a warning unless
 * nowarn is passed.
 */
class RftWriter : public TokenWriter
{
public:
	RftWriter(std::ostream& os, const Tagset& tagset,
			const string_range_vector& params);

	void write_token(const Token& t);

	void write_sentence(const Sentence& s);

	void write_chunk(const Chunk &p);

	static bool registered;

private:
	bool warn_on_no_lexemes_;
};

class RftReader : public BufferedSentenceReader
{
public:
	RftReader(const Tagset& tagset, std::istream& is, bool disamb);

	std::istream& is() {
		return is_;
	}

protected:
	/// BufferedSentenceReader override
	Sentence* actual_next_sentence();

	std::istream& is_;

	bool disamb_;

};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_IO_RFT_H

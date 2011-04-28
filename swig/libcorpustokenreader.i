#ifndef SWIG_LIBCORPUS2_TOKENREADER_I
#define SWIG_LIBCORPUS2_TOKENREADER_I

%module libcorpustokenreader
%{
  #include <libcorpus2/io/reader.h>
%}

%include "libcorpustag.i"
%include "libcorpustoken.i"
%include "libcorpuschunk.i"
%include "libcorpustagset.i"
%include "libcorpussentence.i"
%include "std_string.i"
%include "std_vector.i"
%include "boost_shared_ptr.i"

%nodefaultctor Corpus2::TokenReader;
%template(TokenReaderPtr) boost::shared_ptr<Corpus2::TokenReader>;
// %template(StdStringVector) std::vector<std::string>;
// %template(ChunkPtr) boost::shared_ptr<Corpus2::Chunk>;

namespace Corpus2 {
  class TokenReader {
  public:
    typedef boost::shared_ptr<TokenReader> TokenReaderPtr;

    /* --------------------------------------------------------------------- */
    explicit TokenReader(const Tagset& tagset);
    virtual ~TokenReader();
    
    /* --------------------------------------------------------------------- */
    static TokenReaderPtr create_path_reader(
      const std::string& class_id,
      const Tagset& tagset,
      const std::string& path);

    static TokenReaderPtr create_stream_reader(
      const std::string& class_id,
      const Tagset& tagset,
      std::istream& stream);

    /* --------------------------------------------------------------------- */
    virtual Token* get_next_token() = 0;
    virtual Sentence::Ptr get_next_sentence() = 0;
    virtual boost::shared_ptr<Chunk> get_next_chunk() = 0;

    /* --------------------------------------------------------------------- */
    virtual void set_option(const std::string& option);
    virtual std::string get_option(const std::string& option) const;

    /* --------------------------------------------------------------------- */
    const Tagset& tagset() const;
    static std::vector<std::string> available_reader_types();

    /* --------------------------------------------------------------------- */
    static std::string reader_help(const std::string& class_id);
    static std::vector<std::string> available_reader_types_help();
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_TOKENREADER_I */
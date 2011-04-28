#ifndef SWIG_LIBCORPUS2_SENTENCE_I
#define SWIG_LIBCORPUS2_SENTENCE_I

%module libcorpussentence
%{
  #include <libcorpus2/sentence.h>
%}

%include "std_string.i"
%include "libcorpustoken.i"
%include "boost_shared_ptr.i"

%template(Ptr) boost::shared_ptr<Corpus2::Sentence>;
%template(ConstPtr) boost::shared_ptr<const Corpus2::Sentence>;
%template(TokenPtrVector) std::vector<Corpus2::Token*>;

namespace Corpus2 {
  class Sentence {
  public:
    typedef boost::shared_ptr<Sentence> Ptr;
    typedef boost::shared_ptr<const Sentence> ConstPtr;
    
    /* --------------------------------------------------------------------- */

    Sentence();
    ~Sentence();

    /* --------------------------------------------------------------------- */

    virtual Ptr clone_shared() const;
    void release_tokens();
    bool empty() const;
    size_t size() const;
    
    %rename(GetToken) operator[];
    Token* operator[](size_t idx);

    %rename(GetConstToken) operator[];
    const Token* operator[](size_t idx) const;

    const std::vector<Token*>& tokens() const;
    std::vector<Token*>& tokens();

    virtual void append(Token* t);
    const Token* first_token() const;
  };
}

using namespace std;
using namespace Corpus2;

#endif /* SWIG_LIBCORPUS2_SENTENCE_I */
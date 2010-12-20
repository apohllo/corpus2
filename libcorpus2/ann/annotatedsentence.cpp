#include <libcorpus2/ann/annotatedsentence.h>
#include <libcorpus2/ann/view.h>
#include <boost/make_shared.hpp>

namespace Corpus2 {

AnnotatedSentence::AnnotatedSentence()
	: Sentence(), channels_()
{
}

AnnotatedSentence::~AnnotatedSentence()
{
}

Sentence::Ptr AnnotatedSentence::clone_shared() const
{
	boost::shared_ptr<AnnotatedSentence> copy;
	copy = boost::make_shared<AnnotatedSentence>();
	foreach (const Token* t, tokens_) {
		copy->append(t->clone());
	}
	copy->channels_ = channels_;
	return copy;
}

void AnnotatedSentence::create_channel(const std::string& name)
{
	channels_.insert(std::make_pair(name, AnnotationChannel(tokens_.size())));
}


boost::shared_ptr<AnnotationView> create_view(
	const boost::shared_ptr<AnnotatedSentence>& s,
	const std::string& ann_name)
{
	const AnnotationChannel& chan = s->get_channel(ann_name);
	std::vector<Annotation> ann = chan.make_annotation_vector();
	boost::shared_ptr<AnnotationView> view;
	view = boost::make_shared<AnnotationView>(s, ann_name);
	foreach (const Annotation& a, ann) {
		UnicodeString orth;
		orth = s->tokens()[a.indices[0]]->orth();
		for (size_t idxi = 1; idxi < a.indices.size(); ++idxi) {
			int idx = a.indices[idxi];
			orth += PwrNlp::Whitespace::to_whitespace(s->tokens()[idx]->wa());
			orth += s->tokens()[idx]->orth();
		}
		Token* t = new Token(orth, s->tokens()[a.indices[0]]->wa());
		Token* head_token = s->tokens()[a.head_index];
		std::copy(head_token->lexemes().begin(), head_token->lexemes().end(),
			std::back_inserter(t->lexemes()));
		view->append(t);
	}
	return view;
}

} /* end ns Corpus2 */
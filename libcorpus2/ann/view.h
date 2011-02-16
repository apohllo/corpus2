#ifndef LIBCORPUS2_ANN_VIEW_H
#define LIBCORPUS2_ANN_VIEW_H

#include <libcorpus2/sentence.h>

namespace Corpus2 {

class AnnotatedSentence;

/**
 * Exception class for signalling OOS between a View and a Sentence during
 * e.g. commits
 */
class AnnotationViewOutOfSync : public Corpus2Error
{
public:
	AnnotationViewOutOfSync(const std::string& reason)
		: Corpus2Error("AnnotationView out of sync with base sentence: " + reason)
	{
	}

	~AnnotationViewOutOfSync() throw()
	{
	}
};

/**
 * A class representing an AnnotatedSentence viewed through the segmentation
 * some Annotation specifies.
 *
 * It has its own Tokens but can update the original Sentence via commit().
 */
class AnnotationView : public Sentence
{
public:
	/**
	 * Create an AnnotationView from a sentence and an annotation name.
	 * The AnnotationView keeps a handle to the sentence.
	 *
	 * This shoudl not be used directly, use create_view.
	 */
	AnnotationView(const boost::shared_ptr<AnnotatedSentence>& original,
		const std::string& ann_name_);

	~AnnotationView();

	Ptr clone_shared() const;

	/**
	 * Push changes to the original Sentence.
	 */
	void commit();

	/**
	 * Push changes to a different Sentence, but same channel
	 */
	void commit_to(const boost::shared_ptr<AnnotatedSentence>& original);

	/**
	 * Push changes to ana arbitary channel in an arbitrary sentence
	 */
	void commit_to(const boost::shared_ptr<AnnotatedSentence>& original,
		const std::string& ann_name);

	/**
	 * Release the original Sentence. Use this when you no longer expect
	 * to need to go back or commit() changes.
	 */
	void release_original();
private:
	boost::shared_ptr<AnnotatedSentence> original_;
	const std::string ann_name_;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_ANN_VIEW_H

#include <boost/test/unit_test.hpp>
#include <set>
#include <libpwrutils/foreach.h>
#include <libcorpus2/tagset.h>
#include <libcorpus2/token.h>

BOOST_AUTO_TEST_SUITE( tag_split )

struct F {
	F() {
		const char tagset_string[] = "[ATTR]\n"
			"A tag tog other a3 \n"
			"B data thing tag-thing thang\n"
			"C a b c \n"
			"[POS]\n some A B [C]\n same A B\n";
		tagset.reset(new Corpus2::Tagset(tagset_string));
	}
	boost::shared_ptr<Corpus2::Tagset> tagset;

	std::vector<Corpus2::Tag> check_split(const std::string s, const std::set<std::string> expect)
	{
		std::set<std::string> actual;
		std::vector<Corpus2::Tag> tags;
		Corpus2::Token t;
		tagset->lexemes_into_token(t, UnicodeString(), s);
		foreach (const Corpus2::Lexeme& lex, t.lexemes()) {
			const Corpus2::Tag& tag = lex.tag();
			BOOST_WARN(tagset->validate_tag(tag, false));
			actual.insert(tagset->tag_to_string(tag));
			tags.push_back(tag);
		}
		BOOST_CHECK_EQUAL_COLLECTIONS(actual.begin(), actual.end(), expect.begin(), expect.end());
		return tags;
	}
};


BOOST_FIXTURE_TEST_CASE( plain, F )
{
	const char tag[] = "some:tag:data";
	std::set<std::string> r;
	r.insert(tag);
	check_split(tag, r);
}

BOOST_FIXTURE_TEST_CASE( plus, F )
{
	const char tag[] = "some:tag:data+some:other:tag-thing";
	std::set<std::string> result;
	result.insert("some:tag:data");
	result.insert("some:other:tag-thing");
	check_split(tag, result);
}

BOOST_FIXTURE_TEST_CASE( dot, F )
{
	const char tag[] = "some:tag.tog:data";
	std::set<std::string> result;
	result.insert("some:tag:data");
	result.insert("some:tog:data");
	check_split(tag, result);
}

BOOST_FIXTURE_TEST_CASE( dots, F )
{
	const char tag[] = "some:tag.tog:data:a.b.c";
	std::set<std::string> result;
	result.insert("some:tag:data:a");
	result.insert("some:tog:data:a");
	result.insert("some:tag:data:b");
	result.insert("some:tog:data:b");
	result.insert("some:tag:data:c");
	result.insert("some:tog:data:c");
	check_split(tag, result);
}

BOOST_FIXTURE_TEST_CASE( dots_plus, F )
{
	const char tag[] = "some:tag.tog:data:a.b+some:other:thing.thang";
	std::set<std::string> result;
	result.insert("some:tag:data:a");
	result.insert("some:tog:data:a");
	result.insert("some:tag:data:b");
	result.insert("some:tog:data:b");
	result.insert("some:other:thing");
	result.insert("some:other:thang");
	check_split(tag, result);
}

BOOST_FIXTURE_TEST_CASE( missing, F )
{
	const char tag[] = "some:data";
	std::set<std::string> r;
	r.insert("some::data");
	check_split(tag, r);
}

BOOST_FIXTURE_TEST_CASE( bad_value, F )
{
	const char tag[] = "some:bad:data";
	std::set<std::string> r;
	BOOST_CHECK_THROW(
			check_split(tag, r), Corpus2::TagParseError
	);
}

BOOST_FIXTURE_TEST_CASE( bad_pos, F )
{
	const char tag[] = "something:data";
	std::set<std::string> r;
	BOOST_CHECK_THROW(
			check_split(tag, r), Corpus2::TagParseError
	);
}

BOOST_FIXTURE_TEST_CASE( underscore, F )
{
	const char tag[] = "some:_:data";
	std::set<std::string> r;
	r.insert("some:tag:data");
	r.insert("some:tog:data");
	r.insert("some:other:data");
	r.insert("some:a3:data");
	check_split(tag, r);
}

BOOST_FIXTURE_TEST_CASE( underscores, F )
{
	const char tag[] = "some:_:data:_";
	std::set<std::string> r0;
	r0.insert("some:tag:data");
	r0.insert("some:tog:data");
	r0.insert("some:other:data");
	r0.insert("some:a3:data");
	std::set<std::string> r;
	foreach (const std::string& s, r0) {
		r.insert(s + ":a");
		r.insert(s + ":b");
		r.insert(s + ":c");
	}

	check_split(tag, r);
}

BOOST_FIXTURE_TEST_CASE( underscore_dots, F )
{
	const char tag[] = "some:_:data:c.a";
	std::set<std::string> r0;
	r0.insert("some:tag:data");
	r0.insert("some:tog:data");
	r0.insert("some:other:data");
	r0.insert("some:a3:data");
	std::set<std::string> r;
	foreach (const std::string& s, r0) {
		r.insert(s + ":a");
		r.insert(s + ":c");
	}

	check_split(tag, r);
}


BOOST_FIXTURE_TEST_CASE( tag_size, F )
{
	Corpus2::Tag t = tagset->parse_simple_tag("some:tag:data", false);
	Corpus2::Tag t2 = tagset->parse_simple_tag("some:tog", false);
	Corpus2::Tag t3 = tagset->parse_simple_tag("same", false);
	BOOST_CHECK(tagset->tag_is_singular(t));
	BOOST_CHECK_EQUAL(tagset->tag_size(t), 1);
	BOOST_CHECK(tagset->tag_is_singular(t2));
	BOOST_CHECK_EQUAL(tagset->tag_size(t2), 1);
	BOOST_CHECK(tagset->tag_is_singular(t3));
	BOOST_CHECK_EQUAL(tagset->tag_size(t3), 1);
	t.add_values(t2.get_values());
	BOOST_CHECK(!tagset->tag_is_singular(t));
	BOOST_CHECK_EQUAL(tagset->tag_size(t), 2);
	t.add_pos(t3.get_pos());
	BOOST_CHECK(!tagset->tag_is_singular(t));
	BOOST_CHECK_EQUAL(tagset->tag_size(t), 4);
	Corpus2::Tag t4 = tagset->parse_simple_tag("same:other:thang", true);
	t.add_values(t4.get_values() & tagset->get_attribute_mask(std::string("A")));
	BOOST_CHECK_EQUAL(tagset->tag_size(t), 6);
}

BOOST_AUTO_TEST_SUITE_END()

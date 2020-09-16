#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <future>
#include <future>
using namespace std;

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other){
	  for(auto & a:other.word_frequences)
		  if (word_frequences.count(a.first) == 0)
			  word_frequences[a.first] = a.second;
		  else
			  word_frequences[a.first] += a.second;
  }
};

Stats ExploreLine(const set<string>& key_words, const string& line){
	Stats result;

	string::size_type pos = 0;
	string::size_type prev = 0;
	while ((pos = line.find(' ', prev)) != std::string::npos)
	{
		string tmp = line.substr(prev, pos - prev);
		if (key_words.find(tmp)!=key_words.end())
			result.word_frequences[move(tmp)] +=1;
		prev = pos + 1;
	}
	string tmp = line.substr(prev);
	if (key_words.find(tmp)!=key_words.end())
		result.word_frequences[move(tmp)] +=1;
	return result;
}

Stats ExploreKeyWords(
  const set<string>& key_words, istream& input) {
	vector<future<Stats>> store;
	Stats result;
	for (string line; getline(input, line); )
	  store.push_back(async(ExploreLine, ref(key_words), line));
	for(auto & elem: store)
		result+=elem.get();
	return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}

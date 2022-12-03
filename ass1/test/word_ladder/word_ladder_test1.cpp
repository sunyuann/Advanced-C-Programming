//
//  Copyright UNSW Sydney School of Computer Science and Engineering
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include <comp6771/word_ladder.hpp>

#include <string>
#include <vector>

#include <catch2/catch.hpp>

// modified this test to use custom lexicon for correct testing
/*
- Since assumptions are given by spec, we can assume that this is the controlled environment the
word ladder operates in. Hence, we do not need to test for word ladder generator's behaviour when
assumptions are untrue.
- In the case where these assumptions aren't given, I have provided a way to test for these
assumptions, as shown in the comments below. Assumptions given by spec:
   - You can assume that the start word and end word have the same length (i.e. number of
characters).
      - no need to test for different start and end word lengths
      - can test by: aaa -> hansel
   - You can assume that both the start and the end word are in the lexicon.
      - no need to test for starting/ending word not being in lexicon
      - can test for starting by: baaa -> abba
      - can test for ending by:   aaaa -> bbba
   - You can assume the start word and the end word will not be the same word.
      - no need to test for starting/ending word being the same word
      - can test by: aaaa -> aaaa
*/

// functionality test case (test that the word ladder generator actually generates correct output)
TEST_CASE("functionality, correctness test (word to word, correct output)") {
	// no possible ladder output, only 6 letter words in lexicon are hansel and gretel.
	SECTION("testing no ladder output (impossible to form word ladder for given words "
	        "(hansel->gretel") {
		const auto english_lexicon = word_ladder::read_lexicon("../../test/word_ladder/"
		                                                       "english_test.txt");
		const auto ladders = word_ladder::generate("hansel", "gretel", english_lexicon);
		// no ladder returned (size = 0)
		CHECK(std::size(ladders) == 0);
		CHECK(std::is_sorted(ladders.begin(), ladders.end()));
	}

	// only 1 possible ladder output given to test basic word ladder works
	SECTION("testing singular ladder output (aaa->bbb)") {
		const auto english_lexicon = word_ladder::read_lexicon("../../test/word_ladder/"
		                                                       "english_test.txt");
		const auto ladders = word_ladder::generate("aaa", "bbb", english_lexicon);
		CHECK(std::size(ladders) == 1);
		CHECK(std::is_sorted(ladders.begin(), ladders.end()));
		CHECK(std::count(ladders.begin(),
		                 ladders.end(),
		                 std::vector<std::string>{"aaa", "aab", "abb", "bbb"})
		      == 1);
	}

	// 2 possible ladder outputs given to test multiple word ladders of same length works
	SECTION("testing multiple ladder outputs (aaaa->abba)") {
		const auto english_lexicon = word_ladder::read_lexicon("../../test/word_ladder/"
		                                                       "english_test.txt");
		const auto ladders = word_ladder::generate("aaaa", "abba", english_lexicon);
		CHECK(std::size(ladders) == 2);
		CHECK(std::is_sorted(ladders.begin(), ladders.end()));
		CHECK(
		   std::count(ladders.begin(), ladders.end(), std::vector<std::string>{"aaaa", "abaa", "abba"})
		   == 1);
		CHECK(
		   std::count(ladders.begin(), ladders.end(), std::vector<std::string>{"aaaa", "aaba", "abba"})
		   == 1);
	}

	// testing whether it takes the shortest word ladder only (potential longer paths will not be
	// returned) if shortest path not taken, potential longer paths e.g.:
	//     - aaaaa aaaab aaaac aaaad aaaae baaae
	//     - aaaaa aaaad aaaae baaae, etc
	// if shortest path taken: only:
	//     - aaaaa aaaae baaae
	SECTION("testing ladders of multiple lengths (aaaaa->baaae)") {
		const auto english_lexicon = word_ladder::read_lexicon("../../test/word_ladder/"
		                                                       "english_test.txt");
		const auto ladders = word_ladder::generate("aaaaa", "baaae", english_lexicon);
		CHECK(std::size(ladders) == 1);
		CHECK(std::is_sorted(ladders.begin(), ladders.end()));
		CHECK(std::count(ladders.begin(),
		                 ladders.end(),
		                 std::vector<std::string>{"aaaaa", "aaaae", "baaae"})
		      == 1);
	}

	// full functionality test
	// complex test case: multiple solution word ladder (given in example from spec)
	SECTION("testing complex case with 12 ladders and 7 length (work->play)") {
		const auto english_lexicon = word_ladder::read_lexicon("../../test/word_ladder/english.txt");
		const auto ladders = word_ladder::generate("work", "play", english_lexicon);
		CHECK(std::size(ladders) == 12);
		CHECK(std::is_sorted(ladders.begin(), ladders.end()));
		// check all 12 lines given in spec example
		std::vector<std::string> ladder01 = {"work", "fork", "form", "foam", "flam", "flay", "play"};
		std::vector<std::string> ladder02 = {"work", "pork", "perk", "peak", "pean", "plan", "play"};
		std::vector<std::string> ladder03 = {"work", "pork", "perk", "peak", "peat", "plat", "play"};
		std::vector<std::string> ladder04 = {"work", "pork", "perk", "pert", "peat", "plat", "play"};
		std::vector<std::string> ladder05 = {"work", "pork", "porn", "pirn", "pian", "plan", "play"};
		std::vector<std::string> ladder06 = {"work", "pork", "port", "pert", "peat", "plat", "play"};
		std::vector<std::string> ladder07 = {"work", "word", "wood", "pood", "plod", "ploy", "play"};
		std::vector<std::string> ladder08 = {"work", "worm", "form", "foam", "flam", "flay", "play"};
		std::vector<std::string> ladder09 = {"work", "worn", "porn", "pirn", "pian", "plan", "play"};
		std::vector<std::string> ladder10 = {"work", "wort", "bort", "boat", "blat", "plat", "play"};
		std::vector<std::string> ladder11 = {"work", "wort", "port", "pert", "peat", "plat", "play"};
		std::vector<std::string> ladder12 = {"work", "wort", "wert", "pert", "peat", "plat", "play"};
		CHECK(std::count(ladders.begin(), ladders.end(), ladder01) == 1);
		CHECK(std::count(ladders.begin(), ladders.end(), ladder02) == 1);
		CHECK(std::count(ladders.begin(), ladders.end(), ladder03) == 1);
		CHECK(std::count(ladders.begin(), ladders.end(), ladder04) == 1);
		CHECK(std::count(ladders.begin(), ladders.end(), ladder05) == 1);
		CHECK(std::count(ladders.begin(), ladders.end(), ladder06) == 1);
		CHECK(std::count(ladders.begin(), ladders.end(), ladder07) == 1);
		CHECK(std::count(ladders.begin(), ladders.end(), ladder08) == 1);
		CHECK(std::count(ladders.begin(), ladders.end(), ladder09) == 1);
		CHECK(std::count(ladders.begin(), ladders.end(), ladder10) == 1);
		CHECK(std::count(ladders.begin(), ladders.end(), ladder11) == 1);
		CHECK(std::count(ladders.begin(), ladders.end(), ladder12) == 1);
	}
}

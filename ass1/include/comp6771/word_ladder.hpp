// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#ifndef COMP6771_WORD_LADDER_HPP
#define COMP6771_WORD_LADDER_HPP

#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <iterator>
#include <string>
#include <vector>

namespace word_ladder {
	[[nodiscard]] auto read_lexicon(std::string const& path) -> std::unordered_set<std::string>;

	// Given a start word and destination word, returns all the shortest possible paths from the
	// start word to the destination, where each word in an individual path is a valid word per the
	// provided lexicon. Pre: ranges::size(from) == ranges::size(to) Pre: valid_words.contains(from)
	// and valid_words.contains(to)
	[[nodiscard]] auto generate(const std::string& from,
	                            const std::string& to,
	                            const std::unordered_set<std::string>& lexicon)
	   -> std::vector<std::vector<std::string>>;

	// helper function declarations
	auto
	singleLetterDiff(const std::string& word,
	                 const std::unordered_set<std::string>& lexicon,
	                 std::deque<std::string>& single_letter_diff_queue,
	                 std::unordered_map<std::string, std::vector<std::string>>& single_letter_diff_map);

	auto bfs(const std::string& src_word,
	         const std::string& dest_word,
	         const std::unordered_map<std::string, std::vector<std::string>>& single_letter_diff_map)
	   -> std::unordered_map<std::string, int>;

	auto dfs(const std::string& src_word,
	         const std::string& dest_word,
	         const std::unordered_map<std::string, std::vector<std::string>>& single_letter_diff_map,
	         const std::unordered_map<std::string, int>& num_hops,
	         std::vector<std::vector<std::string>>& paths,
	         std::vector<std::string> curr_path);
} // namespace word_ladder

#endif // COMP6771_WORD_LADDER_HPP

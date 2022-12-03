#include <comp6771/word_ladder.hpp>

namespace word_ladder {
	// takes in a given word and stores an array of words with a single letter difference compared
	// to the given word and is also present in the lexicon given.
	// e.g. cat (key) = [cot, oat, cap] (array of values) (just example)
	auto
	singleLetterDiff(const std::string& word,
	                 const std::unordered_set<std::string>& lexicon,
	                 std::deque<std::string>& single_letter_diff_queue,
	                 std::unordered_map<std::string, std::vector<std::string>>& single_letter_diff_map) {
		std::string temp = word;
		// temp letter diff words to keep track of words that are single letter diff from source word
		std::vector<std::string> temp_diff_words;
		// loop through each char of source word
		for (int i = 0; i < word.size(); ++i) {
			// loop through each alphabet per char
			for (int j = 0; j < 26; ++j) {
				// change alphabet in char position
				temp[i] = (char)(int('a') + j);
				// find single letter diff word in lexicon, if present
				auto iterated_word = lexicon.find(temp);
				if (word != temp && iterated_word != lexicon.end()) {
					// if word found and is not source word,
					// add word to temp_diff_words array (array of found words that differ by 1 char from
					// src word) and add to queue
					temp_diff_words.emplace_back(temp);
					single_letter_diff_queue.emplace_back(temp);
				}
			}
			// after each char position iteration loop, reset temp to source word for next iteration
			temp = word;
		}
		single_letter_diff_map[word] = temp_diff_words;
	}

	// bfs that takes the map containing key (word given) and values (single letter diff letters) of
	// the key then returns unordered map of number of hops from destination word, source word and
	// single letter diff letters to source word
	auto bfs(const std::string& src_word,
	         const std::string& dest_word,
	         const std::unordered_map<std::string, std::vector<std::string>>& single_letter_diff_map)
	   -> std::unordered_map<std::string, int> {
		// num_hops = unordered map that we return
		std::unordered_map<std::string, int> num_hops;
		// queue for bfs
		std::deque<std::string> queue;

		// start with source word in queue (num hops = 0)
		queue.emplace_front(src_word);
		num_hops[src_word] = 0;
		// bfs algorithm
		while (!queue.empty()) {
			// if dest word reached return num hops
			if (std::find(queue.begin(), queue.end(), dest_word) != queue.end()) {
				return num_hops;
			}
			// store word in front of queue then pop given word
			auto popped_word = queue.at(0);
			queue.pop_front();
			// get single letter diff list of popped word, if found then go through the list
			auto popped_word_sld_list = single_letter_diff_map.find(popped_word);
			if (popped_word_sld_list != single_letter_diff_map.end()) {
				// loop through single letter diff words in currently popped single letter diffs words
				// list
				// if word not present in num_hops map (haven't been found yet, add to queue
				// and add 1 from popped word's numhops to the word's numhops key
				for (const auto& single_letter_diff_word : popped_word_sld_list->second) {
					if (num_hops.find(single_letter_diff_word) == num_hops.end()) {
						queue.emplace_back(single_letter_diff_word);
						num_hops[single_letter_diff_word] = num_hops[popped_word] + 1;
					}
				}
			}
		}
		return num_hops;
	}

	// dfs from the num_hops map obtained from bfs function
	// returns vector of path of words from source word to destination word
	auto dfs(const std::string& src_word,
	         const std::string& dest_word,
	         const std::unordered_map<std::string, std::vector<std::string>>& single_letter_diff_map,
	         const std::unordered_map<std::string, int>& num_hops,
	         std::vector<std::vector<std::string>>& paths,
	         std::vector<std::string> curr_path) {
		curr_path.emplace_back(src_word);
		if (src_word == dest_word) {
			paths.emplace_back(curr_path);
			return;
		}
		auto single_letter_diff_list = single_letter_diff_map.find(src_word);
		if (single_letter_diff_list == single_letter_diff_map.end()) {
			return;
		}
		for (const auto& single_letter_diff_word : single_letter_diff_list->second) {
			if (num_hops.find(single_letter_diff_word) != num_hops.end()) {
				// nested if statement instead of using && in case of segmentation fault caused by
				// using ->second if .find is at the end
				if ((num_hops.find(single_letter_diff_word))->second
				    == ((num_hops.find(src_word))->second) + 1) {
					dfs(single_letter_diff_word, dest_word, single_letter_diff_map, num_hops, paths, curr_path);
				}
			}
		}
	}

	// main function, generates array of array of strings containing shortest path from "from" to
	// "to"
	[[nodiscard]] auto generate(const std::string& from,
	                            const std::string& to,
	                            const std::unordered_set<std::string>& lexicon)
	   -> std::vector<std::vector<std::string>> {
		std::vector<std::string> temp_words;
		temp_words.emplace_back(from);
		std::unordered_map<std::string, std::vector<std::string>> single_letter_diff_map = {};
		std::deque<std::string> single_letter_diff_queue;
		while (true) {
			single_letter_diff_queue.clear();
			for (const auto& single_letter_diff_word : temp_words) {
				if (single_letter_diff_map.find(single_letter_diff_word) == single_letter_diff_map.end())
				{
					singleLetterDiff(single_letter_diff_word,
					                 lexicon,
					                 single_letter_diff_queue,
					                 single_letter_diff_map);
				}
			}
			// clear temp_words, then copy single letter diff queue that returned from singleLetterDiff
			// into temp_words
			temp_words.clear();
			std::copy(single_letter_diff_queue.begin(),
			          single_letter_diff_queue.end(),
			          std::back_inserter(temp_words));
			// break loop when queue is empty
			if (single_letter_diff_queue.empty()) {
				break;
			}
		}
		// perform bfs to get number of hops from "from" to given word
		std::unordered_map<std::string, int> num_hops = bfs(from, to, single_letter_diff_map);
		// curr_path variable for recursion in dfs
		std::vector<std::string> curr_path;
		std::vector<std::vector<std::string>> paths;
		// use dfs algorithm to get paths from sing;e letter diff map, then sort paths and return
		dfs(from, to, single_letter_diff_map, num_hops, paths, curr_path);
		std::sort(paths.begin(), paths.end());
		return paths;
	}
} // namespace word_ladder
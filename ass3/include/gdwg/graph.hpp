#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <map>
#include <set>
#include <vector>
#include <ostream>

namespace gdwg {
	template<typename N, typename E>
	class graph {
	public:
		// iterator class declared
		class iterator;
		// value_type member type
		struct value_type {
			N from;
			N to;
			E weight;
		};

		// compares the start and ending of vertices and return boolean value
		// (sorts in order of starting vertex, then ending vertex)
		// returns true if edge1 is "more" than edge2 (edge1 > edge2)
		// (used as key to keep map of edges sorted)
		struct edges_verticescompare {
			auto operator()(std::pair<N*, N*> vertices_edge1, std::pair<N*, N*> vertices_edge2) -> bool {
				// (if starting vertices are equal (edge1->first = edge2->first),
				// return true if edge1's destination vertex is bigger than edge2's destination vertex
				if (*vertices_edge1->first == *vertices_edge2->first) {
					return *vertices_edge1->second < *vertices_edge2->second;
				}
				// if starting vertices are not equal,
				// return true if edge1's starting vertex is bigger than edge1's starting vertex
				return *vertices_edge1->first < *vertices_edge2->first;
			}
			auto operator()(std::pair<N*, N*> vertices_edge1, std::pair<N*, N*> vertices_edge2) const -> bool {
				// (if starting vertices are equal (edge1->first = edge2->first),
				// return true if edge1's destination vertex is bigger than edge2's destination vertex
				if (*vertices_edge1.first == *vertices_edge2.first) {
					return *vertices_edge1.second < *vertices_edge2.second;
				}
				// if starting vertices are not equal,
				// return true if edge1's starting vertex is bigger than edge1's starting vertex
				return *vertices_edge1.first < *vertices_edge2.first;
			}
		};

		/////// CONSTRUCTORS /////////

		// default constructor
		graph() noexcept {
			// value initialises all members (calls default constructor for nodes set and edges map)
			this->node_set_ = std::set<std::unique_ptr<N>>();
			// pass in edges_verticescompare as key to keep edge_map_ sorted (same for all other constructors)
			this->edge_map_ = std::map<std::pair<N*, N*>, std::set<std::unique_ptr<E>>, edges_verticescompare>();
		}

		// initializer list constructor
		graph(std::initializer_list<N> il) {
			// value initialises all members (calls default constructor for nodes set and edges map)
			this->node_set_ = std::set<std::unique_ptr<N>>();
			this->edge_map_ = std::map<std::pair<N*, N*>, std::set<std::unique_ptr<E>>, edges_verticescompare>();
			// loops through list and adds all nodes in list to set of nodes
			for (auto node = il.begin(); node != il.end(); node++) {
				this->insert_node(*node);
			}
		}

		// input iterator list constructor
		template<typename InputIt>
		graph(InputIt first, InputIt last) {
			// value initialises all members (calls default constructor for nodes set and edges map)
			this->node_set_ = std::set<std::unique_ptr<N>>();
			this->edge_map_ = std::map<std::pair<N*, N*>, std::set<std::unique_ptr<E>>, edges_verticescompare>();
			// loops through input iterator and adds all nodes in input iterator to set of nodes
			for (auto node = first; node != last; node++) {
				this->insert_node(*node);
			}
		}

		graph(graph&& other) noexcept {
			// moves set of nodes and map of edges from given graph into this graph,
			// then invalidate the elements of the given graph
			this->node_set_ = std::move(other.node_set_);
			this->edge_map_ = std::move(other.edge_map_);
		}

		auto operator=(graph&& other) noexcept -> graph& {
			// swap elements then clear other (other now points to elements owned by *this)
			std::swap(this->node_set_, other.node_set_);
			std::swap(this->edge_map_, other.edge_map_);
			other.clear();
			return *this;
		}

		// postcondition: *this == other is true
		graph(graph const& other) {
			this->node_set_ = std::set<std::unique_ptr<N>>();
			this->edge_map_ = std::map<std::pair<N*, N*>, std::set<std::unique_ptr<E>>, edges_verticescompare>();
			// loop through and insert nodes into current graph
			for (auto node: other.nodes()) {
				this->insert_node(node);
			}
			// loop through and insert edges into current graph
			for (auto [vertices, weights] : other.get_edges()) {
				for (auto weight: weights) {
					this->insert_edge(vertices.first, vertices.second, weight);
				}
			}
		}

		// invalidate all iterators pointing to elements of *this
		auto operator=(graph const& other) -> graph& {
			if (this == other) {
				return *this;
			}
			// assign, then swap
			auto other_graph = other;
			std::swap(this->node_set_, other_graph.node_set_);
			std::swap(this->edge_map_, other_graph.edge_map_);
			return *this;
		}

		/////// MODIFIERS ////////
		// insert_node
		auto insert_node(N const& value) -> bool {
			// check if value given already stored in graph (if so, return false)
			if (is_node(value)) {
				return false;
			}
			// if not, add node to graph and return true
			auto new_node = value;
			auto in_node = std::make_unique<N>(new_node);
			this->node_set_.insert(std::move(in_node));
			return true;
		}

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			// exception thrown if src or dst is not valid node
			if ((!is_node(src)) || (!is_node(dst))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not exist");
			}

			// obtain node pointer for src and dst
			auto src_ptr = this->get_graph_node_pointer(src);
			auto dst_ptr = this->get_graph_node_pointer(dst);
			// for pair of src to dst in edge_map, iterate through all edge weights
			for (auto& edges: edge_map_[std::make_pair(src_ptr, dst_ptr)]) {
				// returns false if src,dst,weight edge already exists (node not added to graph)
				if (weight == *edges.get()) {
					return false;
				}
			}

			// no edge equivalent to (src, dst, weight), so add to graph and return true
			auto new_edge = std::make_unique<E>(weight);
			// use std::move(new_edge) to indicate that new_edge can be moved from (more efficient)
			edge_map_[std::make_pair(src_ptr, dst_ptr)].insert(std::move(new_edge));
			return true;
		}

		auto replace_node(N const& old_data, N const& new_data) -> bool {
			// exception thrown if is_node(old_data) is false
			if (!is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");
			}

			if (!is_node(new_data)) {
				// if new_data does not exist, point old_data's pointer to new_data
				auto old_data_ptr = get_graph_node_pointer(old_data);
				*old_data_ptr = new_data;
				return true;
			}
			// returns false if new_data already exists
			return false;
		}

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			// exception thrown if old_data or new_data is not valid node
			if (!is_node(old_data) || !is_node(new_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
			}

			// vector of edge vertices that need to be replaced
			auto replacement_edges = std::vector<std::pair<N*, N*>>();
			// loop through edges in edge map and check if edges contain new_data node and add
			// them into replacement_edges array
			for (auto edge = edge_map_.begin(); edge != edge_map_.end(); edge++) {
				if (*(edge->first).first == new_data || *(edge->first).second == new_data) {
					replacement_edges.push_back(edge->first);
				}
			}

			// obtain node pointer for old_data and new_data
			auto old_data_ptr = get_graph_node_pointer(old_data);
			auto new_data_ptr = get_graph_node_pointer(new_data);
			// loop through all edges that contain node new_data and change new_data into old_data
			for (auto replacement_edge: replacement_edges) {
				// extract each edge vertices from edge_map, change new_data to old_data node
				auto changed_edge = edge_map_.extract(replacement_edge);
				if (changed_edge.key().first == new_data_ptr) {
					changed_edge.key().first = old_data_ptr;
				}
				if (changed_edge.key().second == new_data_ptr) {
					changed_edge.key().second = old_data_ptr;
				}
				// reinsert back into edge_map
				edge_map_.insert(std::move(changed_edge));
			}
			// delete new_data node
			erase_node(new_data);
			// point old data pointer to new data
			*old_data_ptr = new_data;
		}

		auto erase_node(N const& value) -> bool {
			// erase node and return true if value already exists in graph
			if (is_node(value)) {
				// loop through all edges in edge map and erase edges that have either:
				// src = value, or dest = value
				for (auto edges = edge_map_.begin(); edges != edge_map_.end();) {
					if ((*(edges->first).first == value) || (*(edges->first).second == value)) {
						edges = edge_map_.erase(edges);
					} else {
						edges++;
					}
				}
				// loop through all nodes in node set and erase nodes that are equal to value
				for (auto node = node_set_.begin(); node != node_set_.end();) {
					if (**node == value) {
						node = node_set_.erase(node);
					} else {
						node++;
					}
				}
				// returns true when all instances of value node has been removed from graph
				return true;
			}
			// returns false if value is not a node
			return false;
		}

		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			// throws error if src and dst nodes do not exist in graph
			if ((!is_node(src)) || (!is_node(dst))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
			}
			// returns false if no edges from src to dst exist in graph, no edge removed
			if (!is_connected(src, dst)) {
				return false;
			}

			auto src_ptr = get_graph_node_pointer(src);
			auto dst_ptr = get_graph_node_pointer(dst);

			// loop through vertices in edge map with src and dst vertices
			auto src_dst_edgevertices = std::make_pair(src_ptr, dst_ptr);
			for (auto src_dst_edge = edge_map_.at(src_dst_edgevertices).begin(); src_dst_edge != edge_map_.at(src_dst_edgevertices).end(); src_dst_edge++) {
				// if given edge has same weight, then remove edge
				if (**src_dst_edge == weight) {
					// removes edge
					(edge_map_.at(src_dst_edgevertices)).erase(src_dst_edge);
					// if after edge removal, no more edges from src to dst, then erase src dst edge
					// from edge_map
					if ((edge_map_.at(src_dst_edgevertices)).empty()) {
						edge_map_.erase(src_dst_edgevertices);
					}
					return true;
				}
			}
			// if no edge found with src dst and weight, then return false
			return false;
		}

		auto erase_edge(iterator i) -> iterator {
			// if no element exists, return this->end()
			if (i == this->end()) {
				return this->end();
			}
			auto next_elem = i;
			next_elem++;

			// erase edge pointed by i (0 = dst, 1 = src, 2 = weight)
			erase_edge(std::get<0>(*i), std::get<1>(*i), std::get<2>(*i));

			// if next element in iterator is the end, return end()
			if (next_elem == this->end()) {
				return this->end();
			}
			// if next element exists, return iterator pointing to next element after i
			return find(std::get<0>(next_elem), std::get<1>(next_elem), std::get<2>(next_elem));
		}

		// runs in O(d), erase edges from i to s
		auto erase_edge(iterator i, iterator s) -> iterator {
			auto curr_iterated_elem = i;
			// loop through all iterators starting from i and return iterators
			// if elements iterated to the end, curr_iterated_elem will be last element in iterator,
			// which is s.
			// if no elements, erase_edge(i) will return end()
			while (curr_iterated_elem != s) {
				curr_iterated_elem = erase_edge(curr_iterated_elem);
			}
			return curr_iterated_elem;
		}

		// erases all nodes from graph (erases all edges indirectly too)
		auto clear() noexcept -> void {
			// empty node set and edge map
			node_set_.clear();
			edge_map_.clear();
		}

		/////// ACCESSORS /////////
		// non-const version
		[[nodiscard]] auto is_node(N const& value) -> bool {
			// if node can be found, then return true (node exists)
			if (get_graph_node_pointer(value) != nullptr) {
				return true;
			}
			// if not, return false
			return false;
		}
		// const version
		[[nodiscard]] auto is_node(N const& value) const -> bool {
			// if node can be found, then return true (node exists)
			if (get_graph_node_pointer(value) != nullptr) {
				return true;
			}
			// if not, return false
			return false;
		}

		[[nodiscard]] auto empty() -> bool {
			// if node set is empty, no nodes in the graph, returns true
			// (edge map empty if node set empty)
			if (node_set_.empty()) {
				return true;
			}
			return false;
		}

		[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool {
			// if either is_node(src) or is_node(dst) is false, throw error
			if ((!is_node(src)) || (!is_node(dst))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph");
			}
			auto src_ptr = get_graph_node_pointer(src);
			auto dst_ptr = get_graph_node_pointer(dst);
			// if no edges from src node to dst node in edge map, return false
			if (edge_map_[std::make_pair(src_ptr, dst_ptr)].empty()) {
				return false;
			}
			// if statement above did not return, return true
			return true;
		}

		// non-const version
		[[nodiscard]] auto nodes() -> std::vector<N> {
			auto return_node_set = std::vector<N>();
			// loop through node set and add all nodes into return_node_set
			for (auto &node: node_set_) {
				return_node_set.push_back(*node.get());
			}
			// sort and return sorted sequence of all stored nodes
			std::sort(return_node_set.begin(), return_node_set.end());
			return return_node_set;
		}
		// const version
		[[nodiscard]] auto nodes() const -> std::vector<N> {
			auto return_node_set = std::vector<N>();
			// loop through node set and add all nodes into return_node_set
			for (auto& node: node_set_) {
				return_node_set.push_back(*node.get());
			}
			// sort and return sorted sequence of all stored nodes
			std::sort(return_node_set.begin(), return_node_set.end());
			return return_node_set;
		}

		// returns sequence of weights from src to dst sorted in ascending order
		[[nodiscard]] auto weights(N const& src, N const& dst) -> std::vector<E> {
			// throws error if either is_node(src) or is_node(dst) is false
			if ((!is_node(src)) || (!is_node(dst))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in the graph");
			}
			// initialise return weight vector
			auto return_weight_seq = std::vector<E>();
			// loop through all edges in edge map with src and dst vertices,
			// then add all weights to return_weight_seq
			for (auto& edge: (edge_map_[std::make_pair(this->get_graph_node_pointer(src), this->get_graph_node_pointer(dst))])) {
				return_weight_seq.push_back(*edge.get());
			}
			// sort return_weight_seq in ascending order then return
			std::sort(return_weight_seq.begin(), return_weight_seq.end());
			return return_weight_seq;
		}


		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) -> iterator {
			// loop through and check if edge with src,dst,weight exists
			for (auto edge = begin(); edge != end(); edge++) {
				if ((std::get<0>(*edge) == src) && (std::get<1>(*edge) == dst) && (std::get<2>(*edge) == weight)) {
					return edge;
				}
			}
			// tried to find edge equal to src,dst,weight but no edge exists, return end()
			return end();
		}

		// non-const version
		[[nodiscard]] auto connections(N const& src) -> std::vector<N> {
			// if is_node(src) is false, throw error
			if (!is_node(src)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the graph");
			}
			// get set of outgoing nodes by looping through edge_map_
			auto outgoing_nodes = std::set<N>();
			for (auto& [vertices, weights] : edge_map_) {
				if (*vertices.first == src) {
					outgoing_nodes.insert(*vertices.second);
				}
			}
			// cast to change to vector of outgoing nodes then sort and return
			auto return_outgoing_nodes = std::vector<N>(outgoing_nodes.begin(), outgoing_nodes.end());
			std::sort(return_outgoing_nodes.begin(), return_outgoing_nodes.end());
			return return_outgoing_nodes;
		}
		// const version
		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			// if is_node(src) is false, throw error
			if (!is_node(src)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the graph");
			}
			// get set of outgoing nodes by looping through edge_map_
			auto outgoing_nodes = std::set<N>();
			for (auto& [vertices, weights] : edge_map_) {
				if (*vertices.first == src) {
					outgoing_nodes.insert(*vertices.second);
				}
			}
			// cast to change to vector of outgoing nodes then sort and return
			auto return_outgoing_nodes = std::vector<N>(outgoing_nodes.begin(), outgoing_nodes.end());
			std::sort(return_outgoing_nodes.begin(), return_outgoing_nodes.end());
			return return_outgoing_nodes;
		}

		/////// ITERATOR ACCESS /////////
		[[nodiscard]] auto begin() const -> iterator {
			if (edge_map_.empty()) {
				// declare the type as edge weights type (since edge map is empty)
				return iterator(edge_map_, edge_map_.begin(), decltype(edge_map_.begin()->second.begin()){});
			}
			// return edge_map_ iterator with point towards edge_map.begin(), and point to edge weights
			return iterator(edge_map_, edge_map_.begin(), edge_map_.begin()->second.begin());
		}

		// returns iterator denoting end of iterable list that begin() points to
		[[nodiscard]] auto end() const -> iterator {
			// return edge_map_ iterator with point towards edge_map.end()
			return iterator(edge_map_, edge_map_.end(), {});
		}

		////////// COMPARISONS ////////////
		// complexity = O(n+e), iterate through all nodes in this and other,
		// then iterates through all edges in this and other and compares
		[[nodiscard]] auto operator==(graph const& other) const -> bool {
			// compare graphs, check if nodes and edges are the same as other
			// if so, return true
			if ((this->nodes() == other.nodes()) && (this->get_edges() == other.get_edges())) {
				return true;
			}
			// return false if not
			return false;
		}

		////////// EXTRACTOR ////////////
		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			// get nodes and edges of graph g
			auto graph_nodes = g.nodes();
			auto graph_edges = g.get_edges();
			// loop through all nodes in graph g
			for (auto graph_node: graph_nodes) {
				// print node and starting bracket
				os << graph_node << " (" << std::endl;
				// loop through connections of iterated nodes
				auto graph_connected_nodes = g.connections(graph_node);
				for (auto graph_connected_node: graph_connected_nodes) {
					// for each connection, make pair of graph_node and graph_connected_node,
					// then print weight of all edges
					for (auto weight: graph_edges[std::make_pair(graph_node, graph_connected_node)]) {
						os << "  " << graph_connected_node << " | " << weight << std::endl;
					}
				}
				// print ending bracket
				os << ")" << std::endl;
			}
			return os;
		}

		/////////// ITERATORS ////////////
		class iterator {
		public:
			using value_type = graph<N, E>::value_type;
			using reference = value_type;
			using pointer = void;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			// same_vertices_edge_iterator is used to iterate through edges with different vertices in edge_map
			using same_vertices_edge_iterator = typename std::map<std::pair<N*, N*>, std::set<std::unique_ptr<E>>>::const_iterator;
			// edge_weight_iterator is used to iterate through weights of different edges with same vertices in edge_map
			using edge_weight_iterator = typename std::set<std::unique_ptr<E>>::const_iterator;

			// Iterator constructor
			// use default (value initialisation)
			iterator() = default;

			// Iterator source
			auto operator*() -> reference {
				// return current from to and weight
				return reference{*(same_vertices_edges_iter_->first).first, *(same_vertices_edges_iter_->first).second, **weight_iter_};
			};

			// Iterator traversal
			auto operator++() -> iterator& {
				// iterates to next element if weight_iter_ has not reached end of same_vertices_edges_iter_
				if (weight_iter_ != (same_vertices_edges_iter_->second).end()) {
					weight_iter_++;
					// if after iterated to next element and next element is not end, then return *this
					if (weight_iter_ != (same_vertices_edges_iter_->second.end())) {
						// do not iterate to next vertice pair edge set since weight set is not iterated
						// until the end yet
						return *this;
					}
				}
				// iterates to next edge with different vertice pair
				same_vertices_edges_iter_++;
				// if after iterated to next element and next element is not end,
				// (still have vertice pair edges to iterate through),
				// then set weight_iter_ to next element's begin of set of weights
				if (same_vertices_edges_iter_ != this_edge_map_->end()) {
					weight_iter_ = same_vertices_edges_iter_->second.begin();
				} else {
					// if reached the end, then set to new edge_weight_iterator
					weight_iter_ = edge_weight_iterator();
				}
				// return *this
				return *this;
			};

			auto operator++(int) -> iterator {
				// referred from spec given
				auto temp = *this;
				*this++;
				return temp;
			}

			auto operator--() -> iterator& {
				// if weight_iter does not point to anything, means that we have iterated to end of
				// entire edge map
				// (Hence, we point to the last weight in the last vertice pair we iterated through)
				if (weight_iter_ == edge_weight_iterator()) {
					// set same_vertices_edges_iter to edge vertice set before end of edge map
					same_vertices_edges_iter_ = std::prev(this_edge_map_->end());
					// set weight_iter to last weight in the last edge vertice set (before end of edge map)
					weight_iter_ = std::prev(same_vertices_edges_iter_->second.end());
					return *this;
				}

				// if weight_iter is pointing to the beginning of weight set for same vertice edge iterator,
				// this means that the previously iterated weight is from the previous set of same vertice edge
				// and the previously iterated weight is the last weight from the previous set
				if (weight_iter_ == (same_vertices_edges_iter_->second.begin())) {
					// decrements and goes back to previous set of same vertice edge set
					same_vertices_edges_iter_--;
					// set to last weight before the end of the previous set of same vertice edges
					weight_iter_ = std::prev(same_vertices_edges_iter_->second.end());
					return *this;
				}

				// if weight_iter not at the end of the entire iterator nodes' weights and
				// weight_iter is not at the beginning of any weight set,
				// this means the previously iterated weight is from the same vertice edge iterator as curr
				// (only need to decrement weight_iter and set to previously iterated weight)
				weight_iter_--;
				return *this;
			}

			auto operator--(int) -> iterator {
				// referred from spec given
				auto temp = *this;
				--*this;
				return temp;
			}

			// Iterator comparison
			// use default comparator
			auto operator==(iterator const& other) const -> bool = default;

		private:
			// initialise this_edge_map_ to nullptr
			std::map<std::pair<N*, N*>, std::set<std::unique_ptr<E>>, edges_verticescompare> const* this_edge_map_ = nullptr;
			same_vertices_edge_iterator same_vertices_edges_iter_;
			edge_weight_iterator weight_iter_;
			friend class graph;
			// iterator constructor
			explicit iterator(
				std::map<std::pair<N*, N*>, std::set<std::unique_ptr<E>>, edges_verticescompare> const& this_edge_map,
				same_vertices_edge_iterator same_vertices_edge_iter, edge_weight_iterator weight_iter):
				this_edge_map_(&this_edge_map), same_vertices_edges_iter_(same_vertices_edge_iter), weight_iter_(weight_iter) {}
		};

		// helper function to get edges in map of vertice pairs and set of weights
		// (non-const version and const version for different functions)
		[[nodiscard]] auto get_edges() -> std::map<std::pair<N, N>, std::set<E>> {
			auto return_edge_map = std::map<std::pair<N, N>, std::set<E>>();
			for (auto& [vertices, weights] : edge_map_) {
				auto weight_set = std::set<E>();
				for (auto& weight : weights) {
					weight_set.insert(*weight);
				}
				return_edge_map[std::make_pair(*vertices.first, *vertices.second)] = weight_set;
			}
			return return_edge_map;
		}
		[[nodiscard]] auto get_edges() const -> std::map<std::pair<N, N>, std::set<E>> {
			auto return_edge_map = std::map<std::pair<N, N>, std::set<E>>();
			for (auto& [vertices, weights] : edge_map_) {
				auto weight_set = std::set<E>();
				for (auto& weight : weights) {
					weight_set.insert(*weight);
				}
				return_edge_map[std::make_pair(*vertices.first, *vertices.second)] = weight_set;
			}
			return return_edge_map;
		}

	private:
		std::set<std::unique_ptr<N>> node_set_;
		std::map<std::pair<N*, N*>, std::set<std::unique_ptr<E>>, edges_verticescompare> edge_map_;

		// helper function to help get node pointer for given graph node passed in
		// non-const version
		auto get_graph_node_pointer(N graph_node) -> N* {
			// get unary predicate to pass into find_if, with graph_node as key
			auto node_pointer = [graph_node](auto& node_value) {
				return (graph_node == *node_value);
			};
			// iterate through node set and check if graph_node present
			auto node_iter = find_if(node_set_.begin(), node_set_.end(), node_pointer);
			// if iterated node is not at the end of the node set, graph_node pointer found,
			// return pointer
			if (node_iter != node_set_.end()) {
				auto& temp = *node_iter;
				return temp.get();
			}
			// return nullptr if node pointer not found
			return nullptr;
		}
		// const version
		auto get_graph_node_pointer(N graph_node) const -> N* {
			// get unary predicate to pass into find_if, with graph_node as key
			auto node_pointer = [graph_node](auto& node_value) {
				return (graph_node == *node_value);
			};
			// iterate through node set and check if graph_node present
			auto node_iter = find_if(node_set_.begin(), node_set_.end(), node_pointer);
			// if iterated node is not at the end of the node set, graph_node pointer found,
			// return pointer
			if (node_iter != node_set_.end()) {
				auto& temp = *node_iter;
				return temp.get();
			}
			// return nullptr if node pointer not found
			return nullptr;
		}
	};
} // namespace gdwg

#endif // GDWG_GRAPH_HPP

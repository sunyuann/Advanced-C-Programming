// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <asm-generic/errno.h>
#include <cassert>
#include <cmath>
#include <comp6771/euclidean_vector.hpp>
#include <ostream>

namespace comp6771 {
	//////////// CONSTRUCTORS ////////////////
	// default constructor
	euclidean_vector::euclidean_vector() {
		this->find_normal = true;
		this->normal = 0;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		this->magnitudes_ = std::make_unique<double[]>(1);
		this->num_dimensions_ = 1;
	}

	// single-argument constructor
	euclidean_vector::euclidean_vector(int num_dimensions) {
		this->find_normal = true;
		this->normal = 0;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		this->magnitudes_ = std::make_unique<double[]>(static_cast<size_t>(num_dimensions));
		// sets mag in each direction to 0.0
		for (int i = 0; i < num_dimensions; i++) {
			this->magnitudes_[static_cast<size_t>(i)] = 0.0;
		}
		this->num_dimensions_ = num_dimensions;
	}

	// constructor
	// (not sure how to remove clang for this, since it is what the spec gave
	// clang error: bugprone easily swappable params)
	euclidean_vector::euclidean_vector(int num_dimensions, double magnitude) {
		this->find_normal = true;
		this->normal = 0;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		this->magnitudes_ = std::make_unique<double[]>(static_cast<size_t>(num_dimensions));
		for (int i = 0; i < num_dimensions; i++) {
			this->magnitudes_[static_cast<size_t>(i)] = magnitude;
		}
		this->num_dimensions_ = num_dimensions;
	}

	// constructor
	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator begin_iterator,
	                                   std::vector<double>::const_iterator end_iterator) {
		this->find_normal = true;
		this->normal = 0;
		// get num dimensions based on number of iterators from begin iter to end iter
		int num_dimensions = 0;
		for (auto iter = begin_iterator; iter != end_iterator; iter++) {
			num_dimensions++;
		}
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		this->magnitudes_ = std::make_unique<double[]>(static_cast<size_t>(num_dimensions));
		int index = 0;
		for (auto iter = begin_iterator; iter != end_iterator; iter++) {
			this->magnitudes_[static_cast<size_t>(index)] = *iter;
			index++;
		}
		this->num_dimensions_ = num_dimensions;
	}

	// constructor
	euclidean_vector::euclidean_vector(std::initializer_list<double> init_list) {
		this->find_normal = true;
		this->normal = 0;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		this->magnitudes_ = std::make_unique<double[]>(static_cast<size_t>(init_list.size()));
		int num_dimensions = 0;
		for (auto iter : init_list) {
			this->magnitudes_[static_cast<size_t>(num_dimensions)] = iter;
			num_dimensions++;
		}
		this->num_dimensions_ = num_dimensions;
	}

	// copy constructor
	euclidean_vector::euclidean_vector(euclidean_vector const& src_vector) noexcept {
		this->find_normal = src_vector.find_normal;
		this->normal = src_vector.normal;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		this->magnitudes_ = std::make_unique<double[]>(static_cast<size_t>(src_vector.dimensions()));
		for (int i = 0; i < src_vector.dimensions(); i++) {
			magnitudes_[static_cast<size_t>(i)] = src_vector.magnitudes_[static_cast<size_t>(i)];
		}
		this->num_dimensions_ = src_vector.dimensions();
	}

	// move constructor
	euclidean_vector::euclidean_vector(euclidean_vector&& src_vector) noexcept {
		this->find_normal = std::exchange(src_vector.find_normal, true);
		this->normal = std::exchange(src_vector.normal, 0);
		this->magnitudes_ = std::exchange(src_vector.magnitudes_, nullptr);
		this->num_dimensions_ = std::exchange(src_vector.num_dimensions_, 0);
	}

	// deconstructor
	euclidean_vector::~euclidean_vector() = default;

	////////////////// OPERATIONS ///////////////////
	// copy assignment
	auto euclidean_vector::operator=(euclidean_vector const& orig) -> euclidean_vector& {
		auto copy = orig;
		std::swap(find_normal, copy.find_normal);
		std::swap(normal, copy.normal);
		std::swap(magnitudes_, copy.magnitudes_);
		std::swap(num_dimensions_, copy.num_dimensions_);
		return *this;
	}

	// move assignment
	auto euclidean_vector::operator=(euclidean_vector&& orig) noexcept -> euclidean_vector& {
		std::swap(find_normal, orig.find_normal);
		std::swap(normal, orig.normal);
		std::swap(magnitudes_, orig.magnitudes_);
		std::swap(num_dimensions_, orig.num_dimensions_);
		auto* temp = orig.magnitudes_.release();
		delete[] temp;
		// reset all to default after release and deletion
		// (move requires deletion of old euclidean vector)
		orig.find_normal = true;
		orig.normal = 0;
		orig.magnitudes_ = nullptr;
		orig.num_dimensions_ = 0;
		return *this;
	}

	// subscript (const)
	auto euclidean_vector::operator[](int index) const -> double {
		// check if index in range
		assert(index >= 0 && index < static_cast<int>(sizeof(this->magnitudes_)));
		return magnitudes_[static_cast<size_t>(index)];
	}

	// subscript (non-const)
	auto euclidean_vector::operator[](int index) -> double& {
		// normal need to be recalculated since euclidean vector changed
		this->find_normal = true;
		// check if index in range
		assert(index >= 0 && index < static_cast<int>(sizeof(this->magnitudes_)));
		return magnitudes_[static_cast<size_t>(index)];
	}

	// unary plus (returns copy of curr obj)
	auto euclidean_vector::operator+() -> euclidean_vector {
		// get magnitude, then pass into constructor and then return created object
		auto magnitudes = this->magnitudes();
		euclidean_vector return_vector = euclidean_vector(magnitudes.begin(), magnitudes.end());
		return return_vector;
	}

	// negation
	auto euclidean_vector::operator-() -> euclidean_vector {
		// (euclidean norm for negated euclidean vector is the same as original, so no recalculation
		// required)
		// get magnitude, then pass into constructor and then return created object
		auto magnitudes = this->magnitudes();
		// reverse polarity for every magnitude
		for (auto& magnitude : magnitudes) {
			magnitude = magnitude * -1;
		}
		euclidean_vector return_vector = euclidean_vector(magnitudes.begin(), magnitudes.end());
		return return_vector;
	}

	// compound addition
	auto euclidean_vector::operator+=(euclidean_vector const& vector) -> euclidean_vector& {
		// normal need to be recalculated since euclidean vector changed
		this->find_normal = true;
		// throw exception if different dimension vectors
		if (this->dimensions() != vector.dimensions()) {
			throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(this->dimensions())
			                             + ") and RHS(" + std::to_string(vector.dimensions())
			                             + ") do not match");
		}
		// loop through all magnitudes and add pairs up with same index
		for (int i = 0; i < this->dimensions(); i++) {
			this->magnitudes_[static_cast<size_t>(i)] += vector.magnitudes_[static_cast<size_t>(i)];
		}
		return *this;
	}

	// compound subtraction
	auto euclidean_vector::operator-=(euclidean_vector const& vector) -> euclidean_vector& {
		// normal need to be recalculated since euclidean vector changed
		this->find_normal = true;
		// throw exception if different dimension vectors
		if (this->dimensions() != vector.dimensions()) {
			throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(this->dimensions())
			                             + ") and RHS(" + std::to_string(vector.dimensions())
			                             + ") do not match");
		}
		// loop through all magnitudes and add pairs up with same index
		for (int i = 0; i < this->dimensions(); i++) {
			this->magnitudes_[static_cast<size_t>(i)] -= vector.magnitudes_[static_cast<size_t>(i)];
		}
		return *this;
	}

	// compound multiplication
	auto euclidean_vector::operator*=(double multiplier) -> euclidean_vector& {
		// normal need to be recalculated since euclidean vector changed
		this->find_normal = true;
		// loop through and multiply all magnitudes by mult_val
		for (auto i = 0; i < this->dimensions(); i++) {
			this->magnitudes_[static_cast<size_t>(i)] *= multiplier;
		}
		return *this;
	}

	// compound division
	auto euclidean_vector::operator/=(double divisor) -> euclidean_vector& {
		// normal need to be recalculated since euclidean vector changed
		this->find_normal = true;
		if (divisor == 0.0) {
			throw "Invalid vector division by 0";
		}
		for (auto i = 0; i < this->dimensions(); i++) {
			this->magnitudes_[static_cast<size_t>(i)] /= divisor;
		}
		return *this;
	}

	// vector type conversion (const) (typecasting to std::vector)
	euclidean_vector::operator std::vector<double>() const {
		// initialisation
		auto return_vector = std::vector<double>();
		// allocate space for return vector
		return_vector.reserve(static_cast<size_t>(this->dimensions()));
		// loop through and add all magnitudes to return vector, then return it.
		for (auto i = 0; i < this->dimensions(); i++) {
			return_vector.push_back(this->magnitudes_[static_cast<size_t>(i)]);
		}
		return return_vector;
	}

	// vector type conversion (non-const) (typecasting to std::vector)
	euclidean_vector::operator std::vector<double>() {
		// initialisation
		auto return_vector = std::vector<double>();
		// allocate space for return vector
		return_vector.reserve(static_cast<size_t>(this->dimensions()));
		// loop through and add all magnitudes to return vector, then return it.
		for (auto i = 0; i < this->dimensions(); i++) {
			return_vector.push_back(this->magnitudes_[static_cast<size_t>(i)]);
		}
		return return_vector;
	}

	// list type conversion (const) (typecasting to std::list)
	euclidean_vector::operator std::list<double>() const {
		// initialisation
		auto return_list = std::list<double>();
		// loop through and add all magnitudes to return list, then retun it.
		for (auto magnitude : this->magnitudes()) {
			return_list.push_back(magnitude);
		}
		return return_list;
	}

	// list type conversion (non-const) (typecasting to std::list)
	euclidean_vector::operator std::list<double>() {
		// initialisation
		auto return_list = std::list<double>();
		// loop through and add all magnitudes to return list, then retun it.
		for (auto magnitude : this->magnitudes()) {
			return_list.push_back(magnitude);
		}
		return return_list;
	}

	/////// MEMBER FUNCTIONS ///////
	// return the value of the magnitude in the dimension given by param (const)
	auto euclidean_vector::at(int dimension) const -> double {
		// exception if given dimension index out of range
		if (dimension < 0 || dimension >= this->dimensions()) {
			throw euclidean_vector_error("Index " + std::to_string(dimension)
			                             + " is not valid for this euclidean_vector object");
		}
		// if no exception then return magnitude of given dimension index
		return this->magnitudes_[static_cast<size_t>(dimension)];
	}

	// return the value of the magnitude in the dimension given by param (non-const)
	auto euclidean_vector::at(int dimension) -> double& {
		// exception if given dimension index out of range
		if (dimension < 0 || dimension >= this->dimensions()) {
			throw euclidean_vector_error("Index " + std::to_string(dimension)
			                             + " is not valid for this euclidean_vector object");
		}
		// if no exception then return magnitude of given dimension index
		return this->magnitudes_[static_cast<size_t>(dimension)];
	}

	// return the number of dimensions in a particular euclidean_vector
	auto euclidean_vector::dimensions() const -> int {
		return this->num_dimensions_;
	}

	///////// FRIENDS /////////////
	// equal
	auto operator==(euclidean_vector const& first, euclidean_vector const& second) -> bool {
		// if dimensions are not equal, return false
		if (first.dimensions() != second.dimensions()) {
			return false;
		}
		// loop and check if magnitude of all dimensions are equal
		for (auto i = 0; i < first.dimensions(); i++) {
			if (first.magnitudes()[static_cast<size_t>(i)] != second.magnitudes_[static_cast<size_t>(i)])
			{
				return false;
			}
		}
		// if dimensions equal and all magnitudes equal, return true
		return true;
	}

	// not equal
	auto operator!=(euclidean_vector const& first, euclidean_vector const& second) -> bool {
		// if dimensions are not equal, return true
		if (first.dimensions() != second.dimensions()) {
			return true;
		}
		// loop and check if magnitude of all dimensions are equal
		for (auto i = 0; i < first.dimensions(); i++) {
			if (first.magnitudes()[static_cast<size_t>(i)]
			    != second.magnitudes()[static_cast<size_t>(i)]) {
				return true;
			}
		}
		// if dimensions equal and all magnitudes equal, return false
		return false;
	}

	// addition
	auto operator+(euclidean_vector const& first, euclidean_vector const& second) -> euclidean_vector {
		if (first.dimensions() != second.dimensions()) {
			throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(first.dimensions())
			                             + ") and RHS(" + std::to_string(second.dimensions())
			                             + ") do not match");
		}
		auto first_magnitudes = first.magnitudes();
		auto second_magnitudes = second.magnitudes();
		auto result_magnitudes = std::vector<double>();
		// loop through first and second's magnitudes and add, then store in result_magnitudes
		for (auto i = 0; i < first.dimensions(); i++) {
			result_magnitudes.push_back(first_magnitudes[static_cast<size_t>(i)]
			                            + second_magnitudes[static_cast<size_t>(i)]);
		}
		euclidean_vector retval(result_magnitudes.begin(), result_magnitudes.end());
		return retval;
	}

	// subtraction
	auto operator-(euclidean_vector const& first, euclidean_vector const& second) -> euclidean_vector {
		// dimensions dont match, throw exception
		if (first.dimensions() != second.dimensions()) {
			throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(first.dimensions())
			                             + ") and RHS(" + std::to_string(second.dimensions())
			                             + ") do not match");
		}
		// return 0 dimensions with 0 magnitude if both dimensions are equal and are equal to 0.
		if (first.dimensions() == 0) {
			euclidean_vector retval(0, 0.0);
			return retval;
		}
		auto first_magnitudes = first.magnitudes();
		auto second_magnitudes = second.magnitudes();
		auto result_magnitudes = std::vector<double>();
		// loop through first and second's magnitudes and subtract, then store in result_magnitudes
		for (auto i = 0; i < first.dimensions(); i++) {
			result_magnitudes.push_back(first_magnitudes[static_cast<size_t>(i)]
			                            - second_magnitudes[static_cast<size_t>(i)]);
		}
		euclidean_vector retval(result_magnitudes.begin(), result_magnitudes.end());
		return retval;
	}

	// multiply (vector * multiplier) case
	auto operator*(euclidean_vector const& vector, double multiplier) -> euclidean_vector {
		// return 0 dimensions with 0 magnitude if vector has 0 dimensions
		// (multiplied by anything = 0)
		if (vector.dimensions() == 0) {
			euclidean_vector retval(0, 0.0);
			return retval;
		}
		auto vector_magnitudes = vector.magnitudes();
		auto result_magnitudes = std::vector<double>();
		for (auto i = 0; i < vector.dimensions(); i++) {
			result_magnitudes.push_back(vector_magnitudes[static_cast<size_t>(i)] * multiplier);
		}
		euclidean_vector retval(result_magnitudes.begin(), result_magnitudes.end());
		return retval;
	}
	// multiply (multiplier * vector) case
	auto operator*(double multiplier, euclidean_vector const& vector) -> euclidean_vector {
		// return 0 dimensions with 0 magnitude if vector has 0 dimensions
		// (multiplied by anything = 0)
		if (vector.dimensions() == 0) {
			euclidean_vector retval(0, 0.0);
			return retval;
		}
		auto vector_magnitudes = vector.magnitudes();
		auto result_magnitudes = std::vector<double>();
		for (auto i = 0; i < vector.dimensions(); i++) {
			result_magnitudes.push_back(vector_magnitudes[static_cast<size_t>(i)] * multiplier);
		}
		euclidean_vector retval(result_magnitudes.begin(), result_magnitudes.end());
		return retval;
	}

	// divide
	auto operator/(euclidean_vector const& vector, double divisor) -> euclidean_vector {
		// throw exception if divisor = 0
		if (divisor == 0) {
			throw euclidean_vector_error("Invalid vector division by 0");
		}
		// return 0 dimensions with 0 magnitude if vector has 0 dimensions
		// (divided by anything = 0)
		if (vector.dimensions() == 0) {
			euclidean_vector retval(0, 0.0);
			return retval;
		}
		// carry out divide operation
		auto vector_magnitudes = vector.magnitudes();
		auto result_magnitudes = std::vector<double>();
		for (auto i = 0; i < vector.dimensions(); i++) {
			result_magnitudes.push_back(vector_magnitudes[static_cast<size_t>(i)] / divisor);
		}
		euclidean_vector retval(result_magnitudes.begin(), result_magnitudes.end());
		return retval;
	}

	// output stream
	auto operator<<(std::ostream& output, euclidean_vector const& vector) -> std::ostream& {
		// follow given output format, [ at the start, magnitudes with space in between then ] at the
		// end
		auto vector_magnitudes = vector.magnitudes();
		output << "[";
		auto counter = 0;
		for (auto magnitude : vector_magnitudes) {
			if (counter != 0) {
				output << " ";
			}
			output << magnitude;
			counter++;
		}
		output << "]";
		return output;
	}

	////// UTILITY FUNCTIONS ///////
	auto euclidean_norm(euclidean_vector const& vector) -> double {
		if (vector.dimensions() == 0) {
			return 0;
		}
		// if no need to find normal (normal already found/cached, return vector)
		if (!vector.find_normal) {
			return vector.normal;
		}
		// if normal not found/cached, use formula to obtain euclidean normal
		auto vector_magnitudes = vector.magnitudes();
		auto euclidean_norm = 0.0;
		for (auto magnitude : vector_magnitudes) {
			euclidean_norm += (magnitude * magnitude);
		}
		euclidean_norm = sqrt(euclidean_norm);
		// set find_normal to false and cache normal into norm
		// (prevent further calculations for given vector unless given vector is altered)
		vector.find_normal = false;
		vector.normal = euclidean_norm;
		return euclidean_norm;
	}

	auto unit(euclidean_vector const& vector) -> euclidean_vector {
		// if no dimensions, throw exception
		if (vector.dimensions() == 0) {
			throw euclidean_vector_error("euclidean_vector with no dimensions does not have a unit "
			                             "vector");
		}
		// if euclidean norm is 0, throw exception
		auto normal = euclidean_norm(vector);
		if (normal == 0) {
			throw euclidean_vector_error("euclidean_vector with zero euclidean normal does not have a "
			                             "unit vector");
		}
		// maybe I could just use divide operator on vector/normal (? TODO)
		auto vector_magnitudes = vector.magnitudes();
		auto result_magnitudes = std::vector<double>();
		for (auto i = 0; i < vector.dimensions(); i++) {
			result_magnitudes.push_back(vector_magnitudes[static_cast<size_t>(i)] / normal);
		}
		euclidean_vector retval(result_magnitudes.begin(), result_magnitudes.end());
		return retval;
	}

	auto dot(euclidean_vector const& first, euclidean_vector const& second) -> double {
		// dimensions dont match, throw exception
		if (first.dimensions() != second.dimensions()) {
			throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(first.dimensions())
			                             + ") and RHS(" + std::to_string(second.dimensions())
			                             + ") do not match");
		}
		// return 0 if both dimensions are equal and are equal to 0.
		if (first.dimensions() == 0) {
			return 0;
		}
		auto first_magnitudes = first.magnitudes();
		auto second_magnitudes = second.magnitudes();
		auto result = 0.0;
		// loop through first and second's magnitudes and multiply both values, then add into result
		for (auto i = 0; i < first.dimensions(); i++) {
			result +=
			   (first_magnitudes[static_cast<size_t>(i)] * second_magnitudes[static_cast<size_t>(i)]);
		}
		return result;
	}

	/////// HELPER FUNCTIONS //////
	// getter for magnitudes (const and non const since there are const and non const functions that
	// call it)
	auto euclidean_vector::magnitudes() const -> std::vector<double> {
		auto result_magnitudes = std::vector<double>();
		result_magnitudes.reserve(static_cast<size_t>(this->dimensions()));
		for (auto i = 0; i < this->dimensions(); i++) {
			auto magnitude = this->magnitudes_[static_cast<size_t>(i)];
			result_magnitudes.push_back(magnitude);
		}
		return result_magnitudes;
	}
	auto euclidean_vector::magnitudes() -> std::vector<double> {
		auto result_magnitudes = std::vector<double>();
		result_magnitudes.reserve(static_cast<size_t>(this->dimensions()));
		for (auto i = 0; i < this->dimensions(); i++) {
			auto magnitude = this->magnitudes_[static_cast<size_t>(i)];
			result_magnitudes.push_back(magnitude);
		}
		return result_magnitudes;
	}
} // namespace comp6771

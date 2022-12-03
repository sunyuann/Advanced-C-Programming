#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include <algorithm>
#include <functional>
#include <list>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what)
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		// variables (for caching euclidean norm utility function)
		// if find_normal is true, this means that euclidean normal has not been calculated for
		// current euclidean vector
		// (this could be due to euclidean_norm function never being called, or euclidean vector's
		// values changed after euclidean_norm has been called, which requires us to find the
		// euclidean normal again)
		bool mutable find_normal;
		// normal variable to cache and store euclidean normal of current euclidean vector
		double mutable normal;

		////// HELPER FUNCTIONS ///////
		// returns magnitudes of given function (for const and non-const)
		auto magnitudes() -> std::vector<double>;
		auto magnitudes() const -> std::vector<double>;

		/////// CONSTRUCTORS ////////
		// default constructor
		euclidean_vector();
		// single-argument constructor
		explicit euclidean_vector(int num_dimensions);
		// other constructors
		euclidean_vector(int num_dimensions, double magnitude);
		euclidean_vector(std::vector<double>::const_iterator begin_iterator,
		                 std::vector<double>::const_iterator end_iterator);
		euclidean_vector(std::initializer_list<double> init_list);
		// copy constructor
		euclidean_vector(euclidean_vector const& src_vector) noexcept;
		// move constructor
		euclidean_vector(euclidean_vector&& src_vector) noexcept;

		/////// DESTRUCTOR /////////
		~euclidean_vector();

		//////// OPERATIONS ///////
		// copy assignment
		auto operator=(euclidean_vector const& orig) -> euclidean_vector&;
		// move assignment
		auto operator=(euclidean_vector&& orig) noexcept -> euclidean_vector&;
		// subscript (const and non const)
		auto operator[](int index) const -> double;
		auto operator[](int index) -> double&;
		// unary plus
		auto operator+() -> euclidean_vector;
		// negation
		auto operator-() -> euclidean_vector;
		// compound addition
		auto operator+=(euclidean_vector const& vector) -> euclidean_vector&;
		/// compound subtraction
		auto operator-=(euclidean_vector const& vector) -> euclidean_vector&;
		// compound multiplication
		auto operator*=(double multiplier) -> euclidean_vector&;
		// compound division
		auto operator/=(double divisor) -> euclidean_vector&;
		// vector type conversion
		explicit operator std::vector<double>() const;
		explicit operator std::vector<double>();
		// list type conversion
		explicit operator std::list<double>() const;
		explicit operator std::list<double>();

		///// MEMBER FUNCTIONS ///////
		auto at(int dimension) const -> double;
		auto at(int dimension) -> double&;
		auto dimensions() const -> int;

		/////// FRIENDS /////////
		// equal
		friend auto operator==(euclidean_vector const& first, euclidean_vector const& second) -> bool;
		// not equal
		friend auto operator!=(euclidean_vector const& first, euclidean_vector const& second) -> bool;
		// addition
		friend auto operator+(euclidean_vector const& first, euclidean_vector const& second)
		   -> euclidean_vector;
		// subtraction
		friend auto operator-(euclidean_vector const& first, euclidean_vector const& second)
		   -> euclidean_vector;
		// multiply
		friend auto operator*(euclidean_vector const& vector, double multiplier) -> euclidean_vector;
		friend auto operator*(double multiplier, euclidean_vector const& vector) -> euclidean_vector;
		// divide
		friend auto operator/(euclidean_vector const& vector, double divisor) -> euclidean_vector;
		// output stream
		friend auto operator<<(std::ostream& output, euclidean_vector const& vector) -> std::ostream&;

	private:
		// ass2 spec requires we use std::unique_ptr<double[]>
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitudes_;
		int num_dimensions_;
	};

	/////// UTILITY FUNCTIONS ///////
	// (put outside of public since it is not supposed to be part of euclidean_vector interface)
	auto euclidean_norm(euclidean_vector const& vector) -> double;
	auto unit(euclidean_vector const& vector) -> euclidean_vector;
	auto dot(euclidean_vector const& first, euclidean_vector const& second) -> double;

} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP

#ifndef ERRORS_HPP
#define ERRORS_HPP


#include <system_error>  // error_category
#include <string>


// TBD exceptions that inherit from std::system_error, see:
//   - https://breese.github.io/2017/05/12/customizing-error-codes.html

namespace errors {

namespace system {

std::string
message( const std::string& msg = {} );

const std::system_error
exception( const std::string& msg = {} );

}  // namespace errors::system

namespace getaddrinfo {

namespace detail {

struct Category final : public std::error_category {
    const char*
    name() const noexcept final;

    std::string
    message( const int code ) const final;
};

}  // namespace errors::getaddrinfo::detail

std::string
message( const int gai_ret, const std::string& msg = {} );

const std::system_error
exception( const int gai_ret, const std::string& msg = {} );

}  // namespace errors::getaddrinfo

}  // namespace errors


#endif  // ERRORS_HPP

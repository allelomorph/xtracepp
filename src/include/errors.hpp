#ifndef ERRORS_HPP
#define ERRORS_HPP


#include <system_error>  // error_category
#include <string>


// TBD Create exception that inherits from std::system_error for getaddrinfo, see:
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
    message( int i ) const final;
};

}  // namespace errors::getaddrinfo::detail

const std::error_category& category();

inline const std::error_code
errorCode( const int e ) {
    return std::error_code{ e, category() };
}

std::string
message( const int gai_ret, const std::string& msg = {} );

const std::system_error
exception( const int gai_ret, const std::string& msg = {} );

}  // namespace errors::getaddrinfo

}  // namespace errors


#endif  // ERRORS_HPP

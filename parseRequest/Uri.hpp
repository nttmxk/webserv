#ifndef URI_HPP
# define URI_HPP

# include <iostream>
# include <sstream>

/** Uri class
 * author: jinoh
 * date: 2022.12.29
 */

// RFC 5234
# define UPPER_ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define LOWER_ALPHA "abcdefghijklmnopqrstuvwxyz"
# define ALPHA UPPER_ALPHA LOWER_ALPHA
# define DIGIT "0123456789"
# define HEXDIG "0123456789ABCDEF"
// RFC 3986
# define GEN_DELIMS ":/?#[]@"
# define SUB_DELIMS "!$&'()*+,;="
# define RESERVED GEN_DELIMS SUB_DELIMS
# define UNRESERVED ALPHA DIGIT "-._~"
# define PCHAR UNRESERVED SUB_DELIMS ":@"

//# define VCHAR %x21-7E

class Uri {
public:
	bool		_valid;
	std::string	_scheme;
	std::string	_host;
	std::string	_port;
	std::string	_path;
	std::string	_query;
	void parseTarget(std::string &uri);

	Uri();
	~Uri();
	Uri(const Uri &orig);
	Uri& operator=(const Uri &orig);
private:
	void	checkScheme(std::string &uri, size_t &pos);
	void	checkHier(std::string &uri, size_t &pos);
	void	checkAuth(std::string &uri, size_t &pos);
	void	checkPath(std::string &uri, size_t &pos);
	void	checkQuery(std::string &uri, size_t pos);
	void	decodeHex(std::string &uri, size_t pos);
	void	encodeToHex(std::string &uri);

	bool	isCharset(std::string charset, char c);
};

#endif

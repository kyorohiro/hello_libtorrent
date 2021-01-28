#include<iostream>
#include<sstream>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/algorithm/string.hpp>

//
// ref
//   - https://code-examples.net/en/q/6ba0e2
//   - https://stackoverflow.com/questions/7053538/how-do-i-encode-a-string-to-base64-using-only-boost
//
std::string decode(const std::string &val) {
    using namespace boost::archive::iterators;
    using base64_txt = boost::archive::iterators::transform_width<
                binary_from_base64<std::string::const_iterator>, 8, 6>;
    std::stringstream ss;

    std::copy(
        base64_txt(val.c_str()),
        base64_txt(val.c_str() + val.size()),
        ostream_iterator<char>(ss)
    );

    return ss.str();
}

std::string encode(const std::string &val) {
    using base64_txt = boost::archive::iterators::base64_from_binary<
        boost::archive::iterators::transform_width<
            std::string::const_iterator, 6, 8>>;
    auto tmp = std::string(base64_txt(std::begin(val)), base64_txt(std::end(val)));
    return tmp.append((3 - val.size() % 3) % 3, '=');
}

int main(int argc, char*argv[]){
    std::cout << "Hello,World!!" << std::endl;
    std::cout << decode("44GT44KT44Gr44Gh44Gv") << std::endl;
    std::cout << encode(decode("44GT44KT44Gr44Gh44Gv")) << std::endl;

    return 0;
}
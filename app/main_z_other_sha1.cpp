#include<iostream>
#include<boost/uuid/name_generator_sha1.hpp>
#include<string>

std::string encodeHex(const std::string &val)
{
    static char const _hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B','C','D','E','F'};
    const char *v = val.c_str();
    int size = val.size();
    std::string ret(size*2, ' ');
    for(int i=0;i<size;i++) {
        ret[i*2] = _hex[(val[i]>>4)&0xf];
        ret[i*2+1] = _hex[(val[i])&0xf];
    }
    return ret;
}
std::string generate_sha1_string(std::string v) {
    boost::uuids::detail::sha1 sha1;
    boost::uuids::detail::sha1::digest_type digest;
    char sha1string[sizeof(digest)+1];

    sha1.process_bytes(v.c_str(), v.size());
    sha1.get_digest(digest);

    char hash[20];
	for(int i = 0; i < 5; ++i)
	{
		const char* tmp = reinterpret_cast<char*>(digest);
		hash[i*4] = tmp[i*4+3];
		hash[i*4+1] = tmp[i*4+2];
		hash[i*4+2] = tmp[i*4+1];
		hash[i*4+3] = tmp[i*4];
	}

    return encodeHex(std::string(hash,20));
}

int main(int argc, char *argv[]) {
    std::cout << generate_sha1_string("Hello") << std::endl;
    return 0;
}
//
//  g++ main_bdecode_dump.cpp  -ltorrent-rasterbar -lpthread
//
#include<iostream>
#include<fstream>
#include<vector>
#include <iterator>
#include<libtorrent/bdecode.hpp>
#include<regex>
#include <string>

void print_usage(std::string args0);
lt::bdecode_node decode_file(std::string filepath);
void print_bencode_node(lt::bdecode_node target);
std::string to_asis_hex_string(std::string target, int max);

int main(int argc, char *argv[]) {
    if(argc == 1) {
        print_usage(std::string(argv[0]));
        return -1;
    }

    // decode to entry from a file  
    std::ifstream input(std::string(argv[1]), std::ios::binary);
    std::vector<char> source((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    lt::error_code ec;
    lt::bdecode_node node = lt::bdecode(source, ec);
    // lt::bdecode_node node = decode_file(std::string(argv[1]));
    // decode_file() is wrong, do Segmentation fault (core dumped)
    print_bencode_node(node);
    return 0;
}


void print_usage(std::string args0) {
    std::cerr << ""<< args0 << " <filename>" << std::endl;
}

lt::bdecode_node decode_file(std::string filepath) {
    std::ifstream input(filepath, std::ios::binary);
    std::vector<char> source((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    lt::error_code ec;
    lt::bdecode_node node = lt::bdecode(source, ec);
    return node;
}

// display all
enum item_type{
    ipair,inode, ilistend,idictend
};

class item{
    public:
    std::pair<lt::string_view, lt::bdecode_node> pair;
    lt::bdecode_node node;
    item_type type;
    int depth;
    item(lt::bdecode_node node, int depth) {
        this->node = node;
        this->type = item_type::inode;
        this->depth = depth;
    }
    item(std::pair<lt::string_view, lt::bdecode_node> pair, int depth) {
        this->pair = pair;
        this->type = item_type::ipair;
        this->depth = depth;
    }
    item(item_type type, int depth) {
        this->type = type;
    }
};

void print_bencode_node(lt::bdecode_node target) {
    std::vector<item> stack;
    stack.push_back(item(target, 1));
    
    while(stack.size() > 0) {
        item im = stack.back();
        stack.pop_back();
        
        std::string spaces = "";
        for(int i=0;i<im.depth;i++) {
            spaces += "  ";
        }
        if(im.type == item_type::inode) {
            lt::bdecode_node node = im.node;
            if(node.type() == lt::bdecode_node::dict_t) {
                //std::cout  << std::endl;
                std::cout <<std::endl<<  spaces + " {" ;
                stack.push_back(item(item_type::idictend,im.depth));
                for(int i=0;i<node.dict_size();i++) {
                    stack.push_back(item(node.dict_at(node.dict_size()-1-i),im.depth+1));
                }
            }else if(node.type() == lt::bdecode_node::list_t) {
                std::cout << " [" <<std::endl << spaces + "   ";
                stack.push_back(item(item_type::ilistend,im.depth));
                for(int i=0;i<node.list_size();i++) {
                    stack.push_back(item( node.list_at(node.dict_size()-1-i),im.depth+1));
                }
            } else if(node.type() == lt::bdecode_node::int_t) {
                std::cout  << "'" << node.int_value() << "'";
            } else if(node.type() == lt::bdecode_node::string_t) {
                std::cout << "'"<< to_asis_hex_string(node.string_value().to_string(),100) << "'";
            } else if(node.type() == lt::bdecode_node::none_t) {
                std::cout << "<none>" ;
            } else {
                std::cout << "<else>"<<node.type() << std::endl;
            }
        } else if(im.type == item_type::ilistend) {
            std::cout<<std::endl<<spaces <<  "]"<< std::endl;
        }else if(im.type == item_type::idictend) {
            std::cout << std::endl <<spaces <<  "}";
        }
        else {
            std::pair<lt::string_view, lt::bdecode_node> pair= im.pair;
            std::cout << std::endl << spaces << "<" << to_asis_hex_string(im.pair.first.to_string(),100) << "> : ";// << std::endl;
            stack.push_back(item(pair.second,im.depth+1));
        }
    }
    std::cout << std::endl;
}

std::string to_asis_hex_string(std::string target, int max) {
    
    bool isAscii = true;
    for(int i=0;i<target.size();i++) {
        if(!(0<=target[i] && target[i] < 127)) {
            isAscii = false;
            break;
        }
    }
    if(isAscii){
        // ascii
        return target.substr(0,max);
    } else {
        if(max > target.size()*2) {
            max = target.size()*2+1;
        }
        std::vector<char> ret(max);
        const char x[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
        for(int i=0;i<target.size() && i*2 < max;i++) {
            int v = (unsigned char)target[i];
            ret[i*2] = x[(0xf0 & v) >>4]; 
            ret[i*2+1] =  x[(0x0f & v)];
        }
        return std::string("(")+std::to_string(target.size())+std::string(")") + std::string(ret.begin(),ret.end());
    }
}
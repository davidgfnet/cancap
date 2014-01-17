
#include <iostream>

unsigned int hex_to_int(const std::string s) {
	unsigned int ret = 0;
	for (int i = 0; i < s.size(); i++) {
		char c = tolower(s[s.size()-i-1]);
		unsigned int val = (c >= '0' && c <= '9') ? (c-'0') : (c-'a'+10);
		val <<= (i<<2);
		ret |= val;
	}
	return ret;
}

void hex_to_byte(const std::string s, unsigned char * buffer) {
	for (int i = 0; i < s.size(); i+=2) {
		unsigned char hi = tolower(s[i]);
		unsigned char lo = tolower(s[i+1]);
		hi = (hi >= '0' && hi <= '9') ? (hi-'0') : (hi-'a'+10);
		lo = (lo >= '0' && lo <= '9') ? (lo-'0') : (lo-'a'+10);
		buffer[i/2] = (hi<<4)|lo;
	}
}

std::string strip_spaces (std::string s) {
	std::string ret;
	for (int i = 0; i < s.size(); i++) {
		if ( 	(s[i] >= '0' && s[i] <= '9') || 
			(s[i] >= 'a' && s[i] <= 'f') || 
			(s[i] >= 'A' && s[i] <= 'F') ) {
			
			ret += s[i];
		}
	}
	return ret;
}



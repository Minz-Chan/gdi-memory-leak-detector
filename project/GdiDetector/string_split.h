#ifndef _STRING_SPLIT_H
#define _STRING_SPLIT_H
#include <string>
#include <vector>
#include <map>

using namespace std;

namespace Daniweb
{
    
    typedef string::size_type (string::*find_t)(const string& delim, 
		string::size_type offset) const;
    /// <summary>
    /// Splits the string s on the given delimiter(s) and
    /// returns a list of tokens without the delimiter(s)
    /// </summary>
    /// <param name=s>The string being split</param>
    /// <param name=match>The delimiter(s) for splitting</param>
    /// <param name=removeEmpty>Removes empty tokens from the list</param>
    /// <param name=fullMatch>
    /// True if the whole match string is a match, false
    /// if any character in the match string is a match
    /// </param>
    /// <returns>A list of tokens</returns>
    vector<string> Split(const string& s,
		const string& match,
		bool removeEmpty=false,
		bool fullMatch=false)
    {
        vector<string> result;                 // return container for tokens
        string::size_type start = 0,           // starting position for searches
			skip = 1;            // positions to skip after a match
        find_t pfind = &string::find_first_of; // search algorithm for matches
        if (fullMatch)
        {
            // use the whole match string as a key
            // instead of individual characters
            // skip might be 0. see search loop comments
            skip = match.length();
            pfind = &string::find;
        }
        while (start != string::npos)
        {
            // get a complete range [start..end)
            string::size_type end = (s.*pfind)(match, start);
            // null strings always match in string::find, but
            // a skip of 0 causes infinite loops. pretend that
            // no tokens were found and extract the whole string
            if (skip == 0) end = string::npos;
            string token = s.substr(start, end - start);
            if (!(removeEmpty && token.empty()))
            {
                // extract the token and add it to the result list
                result.push_back(token);
            }
            // start the next range
            if ((start = end) != string::npos) start += skip;
        }
        return result;
    }
}

/*************************TEST CODE*****************************
	string s="adf|asdf|fff|gg||ppp|";
	vector<string> strList;

	strList = Daniweb::Split(s,"|",true,false);

	vector<string>::iterator it;

	for(it=strList.begin(); it !=strList.end(); it++){
		MessageBox(it->c_str());
	}
******************************************************************/

#endif // _STRING_SPLIT_H
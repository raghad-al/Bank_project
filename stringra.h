#pragma once
#include <iostream>
#include <vector>

using namespace std;

namespace check {
    bool is_upper(char c) {
        return (int(c) >= int('A') && int(c) <= int('Z'));
    }

    bool is_lower(char c) {
        return (int(c) >= int('a') && int(c) <= int('z'));
    }

    bool is_letter(char c) {
        return (is_upper(c) || is_lower(c));
    }
}

namespace formating {

    char to_lower(char c) {

        if ((int)c >= (int)'a' && (int)c <= (int)'z') return c;

        return (int)c + 32;
    }

    string to_lower(string str) {
        string strLower = "";

        for (short i = 0; i < str.length(); i++) {
            strLower.push_back(to_lower(str[i]));
        }

        return strLower;
    }

    char to_upper(char c) {

        if ((int)c >= (int)'A' && (int)c <= (int)'Z') return c;

        return (int)c - 32;
    }

    string to_upper(string str) {
        string strUpper = "";

        for (short i = 0; i < str.length(); i++) {
            strUpper.push_back(to_upper(str[i]));
        }

        return strUpper;
    }
}

namespace convert {

    //convert string to vector
    vector<string> SplitString(string orgStr, string delim = " ") {
        short pos = 0;
        string word = "";
        vector<string> vWords;

        while ((pos = orgStr.find(delim)) != string::npos) {
            word = orgStr.substr(0, pos);

            vWords.push_back(word);

            orgStr.erase(0, pos + delim.length());

        }

        if (orgStr != "") vWords.push_back(orgStr);

        return vWords;
    }

    //Convert vector to string
    string JoinString(vector<string>vElmnts, string seperator = " ") {

        string str = "";

        for (string& elmnt : vElmnts) {
            str = str + elmnt;

            if (elmnt != vElmnts.back()) str = str + seperator;
        }

        return str;
    }

    //Convert array to string
    string JoinString(string arrElmnts[], short length, string seperator = " ") {

        string str = "";

        for (short i = 0; i < length; i++) {

            str = str + arrElmnts[i];

            if (i != length - 1) str = str + seperator;
        }

        return str;
    }
}
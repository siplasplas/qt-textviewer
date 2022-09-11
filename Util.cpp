#include <iostream>
#include <fstream>
#include "utf/UTF.hpp"
#include "Util.h"

#include <vector>
using namespace std;

vector<string> readLines(string filename)
{
    ifstream infile(filename);
    vector<string> v;
    string line;
    bool readedNewline;
    if (infile.peek() != EOF)
    while(true) {
        readedNewline = getline(infile, line).good();
        v.push_back(line);
        if (!readedNewline) break;
    }
    return v;
}


string trimLeft(const string& str)
{
    const auto strBegin = str.find_first_not_of(" \t");
    return str.substr(strBegin, str.length() - strBegin);
}


string trimRight(const string& str)
{
    const auto strEnd = str.find_last_not_of(" \t\r");
    return str.substr(0, strEnd + 1);
}

string trim(const string& str) {
    return trimLeft(trimRight(str));
}

string checkNoTabs(const string& line) {
    for (char c: line) {
        if (c=='\t') throw runtime_error("must not have literal tabs, use \\t");
    }
    return line;
}

/* trimRight for all lines
 * remove last mepty line if exists
 * */
vector<string> clear(const vector<string>& lines) {
    vector<string> v;
    for (auto &line: lines)
        v.push_back(checkNoTabs(trimRight(line)));
    if (v.size()>0 && v.back().empty())
        v.pop_back();
    return v;
}

string unslash(const string &line) {
    string result;
    int k = 0;
    while (k<line.length()) {
        char c = line[k];
        if (c=='\\') {
            char c1 = line[k+1];
            switch (c1) {
                case 't':
                    result += '\t';
                    break;
                case 'r':
                    result += '\r';
                    break;
                case 'n':
                    result += '\n';
                    break;
                default : throw runtime_error("bad slashed char");
            }
            k++;
        } else
            result += c;
        k++;
    }
    return result;
}

string unslashTest(string line, bool last) {
    line = unslash(line);
    for (int i=0; i<line.length(); i++) {
        char c = line[i];
        if (c=='\r' || c=='\n') {
            if (c=='\r') {
                if (i<line.length()-2)
                    throw runtime_error("\\r must only be at end or before (1) \\n");
                else if (i<line.length()-1 && line[i+1]!='\n')
                    throw runtime_error("\\r must only be at end or before (2)\\n");
            }
            else {
                if (i<line.length()-1)
                    throw runtime_error("\\n must only be at end");
            }
        }
        else {
            if (!last && i==line.length()-1)
                throw runtime_error("not last line must ends with \r or \n");
        }
    }
    return line;
}

vector<string> unslashTest(const vector<string>& lines) {
    vector<string> v;
    for (int i=0; i<lines.size(); i++)
        v.push_back(unslashTest(lines[i], i==lines.size()-1));
    return v;
}

string unslashExpect(string line, bool last) {
    line = unslash(line);
    for (int i=0; i<line.length(); i++) {
        char c = line[i];
        if (c=='\n') {
            if (i<line.length()-1)
                throw runtime_error("\\n must only be at end");
        }
        else {
            if (!last && i==line.length()-1)
                throw runtime_error("not last line must ends with \r or \n");
        }
    }
    return line;
}

vector<string> unslashExpect(const vector<string>& lines) {
    vector<string> v;
    for (int i=0; i<lines.size(); i++)
        v.push_back(unslashExpect(lines[i], i==lines.size()-1));
    return v;
}

vector<string> removeLFExpect(const vector<string>& lines) {
    vector<string> v;
    bool addEmpty = lines.size()>0 && lines.back().back()=='\n';
    for (int i=0; i<lines.size(); i++) {
        string newLine = lines[i];
        if (!newLine.empty() && newLine[newLine.length()-1]=='\n')
            newLine.pop_back();
        v.push_back(newLine);
    }
    if (addEmpty)
        v.push_back("");
    return v;
}

vector<wstring> toUTF16(const vector<string>& lines) {
    vector<wstring> v;
    UTF utf;
    for (auto &line: lines) {
        v.push_back(utf.u8to16(line));
    }
    return v;
}

string join(const vector<string>& v) {
    string result;
    for (auto &line: v) {
        result += line;
    }
    return result;
}


string makeContent(string filename) {
    auto v = unslashTest(clear(readLines(filename)));
    string content = join(v);
    if (v.empty())
        throw runtime_error("empty test sample");
    return content;
}

vector<wstring> makeExpect(string filename) {
    auto v = toUTF16(removeLFExpect(unslashExpect(clear(readLines(filename)))));
    if (v.empty())
        throw runtime_error("empty test expect");
    return v;
}

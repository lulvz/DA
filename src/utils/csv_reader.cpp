//
// Created by lulaz on 3/13/23.
//

#include "csv_reader.h"
#include <iostream>

using namespace std;

CsvReader::CsvReader(string fname) : file_name(fname) {
    this->file_read = fstream(fname);
    this->file_write = fstream(fname, ios::app);

    // simple error handling
    if(!this->file_read || !this->file_write) {
        this->error = true;
        return;
    }

    // read the header of the csv
    string line;
    getline(this->file_read, line);
    stringstream ss(line); 

    string s;
    while(getline(ss, s, ',')) {
        this->header.push_back(s);
    }
}

vector<string> CsvReader::read_line() {
    // if(file_read.eof()) {return vector<string>();}

    // string line;
    // vector<string> result;
    
    // if(getline(this->file_read, line)) {
    //     istringstream ss(line);

    //     string s;
    //     while(getline(ss, s, ',')) {
    //         result.push_back(s);
    //         while(getline(ss, s, '"')) {
    //             result.push_back(s);
    //         }
    //     }
    //     for(auto r : result) {
    //         std::cout << r << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // return result;

    std::string line;
    std::vector<std::string> words;

    if (std::getline(file_read, line)) {
        std::istringstream iss(line);
        std::string cell;
        bool in_quotes = false;
        for (char c : line) {
            if (c == '"') {
                in_quotes = !in_quotes;
            } else if (c == ',' && !in_quotes) {
                words.push_back(cell);
                cell.clear();
            } else {
                cell += c;
            }
        }
        words.push_back(cell);
    }
    return words;
}

void CsvReader::clear_file() {
    this->file_write.close();
    this->file_write.open(this->file_name, ios::out | ios::trunc);
    this->file_write.close();
    this->file_write.open(this->file_name, ios::app);
}

void CsvReader::write_line(vector<string> line) {
    string result;

    for(string s : line) {
        result += s + ",";
    }

    // remove the last ","
    result.pop_back();

    file_write << result << endl;
}

vector<string> CsvReader::get_header() const {
    return header;
}

bool CsvReader::is_eof() const {
    return this->file_read.eof();
}

bool CsvReader::is_error() const {
    return this->error;
}
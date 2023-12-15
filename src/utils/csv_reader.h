//
// Created by lulaz on 3/13/23.
//

#ifndef PROJ_DA_1_CSV_READER_H
#define PROJ_DA_1_CSV_READER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class CsvReader {
public:
    // open the file and read the first line (header)
    CsvReader(string fname);

    // read the next line of the file
    vector<string> read_line();
    // clear the whole file
    void clear_file();
    // write a line to the file
    void write_line(vector<string> line);

    // get the header of the file
    vector<string> get_header() const;
    // goes true if end of file is achieved via read_line()
    bool is_eof() const;

    bool is_error() const;

private:
    string file_name;
    fstream file_read;
    fstream file_write;

    vector<string> header;

    bool error = false;
};

#endif //PROJ_DA_1_CSV_READER_H

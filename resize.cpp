#include "processing.hpp"
#include "fstream"
#include "sstream"
#include "cstring"
#include <iostream>
#include <string>

using namespace std;

void error_message() {
    cout << "Usage: resize.exe IN_FILENAME OUT FILE_NAME WIDTH [HEIGHT]\n"
     << "WIDTH and HEIGHT must be less than or equal to original" << endl;
}

int main(int argc, char* argv[]) {
if (argc < 4 || argc > 5) {
    error_message();
    return 69;
}

string input = argv[1];
ifstream i(input);
Image img = Image::read_ppm(i);
int oldHeight = img.get_height();
int oldWidth = img.get_width();
string newFileName = argv[2];
string oldFileName = argv[1];
int newWidth = stoi(argv[3]);
int newHeight = 0;

if (!i.is_open()) {
    cout << "Error opening file: " << oldFileName << endl;
}

if (newWidth < 0 || newWidth > oldWidth || 
    newHeight < 0 || newHeight > oldHeight) {
        error_message();
    return 420;
}

if (argc == 4) {
    newHeight = oldHeight;
    Image newImage = seam_carve(img, newWidth, newHeight);
    ofstream output(newFileName);
    newImage.print(output);
    return 0;
} else if (argc == 5) {
    newHeight = stoi(argv[4]);
    Image newImage = seam_carve(img, newWidth, newHeight);
    ofstream output(newFileName);
    newImage.print(output);
    return 0;
} 
}
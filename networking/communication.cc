#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* args[]){

    string python_message = "";
    bool quit = false;

    while (!quit){
        cin >> python_message;

        if (python_message == "quit"){
            quit = true;
        }
        else if (python_message == "first"){
            cout << "First Hello!" << endl;
        }
        else if (python_message == "second"){
            cout << "Second Hello!" << endl;
        }
        else if (python_message == "on"){
            cout << "Third Hello!" << endl;
        }
        else {
            cout << python_message << endl;
        }

    }
    return 0;
}

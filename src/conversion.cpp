#include <string>
#include <vector>

using namespace std;

#include "../include/conversion.h"

int simple_digit_int(char c){
    switch (c)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
        break;
    
    default:
        return 0;
        break;
    }
}

int64_t to_int(string expr){
    int64_t result = 0;
    if (expr[0] == '-'){
        for (int i = 1; i < expr.size(); ++i){
            result *= 10;
            result += simple_digit_int(expr[i]);
        }
        result *= -1;
    } else {
        for (int i = 0; i < expr.size(); ++i){
            result *= 10;
            result += simple_digit_int(expr[i]);
        }
    }
    return result;
}

string to_string(string expr){
    return "\""+expr+"\"";
}


string del_string(string expr){
    string res;
    for (int i = 1; i<(expr.size()-1); ++i){
        res += expr[i];
    }
    return res;
}

string add_to(vector<string> to, int max)
{
    string total = to[0];
    for (int x = 1; x<max; ++x)
    {
        total += "."+to[x];
    }
    return total;
}


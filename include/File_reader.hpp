#include<fstream>
#include<cctype>
#include<string>
#include<list>
using namespace std;
template<typename T>
struct Reader
{
    string text;
    list<T> elements;
    bool is_word_number(string word, bool& is_decimal)
    {
        if(word.empty()) return empty;
        size_t size = word.size();
        bool coma = false;
        for(size_t i = 0, i < size; i++)
        {
            if (i == 0 && (s[i] == '-' || s[i] == '+')) 
            {
                continue;
            }
            if(word[i] == ".")
            {
                if(coma) return false;
                coma = true;
                is_decimal = true;
                continue;
            }
            if (!std::isdigit(s[i])) 
            {
                return false;
            }   
        }
        return true;
    }
    bool load_from_file(string route)
    {
        ifstream file(route);

        if(!file.is_open()) return false;
        while(file >> word)
        {
            bool is_decimal = false;
            if(is_word_number(word, is_decimal))
            {
                if(is_decimal)
                {

                }
                else
                {
                    //Enteros
                }
            }
            else
            {
                //Strings
            }
        }
    }
    void fill_text(string msg)
    {
        size_t size = msg.size();
        string current_element = "";
        for(size_t i = 0; i < size, i++)
        {
            if(msg[i] == "," || msg[i] == ";")
            {
                elements.push_back(static_cast<T>(current_element));
                current_element = "";
                continue;
            }
            if(msg[i] == "." && current_element.empty())
            {
                continue;
            }
            else
            {
                current_element.append(msg[i]);
            }
        }
    }
};

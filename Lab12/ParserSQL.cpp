#include "ParserSQL.h"
#include "Options.h"

#include<variant>
#include<sstream>


std::variant<int, bool, float, double, std::string> GetValue(const std::string& str, const std::string& type) {
    std::istringstream iss(str);
    int i;
    float f;
    double d;

    if (type == "int") {
        iss>>i;
        return i;
    } else if (type == "float") {
        iss>>f;
        return f;
    } else if (type == "double") {
        iss>>d;
        return d;
    } else if (type == "string") {
        return str;
    }

    return str;
}

std::string ReadWordFromBegin(std::string& s, bool only_one_word) {
    size_t i = 0;
    std::string word = "";
    while (s[i] != ' '  && i != s.length()) {
        if (only_one_word && s[i] == ',') break;
        word += s[i];
        ++i;
    }
    if (i != s.length() && s[i] == ',') ++i;

    if (i != s.length()) s = s.substr(i + 1);
    else s = "";

    return word;
}

std::string ReadBeforeComma(std::string& s) {
    size_t i = 0;
    std::string str = "";
    while (s[i] != '\0' && s[i] != ',') {
        str += s[i];
        ++i;
    }

    while (i != s.length() && (s[i] == ',')) {
        ++i;
    }

    if (i != s.length()) s = s.substr(i + 1);
    else s = "";

    return str;
};

bool Parse(std::ostream& out, std::string request, MyCoolBD& bd) {
    std::string word;
    word = ReadWordFromBegin(request, true);
    if (word == "SELECT") {
        std::vector<std::pair<std::string, std::string>> keys;
        word = ReadWordFromBegin(request, true);
        bool flag_for_all = false;
        while (word != "FROM") {
            if (word != "*") {
                std::pair<std::string, std::string> table_key = GetTableAndKey(word);
                if (table_key.second == "*") {
                    for (const std::string& key : bd.GetTableKeys(table_key.first)) {
                        keys.emplace_back(table_key.first, key);
                    }
                } else keys.push_back(table_key);
            } else {
                flag_for_all = true;
            }
            word = ReadWordFromBegin(request, true);
        }

        std::vector<std::string> tableNames;
        word = ReadWordFromBegin(request, true);
        while (!request.empty() && word != "WHERE" && word != "INNER" && word != "LEFT" && word != "RIGHT") {
            tableNames.push_back(word);
            word = ReadWordFromBegin(request, true);
        }
        tableNames.push_back(word);

        if (flag_for_all) {
            for (std::string& tableName : tableNames) {
                for (const std::string& key : bd.GetTableKeys(tableName)) {
                    keys.emplace_back(tableName, key);
                }
            }
        }


        if (request.empty()) {
            bd.SelectFrom(out, keys);
            return true;
        } else if (word == "WHERE") {
            std::shared_ptr<Node> parserRoot = ParserTree(request, bd);
            bd.SelectFromWhere(out, keys, parserRoot);
        } else {/// [...] JOIN
            const std::string& join_type = word;
            ReadWordFromBegin(request, true);///JOIN
            std::string second_table = ReadWordFromBegin(request, true);
            ReadWordFromBegin(request, true);///ON
            std::shared_ptr<Node> parserRoot = ParserTree(request, bd);
            if (join_type == "INNER") {
                bd.SelectFromInnerJoin(out, keys, parserRoot, tableNames[0], second_table);
            } else if (join_type == "LEFT") {
                bd.SelectFromLeftJoin(out, keys, parserRoot, tableNames[0], second_table);
            } else {
                bd.SelectFromRightJoin(out, keys, parserRoot, tableNames[0], second_table);
            }
        }


    } else if (word == "INSERT") {
        if (ReadWordFromBegin(request, true) != "INTO") return false;

        std::vector<std::string> keys;
        std::string tableName = ReadWordFromBegin(request, true);
        ReadWordFromBegin(request, true);///(

        word = ReadWordFromBegin(request, true);
        while (word != ")") {
            keys.push_back(word);
            word = ReadWordFromBegin(request, true);
        }

        if (ReadWordFromBegin(request, true) != "VALUES") return false;

        ReadWordFromBegin(request, true);///(

        std::vector<std::string> values;
        word = ReadWordFromBegin(request, true);
        size_t i = 0;
        while (request != ")") {
            values.push_back(word);
            word = ReadWordFromBegin(request, true);
            ++i;
        }
        values.push_back(word);
        bd.Insert(out, tableName, keys, values);

    } else if (word == "UPDATE") {
        std::string tableName = ReadWordFromBegin(request, true);
        std::vector<std::string> keys;
        std::vector<std::string> values;

        word = ReadWordFromBegin(request, true);
        if (word != "SET") return false;

        word = ReadWordFromBegin(request, true);
        while (word != "WHERE") {
            keys.push_back(word);
            ReadWordFromBegin(request, true);
            word = ReadWordFromBegin(request, true);
            values.push_back(word);
            word = ReadWordFromBegin(request, true);
        }

        std::shared_ptr<Node> parserRoot = ParserTree(request, bd);
        bd.Update(tableName, keys, values, parserRoot);
    } else if (word == "DELETE") {
        if (ReadWordFromBegin(request, true) != "FROM") return false;

        std::string tableName = ReadWordFromBegin(request, true);

        if (ReadWordFromBegin(request, true) != "WHERE") return false;

        std::shared_ptr<Node> parserRoot = ParserTree(request, bd);
        bd.Delete(tableName, parserRoot);

    } else if (word == "CREATE") {
        request = request.substr(0, request.length() - 1) + ",";
        if (ReadWordFromBegin(request, true) != "TABLE") return false;

        std::string tableName = ReadWordFromBegin(request, true);

        if (ReadWordFromBegin(request, true) != "(") return false;

        std::map<std::string, std::tuple<std::string, bool, std::pair<std::string, std::string>>> keys;

        std::string key;
        std::string type;
        std::string str;


        while (!request.empty()) {
            str = ReadBeforeComma(request);
            if (str.empty()) break;
            word = ReadWordFromBegin(str, true);
            if (word == "FOREIGN_KEY") {
                std::string key1 = ReadWordFromBegin(str, true);
                if (ReadWordFromBegin(str, true) != "REFERENCES") return false;
                std::string keyTableName = ReadWordFromBegin(str, true);
                ReadWordFromBegin(str, true); ///(
                std::string key2 = ReadWordFromBegin(str, true);
                std::get<2>(keys[key1]) = {keyTableName, key2};
                continue;
            }
            key = word;
            type = ReadWordFromBegin(str, true);

            if (str.empty()) {
                keys.insert({key, std::make_tuple(type, false, std::make_pair("", ""))});
            } else {
                ReadWordFromBegin(str, true);///PRIMARY_KEY
                keys.insert({key, std::make_tuple(type, true, std::make_pair("", ""))});
            }
        }

        bd.CreateTable(tableName, keys);

    } else if (word == "DROP") {
        if (ReadWordFromBegin(request, true) != "TABLE") return false;

        bd.DropTable(ReadWordFromBegin(request, true));
    } else if (word == "PRINT") {
        bd.Print(out, ReadWordFromBegin(request, true));
    } else if (word == "SAVE") {
        ReadWordFromBegin(request, true);///INTO
        std::string path = ReadWordFromBegin(request, true);
        bd.SaveIntoFile(path);
    }

}

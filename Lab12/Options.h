#pragma once

#include "ParserTree.h"
#include "ParserTree.h"

#include<iostream>
#include<variant>
#include<vector>
#include<map>

struct S {
    bool is_primary_key = false;
    std::pair<std::string, std::string> is_foreign_key = {"", ""}; ///{tableName, column}
    std::string type;
    std::vector<std::string> values;

    S() = default;
    S(const std::string& type_) : type(type_){}
    ~S() = default;


};

class MyCoolBD {
public:
    std::map <std::string, std::map<std::string, S>> BD;
    std::map <std::string, uint64_t > table_sizes;

    void CreateTable(const std::string& tableName_, std::map<std::string, std::tuple<std::string, bool, std::pair<std::string, std::string>>>& keys);

    void DropTable(const std::string& tableName_);

    void Insert(std::ostream& out, const std::string& destination_table, std::vector<std::string>& keys, std::vector<std::string>& values_);

    void Update(const std::string& destination_table, std::vector<std::string>& keys, std::vector<std::string>& values_, const std::shared_ptr<Node>& Where);

    void Delete(const std::string& destination_table, const std::shared_ptr<Node>& Where);

    void SelectFrom(std::ostream& out, std::vector<std::pair<std::string, std::string>>& what_columns);

    void SelectFromWhere(std::ostream& out, std::vector<std::pair<std::string, std::string>>& what_columns, const std::shared_ptr<Node>& Where);

    void SelectFromInnerJoin(std::ostream& out, std::vector<std::pair<std::string, std::string>>& what_columns, const std::shared_ptr<Node>& Join,
                             const std::string& tableName1, const std::string& tableName2);

    void SelectFromLeftJoin(std::ostream& out, std::vector<std::pair<std::string, std::string>>& what_columns, const std::shared_ptr<Node>& Join,
                            const std::string& tableName1, const std::string& tableName2);

    void SelectFromRightJoin(std::ostream& out, std::vector<std::pair<std::string, std::string>>& what_columns, const std::shared_ptr<Node>& Join,
                             const std::string& tableName1, const std::string& tableName2);


    void Print(std::ostream& out, const std::string& tableName);

    void SaveIntoFile(const std::string& path);

    std::vector<std::string> GetTableKeys(const std::string& tableName);
};
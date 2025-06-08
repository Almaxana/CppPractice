#include "ParserSQL.h"

#include <algorithm>


int main() {
    MyCoolBD bd1;

    Parse(std::cout, "CREATE TABLE table1 ( id int, name varchar(50), rating double)", bd1);
    Parse(std::cout, "CREATE TABLE table2 ( id_2 int, name_2 varchar(50), active double)", bd1);

    Parse(std::cout, "INSERT INTO table1 ( id, name, rating ) VALUES ( 1, \"A\", 10 )", bd1);
    Parse(std::cout, "INSERT INTO table1 ( id, name, rating ) VALUES ( 2, \"B\", 5 )", bd1);
    Parse(std::cout, "INSERT INTO table1 ( id, name, rating ) VALUES ( 3, \"C\", 7 )", bd1);
    Parse(std::cout, "INSERT INTO table1 ( id, name, rating ) VALUES ( 4, \"D\", NULL )", bd1);

    Parse(std::cout, "INSERT INTO table2 ( id_2, name_2, active ) VALUES ( 1, \"A\", 2 )", bd1);
    Parse(std::cout, "INSERT INTO table2 ( id_2, name_2, active ) VALUES ( 2, \"B\", 10 )", bd1);
    Parse(std::cout, "INSERT INTO table2 ( id_2, name_2, active ) VALUES ( 3, \"M\", NULL )", bd1);
    Parse(std::cout, "INSERT INTO table2 ( id_2, name_2, active ) VALUES ( 4, \"D\", 7 )", bd1);
    Parse(std::cout, "INSERT INTO table2 ( id_2, name_2, active ) VALUES ( 5, \"E\", NULL )", bd1);

    Parse(std::cout, "SELECT * FROM table1, table2", bd1);
    std::cout<<"\n\n\n";

    Parse(std::cout, "SELECT * FROM table1, table2 INNER JOIN table2 ON ( table1.id = 2 )", bd1);
    std::cout<<"\n\n\n";

    Parse(std::cout, "SELECT table1.* table2.* FROM table2 RIGHT JOIN table1 ON ( table1.id = table2.id_2 )", bd1);
    std::cout<<"\n\n\n";

    Parse(std::cout, "DELETE FROM table1 WHERE ( ( ( table1.id = 2 ) OR (table1.id = 3 ) ) AND ( table1.rating > 2 ) )", bd1);

    Parse(std::cout, "SELECT * FROM table1, table2", bd1);
    std::cout<<"\n\n\n";

    Parse(std::cout, "SAVE INTO test", bd1);

    Parse(std::cout, "DROP TABLE table2", bd1);

    return 0;
}

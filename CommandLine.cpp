/*
 * CommandLine.cpp
 *
 *  Created on: 20 April 2012
 *      Author: Max Foster
 */

#include <iostream>
#include <stdexcept>
#include <map>
using namespace std;

#include <QString>
#include <QStringList>

#include "CommandLine.h"

#include "Customer.h"
#include "Job.h"
#include "Part.h"
#include "Task.h"
#include "Expense.h"
#include "VatRate.h"

#include "Databases.h"
#include "Database.h"

/*
 * BNF for commands
 *
 *           <command> ::= <action> <database> <database-property> { <value> } { ' , ' <value> }
 *            <action> ::= 'get' | 'set'
 *          <database> ::= 'customer' | 'job' | 'part' | 'task' | 'expense' | 'vatrate'
 * <database-property> ::= 'idcounter' | 'filename' | 'count' | 'record'
 *             <value> ::= <integer> | <real> | <string> | <object>
 *           <integer> ::= obvious
 *              <real> ::= obvious
 *            <string> ::= '"' { <character>* } '"'
 *            <object> ::= '{' { ( <object-property> ':' <value> )* } '}'
 *   <object-property> ::= a record field name
 */

class Value;

typedef map<QString, Value> Object;

class Value
{
public:
    enum TypeEnum
    {
        INTEGER,
        REAL,
        STRING,
        OBJECT,
        NIL
    };

    Value(const int integerValue) : type_(INTEGER), integerValue_(integerValue) {}
    Value(const double realValue) : type_(REAL), realValue_(realValue) {}
    Value(const string &stringValue) : type_(STRING), stringValue_(stringValue) {}
    Value(const Object &objectValue) : type_(OBJECT), objectValue_(objectValue) {}
    Value() : type_(NIL) {}

    TypeEnum type() const { return type_; }
    int integerValue() const { return integerValue_; }
    double realValue() const { return realValue_; }
    const string & stringValue() const { return stringValue_; }
    const Object & objectValue() const { return objectValue_; }

private:
    TypeEnum type_;

    union
    {
        int integerValue_;
        double realValue_;
    };
    string stringValue_;
    Object objectValue_;
};

ostream & operator <<(ostream &stream, const Value &value)
{
    switch (value.type())
    {
    case Value::INTEGER: stream << value.integerValue(); break;
    case Value::REAL: stream << value.realValue(); break;
    case Value::STRING: stream << value.stringValue(); break;
    case Value::OBJECT: stream << value.objectValue(); break;
    default: stream << "NIL"; break;
    }
    return stream;
}

enum ActionEnum
{
    GET,
    SET
};

ActionEnum getAction(const QString &input)
{
    if (input == "get") return GET;
    if (input == "set") return SET;
    throw std::runtime_error("Unknown action '" + input.toStdString() + "'");
}

enum DatabaseEnum
{
    CUSTOMER,
    JOB,
    PART,
    TASK,
    EXPENSE,
    VAT_RATE
};

DatabaseEnum getDatabase(const QString &input)
{
    if (input == "customer") return CUSTOMER;
    if (input == "job") return JOB;
    if (input == "part") return PART;
    if (input == "task") return TASK;
    if (input == "expense") return EXPENSE;
    if (input == "vatrate") return VAT_RATE;
    throw std::runtime_error("Unknown database '" + input.toStdString() + "'");
}

enum DatabasePropertyEnum
{
    ID_COUNTER,
    FILENAME,
    COUNT,
    RECORD
};

DatabasePropertyEnum getDatabaseProperty(const QString &input)
{
    if (input == "idcounter") return ID_COUNTER;
    if (input == "filename") return FILENAME;
    if (input == "count") return COUNT;
    if (input == "record") return RECORD;
    throw std::runtime_error("Unknown database property '" + input.toStdString() + "'");
}

Value getValue(const QStringList &inputs)
{
    const string firstString = inputs.first().toStdString();
    const char firstChar = firstString[0];

    switch (firstChar)
    {
    case '"': // string
        return Value();
        break;

    case '{': // object
        return Value();
        break;

    default: // possibly integer or real
    {
        int firstDigit;

        if (firstChar == '-')
        {
            if (firstString.size() < 2) throw std::runtime_error("Integer or real value expected ('-' found)");
            firstDigit = firstString[1] - '0';
        }
        else firstDigit = firstString[0] - '0';

        if ((firstDigit < 0) || (firstDigit > 9))
            throw std::runtime_error("'" + firstString + "' is not a valid integer or real");

        if (firstString.find('.') == string::npos) return Value(atoi(firstString.c_str()));
        return Value(strtod(firstString.c_str(), NULL));
    }
    }
}

template <typename T>
void getRecord(T &database, const Value &value1, const Value &value2)
{
    typename T::recordListPtr records;
    switch (value1.type())
    {
    case Value::NIL:
        records = database.allRecords();
        break;

    case Value::INTEGER:
        records = database.findRecords("id", value1.integerValue());
        break;

    default: throw std::runtime_error("Invalid search term ('" + toString(value1) + "' given)");
    }

    cout << endl;
    if (records->empty()) cout << "No records found" << endl;
    else for (unsigned i = 0; i < records->size(); ++i) cout << records->at(i);
}

template <typename T>
void doGet(T &database, DatabasePropertyEnum databaseProperty, const Value &value1, const Value &value2)
{
    switch (databaseProperty)
    {
    case ID_COUNTER:
        cout << "ID counter: " << database.getIdCounter() << " ";
        break;

    case FILENAME:
        cout << "File name: " << database.filename() << " ";
        break;

    case COUNT:
        cout << "Record count: " << database.recordCount() << " ";
        break;

    case RECORD:
        getRecord(database, value1, value2);
        break;

    default: throw std::runtime_error("Unknown database property");
    }
}

template <typename T>
void doSet(T &database, DatabasePropertyEnum databaseProperty, const Value &value1, const Value &value2)
{

}

void interpretInput(const QString &input)
{
    const QStringList keywords = input.toLower().split(' ', QString::SkipEmptyParts);
    if (keywords.size() < 3) throw std::runtime_error("Invalid command given");

    const ActionEnum action = getAction(keywords[0]);
    const DatabaseEnum database = getDatabase(keywords[1]);
    const DatabasePropertyEnum databaseProperty = getDatabaseProperty(keywords[2]);

    Value value1, value2;

    if (keywords.size() > 3)
    {
        const int commaIndex = keywords.indexOf(",", 4);
        if ((commaIndex > -1) && (commaIndex < keywords.size() - 1))
        {
            value1 = getValue(keywords.mid(3, commaIndex - 1));
            value2 = getValue(keywords.mid(commaIndex + 1));
        }
        else value1 = getValue(keywords.mid(3));
    }

    switch (database)
    {
    case CUSTOMER:
        cout << "Customer database ";
        if (action == GET) doGet(Databases::customers(), databaseProperty, value1, value2);
        else doSet(Databases::customers(), databaseProperty, value1, value2);
        break;

    case JOB:
        cout << "Job database ";
        if (action == GET) doGet(Databases::jobs(), databaseProperty, value1, value2);
        else doSet(Databases::jobs(), databaseProperty, value1, value2);
        break;

    case PART:
        cout << "Part database ";
        if (action == GET) doGet(Databases::parts(), databaseProperty, value1, value2);
        else doSet(Databases::parts(), databaseProperty, value1, value2);
        break;

    case TASK:
        cout << "Task database ";
        if (action == GET) doGet(Databases::tasks(), databaseProperty, value1, value2);
        else doSet(Databases::tasks(), databaseProperty, value1, value2);
        break;

    case EXPENSE:
        cout << "Expense database ";
        if (action == GET) doGet(Databases::expenses(), databaseProperty, value1, value2);
        else doSet(Databases::expenses(), databaseProperty, value1, value2);
        break;

    case VAT_RATE:
        cout << "VAT rate database ";
        if (action == GET) doGet(Databases::vatRates(), databaseProperty, value1, value2);
        else doSet(Databases::vatRates(), databaseProperty, value1, value2);

    default: throw std::runtime_error("Unknown database");
    }

    cout << endl;
}

void CommandLine::run()
{
    string input;
    while (true)
    {
        getline(cin, input);
        if ((input == "quit") || (input == "exit")) break;
        try { interpretInput(input.c_str()); } catch(const std::exception &e) { cout << e.what() << endl; }
        input.clear();
    }
}

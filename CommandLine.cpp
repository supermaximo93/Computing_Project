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

typedef map<string, Value> Object;

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
    if ((input == "record") || (input == "records")) return RECORD;
    throw std::runtime_error("Unknown database property '" + input.toStdString() + "'");
}

Value getValue(const QStringList &inputs);

Object getObject(QStringList inputs)
{
    Object returnObject;

    if (inputs.first().indexOf('{') == 0) inputs.first().remove(0, 1);
    if (inputs.last().lastIndexOf('}') == inputs.last().size() - 1) inputs.last().chop(1);

    QString property;
    for (int i = 0, count = 1; i < inputs.size(); ++i)
    {
        property = inputs[i];
        if (property.size() == 0) continue;

        int colonIndex = property.indexOf(':');
        if ((colonIndex < 1) || (colonIndex == property.size() - 1))
            throw std::runtime_error("Property " + toString(count)
                                     + " is invalid (colon in wrong place or not present)");

        QString propertyName = property.left(colonIndex),
                propertyValue = property.mid(colonIndex + 1);

        returnObject[propertyName.toStdString()] = getValue(propertyValue.split(' ', QString::SkipEmptyParts));

        ++count;
    }

    return returnObject;
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
        return Value(getObject(inputs));
        break;

    case 'n': // possibly nil
        if (firstString == "nil") return Value();
        // else fall through to default

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

    return Value();
}

template <typename T>
typename T::recordListPtr getRecords(T &database, const Value &value1, const Value &value2)
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

    case Value::OBJECT:
    {
        Object::const_iterator iterator = value1.objectValue().begin(), end = value1.objectValue().end();
        Value firstValue = iterator->second;
        switch (firstValue.type())
        {
        case Value::INTEGER: records = database.findRecords(iterator->first, firstValue.integerValue()); break;
        case Value::REAL: records = database.findRecords(iterator->first, firstValue.realValue()); break;
        default: throw std::runtime_error("Invalid object property value ('" + toString(firstValue) + "' given)");
        }

        if (iterator != end) ++iterator;
        for (; iterator != end; ++iterator)
        {
            switch (iterator->second.type())
            {
            case Value::INTEGER: database.keepRecords(*records, iterator->first, iterator->second.integerValue());break;
            case Value::REAL: database.keepRecords(*records, iterator->first, iterator->second.realValue()); break;
            default: throw std::runtime_error("Invalid object property value ('" + toString(firstValue) + "' given)");
            }
        }
        break;
    }

    default: throw std::runtime_error("Invalid search term ('" + toString(value1) + "' given)");
    }

    cout << endl;
    if (records->empty()) cout << "No records found" << endl;
    else
    {
        if (value2.type() == Value::INTEGER)
        {
            if (value2.integerValue() < (int)records->size())
            {
                cout << records->at(value2.integerValue());
                typename T::recordListPtr record(new typename T::recordList);
                record->push_back(records->at(value2.integerValue()));
                return record;
            }
            else cout << "No records found" << endl;
        }
        else for (unsigned i = 0; i < records->size(); ++i) cout << records->at(i);
    }

    return records;
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
        getRecords(database, value1, value2);
        break;

    default: throw std::runtime_error("Unknown database property");
    }
}

void CommandLine::setRecordValues(Customer &customer, const Value &object)
{
    Object::const_iterator iterator = object.objectValue().begin(), end = object.objectValue().end();

    string fieldName;
    for (; iterator != end; ++iterator)
    {
        fieldName = iterator->first;
        if (fieldName == "id")
        {
            if (iterator->second.type() == Value::INTEGER) customer.id = iterator->second.integerValue();
            else throw std::runtime_error("ID value must be an integer");
        }

        //TODO: the other fields
    }

    Databases::customers().updateRecordAtPosition(customer);
}

void CommandLine::setRecordValues(Job &job, const Value &object)
{
    Object::const_iterator iterator = object.objectValue().begin(), end = object.objectValue().end();

    string fieldName;
    for (; iterator != end; ++iterator)
    {
        fieldName = iterator->first;
        if (fieldName == "id")
        {
            if (iterator->second.type() == Value::INTEGER) job.id = iterator->second.integerValue();
            else throw std::runtime_error("ID value must be an integer");
        }

        //TODO: the other fields
    }

    Databases::jobs().updateRecordAtPosition(job);
}

void CommandLine::setRecordValues(Part &part, const Value &object)
{
    Object::const_iterator iterator = object.objectValue().begin(), end = object.objectValue().end();

    string fieldName;
    for (; iterator != end; ++iterator)
    {
        fieldName = iterator->first;
        if (fieldName == "id")
        {
            if (iterator->second.type() == Value::INTEGER) part.id = iterator->second.integerValue();
            else throw std::runtime_error("ID value must be an integer");
        }

        //TODO: the other fields
    }

    Databases::parts().updateRecordAtPosition(part);
}

void CommandLine::setRecordValues(Task &task, const Value &object)
{
    Object::const_iterator iterator = object.objectValue().begin(), end = object.objectValue().end();

    string fieldName;
    for (; iterator != end; ++iterator)
    {
        fieldName = iterator->first;
        if (fieldName == "id")
        {
            if (iterator->second.type() == Value::INTEGER) task.id = iterator->second.integerValue();
            else throw std::runtime_error("ID value must be an integer");
        }

        //TODO: the other fields
    }

    Databases::tasks().updateRecordAtPosition(task);
}

void CommandLine::setRecordValues(Expense &expense, const Value &object)
{
    Object::const_iterator iterator = object.objectValue().begin(), end = object.objectValue().end();

    string fieldName;
    for (; iterator != end; ++iterator)
    {
        fieldName = iterator->first;
        if (fieldName == "id")
        {
            if (iterator->second.type() == Value::INTEGER) expense.id = iterator->second.integerValue();
            else throw std::runtime_error("ID value must be an integer");
        }

        //TODO: the other fields
    }

    Databases::expenses().updateRecordAtPosition(expense);
}

void CommandLine::setRecordValues(VatRate &vatRate, const Value &object)
{
    Object::const_iterator iterator = object.objectValue().begin(), end = object.objectValue().end();

    string fieldName;
    for (; iterator != end; ++iterator)
    {
        fieldName = iterator->first;
        if (fieldName == "id")
        {
            if (iterator->second.type() == Value::INTEGER) vatRate.id = iterator->second.integerValue();
            else throw std::runtime_error("ID value must be an integer");
        }

        //TODO: the other fields
    }

    Databases::vatRates().updateRecordAtPosition(vatRate);
}

template <typename T>
void doSet(T &database, DatabasePropertyEnum databaseProperty, const Value &value1, const Value &value2)
{
    string input;
    switch (databaseProperty)
    {
    case ID_COUNTER:
        cout << "ID counter: " << database.getIdCounter() << endl
             << "Set to: ";
        getline(cin, input);
        if ((input == "quit") || (input == "exit")) break;
        database.setIdCounter(atoi(input.c_str()));
        break;

    case FILENAME:
        cout << "File name: " << database.filename() << endl
             << "File name setting is forbidden" << endl;
        break;

    case COUNT:
        cout << "Record count: " << database.recordCount() << endl
             << "Record count setting is forbidden" << endl;
        break;

    case RECORD:
    {
        typename T::recordListPtr records = getRecords(database, value1, value2);
        if (records->empty()) break;
        else if (records->size() > 1) cout << "Warning! Mass assignment!" << endl;

        cout << "Set to: ";
        getline(cin, input);
        if ((input == "quit") || (input == "exit")) break;

        Value object = getValue(QString(input.c_str()).split(' ', QString::SkipEmptyParts));
        if (object.type() != Value::OBJECT) throw std::runtime_error("Object expected");

        for (unsigned i = 0; i < records->size(); ++i) CommandLine::setRecordValues(records->at(i), object);

        break;
    }

    default: throw std::runtime_error("Unknown database property");
    }
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
            value1 = getValue(keywords.mid(3, commaIndex - 3));
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

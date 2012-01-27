/*
 * Database.h
 *
 *  Created on: 26 Nov 2011
 *      Author: Max Foster
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <tr1/memory>
#include <tr1/shared_ptr.h>

#include "Utils.h"

template<class recordType>
class Database
{
public:
    typedef std::vector<recordType> recordList;
    typedef std::tr1::shared_ptr<recordList> recordListPtr;

#ifdef COMPILE_TESTS
    Database(bool testing = false);
#else
    Database();
#endif
    ~Database();

    /*
    * Adds a new record to the database, and sets the record's ID. Throws an exception if the record does not pass its
    * validation (which should be done before addRecord is called anyway), or if the database file could not be opened
    */
    void addRecord(recordType & record);

    /*
    * Updates an existing record that has been found from the database or has just been added to it (i.e. the ID of the
    * record is greater than -1). If the record was updated successfully, then true is returned. An exception is thrown
    * if the record is a null record (i.e. ID < 0), or if the database file could not be opened
    */
    bool updateRecord(const recordType & record);

    /*
    * Attempts to delete the record with the specified ID (if any), returning true on success. Throws an exception if
    * the database file could not be opened
    */
    bool deleteRecord(const int id);

    /*
    * Attempts to find the first record whose field specified matches the search term. Returns a null record
    * (i.e. ID < 0) if a record could not be found. Throws an exception if the database file could not be opened
    */
    template<typename type>
    recordType findRecord(const std::string & fieldName, const type & searchTerm);

    /*
    * Finds all the records whose field specified matches the search term, and returns them in a recordList (which may
    * be empty if no matching records where found). Throws an exception if the database file could not be opened
    */
    template<typename type>
    recordListPtr findRecords(const std::string & fieldName, const type & searchTerm);

    /*
    * Finds all the records whose field specified matches the search term from the recordList given, and returns them
    * in a recordList (which may be empty if no matching records where found)
    */
    template<typename type>
    recordListPtr findRecords(const recordList & recordsToSearch, const std::string & fieldName,
                              const type & searchTerm);

    /*
    * Removes all the records from the recordList given whose field specified does not match the search term given
    */
    template<typename type>
    void keepRecords(recordList & records, const std::string & fieldName, const type & searchTerm);

    /*
    * Removes all the records from the recordList given whose field specified matches the search term given
    */
    template<typename type>
    void removeRecords(recordList & records, const std::string & fieldName, const type & searchTerm);

    /*
    * Returns all of the records in the database. Throws an exception if the database file could not be opened
    */
    recordListPtr allRecords();

    /*
    * Returns the record at a particular position of the database file, given by an index. Throws an exception if the
    * database file could not be opened
    */
    recordType recordAt(const int index);

    /*
    * Returns the number of records in the database. Throws an exception if the database file could not be opened
    */
    unsigned recordCount();

private:
    std::string filename;
    int idCounter;
};

template<class recordType>
#ifdef COMPILE_TESTS
Database<recordType>::Database(bool testing)
#else
Database<recordType>::Database()
#endif
{
    filename = recordType::databaseFilename;

#ifdef COMPILE_TESTS
    if (testing) filename += ".test";
#endif

    std::ifstream file;
    file.open(filename.c_str(), std::ios::binary);
    if (file.is_open())
    {
        file.seekg(0, std::ios_base::beg);
        file.read(reinterpret_cast<char *>(&idCounter), sizeof(idCounter));
        file.close();
    }
    else
    {
        idCounter = 0;
        std::ofstream file;
        file.open(filename.c_str(), std::ios::binary);
        if (file.is_open())
        {
            file.write(reinterpret_cast<const char *>(&idCounter), sizeof(idCounter));
            file.close();
        }
        else std::cout << "Could not create " + filename << std::endl;
    }
}

template<class recordType>
Database<recordType>::~Database()
{
    std::fstream newFile;
    newFile.open(("temp_" + filename).c_str(), std::ios::out | std::ios::binary);
    if (newFile.is_open())
    {
        newFile.write(reinterpret_cast<const char *>(&idCounter), sizeof(idCounter));

        bool fileCopied = false;
        recordType tempRecord;
        std::fstream file;
        file.open(filename.c_str(), std::ios::in | std::ios::binary);
        if (file.is_open())
        {
            file.seekg(sizeof(idCounter), std::ios_base::beg);

            while (true)
            {
                tempRecord.readFromFile(file);
                if (file.eof()) break;
                if (!tempRecord.null()) tempRecord.writeToFile(newFile);
            }
            fileCopied = true;
            file.close();
        }
        else std::cout << "Could not open file " + filename << std::endl;
        newFile.close();

        if (fileCopied)
        {
            remove(filename.c_str());
            rename(("temp_" + filename).c_str(), filename.c_str());
        }
        else remove(("temp_" + filename).c_str());
    }
    else std::cout << "Could not create temporary file temp_" + filename << std::endl;
}

template<class recordType>
void Database<recordType>::addRecord(recordType & record)
{
    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file.seekp(0, std::ios_base::end);
        record.id = idCounter++;
        record.writeToFile(file);
        file.close();
    }
    else throw(std::runtime_error("Could not open file " + filename));
}

template<class recordType>
bool Database<recordType>::updateRecord(const recordType & record)
{
    if (record.null()) throw(std::runtime_error("Record ID must be initialised"));

    recordType tempRecord;
    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file.seekg(sizeof(idCounter), std::ios_base::beg);

        while (true)
        {
            tempRecord.readFromFile(file);
            if (file.eof()) break;
            if (tempRecord == record)
            {
                file.seekp(-recordType::size(), std::ios::cur);
                record.writeToFile(file);
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }
    else throw(std::runtime_error("Could not open file " + filename));
}

template<class recordType>
bool Database<recordType>::deleteRecord(const int id)
{
    recordType tempRecord;
    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file.seekg(sizeof(idCounter), std::ios_base::beg);

        while (true)
        {
            tempRecord.readFromFile(file);
            if (file.eof()) break;
            if (tempRecord.getId() == id)
            {
                file.seekp(-recordType::size(), std::ios::cur);
                recordType().writeToFile(file);
                file.close();
                return true;
            }
        }
        file.close();
    }
    else throw(std::runtime_error("Could not open file " + filename));

    std::cout << "Record could not be found. No records were deleted" << std::endl;
    return false;
}

template<class recordType> template<typename type>
recordType Database<recordType>::findRecord(const std::string & fieldName, const type & searchTerm)
{
    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        std::string lowercaseFieldName = lowerCase(fieldName);
        recordType tempRecord;

        file.seekg(sizeof(idCounter), std::ios_base::beg);

        while (true)
        {
            tempRecord.readFromFile(file);
            if (file.eof()) break;
            if (tempRecord.hasMatchingField(lowercaseFieldName, searchTerm))
            {
                file.close();
                return tempRecord;
            }
        }
        file.close();
    }
    else throw(std::runtime_error("Could not open file " + filename));

    return recordType();
}

template<class recordType> template <typename type>
std::tr1::shared_ptr< std::vector<recordType> >
Database<recordType>::findRecords(const std::string & fieldName, const type & searchTerm)
{
    recordListPtr returnList(new recordList);

    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        std::string lowercaseFieldName = lowerCase(fieldName);
        recordType tempRecord;

        file.seekg(sizeof(idCounter), std::ios_base::beg);

        while (true)
        {
            tempRecord.readFromFile(file);
            if (file.eof()) break;
            if (tempRecord.hasMatchingField(lowercaseFieldName, searchTerm)) returnList->push_back(tempRecord);
        }
        file.close();
    }
    else throw(std::runtime_error("Could not open file " + filename));

    return returnList;
}

template<class recordType> template<typename type>
std::tr1::shared_ptr< std::vector<recordType> >
Database<recordType>::findRecords(const std::vector<recordType> & recordsToSearch, const std::string & fieldName,
                                  const type & searchTerm)
{
    std::string lowercaseFieldName = lowerCase(fieldName);
    recordListPtr returnList(new recordList);

    for (unsigned i = 0; i < recordsToSearch.size(); ++i)
    {
        if (recordsToSearch[i].hasMatchingField(lowercaseFieldName, searchTerm))
            returnList->push_back(recordsToSearch[i]);
    }

    return returnList;
}

template<class recordType> template<typename type>
void Database<recordType>::keepRecords(std::vector<recordType> & records, const std::string & fieldName,
                                       const type & searchTerm)
{
    std::string lowercaseFieldName = lowerCase(fieldName);

    for (unsigned i = 0; i < records.size(); ++i)
    {
        if (!records[i].hasMatchingField(lowercaseFieldName, searchTerm)) records.erase(records.begin() + i);
    }
}

template <class recordType> template<typename type>
void Database<recordType>::removeRecords(std::vector<recordType> & records, const std::string & fieldName,
                                         const type & searchTerm)
{
    std::string lowercaseFieldName = lowerCase(fieldName);

    for (unsigned i = 0; i < records.size(); ++i)
    {
        if (records[i].hasMatchingField(lowercaseFieldName, searchTerm))
            records.erase(records.begin() + i);
    }
}

template<class recordType>
std::tr1::shared_ptr< std::vector<recordType> > Database<recordType>::allRecords()
{
    recordListPtr returnList(new recordList);

    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        recordType tempRecord;

        file.seekg(sizeof(idCounter), std::ios_base::beg);

        while (true)
        {
            tempRecord.readFromFile(file);
            if (file.eof()) break;
            returnList->push_back(tempRecord);
        }
        file.close();
    }
    else throw(std::runtime_error("Could not open file " + filename));

    return returnList;
}

template<class recordType>
recordType Database<recordType>::recordAt(const int index)
{
    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        recordType tempRecord;

        file.seekg(0, std::ios_base::end);
        unsigned size = file.tellg();

        if (sizeof(idCounter) + (recordType::size() * (index + 1)) > size) // index + 1 because index starts from 0
        {
            file.close();
            return recordType();
        }

        file.seekg(sizeof(idCounter) + (recordType::size() * index), std::ios_base::beg);
        tempRecord.readFromFile(file);

        file.close();
        return tempRecord;
    }
    else throw(std::runtime_error("Could not open file " + filename));

    return recordType();
}

template<class recordType>
unsigned Database<recordType>::recordCount()
{
    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        file.seekg(0, std::ios_base::end);
        unsigned size = (unsigned)file.tellg() - sizeof(idCounter);
        file.close();

        return size / recordType::size();
    }
    else throw(std::runtime_error("Could not open file " + filename));

    return 0;
}

#endif /* DATABASE_H_ */

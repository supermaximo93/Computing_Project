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

#include <SuperMaximo_GameLibrary/Utils.h>

template<class recordType>
class Database
{
public:
    Database(bool testing = false);
    ~Database();

    void addRecord(recordType & record);

    template<typename type>
    std::vector<recordType> * findRecords(const std::string & fieldName, const type & searchTerm);

    template<typename type>
    std::vector<recordType> * findRecords(const std::vector<recordType> & recordsToSearch,
                                          const std::string & fieldName, const type & searchTerm);

    template<typename type>
    void keepRecords(std::vector<recordType> & records, const std::string & fieldName, const type & searchTerm);

    template<typename type>
    void removeRecords(std::vector<recordType> & records, const std::string & fieldName, const type & searchTerm);

    template<typename type>
    recordType findRecord(const type & searchTerm);

    void updateRecord(const recordType & record);

    template<typename type>
    void deleteRecord(const type & searchTerm);

    std::vector<recordType> * allRecords();

    recordType recordAt(const int index);

    unsigned recordCount();

private:
    std::string filename;
    int idCounter;
};

template<class recordType>
Database<recordType>::Database(bool testing)
{
    filename = recordType::databaseFilename;
    if (testing) filename += ".test";

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
    else std::cout << "Could not open file " + filename << std::endl;
}

template<class recordType> template <typename type>
std::vector<recordType> * Database<recordType>::findRecords(const std::string & fieldName, const type & searchTerm)
{
    std::string lowercaseFieldName = SuperMaximo::lowerCase(fieldName);
    std::vector<recordType> * returnVector = new std::vector<recordType>;

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
            if (tempRecord.hasMatchingField(lowercaseFieldName, searchTerm)) returnVector->push_back(tempRecord);
        }
        file.close();
    }
    else std::cout << "Could not open file " + filename << std::endl;

    return returnVector;
}

template<class recordType> template<typename type>
std::vector<recordType> * Database<recordType>::findRecords(const std::vector<recordType> & recordsToSearch,
                                                            const std::string & fieldName, const type & searchTerm)
{
    std::string lowercaseFieldName = SuperMaximo::lowerCase(fieldName);
    std::vector<recordType> * returnVector = new std::vector<recordType>;

    for (unsigned i = 0; i < recordsToSearch.size(); ++i)
    {
        if (recordsToSearch[i].hasMatchingField(lowercaseFieldName, searchTerm))
            returnVector->push_back(recordsToSearch[i]);
    }

    return returnVector;
}

template<class recordType> template<typename type>
void Database<recordType>::keepRecords(std::vector<recordType> & records, const std::string & fieldName,
                                       const type & searchTerm)
{
    std::string lowercaseFieldName = SuperMaximo::lowerCase(fieldName);

    for (unsigned i = 0; i < records.size(); ++i)
    {
        if (!records[i].hasMatchingField(lowercaseFieldName, searchTerm)) records.erase(records.begin() + i);
    }
}

template <class recordType> template<typename type>
void Database<recordType>::removeRecords(std::vector<recordType> & records, const std::string & fieldName,
                                         const type & searchTerm)
{
    std::string lowercaseFieldName = SuperMaximo::lowerCase(fieldName);

    for (unsigned i = 0; i < records.size(); ++i)
    {
        if (records[i].hasMatchingField(lowercaseFieldName, searchTerm))
            records.erase(records.begin() + i);
    }
}

template<class recordType> template<typename type>
recordType Database<recordType>::findRecord(const type & searchTerm)
{
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
            if (tempRecord == searchTerm)
            {
                file.close();
                return tempRecord;
            }
        }
        file.close();
    }
    else std::cout << "Could not open file " + filename << std::endl;
    return recordType();
}

template<class recordType>
void Database<recordType>::updateRecord(const recordType & record)
{
    if (record.null())
    {
        std::cout << "Record ID must be initialised" << std::endl;
        return;
    }

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
            if (tempRecord == record) {
                file.seekp(-recordType::size(), std::ios::cur);
                record.writeToFile(file);
                file.close();
                return;
            }
        }
        file.close();
    }
    else std::cout << "Could not open file " + filename << std::endl;
}

template<class recordType> template<typename type>
void Database<recordType>::deleteRecord(const type & searchTerm)
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
            if (tempRecord == searchTerm)
            {
                file.seekp(-recordType::size(), std::ios::cur);
                recordType().writeToFile(file);
                file.close();
                return;
            }
        }
        file.close();
    }
    else std::cout << "Could not open file " + filename << std::endl;
    std::cout << "Record could not be found. No records were deleted" << std::endl;
}

template<class recordType>
std::vector<recordType> * Database<recordType>::allRecords()
{
    std::vector<recordType> * returnVector = new std::vector<recordType>;
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
            returnVector->push_back(tempRecord);
        }
        file.close();
    }
    else std::cout << "Could not open file " + filename << std::endl;

    return returnVector;
}

template<class recordType>
recordType Database<recordType>::recordAt(const int index)
{
    recordType tempRecord;

    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        file.seekg(0, std::ios_base::end);
        unsigned size = file.tellg();
        if (sizeof(idCounter) + (recordType::size() * (index + 1)) > size) // index + 1 because index starts from 0
        {
            file.close();
            return tempRecord;
        }

        file.seekg(sizeof(idCounter) + (recordType::size() * index), std::ios_base::beg);
        tempRecord.readFromFile(file);

        file.close();
    }
    else std::cout << "Could not open file " + filename << std::endl;

    return tempRecord;
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
    else std::cout << "Could not open file " + filename << std::endl;

    return 0;
}

#endif /* DATABASE_H_ */

/*
 * VatRateController_testing.cc
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include "Databases.h"
#include "VatRateController.h"
#include "VatRate.h"

class VatRateControllerIntegrationTest : public ::testing::Test
{
protected:
    const VatRate exampleVatRate;

    VatRateControllerIntegrationTest()
        : exampleVatRate(20, time(NULL)) {}

    virtual void SetUp()
    {
        Databases::init(true);

        VatRate vatRate(exampleVatRate);
        for (unsigned i = 0; i < 20; ++i)
        {
            vatRate.setValue(vatRate.getValue() + 1.0);
            vatRate.setStartDate(vatRate.getStartDate() + 86400);
            Databases::vatRates().addRecord(vatRate);
        }
    }

    virtual void TearDown()
    {
        std::string customersFilename = Databases::customers().filename(),
                jobsFilename = Databases::jobs().filename(),
                partsFilename = Databases::parts().filename(),
                tasksFilename = Databases::tasks().filename(),
                expensesFilename = Databases::expenses().filename(),
                settingsFilename = Databases::settings().filename(),
                vatRateFilename = Databases::vatRates().filename();

        Databases::finalise();

        remove(customersFilename.c_str());
        remove(jobsFilename.c_str());
        remove(partsFilename.c_str());
        remove(tasksFilename.c_str());
        remove(expensesFilename.c_str());
        remove(settingsFilename.c_str());
        remove(vatRateFilename.c_str());
    }
};

// Does Create Work
TEST_F(VatRateControllerIntegrationTest, DoesCreateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::vatRates().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    VatRate vatRate(exampleVatRate);
    EXPECT_NO_THROW(VatRateController::Create(vatRate, NULL))
            << "The VatRate Controller did not catch an exception";

    EXPECT_FALSE(vatRate.null())
            << "The vat rate was not added to the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::vatRates().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore + 1, recordCountAfter)
            << "The vat rate was not added to the database properly";
}

// Does Update Work
TEST_F(VatRateControllerIntegrationTest, DoesUpdateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::vatRates().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    VatRate vatRate;
    try
    {
        vatRate = Databases::vatRates().recordAt(0);
        vatRate.setValue(vatRate.getValue() + 10.0);
    }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(VatRateController::Update(vatRate, NULL))
            << "The VatRate Controller did not catch an exception";

    VatRate tempVatRate;
    try { tempVatRate = Databases::vatRates().recordAt(0); } catch (const std::exception &e) { FAIL() << e.what(); }
    EXPECT_TRUE(vatRate.completeCompare(tempVatRate))
            << "The vat rate was not updated in the database correctly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::vatRates().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore, recordCountAfter)
            << "A record was added to the database while updating an existing vat rate";
}

// Does Destroy Work
TEST_F(VatRateControllerIntegrationTest, DoesDestroyWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::vatRates().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    VatRate vatRate;
    try { vatRate = Databases::vatRates().recordAt(0); } catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(VatRateController::Destroy(vatRate, NULL))
            << "The VatRate Controller did not catch an exception";

    EXPECT_TRUE(vatRate.null())
            << "The vat rate was not removed from the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::vatRates().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore - 1, recordCountAfter)
            << "The vat rate was not removed from the database properly";
}

#endif // COMPILE_TESTS

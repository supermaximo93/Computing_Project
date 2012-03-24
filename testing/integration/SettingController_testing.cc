/*
 * SettingController_testing.cc
 *
 *  Created on: 23 March 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include "Databases.h"
#include "SettingController.h"
#include "Setting.h"

class SettingControllerIntegrationTest : public ::testing::Test
{
protected:
    const Setting exampleSetting;

    SettingControllerIntegrationTest()
        : exampleSetting("ExampleSetting", "value") {}

    virtual void SetUp()
    {
        Databases::init(true);

        Setting setting(exampleSetting);
        for (unsigned i = 0; i < 20; ++i)
        {
            setting.setKey((exampleSetting.getKey() + toString(i)).c_str());
            setting.setValue((exampleSetting.getValue() + toString(i)).c_str());
            Databases::settings().addRecord(setting);
        }
    }

    virtual void TearDown()
    {
        std::string customersFilename = Databases::customers().filename(),
                jobsFilename = Databases::jobs().filename(),
                partsFilename = Databases::parts().filename(),
                tasksFilename = Databases::tasks().filename(),
                expensesFilename = Databases::expenses().filename(),
                settingsFilename = Databases::settings().filename();

        Databases::finalise();

        remove(customersFilename.c_str());
        remove(jobsFilename.c_str());
        remove(partsFilename.c_str());
        remove(tasksFilename.c_str());
        remove(expensesFilename.c_str());
        remove(settingsFilename.c_str());
    }
};

// Does Create Work
TEST_F(SettingControllerIntegrationTest, DoesCreateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::settings().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Setting setting(exampleSetting);
    EXPECT_NO_THROW(SettingController::Create(setting, NULL))
            << "The Setting Controller did not catch an exception";

    EXPECT_FALSE(setting.null())
            << "The setting was not added to the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::settings().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore + 1, recordCountAfter)
            << "The setting was not added to the database properly";
}

// Does Update Work
TEST_F(SettingControllerIntegrationTest, DoesUpdateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::settings().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Setting setting;
    try
    {
        setting = Databases::settings().recordAt(0);
        setting.setKey("Updated key");
    }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(SettingController::Update(setting, NULL))
            << "The Setting Controller did not catch an exception";

    Setting tempSetting;
    try { tempSetting = Databases::settings().recordAt(0); } catch (const std::exception &e) { FAIL() << e.what(); }
    EXPECT_TRUE(setting.completeCompare(tempSetting))
            << "The setting was not updated in the database correctly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::settings().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore, recordCountAfter)
            << "A record was added to the database while updating an existing setting";
}

// Does Destroy Work
TEST_F(SettingControllerIntegrationTest, DoesDestroyWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::settings().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Setting setting;
    try { setting = Databases::settings().recordAt(0); } catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(SettingController::Destroy(setting, NULL))
            << "The Setting Controller did not catch an exception";

    EXPECT_TRUE(setting.null())
            << "The setting was not removed from the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::settings().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore - 1, recordCountAfter)
            << "The setting was not removed from the database properly";
}

#endif // COMPILE_TESTS


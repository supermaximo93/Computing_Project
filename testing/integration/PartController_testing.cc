/*
 * PartController_testing.cc
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include "Databases.h"
#include "PartController.h"
#include "Part.h"
#include "Job.h"

class PartControllerIntegrationTest : public ::testing::Test
{
protected:
    const Part examplePart;
    const Job exampleJob;

    PartControllerIntegrationTest()
        : examplePart(0, "Tap", "12345", 20.0, 20.0),
          exampleJob(0, time(NULL) + 100000, 50.0, Job::DONE_UNPAID, Job::NA) {}

    virtual void SetUp()
    {
        Databases::init(true);
        Job job(exampleJob);
        Databases::jobs().addRecord(job);

        Part part(examplePart);
        for (unsigned i = 0; i < 20; ++i)
        {
            part.setName((examplePart.getName() + toString(i)).c_str());
            part.setPrice(part.getPrice() + 10.0);
            Databases::parts().addRecord(part);
        }
    }

    virtual void TearDown()
    {
        std::string jobsFilename = Databases::jobs().filename(),
                partsFilename = Databases::parts().filename();

        Databases::finalise();
        remove(jobsFilename.c_str());
        remove(partsFilename.c_str());
    }
};

// Does Create Work
TEST_F(PartControllerIntegrationTest, DoesCreateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::parts().recordCount(); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    Part part(examplePart);
    EXPECT_NO_THROW(PartController::Create(part, NULL))
            << "The Part Controller did not catch an exception";

    EXPECT_FALSE(part.null())
            << "The part was not added to the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::parts().recordCount(); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore + 1, recordCountAfter)
            << "The part was not added to the database properly";
}

// Does Update Work
TEST_F(PartControllerIntegrationTest, DoesUpdateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::parts().recordCount(); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    Part part;
    try
    {
        part = Databases::parts().recordAt(0);
        part.setPrice(part.getPrice() + 10.0);
    }
    catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(PartController::Update(part, NULL))
            << "The Part Controller did not catch an exception";

    Part tempPart;
    try { tempPart = Databases::parts().recordAt(0); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_TRUE(part.completeCompare(tempPart))
            << "The part was not updated in the database correctly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::parts().recordCount(); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore, recordCountAfter)
            << "A record was added to the database while updating an existing part";
}

// Does Destroy Work
TEST_F(PartControllerIntegrationTest, DoesDestroyWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::parts().recordCount(); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    Part part;
    try { part = Databases::parts().recordAt(0); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(PartController::Destroy(part, NULL))
            << "The Part Controller did not catch an exception";

    EXPECT_TRUE(part.null())
            << "The part was not removed from the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::parts().recordCount(); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore - 1, recordCountAfter)
            << "The part was not removed from the database properly";
}

#endif // COMPILE_TESTS

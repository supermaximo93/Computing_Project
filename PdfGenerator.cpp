/*
 * PdfGenerator.cpp
 *
 *  Created on: 29 Feb 2012
 *      Author: Max Foster
 */

#include <cstring>
using namespace std;

#include <hpdf.h>

#include "PdfGenerator.h"

#include "Databases.h"
#include "Database.h"
#include "Job.h"
#include "Customer.h"
#include "CustomerController.h"

struct ErrorData
{
    static const short callerLength = 127;
    char caller[callerLength + 1];
    bool errorOccured;
    unsigned stage;
    ErrorData(const char * caller_) : errorOccured(false), stage(0) { strncpy(caller, caller_, callerLength); }
};

void errorHandler(HPDF_STATUS errorNumber, HPDF_STATUS detailNumber, void * userData)
{
    cout << "Error with generating PDF in function PdfGenerator::" << reinterpret_cast<const char *>(userData) << endl
         << "Stage number:  " << static_cast<ErrorData*>(userData)->stage << endl
         << "Error number:  " << (unsigned int)errorNumber << endl
         << "Detail number: " << (int)detailNumber << endl;

    static_cast<ErrorData*>(userData)->errorOccured = true;
}

#define CHECK_FOR_ERROR() \
    if (errorData.errorOccured) { HPDF_Free(pdf); return false; } ++errorData.stage;

#define Y(value) \
    pageHeight - (value)

#define X(value) \
    (value)         //for consistency

bool drawHeader(HPDF_Doc &pdf, HPDF_Page &page, const Customer &customer, ErrorData &errorData)
{
    float pageWidth = HPDF_Page_GetWidth(page), pageHeight = HPDF_Page_GetHeight(page);

    // draw heading
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica-Bold", NULL);
    CHECK_FOR_ERROR();
    HPDF_Page_SetFontAndSize(page, font, 24);
    CHECK_FOR_ERROR();
    HPDF_Page_TextOut(page, X(50), Y(50), "INVOICE");
    CHECK_FOR_ERROR();

    // draw company details
    {
        HPDF_Page_SetFontAndSize(page, font, 10);
        HPDF_Page_TextOut(page, X(50), Y(90), "Company details");
        CHECK_FOR_ERROR();

        font = HPDF_GetFont(pdf, "Courier", NULL);
        CHECK_FOR_ERROR();
        HPDF_Page_SetFontAndSize(page, font, 8);
        CHECK_FOR_ERROR();

        const char *column1Data[] = {
            "           Name:",
            "Registration No:",
            "         VAT No:",
            "        Address:",
            "",
            "",
            "       Postcode:",
            "      Telephone:"
        };
        const char *column2Data[] = {
            "Ian Foster Services",
            "305566",
            "918 2496 02",
            "96 Bradshaw Road",
            "Watford",
            "Herts",
            "WD24 4DF",
            "01923 238 172 / 07828 679 589"
        };

        const unsigned arrayLength = sizeof(column1Data) / sizeof(const char *);
        for (unsigned i = 0; i < arrayLength; ++i)
        {
            HPDF_Page_TextOut(page, X(30), Y(110 + i * 12), column1Data[i]);
            CHECK_FOR_ERROR();
            HPDF_Page_TextOut(page, X(120), Y(110 + i * 12), column2Data[i]);
            CHECK_FOR_ERROR();
        }

    }

    const unsigned CUSTOMER_DETAIL_PADDING_X = 250;
    // draw customer details
    {
        font = HPDF_GetFont(pdf, "Helvetica-Bold", NULL);
        HPDF_Page_SetFontAndSize(page, font, 10);
        HPDF_Page_TextOut(page, X(CUSTOMER_DETAIL_PADDING_X + 50), Y(90), "Customer details");
        CHECK_FOR_ERROR();

        font = HPDF_GetFont(pdf, "Courier", NULL);
        CHECK_FOR_ERROR();
        HPDF_Page_SetFontAndSize(page, font, 8);
        CHECK_FOR_ERROR();

        const char *column1Data[] = {
            "           Name:",
            "        Address:",
            "",
            "",
            "       Postcode:",
            "      Telephone:",
            "  Email Address:"
        };

        char telephoneString[32];
        strcpy(telephoneString, customer.getHomePhoneNumber());
        if (strcmp(customer.getMobilePhoneNumber(), "") != 0)
        {
            strcat(telephoneString, " / ");
            strcat(telephoneString, customer.getMobilePhoneNumber());
        }

        const char *column2Data[] = {
            createFullName(customer.getForename(), customer.getSurname()),
            customer.getAddressLine1(),
            customer.getAddressLine2(),
            customer.getTown(),
            customer.getPostcode(),
            telephoneString,
            customer.getEmailAddress()
        };

        const unsigned arrayLength = sizeof(column1Data) / sizeof(const char *);
        for (unsigned i = 0; i < arrayLength; ++i)
        {
            HPDF_Page_TextOut(page, X(CUSTOMER_DETAIL_PADDING_X + 30), Y(110 + i * 12), column1Data[i]);
            CHECK_FOR_ERROR();
            HPDF_Page_TextOut(page, X(CUSTOMER_DETAIL_PADDING_X + 120), Y(110 + i * 12), column2Data[i]);
            CHECK_FOR_ERROR();
        }
    }

    return true;
}

bool PdfGenerator::generateInvoice(const char *fileName, const Job &job)
{
    ErrorData errorData("generateInvoice");

    HPDF_Doc pdf;
    pdf = HPDF_New(errorHandler, &errorData);
    if (pdf == NULL) return false; // Error handler should already have been called to output an error

    ++errorData.stage;
    HPDF_Page page = HPDF_AddPage(pdf);
    CHECK_FOR_ERROR();

    HPDF_Page_BeginText(page);
    CHECK_FOR_ERROR();

    Customer customer = CustomerController::getCustomer(job.getCustomerId());
    if (!drawHeader(pdf, page, customer, errorData)) return false;

    HPDF_Page_EndText(page);
    CHECK_FOR_ERROR();

    HPDF_SaveToFile(pdf, fileName);
    CHECK_FOR_ERROR();

    HPDF_Free(pdf);
    return true;
}

bool PdfGenerator::generateReciept(const char *fileName, const Job &job)
{
    return false;
}

bool PdfGenerator::generateReport(const char *fileName)
{
    return false;
}

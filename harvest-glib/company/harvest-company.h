#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

#include "harvest-glib/http/api/harvest-api-client.h"
#include "harvest-glib/http/response/harvest-response.h"

G_BEGIN_DECLS

#define HARVEST_TYPE_COMPANY (harvest_company_get_type())
G_DECLARE_FINAL_TYPE(HarvestCompany, harvest_company, HARVEST, COMPANY, GObject)

typedef enum
{
	HARVEST_WEEK_START_DAY_SATURDAY,
	HARVEST_WEEK_START_DAY_SUNDAY,
	HARVEST_WEEK_START_DAY_MONDAY,
} HarvestWeekStartDay;

typedef enum
{
	HARVEST_TIME_FORMAT_DECIMAL,
	HARVEST_TIME_FORMAT_HOURS_MINUTES,
} HarvestTimeFormat;

typedef enum
{
	HARVEST_PLAN_TYPE_TRIAL,
	HARVEST_PLAN_TYPE_FREE,
	HARVEST_PLAN_TYPE_SIMPLE_V4,
	HARVEST_PLAN_TYPE_BUSINESS,
} HarvestPlanType;

typedef enum
{
	HARVEST_CLOCK_12H,
	HARVEST_CLOCK_24H,
} HarvestClock;

const char *harvest_company_get_full_domain(HarvestCompany *self);
const char *harvest_company_get_name(HarvestCompany *self);

HarvestResponse *harvest_company_get_company(HarvestApiClient *client) G_GNUC_WARN_UNUSED_RESULT;
void harvest_company_get_company_async(
	HarvestApiClient *client, HarvestCompletedCallback *callback, gpointer user_data);

G_END_DECLS

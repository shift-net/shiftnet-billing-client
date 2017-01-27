#ifndef GLOBAL_H
#define GLOBAL_H

#define SNBC_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

#define SNBC_VERSION_MAJOR  1
#define SNBC_VERSION_MINOR  0
#define SNBC_VERSION_PATCH  0
#define SNBC_VERSION        SNBC_VERSION_CHECK(SNBC_VERSION_MAJOR, SNBC_VERSION_MINOR, SNBC_VERSION_PATCH)
#define SNBC_VERSION_STR    "1.0.0"

#define SNBC_APP_NAME         "ShiftNetBillingClient"
#define SNBC_APP_DISPLAY_NAME "Shift Net Billing Client"

#define SNBC_SETTINGS_PATH SNBC_APP_NAME ".ini"

#define SNBC_SK_SERVER_ADDRESS  "Server/address"
#define SNBC_SK_SERVER_PORT     "Server/port"

#define SNBC_SK_CLIENT_ID        "Client/id"
#define SNBC_SK_CLIENT_PASSWORD  "Client/password"

#define SNBC_SK_COMPANY_NAME      "Company/name"
#define SNBC_SK_COMPANY_ADDRESS   "Company/address"

#define SNBC_DEFAULT_SERVER_ADDRESS "192.168.1.250"
#define SNBC_DEFAULT_SERVER_PORT    8812
#define SNBC_DEFAULT_ADMIN_PASSWORD 12345

#endif // GLOBAL_H

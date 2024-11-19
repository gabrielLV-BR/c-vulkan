#if !defined(__info_h__)
#define __info_h__

#define PLATFORM_LINUX 1
#define PLATFORM_WINDOWS 0
#define PLATFORM_APPLE 0

struct version {
    int major, minor, fix;
};

static const char *APP_NAME = "Untitled";
static const struct version APP_VERSION = { 0, 0, 0 };

static const char *ENGINE_NAME = "GECKO";
static const struct version ENGINE_VERSION = { 0, 0, 0 };

static const char *AUTHOR_NAME = "Gabriel L.";

#endif // __info_h__

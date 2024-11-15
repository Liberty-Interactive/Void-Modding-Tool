// Operating System Identifier
const tchar* ArchName()
{
#if defined( __x86_64__) || defined(_M_X64)
    return "amd64";
#elif defined(__i386__) || defined(_X86_) || defined(_M_IX86)
    return "i386";
#elif defined __aarch64__
    return "aarch64";
#elif defined __arm__ || defined _M_ARM
    return "arm";
#else
    return "Unknown architecture";
#endif
}

const tchar* OperatingSystem()
{
#ifdef LINUX	
    return "Linux";
#elif ANDROID 
    return "Android";
#elif PLATFORM_FBSD
    return "FreeBSD";
#elif PLATFORM_BSD
    return "BSD";
#elif WIN32
    return "Windows";
#elif OSX
    return "MacOS";
#elif PLATFORM_HAIKU
    return "Haiku";
#else
    return "Unknown Platform"
#endif
}


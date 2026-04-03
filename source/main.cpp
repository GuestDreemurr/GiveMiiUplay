#include <wups.h>
#include <whb/log.h>
#include <whb/log_module.h>
#include <whb/log_cafe.h>
#include <whb/log_udp.h>
#include <string.h>

#include <coreinit/title.h>

#include <patcher/rplinfo.h>
#include <patcher/patcher.h>

WUPS_PLUGIN_NAME("GiveMiiUplay");
WUPS_PLUGIN_DESCRIPTION("Allows the Uplay app to get past the service token check.");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("GuestDreemurr");
WUPS_PLUGIN_LICENSE("MIT");

#define UPLAY_TITLE_ID 0x000500001011BA00llu // 00050000-1011BA00
#define UPLAY_CLIENT_ID "eb158139563c54df15d3ad895dd80355"


ON_APPLICATION_START()
{
    // If this is not uplay do nothing
    if (OSGetTitleID() != UPLAY_TITLE_ID) {
        return;
    }

    // Init logging
    if (!WHBLogModuleInit()) {
        WHBLogCafeInit();
        WHBLogUdpInit();
    }

    WHBLogPrintf("GiveMiiUplay: applying patches...");

    // Patch the dynload functions so GetRPLInfo works
    if (!PatchDynLoadFunctions()) {
        WHBLogPrintf("GiveMiiUplay: Failed to patch dynload functions");
        return;
    }

    // Get the RPLInfo
    auto rpl_info = TryGetRPLInfo();
    if (!rpl_info) {
        WHBLogPrintf("GiveMiiUplay: Failed to get RPL info");
        return;
    }

    // Find the rpx
    rplinfo rpls = *rpl_info;
    auto lb_shell_rpx = FindRPL(rpls, "UplayBrowser.rpx");
    if (!lb_shell_rpx) {
        WHBLogPrintf("GiveMiiUplay: Failed to find UplayBrowser.rpx");
        return;

    }

    

    // TODO: patch onlineconfigservice.ubi.com, aswell as find a way to patch the urls in the actionscript (maybe hook nlibcurl and modify the request domain at runtime? probably not best practice though..)
}

DECL_FUNCTION(int, AcquireIndependentServiceToken__Q2_2nn3actFPcPCc, uint8_t* token, const char* client_id)
{
    // If this is the Uplay client, return success
    if (client_id && strcmp(client_id, UPLAY_CLIENT_ID) == 0) {
        WHBLogPrintf("GiveMiiUplay: Faking service success for '%s'", client_id);
        return 0;
    }

    return real_AcquireIndependentServiceToken__Q2_2nn3actFPcPCc(token, client_id);
}



#ifndef GGREYSTRIPE_H
#define GGREYSTRIPE_H

#include <gglobal.h>
#include <gevent.h>

enum
{
	GGREYSTRIPE_BANNER_LOADED_EVENT,
	GGREYSTRIPE_BANNER_FAILED_EVENT,
	GGREYSTRIPE_BANNER_CLOSED_EVENT,
	GGREYSTRIPE_BANNER_CLICKED_EVENT,
	GGREYSTRIPE_INTERSTITIAL_LOADED_EVENT,
	GGREYSTRIPE_INTERSTITIAL_FAILED_EVENT,
	GGREYSTRIPE_INTERSTITIAL_CLOSED_EVENT,
	GGREYSTRIPE_INTERSTITIAL_CLICKED_EVENT,
};

#ifdef __cplusplus
extern "C" {
#endif

G_API int ggreystripe_isAvailable();

G_API void ggreystripe_init();
G_API void ggreystripe_cleanup();

G_API void ggreystripe_configure(const char* appId);
G_API void ggreystripe_showBanner();
G_API void ggreystripe_hideBanner();
G_API void ggreystripe_showInterstitial();

G_API g_id ggreystripe_addCallback(gevent_Callback callback, void *udata);
G_API void ggreystripe_removeCallback(gevent_Callback callback, void *udata);
G_API void ggreystripe_removeCallbackWithGid(g_id gid);

#ifdef __cplusplus
}
#endif

#endif

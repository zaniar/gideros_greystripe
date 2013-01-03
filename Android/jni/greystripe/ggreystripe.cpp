#include "ggreystripe.h"
#include <jni.h>
#include <stdlib.h>
#include <glog.h>

extern "C" {
JavaVM *g_getJavaVM();
JNIEnv *g_getJNIEnv();
}

class GGreystripe
{
public:
	GGreystripe()
	{
		gid_ = g_nextgid();

		JNIEnv *env = g_getJNIEnv();

		jclass localClass = env->FindClass("com/nightspade/gideros/android/plugins/greystripe/GGreystripe");
		cls_ = (jclass)env->NewGlobalRef(localClass);
		env->DeleteLocalRef(localClass);

		env->CallStaticVoidMethod(cls_, env->GetStaticMethodID(cls_, "init", "(J)V"), (jlong)this);
	}

	~GGreystripe()
	{
		JNIEnv *env = g_getJNIEnv();

		env->CallStaticVoidMethod(cls_, env->GetStaticMethodID(cls_, "cleanup", "()V"));

		env->DeleteGlobalRef(cls_);

		gevent_removeEventsWithGid(gid_);
	}

	void onBannerLoaded()
	{
		gevent_enqueueEvent(gid_, callback_s, GGREYSTRIPE_BANNER_LOADED_EVENT, NULL, 1, this);
	}

	void onBannerFailed()
	{
		gevent_enqueueEvent(gid_, callback_s, GGREYSTRIPE_BANNER_FAILED_EVENT, NULL, 1, this);
	}

	void onBannerClosed()
	{
		gevent_enqueueEvent(gid_, callback_s, GGREYSTRIPE_BANNER_CLOSED_EVENT, NULL, 1, this);
	}

	void onBannerClicked()
	{
		gevent_enqueueEvent(gid_, callback_s, GGREYSTRIPE_BANNER_CLICKED_EVENT, NULL, 1, this);
	}

	void onInterstitialLoaded()
	{
		gevent_enqueueEvent(gid_, callback_s, GGREYSTRIPE_INTERSTITIAL_LOADED_EVENT, NULL, 1, this);
	}

	void onInterstitialFailed()
	{
		gevent_enqueueEvent(gid_, callback_s, GGREYSTRIPE_INTERSTITIAL_FAILED_EVENT, NULL, 1, this);
	}

	void onInterstitialClosed()
	{
		gevent_enqueueEvent(gid_, callback_s, GGREYSTRIPE_INTERSTITIAL_CLOSED_EVENT, NULL, 1, this);
	}

	void onInterstitialClicked()
	{
		gevent_enqueueEvent(gid_, callback_s, GGREYSTRIPE_INTERSTITIAL_CLICKED_EVENT, NULL, 1, this);
	}

	void configure(const char* appId)
	{
		JNIEnv *env = g_getJNIEnv();

		env->CallStaticVoidMethod(cls_, env->GetStaticMethodID(cls_, "configure", "(Ljava/lang/String;)V"), env->NewStringUTF(appId));
	}

	void showBanner()
	{
		JNIEnv *env = g_getJNIEnv();

		env->CallStaticVoidMethod(cls_, env->GetStaticMethodID(cls_, "showBanner", "()V"));
	}

	void hideBanner()
	{
		JNIEnv *env = g_getJNIEnv();

		env->CallStaticVoidMethod(cls_, env->GetStaticMethodID(cls_, "hideBanner", "()V"));
	}

	void showInterstitial()
	{
		JNIEnv *env = g_getJNIEnv();

		env->CallStaticVoidMethod(cls_, env->GetStaticMethodID(cls_, "showInterstitial", "()V"));
	}

	g_id addCallback(gevent_Callback callback, void *udata)
	{
		return callbackList_.addCallback(callback, udata);
	}

	void removeCallback(gevent_Callback callback, void *udata)
	{
		callbackList_.removeCallback(callback, udata);
	}

	void removeCallbackWithGid(g_id gid)
	{
		callbackList_.removeCallbackWithGid(gid);
	}

	private:
		static void callback_s(int type, void *event, void *udata)
		{
			((GGreystripe*)udata)->callback(type, event);
		}

		void callback(int type, void *event)
		{
			callbackList_.dispatchEvent(type, event);
		}

	private:
		gevent_CallbackList callbackList_;

	private:
		jclass cls_;
		g_id gid_;
};

extern "C" {

	void Java_com_nightspade_gideros_android_plugins_greystripe_GGreystripe_onBannerLoaded(JNIEnv *env, jclass clz, jlong data)
	{
		((GGreystripe*)data)->onBannerLoaded();
	}

	void Java_com_nightspade_gideros_android_plugins_greystripe_GGreystripe_onBannerFailed(JNIEnv *env, jclass clz, jlong data)
	{
		((GGreystripe*)data)->onBannerFailed();
	}

	void Java_com_nightspade_gideros_android_plugins_greystripe_GGreystripe_onBannerClosed(JNIEnv *env, jclass clz, jlong data)
	{
		((GGreystripe*)data)->onBannerClosed();
	}

	void Java_com_nightspade_gideros_android_plugins_greystripe_GGreystripe_onBannerClicked(JNIEnv *env, jclass clz, jlong data)
	{
		((GGreystripe*)data)->onBannerClicked();
	}

	void Java_com_nightspade_gideros_android_plugins_greystripe_GGreystripe_onInterstitialLoaded(JNIEnv *env, jclass clz, jlong data)
	{
		((GGreystripe*)data)->onInterstitialLoaded();
	}

	void Java_com_nightspade_gideros_android_plugins_greystripe_GGreystripe_onInterstitialFailed(JNIEnv *env, jclass clz, jlong data)
	{
		((GGreystripe*)data)->onInterstitialFailed();
	}

	void Java_com_nightspade_gideros_android_plugins_greystripe_GGreystripe_onInterstitialClosed(JNIEnv *env, jclass clz, jlong data)
	{
		((GGreystripe*)data)->onInterstitialClosed();
	}

	void Java_com_nightspade_gideros_android_plugins_greystripe_GGreystripe_onInterstitialClicked(JNIEnv *env, jclass clz, jlong data)
	{
		((GGreystripe*)data)->onInterstitialClicked();
	}
}

static GGreystripe *s_greystripe = NULL;

extern "C" {

int ggreystripe_isAvailable()
{
	return 1;
}

void ggreystripe_init()
{
	s_greystripe = new GGreystripe();
}

void ggreystripe_cleanup()
{
	delete s_greystripe;
	s_greystripe = NULL;
}

void ggreystripe_configure(const char* appId)
{
	s_greystripe->configure(appId);
}

void ggreystripe_showBanner()
{
	s_greystripe->showBanner();
}

void ggreystripe_hideBanner()
{
	s_greystripe->hideBanner();
}

void ggreystripe_showInterstitial()
{
	s_greystripe->showInterstitial();
}

g_id ggreystripe_addCallback(gevent_Callback callback, void *udata)
{
	return s_greystripe->addCallback(callback, udata);
}

void ggreystripe_removeCallback(gevent_Callback callback, void *udata)
{
	s_greystripe->removeCallback(callback, udata);
}

void ggreystripe_removeCallbackWithGid(g_id gid)
{
	s_greystripe->removeCallbackWithGid(gid);
}

}

package com.nightspade.gideros.android.plugins.greystripe;

import java.lang.ref.WeakReference;
import java.util.Hashtable;

import com.greystripe.sdk.GSAd;
import com.greystripe.sdk.GSAdErrorCode;
import com.greystripe.sdk.GSAdListener;
import com.greystripe.sdk.GSFullscreenAd;
import com.greystripe.sdk.GSMobileBannerAdView;

import com.mopub.mobileads.MoPubView;
import com.mopub.mobileads.MoPubView.OnAdClosedListener;
import com.mopub.mobileads.MoPubView.OnAdFailedListener;
import com.mopub.mobileads.MoPubView.OnAdLoadedListener;
import com.mopub.mobileads.MoPubView.OnAdWillLoadListener;
import com.nightspade.gideros.android.plugins.mopub.GMoPub;
import com.nightspade.stackthestuffad.R;
import android.app.Activity;
import android.opengl.Visibility;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;

public class GGreystripe {
	private static WeakReference<Activity> sActivity;
	private static GGreystripe sInstance;
	private static long sData;
	
	private static GSMobileBannerAdView sBannerAd;
	private static GSFullscreenAd sFullscreenAd;
	
	private static Hashtable<String, Integer> sAlign;
	private static String sCurAlignment = "top";
	
	public static void onCreate(Activity activity)
	{
		sActivity = new WeakReference<Activity>(activity);
		
		sAlign = new Hashtable<String, Integer>();
		sAlign.put("top", Gravity.TOP);
		sAlign.put("bottom", Gravity.BOTTOM);
	}
	
	static public void init(long data)
	{
		sData = data;
		sInstance = new GGreystripe(sActivity.get());
	}
	
	static public void cleanup()
	{
		if (sInstance != null)
		{
			sData = 0;
			sBannerAd = null;
			sFullscreenAd = null;
			sInstance = null;
		}
	}
	
	public GGreystripe(Activity activity)
	{
	}
	
	static public void onDestroy()
	{
		cleanup();
	}
	
	public static void configure(final String appId) {
		try
		{
			sActivity.get().runOnUiThread(new Runnable() {
			
				@Override
				public void run() {
					sBannerAd = new GSMobileBannerAdView(sActivity.get(), appId);
					sBannerAd.addListener(new GSAdListener() {
						
						@Override
						public void onFetchedAd(GSAd arg0) {
							onBannerLoaded(sData);
							FrameLayout myView = (FrameLayout)sActivity.get().getWindow().getDecorView();
							
							FrameLayout.LayoutParams adParams = new FrameLayout.LayoutParams(
				                    FrameLayout.LayoutParams.MATCH_PARENT ,
				                    50 ,
				                    sAlign.get(sCurAlignment));
							myView.addView(sBannerAd, adParams);
						}
						
						@Override
						public void onFailedToFetchAd(GSAd arg0, GSAdErrorCode arg1) {
							onBannerFailed(sData);
						}
						
						@Override
						public void onAdDismissal(GSAd arg0) {
							onBannerClosed(sData);
						}
						
						@Override
						public void onAdClickthrough(GSAd arg0) {
							onBannerClicked(sData);
						}
					});
					
					sFullscreenAd = new GSFullscreenAd(sActivity.get(), appId);
					sFullscreenAd.addListener(new GSAdListener() {
						
						@Override
						public void onFetchedAd(GSAd arg0) {
							onInterstitialLoaded(sData);
							sFullscreenAd.display();
						}
						
						@Override
						public void onFailedToFetchAd(GSAd arg0, GSAdErrorCode arg1) {
							onInterstitialFailed(sData);
						}
						
						@Override
						public void onAdDismissal(GSAd arg0) {
							onInterstitialClosed(sData);
						}
						
						@Override
						public void onAdClickthrough(GSAd arg0) {
							onInterstitialClicked(sData);
						}
					});
				}
			});
		
		}
		catch(Exception ex)	{}
	}
	
	public static void showBanner()
	{
		sBannerAd.refresh();
	}
	
	
	public static void hideBanner()
	{
		FrameLayout myView = (FrameLayout)sActivity.get().getWindow().getDecorView();
		myView.removeView(sBannerAd);
	}
	
	
	public static void showInterstitial()
	{
		sFullscreenAd.fetch();
	}
	

	public static native void onBannerLoaded(long data);
	public static native void onBannerFailed(long data);
	public static native void onBannerClosed(long data);
	public static native void onBannerClicked(long data);
	
	public static native void onInterstitialLoaded(long data);
	public static native void onInterstitialFailed(long data);
	public static native void onInterstitialClosed(long data);
	public static native void onInterstitialClicked(long data);
}

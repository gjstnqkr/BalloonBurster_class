//
//  FirebaseHelper.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 2018. 6. 30..
//

#include "interface.h"

#include "FirebaseHelper.hpp"
//#include "FirebaseHelper.h"

#if USE_ADMOB == 1

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

firebase::admob::AdParent getAdParent() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // Returns the iOS RootViewController's main view (i.e. the EAGLView).
    return (id)Director::getInstance()->getOpenGLView()->getEAGLView();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // Returns the Android Activity.
    return JniHelper::getActivity();
#else
    // A void* for any other environments.
    return 0;
#endif
}

#endif



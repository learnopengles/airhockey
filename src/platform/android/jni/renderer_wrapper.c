#include "game.h"
#include "macros.h"
#include <jni.h>

/* These functions are called from Java. */

JNIEXPORT void JNICALL Java_com_learnopengles_airhockey_RendererWrapper_on_1surface_1created(JNIEnv * env, jclass cls) {
	UNUSED(env);
	UNUSED(cls);
	on_surface_created();
}

JNIEXPORT void JNICALL Java_com_learnopengles_airhockey_RendererWrapper_on_1surface_1changed(JNIEnv * env, jclass cls, jint width, jint height) {
	UNUSED(env);
	UNUSED(cls);
	on_surface_changed(width, height);
}

JNIEXPORT void JNICALL Java_com_learnopengles_airhockey_RendererWrapper_on_1draw_1frame(JNIEnv* env, jclass cls) {
	UNUSED(env);
	UNUSED(cls);
	on_draw_frame();
}

JNIEXPORT void JNICALL Java_com_learnopengles_airhockey_RendererWrapper_on_1touch_1press(JNIEnv* env, jclass cls, jfloat normalized_x, jfloat normalized_y) {
	UNUSED(env);
	UNUSED(cls);
	on_touch_press(normalized_x, normalized_y);
}

JNIEXPORT void JNICALL Java_com_learnopengles_airhockey_RendererWrapper_on_1touch_1drag(JNIEnv* env, jclass cls, jfloat normalized_x, jfloat normalized_y) {
	UNUSED(env);
	UNUSED(cls);
	on_touch_drag(normalized_x, normalized_y);
}

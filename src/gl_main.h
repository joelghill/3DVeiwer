/*
 * gl_main.h
 *
 *  Created on: Mar 19, 2015
 *      Author: joelhill
 */

#ifndef GL_MAIN_H_
#define GL_MAIN_H_

#include <screen/screen.h>
#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "bbutil.h"

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

void handleScreenEvent(bps_event_t *event);

int initializegl(void);

void render(void);

int gl_main(
        const char* id,
        int id_length,
        const char* groupArr
        );

#ifdef __cplusplus
}
#endif/*CPLUSPLUS*/

#endif /* GL_MAIN_H_ */

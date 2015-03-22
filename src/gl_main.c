/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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



static const GLfloat vertices[] =
{
    // front
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,

    // right
    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,

    // back
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    // left
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,

    // top
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,

    // bottom
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f
};


static const GLfloat colors[] =
{
    // front
    0.0625f,0.57421875f,0.92578125f,1.0f,
    0.0625f,0.57421875f,0.92578125f,1.0f,
    0.0625f,0.57421875f,0.92578125f,1.0f,
    0.0625f,0.57421875f,0.92578125f,1.0f,
    0.0625f,0.57421875f,0.92578125f,1.0f,
    0.0625f,0.57421875f,0.92578125f,1.0f,

    // right
    0.29296875f,0.66796875f,0.92578125f,1.0f,
    0.29296875f,0.66796875f,0.92578125f,1.0f,
    0.29296875f,0.66796875f,0.92578125f,1.0f,
    0.29296875f,0.66796875f,0.92578125f,1.0f,
    0.29296875f,0.66796875f,0.92578125f,1.0f,
    0.29296875f,0.66796875f,0.92578125f,1.0f,

    // back
    0.52734375f,0.76171875f,0.92578125f,1.0f,
    0.52734375f,0.76171875f,0.92578125f,1.0f,
    0.52734375f,0.76171875f,0.92578125f,1.0f,
    0.52734375f,0.76171875f,0.92578125f,1.0f,
    0.52734375f,0.76171875f,0.92578125f,1.0f,
    0.52734375f,0.76171875f,0.92578125f,1.0f,

    // left
    0.0625f,0.57421875f,0.92578125f,1.0f,
    0.0625f,0.57421875f,0.92578125f,1.0f,
    0.0625f,0.57421875f,0.92578125f,1.0f,
    0.0625f,0.57421875f,0.92578125f,1.0f,
    0.0625f,0.57421875f,0.92578125f,1.0f,
    0.0625f,0.57421875f,0.92578125f,1.0f,

    // top
    0.29296875f,0.66796875f,0.92578125f,1.0f,
    0.29296875f,0.66796875f,0.92578125f,1.0f,
    0.29296875f,0.66796875f,0.92578125f,1.0f,
    0.29296875f,0.66796875f,0.92578125f,1.0f,
    0.29296875f,0.66796875f,0.92578125f,1.0f,
    0.29296875f,0.66796875f,0.92578125f,1.0f,

    // bottom
    0.52734375f,0.76171875f,0.92578125f,1.0f,
    0.52734375f,0.76171875f,0.92578125f,1.0f,
    0.52734375f,0.76171875f,0.92578125f,1.0f,
    0.52734375f,0.76171875f,0.92578125f,1.0f,
    0.52734375f,0.76171875f,0.92578125f,1.0f,
    0.52734375f,0.76171875f,0.92578125f,1.0f
};

void handleScreenEvent(bps_event_t *event) {
    screen_event_t screen_event = screen_event_get_event(event);

    int screen_val;
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE,
            &screen_val);

    switch (screen_val) {
    case SCREEN_EVENT_MTOUCH_TOUCH:
    case SCREEN_EVENT_MTOUCH_MOVE:
    case SCREEN_EVENT_MTOUCH_RELEASE:
        break;
    }
}

int
initializegl() {

    EGLint surface_width, surface_height;

       eglQuerySurface(egl_disp, egl_surf, EGL_WIDTH, &surface_width);
       eglQuerySurface(egl_disp, egl_surf, EGL_HEIGHT, &surface_height);

       EGLint err = eglGetError();
       if (err != 0x3000) {
           fprintf(stderr, "Unable to query EGL surface dimensions\n");
           return EXIT_FAILURE;
       }

       glClearDepthf(1.0f);
       glEnable(GL_CULL_FACE);
       glShadeModel(GL_SMOOTH);

       //set clear color to black
       glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

       glViewport(0, 0, surface_width, surface_height);
       glMatrixMode(GL_PROJECTION);
       glLoadIdentity();

       float nearClip = -2.0f;
       float farClip  = 2.0f;
       float yFOV  = 75.0f;
       float yMax = nearClip * tan(yFOV*M_PI/360.0f);
       float aspect = surface_width/surface_height;
       float xMin = -yMax * aspect;
       float xMax = yMax *aspect;

       glFrustumf(xMin, xMax, -yMax, yMax, nearClip, farClip);

       if (surface_width > surface_height)
       {
           glScalef((float)surface_height/(float)surface_width, 1.0, 1.0f);
       }
       else
       {
           glScalef(1.0, (float)surface_width/(float)surface_height, 1.0f);
       }

       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();
       return 0;
}

void render() {
    //Typical render pass
    //fprintf(stdout, "RENDERING.....\n");
    //fflush(stdout);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, 0, colors);

    glRotatef(0.5f, 0.0f, 1.0f, 0.0f);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    //Use utility code to update the screen
    bbutil_swap();
}

//main entry point for GL stuff....
int gl_main(
        const char* id,
        int id_len,
        const char* groupArr) {
    //fprintf(stdout, "starting gl_main...\n");

    int exit_application = 0;
    static screen_context_t screen_cxt;

    //fprintf(stdout, "starting gl_main...\n");
    //fflush(stdout);
    //Create a screen context that will be used to create an EGL surface to to receive libscreen events
    screen_create_context(&screen_cxt, 0);

    //Initialize BPS library
    bps_initialize();

    //Use utility code to initialize EGL for rendering with GL ES 1.1
    //This is a method I added.
    //based off of bbutil_init_egl - Joel
    if (EXIT_SUCCESS != bbutil_init_egl_child(screen_cxt, id, id_len, groupArr)) {
        fprintf(stderr, "bbutil_init_egl failed\n");
        bbutil_terminate();
        screen_destroy_context(screen_cxt);
        return 0;
    }

    //Initialize application logic
    if (EXIT_SUCCESS != initializegl()) {
        fprintf(stderr, "initialize failed\n");
        bbutil_terminate();
        screen_destroy_context(screen_cxt);
        return 0;
    }

    //Signal BPS library that navigator and screen events will be requested
    if (BPS_SUCCESS != screen_request_events(screen_cxt)) {
        fprintf(stderr, "screen_request_events failed\n");
        bbutil_terminate();
        screen_destroy_context(screen_cxt);
        return 0;
    }

    if (BPS_SUCCESS != navigator_request_events(0)) {
        fprintf(stderr, "navigator_request_events failed\n");
        bbutil_terminate();
        screen_destroy_context(screen_cxt);
        return 0;
    }

    //Signal BPS library that navigator orientation is not to be locked
    if (BPS_SUCCESS != navigator_rotation_lock(false)) {
        fprintf(stderr, "navigator_rotation_lock failed\n");
        bbutil_terminate();
        screen_destroy_context(screen_cxt);
        return 0;
    }

    while (!exit_application) {
        //Request and process all available BPS events
        bps_event_t *event = NULL;

        for(;;) {
            if (BPS_SUCCESS != bps_get_event(&event, 0)) {
                fprintf(stderr, "bps_get_event failed\n");
                break;
            }

            if (event) {
                int domain = bps_event_get_domain(event);

                if (domain == screen_get_domain()) {
                    handleScreenEvent(event);
                } else if ((domain == navigator_get_domain())
                        && (NAVIGATOR_EXIT == bps_event_get_code(event))) {
                    exit_application = 1;
                }
            } else {
                break;
            }
        }
        render();
    }

    //Stop requesting events from libscreen
    screen_stop_events(screen_cxt);

    //Shut down BPS library for this process
    bps_shutdown();

    //Use utility code to terminate EGL setup
    bbutil_terminate();

    //Destroy libscreen context
    screen_destroy_context(screen_cxt);
    return 0;
}

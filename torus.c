/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  torus.c
 *  This program demonstrates the creation of a display list.
 * 
 *  Borrowed from: https://www.opengl.org/archives/resources/code/samples/redbook/torus.c
 * 
 *  Modified in 2021 by Jeremy Carter <jeremy@jeremycarter.ca>
 */

#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ncurses.h>
#include <chrono>

#define PI_ 3.14159265358979323846

static double pi_s = PI_;

#define DEFCRON_TORUS_NUMC_DEFAULT 8
#define DEFCRON_TORUS_NUMT_DEFAULT 25

static int numc_s = DEFCRON_TORUS_NUMC_DEFAULT;
static int numt_s = DEFCRON_TORUS_NUMT_DEFAULT;

static bool keystates[256];
static bool keyspecialstates[256];

// static auto frameAdjust;

GLuint theTorus;

/* Draw a torus */
static void torus(int numc, int numt, double pi)
{
  double scale = 2.0;
  double shrink = 0.72999;

  int i, j, k;
  double s, t, x, y, z, twopi;

  twopi = 2 * pi;

  for (i = 0; i < numc; i++) {
    glBegin(GL_QUAD_STRIP);

    for (j = 0; j <= numt; j++) {

        for (k = 1; k >= 0; k--) {
          glColor3f((float)(i * j % 256)/255.f, (float)((256 - i * k - j * 3) % 256)/255.f, (float)(((i + j + k) * 2) % 256)/255.f);
          
          s = (i + k) % numc + 0.5;
          t = j % numt;

          x = scale * (1+.1*cos(s*twopi/numc))*cos(t*twopi/numt);
          y = scale * (1+.1*cos(s*twopi/numc))*sin(t*twopi/numt);
          z = scale * .1 * sin(s * twopi / numc);
          
          glVertex3f(x, y, z);
        }
    }
    glEnd();
  }


  for (i = 0; i < numc; i++) {    
    glBegin(GL_QUAD_STRIP);

    for (j = 0; j <= numt; j++) {

        for (k = 1; k >= 0; k--) {
          
          if ((i % 17) == 0 || (j % 8) == 0 || (k % 9) == 0) {
            glEnable(GL_DEPTH_TEST);
          }

          glColor3f((float)(i * j % 256)/255.f, (float)((256 - i * k + j * 2) % 256)/255.f, (float)(((i + j + k) * 2) % 256)/255.f);
          
          s = (i + k) % numc + 0.5;
          t = j % numt;

          x = scale * shrink * (1+.1*cos(s*twopi/numc))*cos(t*twopi/numt);
          y = scale * shrink * (1+.1*cos(s*twopi/numc))*sin(t*twopi/numt);
          z = scale * shrink * .1 * sin(s * twopi / numc);
          
          glVertex3f(x, y, z);

          if ((i % 17) == 0 || (j % 8) == 0 || (k % 9) == 0) {
            glDisable(GL_DEPTH_TEST);
          }
        }
    }
    glEnd();
  }
}

/* Create display list with Torus and initialize state */
static void init(void)
{
  refresh();

  mvprintw(0, 0, "pi = %.15f              ", pi_s);
  mvprintw(2, 0, "c  = %d                 ", numc_s);
  mvprintw(4, 0, "t  = %d                 ", numt_s);
  mvprintw(5, 0, "");
  mvprintw(6, 0, "");

  refresh();

  theTorus = glGenLists (1);
  glNewList(theTorus, GL_COMPILE);

  torus(numc_s, numt_s, pi_s);

  glEndList();
}

void handleKeys(double frameAdjust) {
  double speedAdjust = 5000000000.0;

  double rotateSpeed = 0.2;

  bool reInit = false;
  bool screenUpdated = false;

  if (keystates['e'] || keystates['E']) {
    pi_s += 0.003339 / frameAdjust * speedAdjust;

    pi_s = ((pi_s < 0) && (pi_s >= -0.001)) ? (pi_s * -1) + 0.008 : pi_s;

    reInit = true;
    screenUpdated = true;
  }

  if (keystates['q'] || keystates['Q']) {
    pi_s -= 0.003339 / frameAdjust * speedAdjust;

    pi_s = ((pi_s > 0) && (pi_s <= 0.001)) ? (pi_s * -1) - 0.008 : pi_s;

    reInit = true;
    screenUpdated = true;
  }
  
  if (keystates['w'] || keystates['W']) {
    numc_s += 1;

    numc_s = numc_s <= 3 ? 3 : numc_s;

    reInit = true;
    screenUpdated = true;
  }

  if (keystates['s'] || keystates['S']) {
    numc_s -= 1;

    numc_s = numc_s <= 3 ? 3 : numc_s;

    reInit = true;
    screenUpdated = true;
  }

  if (keystates['d'] || keystates['D']) {
    numt_s += 1;

    numt_s = numt_s <= 3 ? 3 : numt_s;

    reInit = true;
    screenUpdated = true;
  }

  if (keystates['a'] || keystates['A']) {
    numt_s -= 1;

    numt_s = numt_s <= 3 ? 3 : numt_s;

    reInit = true;
    screenUpdated = true;
  }

  if (keystates['r'] || keystates['R']) {
    pi_s = PI_;
    pi_s = ((pi_s < 0) && (pi_s >= -0.01)) ? (pi_s * -1) + 0.08 : pi_s;
    pi_s = ((pi_s > 0) && (pi_s <= 0.01)) ? (pi_s * -1) - 0.08 : pi_s;

    numc_s = DEFCRON_TORUS_NUMC_DEFAULT;
    numc_s = numc_s <= 3 ? 3 : numc_s;

    numt_s = DEFCRON_TORUS_NUMT_DEFAULT;
    numt_s = numt_s <= 3 ? 3 : numt_s;

    reInit = true;
    screenUpdated = true;
  }

  if (keyspecialstates[GLUT_KEY_RIGHT]) {
      glRotatef(rotateSpeed / frameAdjust * speedAdjust, 0.0, 1.0, 0.0);

      screenUpdated = true;
  }

  if (keyspecialstates[GLUT_KEY_LEFT]) {
      glRotatef(rotateSpeed / frameAdjust * speedAdjust, 0.0, -1.0, 0.0);

      screenUpdated = true;
  }

  if (keyspecialstates[GLUT_KEY_UP]) {
      glRotatef(rotateSpeed / frameAdjust * speedAdjust, -1.0, 0.0, 0.0);
      
      screenUpdated = true;
  }

  if (keyspecialstates[GLUT_KEY_DOWN]) {
      glRotatef(rotateSpeed / frameAdjust * speedAdjust, 1.0, 0.0, 0.0);
      
      screenUpdated = true;
  }
   
  if (keystates['t'] || keystates['T']) {
      glLoadIdentity();
      gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

      screenUpdated = true;
  }

  if (reInit) {
    init();
  }

  if (screenUpdated) {
    glutPostRedisplay();
  }

  if (keystates[27]) {
      exit(0);
  }
}

/* Clear window and draw torus */
void display(void)
{
  glClearColor(0.13f, 0.45f, 0.63f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glColor3f (1.0, 1.0, 1.0);
  glCallList(theTorus);
  glFlush();
}

/* Handle window resize */
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
}

void onexit() {
  endwin();

  printf("\npi = %.15f\n\n", pi_s);
  printf("c  = %d\n\n", numc_s);
  printf("t  = %d\n\n", numt_s);
}

void onKeyDown(unsigned char key, int x, int y) {
  keystates[key] = true;
}

void onKeyUp(unsigned char key, int x, int y) {
  keystates[key] = false;
}

void onKeyDownSpecial(int key, int x, int y) {
  keyspecialstates[key] = true;
}

void onKeyUpSpecial(int key, int x, int y) {
  keyspecialstates[key] = false;
}

int main(int argc, char **argv)
{
  initscr();

  glutInitWindowSize(1024, 768);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  glutCreateWindow(argv[0]);

  init();

  glutReshapeFunc(reshape);

  glutIgnoreKeyRepeat(1);

  glutKeyboardFunc(onKeyDown);
  glutKeyboardUpFunc(onKeyUp);
  
  glutSpecialFunc(onKeyDownSpecial);
  glutSpecialUpFunc(onKeyUpSpecial);

  glutDisplayFunc(display);
  
  atexit(onexit);


  std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto frameAdjustDuration = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  double frameAdjust = std::chrono::duration<double>(frameAdjustDuration).count();

  while(1)
  {
    now = std::chrono::system_clock::now();
    duration = now.time_since_epoch();
    frameAdjustDuration = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    frameAdjust = std::chrono::duration<double>(frameAdjustDuration).count();

    handleKeys(frameAdjust);
    glutMainLoopEvent();
  }

  return 0;
}
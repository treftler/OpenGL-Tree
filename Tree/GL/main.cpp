////////////////////////////////////////////////////////////////////////////////

//  Created by: Tripper TL Scott

//  注意：以下文字化けやコードの書き方により、すごく難解なコードとなっている
//　　　　　只今再構成中


////////////////////////////////////////////////////////////////////////////////


// WindowsだとOpenGL系のインポートが以下と異なる
#include <stdlib.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <math.h>

#define KEY_ESC 27
#define PI 3.14159265358979323846264338327950288419716939937510582
#define GRASSAREA 20    // パソコンの処理速度に合わせて大きさを変える（草の速度で判断する）
#define RAIN 100        // 100: 雨, 1000: 大雨, 50以下だと少し不自然


void drawGround2D(void);

// 木関連
void drawPlant(void);
float branchshake = 0;
float diskR = 0.0;
float leafR = 0.0;

// 草関連
void drawGrass(void);
float grasscolor = 0.8;

// 雨（＋雷）関連
void drawRain(void);
void setRand(void);
void drawCloud(void);
void lightning(void);

int rainnum = 0;
int maxrain = 0;
float falltheta = 1.0;
int raini[RAIN];
int rainj[RAIN];
int raincounter1 = 0;
int raincounter2 = 0;
int rainfinishcounter = 0;
float rainspeedy[RAIN];
float rainspeedx[RAIN];
float randcloud[10][10];
int lightningnum = 0;

//

void camera(int);
float theta = 0.0;
float thetatree = 0.0;
float thetagrass = 0.0;
float thetawind = 0.0;


float alpha = 0.0;
void polarview(void);       // „Éù„ÅE„É©„Éº„Éì„É•„Éº
void resetview(void);       // „Éì„É•„Éº„ÅÆ„É™„Çª„ÉÅEÉà
void shading(int);          // „Ç∑„Çß„Éº„ÉÅEÇ£„É≥„Ç∞„ÅÆÈÅ∏Êäû„ÅEÈñ¢Êï∞
void initLighting(void);    // ÂÖâÊ∫ê„ÅEË®≠ÂÆö„ÅEÈñ¢Êï∞
unsigned char windFlag = GL_FALSE;      // È¢®„ÅÆ„Éï„É©„Ç∞
unsigned char rainFlag = GL_FALSE;    // Èõ®„ÅÆ„Éï„É©„Ç∞
unsigned char rainFlag2 = GL_FALSE;    // Èõ®„ÅÆ„Éï„É©„Ç∞
unsigned char thunderFlag = GL_FALSE;    // Èõ∑„ÅÆ„Éï„É©„Ç∞
unsigned char cloudFlag = GL_FALSE;    // Èõ≤„ÅÆ„Éï„É©„Ç∞
unsigned char fallFlag = GL_FALSE;    // Èõ≤„ÅÆ„Éï„É©„Ç∞
unsigned char freeFlag = GL_FALSE;    // Èõ≤„ÅÆ„Éï„É©„Ç∞
unsigned char alphaFlag = GL_TRUE;    // Èõ≤„ÅÆ„Éï„É©„Ç∞
unsigned char cloudmovingFlag = GL_FALSE;    // Èõ≤„ÅÆ„Éï„É©„Ç∞
int xBegin, yBegin;         // „Éû„Ç¶„Çπ„ÇØ„É™„ÉÅEÇØÊôÇ„ÅE„Éù„Ç∏„Ç∑„Éß„É≥
int mButton;                // „Éû„Ç¶„Çπ„ÇØ„É™„ÉÅEÇØÊôÇ„ÅE„Éú„Çø„É≥„ÅÆÁ®ÆÈ°?
float distance, twist, elevation, azimuth;  // „Éù„ÅE„É©„Éº„Éì„É•„Éº„ÅÆÂ§âÊï∞
int change = 0;
float plantWidth = 0.2;
float plantHeight = 1.0;
float grow = 0.0;
float growleaves = 0.0;
float growdisks = 0.0;
float t = 100;
float t2 = 0.001;
float t3 = 0.001;
float t4 = 0.0;
float wind = 0;
float wind2 = 0;
int randrotation[2 * GRASSAREA * 2 * GRASSAREA];
int randx[2 * GRASSAREA * 2 * GRASSAREA];
int randz[2 * GRASSAREA * 2 * GRASSAREA];
int randrain[50][50];
float randrainx[50][50];
float randrainz[50][50];
float scalex[5];
float scalez[5];
float movey[5];
int xStart = 0, yStart = 0;
int action;

// 以下、表面の材質定義
/* ゴールド */
float diffuse1[] = { 0.7, 0.6, 0.2, 1.0 };
float specular1[] = { 0.6, 0.5, 0.4, 1.0 };
float ambient1[] = { 0.3, 0.2, 0.1, 1.0 };
float shininess1 = 50.0;
/* プラスチック */
float diffuse2[] = { 0.1, 0.4, 0.1, 1.0 };
float specular2[] = { 0.5, 0.6, 0.5, 1.0 };
float ambient2[] = { 0.0, 0.0, 0.0, 1.0 };
float shininess2 = 128.0;
/* 葉 */
float diffuse3[] = { 0.1, 0.6, 0.1, 1.0 };
float specular3[] = { 0.0, 0.0, 0.0, 1.0 };
float ambient3[] = { 0.1, 0.3, 0.1, 1.0 };
float shininess3 = 128.0;
/* 根っこ */
float diffuse4[] = { 0.7, 0.9, 0.7, 1.0 };
float specular4[] = { 0.0, 0.0, 0.0, 1.0 };
float ambient4[] = { 0.4, 0.6, 0.4, 1.0 };
float shininess4 = 128.0;
/* 木の基 */
float diffuse5[] = { 0.5, 0.25, 0.075 };
float specular5[] = { 0.0, 0.0, 0.0, 1.0 };
float ambient5[] = { 0.2, 0.01, 0.0, 1.0 };
float shininess5 = 128.0;
/* 草 */
float diffuse6[] = { 0.0, 0.5, 0.0 };
float specular6[] = { 0.0, 0.0, 0.0, 1.0 };
float ambient6[] = { 0.0, 0.2, 0.0, 1.0 };
float shininess6= 128.0;

float light0[] = { .0, 1.0, .0, 1.0 };    // ÂÖâÊ∫ê„ÅEÂêë„Åç„Å®Á®ÆÈ°?
float light1[] = { .0, 1.0, .0, 0.0 };    // ÂÖâÊ∫ê„ÅEÂêë„Åç„Å®Á®ÆÈ°?

GLfloat		cpoint[4][4][3] = {
    {
        { -0.05, 0.0, 0.0 },
        { -0.025, 0.0, 0.0 },
        { 0.025, 0.0, 0.0 },
        { 0.05, 0.0, 0.0 }
    },
    {
        { -0.05, 0.25, 0.0 },
        { -0.025, 0.25, 0.0 },
        { 0.025, 0.25, 0.0 },
        { 0.05, 0.25, 0.0 }
    },
    {
        { -0.05, 0.25, 0.0 },
        { -0.025, 0.25, 0.0 },
        { 0.025, 0.25, 0.0 },
        { 0.05, 0.25, 0.0 }
    },
    {
        { 0.0, 0.5, 0.0 },
        { 0.0, 0.5, 0.0 },
        { 0.0, 0.5, 0.0 },
        { 0.0, 0.5, 0.0 }
    }
};
GLfloat knotvec_v[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
GLfloat knotvec_u[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };

GLUnurbsObj *nrb_obj;

void create_nurbs()
{
    
    nrb_obj = gluNewNurbsRenderer();
    gluNurbsProperty(nrb_obj, GLU_SAMPLING_TOLERANCE, 100.0);
    gluNurbsProperty(nrb_obj, GLU_DISPLAY_MODE, GLU_FILL);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // „Ç¶„Ç£„É≥„Éâ„Ç¶„ÅÆËÉåÊôØ„ÇíÂ°ó„Çä„Å§„Å∂„Å? & Èö†Èù¢Âá¶ÁêÅEÇíÂèØËÉΩ„Å´„Åô„Çã
    glEnable(GL_DEPTH_TEST);    // Èö†Èù¢Âá¶ÁêÅEñãÂß?
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    
    glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    polarview();    // „Éù„ÅE„É©„Éº„Éì„É•„Éº„Å∏„ÅÆ„Éì„É•„Éº„Ç§„É≥„Ç∞Â§âÊèõ
    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, light0);
    glPopMatrix();
    
    shading(5);
    drawGround2D();
    
    shading(6);
    glDisable(GL_LIGHTING);
    drawGrass();
    glEnable(GL_LIGHTING);
    
    shading(5);
    drawPlant();
    
    if (rainFlag == GL_TRUE)
        drawRain();
    
    glDisable(GL_LIGHTING);
    
    glDisable(GL_DEPTH_TEST);
    
    drawCloud();
    
    if (thunderFlag == GL_TRUE && rainFlag == GL_TRUE) {
        if (lightningnum == 0 || lightningnum == 4)
            lightning();
        lightningnum++;
        if (lightningnum == 5) {
            lightningnum = 0;
            thunderFlag = GL_FALSE;
        }
        if (rand() % 10 == 4)
            fallFlag = GL_TRUE;
    }
    else thunderFlag = GL_FALSE;
    
    glPopMatrix();
    glutSwapBuffers();
}

void drawGround2D()
{
    float d = GRASSAREA / 4.0;
    float y = 5.0;
    
    glColor3f(0.4, 0.2, 0.1);
    glBegin(GL_POLYGON);
    glVertex3f(d, -y, d);
    glVertex3f(-d, -y, d);
    glVertex3f(-d, -y, -d);
    glVertex3f(d, -y, -d);
    glEnd();
}

void drawGrass()
{
    int i, j, k;
    int num = 0;
    glPushMatrix();
    glTranslatef(0.0, -5.0, 0.0);
    glColor3f(0.0, grasscolor, 0.0);
    for (i = -GRASSAREA; i<GRASSAREA; ++i){
        for (j = -GRASSAREA; j<GRASSAREA; ++j){
            glPushMatrix();
            glTranslatef((0.5 + i + ((randx[num] % (100 + 1)) / 100.0 - 0.5)) / 4.0, 0.0, (0.5 + j + ((randz[num] % (100 + 1)) / 100.0 - 0.5)) / 4.0);
            glRotatef(90 * (randrotation[num] % 101) / 100, 0.0, 1.0, 0.0);
            for (k = 0; k<4; ++k) {
                cpoint[3][k][2] = 0.2*sin(2 * PI*(randrotation[num] % 100 + 1) / 100 + thetagrass*PI / 180.0)*(1 - wind) + (0.5*sin(90 * (randrotation[num] % 101) / 100 * PI / 180.0))*wind;
                cpoint[3][k][0] = 0.5*cos(90 * (randrotation[num] % 101) / 100 * PI / 180.0)*wind;
                cpoint[3][k][1] = 0.5 + 0.05*sin(2 * PI*(randrotation[num] % 100 + 1) / 100 + thetawind*PI / 180.0)*wind;
            }
            gluBeginSurface(nrb_obj);
            gluNurbsSurface(nrb_obj,
                            8, knotvec_u,
                            8, knotvec_v,
                            3,
                            3 * 4,
                            &cpoint[0][0][0],
                            4, 4,
                            GL_MAP2_VERTEX_3
                            );
            gluEndSurface(nrb_obj);
            glPopMatrix();
            ++num;
        }
    }
    glPopMatrix();
}

void lightning()
{
    glEnable(GL_BLEND);
    glColor4f(1.0, 1.0, 1.0, 0.5);
    glutSolidCube(23);
    glDisable(GL_BLEND);
}
void drawCloud()
{
    int i, j;
    
    if (alphaFlag == GL_TRUE)
        glEnable(GL_BLEND);
    else
        glTranslatef(0.0, 10*(powf(2, -alpha*10)) , 0.0);
    glTranslatef(0.0, 6.0, 0.0);
    glColor4f(0.5, 0.5, 0.5, alpha);
    if (alpha > 0) {
        for (i = -4; i<4; ++i) {
            for (j = -4; j<4; ++j) {
                glPushMatrix();
                glTranslatef((float)i, 0.0, (float)j);
                glutSolidSphere(1.3*(1.0 + randcloud[i + 4][j + 4]), 10, 10);
                glPopMatrix();
            }
        }
    }
    glDisable(GL_BLEND);
}
void drawRain()
{
    int k;
    glDisable(GL_LIGHTING);
    
    if (maxrain < RAIN) {
        raini[rainnum] = raincounter1;
        rainj[rainnum] = raincounter2;
        if (rainnum == RAIN)
            rainnum = 0;
        else
            ++rainnum;
        maxrain++;
    }
    
    for (k = 0; k<RAIN; k++){
        if (rainspeedy[k] >= -10.0) {
            
            glPushMatrix();
            glTranslatef(0.0, 5.0, 0.0);
            glTranslatef(rainspeedx[k] + randrainx[raini[k]][rainj[k]] * GRASSAREA / 4., rainspeedy[k], randrainz[raini[k]][rainj[k]] * GRASSAREA / 4.);
            glColor3f(0.5, 0.5, 1.0);
            glBegin(GL_LINES);
            glVertex3f(-0.4*wind2, 0.5, 0.0);
            glVertex3f(0.0, 0.0, 0.0);
            glEnd();
            glPopMatrix();
        }
    }
    
    glEnable(GL_LIGHTING);
}


void drawPlant()
{
    int i;
    glEnable(GL_LIGHTING);
    shading(4);
    glPushMatrix();
    glTranslatef(0.0, -5.0 + plantHeight / 2, 0.0);
    glScalef(0.85, 0.85, 0.85);
    glRotatef(2 * wind, 0.0, 0.0, -1.0);
    glTranslatef(0.0, -plantHeight / 2., 0.0);
    glRotatef(falltheta - 1.0, 0.0, 0.0, -1.0);
    glPushMatrix();
    glRotatef(90, -1, 0, 0);
    
    gluCylinder(gluNewQuadric(), plantWidth, plantWidth, plantHeight, 10, 10);
    
    if (grow>1){
        shading(3);
        glPushMatrix();
        glRotatef(90, -1, 0, 0);
        glRotatef(100.0, 0.0, 1.0, 0.0);
        glTranslatef(plantWidth, -plantHeight / 2.5, 0.0);
        glRotatef(10 * sin(2 * PI*(randrotation[GRASSAREA - 6] % 100 + 1) / 100 + thetawind*PI / 180.0)*wind, 0.0, 0.0, 1.0);
        glRotatef(90, 1, 0, 0);
        gluDisk(gluNewQuadric(), 0.0, diskR, 10, 10);
        glPopMatrix();
        
        glPushMatrix();
        glRotatef(90, -1, 0, 0);
        glRotatef(-45.0, 0.0, 1.0, 0.0);
        glTranslatef(plantWidth, -plantHeight / 2., 0.0);
        glRotatef(10 * sin(2 * PI*(randrotation[GRASSAREA - 7] % 100 + 1) / 100 + thetawind*PI / 180.0)*wind, 0.0, 0.0, 1.0);
        glRotatef(90, 1, 0, 0);
        gluDisk(gluNewQuadric(), 0.0, diskR, 10, 10);
        glPopMatrix();
        
        glPushMatrix();
        glRotatef(90, -1, 0, 0);
        glRotatef(170.0, 0.0, 1.0, 0.0);
        glTranslatef(plantWidth, -plantHeight / 5., 0.0);
        glRotatef(10 * sin(2 * PI*(randrotation[GRASSAREA - 8] % 100 + 1) / 100 + thetawind*PI / 180.0)*wind, 0.0, 0.0, 1.0);
        glRotatef(90, 1, 0, 0);
        gluDisk(gluNewQuadric(), 0.0, diskR, 10, 10);
        glPopMatrix();
    }
    
    glPopMatrix();
    
    glTranslatef(0.0, plantHeight - 0.2, 0.0);
    
    if (falltheta <= 70)
        glTranslatef(0.3*wind, 0.0, 0.0);
    else
        glTranslatef(-0.1*pow(2.71, -t4 / 10)*sin(t4*PI / 4), 0.0, 0.0);
    
    if (grow>2) {
        for (i = 0; i<5; i++){
            glPushMatrix();
            glRotatef((float)i*360. / 5., 0.0, 1.0, 0.0);
            if (falltheta <= 70)
                glTranslatef(0.0, 0.05*sin(2 * PI*(randrotation[GRASSAREA - i] % 100 + 1) / 100 + thetawind*PI / 180.0)*wind, 0.0);
            glTranslatef(growleaves*0.1 + plantWidth, 0.2*movey[i], 0.0);
            glScalef(1. + .5*scalex[i], 0.9, 1. + .2*scalez[i]);
            glutSolidSphere(leafR, 10, 10);
            glPopMatrix();
        }
    }
    
    glPopMatrix();
    glDisable(GL_LIGHTING);
}
void idle(void)
{
    int i;
    
    if (windFlag == GL_FALSE) {
        if (wind > 0)
            wind -= 0.01;
        if (wind2 > 0)
            wind2 -= 0.05;
        thetagrass = fmod(thetagrass - 5, 360.0);
    }
    
    if (fallFlag == GL_TRUE && falltheta <= 70)
        falltheta = 1.1*falltheta;
    
    if (falltheta > 70)
        t4 += 1.0;
    
    if (windFlag == GL_TRUE) {
        if (wind < 1)
            wind += 0.1;
        else
            wind = 1.0;
        if (wind2 < 1)
            wind2 += 0.1;
        else
            wind2 = 1.0;
        thetawind = fmod(thetawind - 90, 360.0);
    }
    
    if (cloudFlag == GL_TRUE && alpha < 0.4) {
        alpha += 0.005;
        diffuse3[1] = 0.95*diffuse3[1] + 0.05*0.1;
        grasscolor = 0.95*grasscolor + 0.05*0.4;
        cloudmovingFlag = GL_TRUE;
    }
    else if (cloudFlag == GL_FALSE && alpha > 0) {
        alpha -= 0.005;
        cloudmovingFlag = GL_TRUE;
    }
    if (cloudFlag == GL_FALSE && alpha <= 0.1) {
        diffuse3[1] = 0.95*diffuse3[1] + 0.05*0.9;
    }
    if (cloudFlag == GL_FALSE && alpha <= 0.2) {
        grasscolor = 0.95*grasscolor + 0.05*0.8;
    }
    if (alpha <= 0) {
        diffuse3[1] = 0.9;
        grasscolor = 0.8;
    }
    if (alpha >= 0.4 || alpha <= 0.0)
        cloudmovingFlag = GL_FALSE;
    
    theta = fmod(theta - 5, 360.0);
    
    for (i = 0; i<RAIN; ++i) {
        if (raini[i] != -1 && rainj[i] != -1) {
            rainspeedy[i] -= 0.5;
            if (rainspeedx[i] + randrainx[raini[i]][rainj[i]] * GRASSAREA / 4. >= GRASSAREA / 4.)
                rainspeedx[i] = -GRASSAREA / 4. - randrainx[raini[i]][rainj[i]] * GRASSAREA / 4.;
            else
                rainspeedx[i] = wind*rainspeedx[i] + 0.4;
        }
        if (rainspeedy[i] <= -10.0) {
            if (rainFlag2 == GL_TRUE) {
                rainspeedy[i] = 0;
                rainspeedx[i] = 0;
                if (maxrain == RAIN)
                    maxrain--;
            }
            ++rainfinishcounter;
        }
        
    }
    if (rainfinishcounter == RAIN) {
        rainFlag = GL_FALSE;
        cloudFlag = GL_FALSE;
        for (i = 0; i<RAIN; ++i) {
            rainspeedy[i] = 0;
            maxrain = 0;
            raini[i] = -1;
            rainj[i] = -1;
        }
    }
    rainfinishcounter = 0;
    
    if (raincounter1 <= 50)
        raincounter1++;
    else if (raincounter1 >= 50) {
        raincounter1 = 0;
        raincounter2++;
        if (raincounter2 >= 50)
            raincounter2 = 0;
    }
    
    t += 0.05;
    plantHeight = 1.0 + 0.6*(grow - powf(2., -t));
    plantWidth = 0.2 + 0.06*(grow - powf(2., -t));
    
    if (grow>2 && growleaves<4)
        t2 += 0.05;
    if (t2 > 0.01)
        leafR = 0.4*(growleaves - powf(2., -t2));
    if (grow>1 && growdisks<4)
        t3 += 0.05;
    diskR = 0.1*(growdisks - powf(2.71, -t3));
    
    if (alpha >= 0.4) {
        rainFlag = GL_TRUE;
        rainFlag2 = GL_TRUE;
    }
    
    glutPostRedisplay();
}
void resetview(void)
{
    distance = 30.0;
    twist = 0.0;
    elevation = -20.0;
    azimuth = 20.0;
}
void polarview(void)
{
    glTranslatef(0.0, 0.0, -distance);
    glRotatef(-twist, 0.0, 0.0, 1.0);
    glRotatef(-elevation, 1.0, 0.0, 0.0);
    glRotatef(-azimuth, 0.0, 1.0, 0.0);
}
/* „Ç¶„Ç£„É≥„Éâ„Ç¶„ÅÆÊã°Â§ßÁ∏ÆÂ∞è„Å´ÂØæ„Åó„Å¶„É≠„Ç±„ÉÅEÉà„ÅÆÈ´ò„Åï„Å®ÂπÅEÅEÊØî„ÇíÊåÅÁ∂ö„Åï„Åõ„Çã */
void myReshape(int width, int height)
{
    float aspect = (float)width / (float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, aspect, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}
/* „Ç∑„Çß„Éº„ÉÅEÇ£„É≥„Ç∞„ÅÆÁ®ÆÈ°û„ÇíÂÆöÁæ©ÅEõÊèèÂÜô„Åô„ÇãÈöõ„Å´„Åì„ÅEÈñ¢Êï∞„ÇíÂëº„Å≥„ÄÅË°®Èù¢Â±ûÊÄß„ÇíË®≠ÂÆö„Åô„Ç? */
void shading(int num)
{
    switch (num) {
        case 1:
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse1);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular1);
            glMaterialfv(GL_FRONT, GL_AMBIENT, ambient1);
            glMaterialf(GL_FRONT, GL_SHININESS, shininess1);
            break;
        case 2:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse2);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular2);
            glMaterialfv(GL_FRONT, GL_AMBIENT, ambient2);
            glMaterialf(GL_FRONT, GL_SHININESS, shininess2);
            break;
        case 3:
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse3);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular3);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient3);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess3);
            break;
        case 4:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse4);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular4);
            glMaterialfv(GL_FRONT, GL_AMBIENT, ambient4);
            glMaterialf(GL_FRONT, GL_SHININESS, shininess4);
            break;
        case 5:
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse5);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular5);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient5);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess5);
            break;
        case 6:
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse6);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular6);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient6);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess6);
            break;
    }
}
void setRand()
{
    int i, j;
    for (i = 0; i<2 * GRASSAREA * 2 * GRASSAREA; ++i){
        randrotation[i] = rand();
        randx[i] = rand();
        randz[i] = rand();
    }
    for (i = 0; i<50; ++i)
        for (j = 0; j<50; ++j) {
            randrainx[i][j] = (rand() % 201) / 100.0 - 1.0;
            randrainz[i][j] = (rand() % 201) / 100.0 - 1.0;
        }
    for (i = 0; i<RAIN; ++i) {
        rainspeedy[i] = 0;
        rainspeedx[i] = 0;
        raini[i] = -1;
        rainj[i] = -1;
    }
    for (i = 0; i<10; ++i)
        for (j = 0; j<10; ++j) {
            randcloud[i][j] = (rand() % 201) / 100.0 - 1.0;
        }
    for (i = 0; i<5; ++i) {
        scalex[i] = (rand() % 201) / 100.0 - 1.0;
        scalez[i] = (rand() % 201) / 100.0 - 1.0;
        movey[i] = (rand() % 201) / 100.0 - 1.0;
    }
}
void camera(int camnum)
{
    switch (camnum) {
        case 1:
            freeFlag = GL_FALSE;
            distance = 30.0;
            twist = 0.0;
            elevation = -20.0;
            azimuth = 20.0;
            break;
        case 2:
            freeFlag = GL_FALSE;
            distance = 30.0;
            twist = 0.0;
            elevation = 0.0;
            azimuth = 0.0;
            break;
        case 3:
            freeFlag = GL_FALSE;
            distance = 30.0;
            twist = 0.0;
            elevation = -20.0;
            azimuth = -20.0;
            break;
        case 4:
            freeFlag = GL_FALSE;
            distance = 20.0;
            twist = 0.0;
            elevation = -90.0;
            azimuth = 0.0;
            break;
        case 5:
            freeFlag = GL_FALSE;
            distance = 15.0;
            twist = 0.0;
            elevation = 15.0;
            azimuth = 0.0;
            break;
        case 6:
            freeFlag = GL_FALSE;
            distance = 14.0;
            twist = 0.0;
            elevation = 18.0;
            azimuth = 45.0;
            break;
        case 7:
            freeFlag = GL_TRUE;
            distance = 30.0;
            twist = 0.0;
            elevation = -20.0;
            azimuth = 20.0;
            break;
    }
}
void myKbd(unsigned char key, int x, int y)
{
    int i;
    switch (key) {
        case '1':
            camera(1);
            break;
        case '2':
            camera(2);
            break;
        case '3':
            camera(3);
            break;
        case '4':
            camera(4);
            break;
        case '5':
            camera(5);
            break;
        case '6':
            camera(6);
            break;
        case '7':
            camera(7);
            break;
            /* w„Ç≠„Éº„ÅßÈ¢®„ÅÆON/OFFÂàÅEÇäÊõø„Å? */
        case 'w':
            windFlag = !windFlag;
            break;
            /* R„Ç≠„Éº„Åß„Éì„É•„Éº„ÅÆ„É™„Çª„ÉÅEÉà */
        case 'R':
            resetview();
            break;
            /* r„Ç≠„Éº„ÅßÈõ®„ÅÆON/OFFÂàÅEÇäÊõø„Å? */
        case 'r':
            cloudFlag = GL_TRUE;
            if (rainFlag2 == GL_TRUE && maxrain >= 10){
                cloudFlag = GL_FALSE;
                rainFlag2 = GL_FALSE;
            }
            break;
        case 't':
            thunderFlag = GL_TRUE;
            break;
        case 'f':
            fallFlag = GL_TRUE;
            break;
        case 'a':
            if (cloudmovingFlag == GL_FALSE)
                alphaFlag = !alphaFlag;
            break;
        case 'g':
            if (grow<5) {
                ++grow;
                t = 0.001;
            }
            if (grow>2 && growleaves<4){
                ++growleaves;
                t2 = 0.001;
            }
            if (grow>1 && growdisks<4){
                ++growdisks;
                t3 = 0.001;
            }
            for (i = 0; i<3; ++i) {
                diffuse4[i] = 0.8*diffuse4[i] + 0.2*diffuse5[i];
                specular4[i] = 0.8*specular4[i] + 0.2*specular5[i];
                ambient4[i] = 0.8*ambient4[i] + 0.2*ambient5[i];
            }
            break;
            /* ESC„Ç≠„Éº„Åß„Éó„É≠„Ç∞„É©„É†„ÅÆÁµÇ‰∫? */
        case KEY_ESC:
            exit(0);
    }
}
void mySkey(int key, int x, int y)
{
    switch (key) {
            
    }
    glutPostRedisplay();
}
/* „Éû„Ç¶„Çπ„ÇØ„É™„ÉÅEÇØÊôÇ„ÅE‰ΩçÁΩÆ„Å®„ÄÅÊäº„Åï„Çå„Åü„ÅE„Çø„É≥„ÅÆÁ®ÆÈ°û„ÅEË®òÈå≤ */
void myMouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        switch (button) {
            case GLUT_LEFT_BUTTON:
                mButton = button;
                break;
            case GLUT_MIDDLE_BUTTON:
                mButton = button;
                break;
            case GLUT_RIGHT_BUTTON:
                mButton = button;
                break;
        }
        xBegin = x;
        yBegin = y;
    }
}
/* „Éû„Ç¶„Çπ„Éâ„É©„ÉÅEÇ∞ÊôÇ„Å´„Éù„ÅE„É©„Éº„Éì„É•„Éº„Åß„ÅÆÂõûËª¢„ÇÅEã°Â§ß„ÄÅÁ∏ÆÂ∞è„ÇíË°å„ÅÜ */
void myMotion(int x, int y)
{
    int xDisp, yDisp;
    
    xDisp = x - xBegin;
    yDisp = y - yBegin;
    
    if (freeFlag == GL_TRUE) {
        switch (mButton) {
            case GLUT_LEFT_BUTTON:
                azimuth += (float)xDisp / 5.0;
                elevation -= (float)yDisp / 5.0;
                break;
            case GLUT_RIGHT_BUTTON:
                distance += (float)yDisp / 20.0;
                break;
            case GLUT_MIDDLE_BUTTON:
                twist += (float)yDisp / 10.0;
                break;
        }
    }
    xBegin = x;
    yBegin = y;
    glutPostRedisplay();
}
void myInit(char *progname)
{
    int width = 600, height = 600;
    float aspect = (float)width / (float)height;
    
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(progname);
    glClearColor(0.8, 1.0, 1.0, 1.0);
    
    glutKeyboardFunc(myKbd);
    glutSpecialFunc(mySkey);
    glutMouseFunc(myMouse);
    glutMotionFunc(myMotion);
    
    resetview();    //„Éì„É•„Éº„ÅÆÂàùÊúüÂå?
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, aspect, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    
    initLighting();
}

/* ÂÖâÊ∫ê„ÅEË®≠ÂÆ? */
void initLighting(void)
{
    float  diffuse[] = { 1.0, 1.0, 1.0, 1.0 };      //Êã°Êï£ÂèçÂ∞ÅEÅEÂàÅEÅEÂº∑Â∫¶
    float  specular[] = { 1.0, 1.0, 1.0, 1.0 };     //Èè°Èù¢ÂèçÂ∞ÅEÅEÂàÅEÅEÂº∑Â∫¶
    float  ambient[] = { 1.0, 1.0, 1.0, 1.0 };      //Áí∞Â¢ÅEÅEÊàêÂÅE„ÅÆÂº∑Â∫¶
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    
    glEnable(GL_LIGHT0);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    myInit(argv[0]);
    create_nurbs();
    setRand();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();
    return(0);
}

///* それぞれのライブラリをインポート */
//#include <stdlib.h>
//#include <GLUT/glut.h>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
//#include <math.h>
//#include <unistd.h>
//#include "myShape.h"
///* ESCキーの定義 */
//#define KEY_ESC 27
//#define PI 3.14159265358979323846264338327950288419716939937510582
//#define GRASSAREA 20
//#define RAIN 100
///* 各変数と関数の定義 */
//void drawGround2D(void);    // 2D地面の描写
//void drawPlant(void);       // 木の根っこ
//void drawGrass(void);       // 草
//void drawRain(void);        // 雨
//void setRand(void);         // ランダム設置
//void drawCloud(void);
//void lightning(void);
//void camera(int);
//float theta = 0.0;
//float thetatree = 0.0;
//float thetagrass = 0.0;
//float thetawind = 0.0;
//int rainnum = 0;            // 落ちている雨粒の番号
//int maxrain = 0;
//float falltheta = 1.0;
//int raini[RAIN];
//int rainj[RAIN];
//int raincounter1 = 0;
//int raincounter2 = 0;
//int rainfinishcounter = 0;
//float rainspeedy[RAIN];
//float rainspeedx[RAIN];
//float randcloud[10][10];
//float branchshake = 0;
//int lightningnum = 0;
//float diskR = 0.0;
//float leafR = 0.0;
//float alpha = 0.0;
//float grasscolor = 0.8;
//void polarview(void);       // ポーラービュー
//void resetview(void);       // ビューのリセット
//void shading(int);          // シェーディングの選択の関数
//void initLighting(void);    // 光源の設定の関数
//unsigned char windFlag = GL_FALSE;      // 風のフラグ
//unsigned char rainFlag = GL_FALSE;    // 雨のフラグ
//unsigned char rainFlag2 = GL_FALSE;    // 雨のフラグ
//unsigned char thunderFlag = GL_FALSE;    // 雷のフラグ
//unsigned char cloudFlag = GL_FALSE;    // 雲のフラグ
//unsigned char fallFlag = GL_FALSE;    // 雲のフラグ
//int xBegin, yBegin;         // マウスクリック時のポジション
//int mButton;                // マウスクリック時のボタンの種類
//float distance, twist, elevation, azimuth;  // ポーラービューの変数
//int change = 0;
//float plantWidth = 0.2;
//float plantHeight = 1.0;
//float grow = 0.0;
//float growleaves = 0.0;
//float growdisks = 0.0;
//float t = 100;
//float t2 = 0;
//float t3 = 0;
//float t4 = 0;
//float wind = 0;
//float wind2 = 0;
//int randrotation[2*GRASSAREA*2*GRASSAREA];
//int randx[2*GRASSAREA*2*GRASSAREA];
//int randz[2*GRASSAREA*2*GRASSAREA];
//int randrain[50][50];
//float randrainx[50][50];
//float randrainz[50][50];
//float scalex[5];
//float scalez[5];
//float movey[5];
//int xStart = 0, yStart = 0;
//int action;
//
///* ゴールド */
//float diffuse1[] = { 0.7, 0.6, 0.2, 1.0 };
//float specular1[] = { 0.6, 0.5, 0.4, 1.0 };
//float ambient1[] = { 0.3, 0.2, 0.1, 1.0 };
//float shininess1 = 50.0;
///* プラスチック */
//float diffuse2[] = { 0.1, 0.4, 0.1, 1.0 };
//float specular2[] = { 0.5, 0.6, 0.5, 1.0 };
//float ambient2[] = { 0.0, 0.0, 0.0, 1.0 };
//float shininess2 = 128.0;
///* 葉 */
//float diffuse3[] = { 0.1, 0.9, 0.1, 1.0 };
//float specular3[] = { 0.0, 0.0, 0.0, 1.0 };
//float ambient3[] = { 0.1, 0.5, 0.1, 1.0 };
//float shininess3 = 128.0;
///* 根っこ */
//float diffuse4[] = { 0.7, 0.9, 0.7, 1.0 };
//float specular4[] = { 0.0, 0.0, 0.0, 1.0 };
//float ambient4[] = { 0.4, 0.6, 0.4, 1.0 };
//float shininess4 = 128.0;
///* 木の本 */
//float diffuse5[] = { 0.5, 0.25, 0.075 };
//float specular5[] = { 0.0, 0.0, 0.0, 1.0 };
//float ambient5[] = { 0.2, 0.01, 0.0, 1.0 };
//float shininess5 = 128.0;
///* 草 */
//float diffuse6[] = { 0.0, 0.5, 0.0 };
//float specular6[] = { 0.0, 0.0, 0.0, 1.0 };
//float ambient6[] = { 0.0, 0.2, 0.0, 1.0 };
//float shininess6= 128.0;
//
//float light0[] = { .0, 1.0, .0, 1.0 };    // 光源の向きと種類
//float light1[] = { .0, 1.0, .0, 0.0 };    // 光源の向きと種類
//
//GLfloat		cpoint[4][4][3]={
//    {
//        {-0.05, 0.0, 0.0},
//        {-0.025, 0.0, 0.0},
//        {0.025, 0.0, 0.0},
//        {0.05, 0.0, 0.0}
//    },
//    {
//        {-0.05, 0.25, 0.0},
//        {-0.025, 0.25, 0.0},
//        {0.025, 0.25, 0.0},
//        {0.05, 0.25, 0.0}
//    },
//    {
//        {-0.05, 0.25, 0.0},
//        {-0.025, 0.25, 0.0},
//        {0.025, 0.25, 0.0},
//        {0.05, 0.25, 0.0}
//    },
//    {
//        {0.0, 0.5, 0.0},
//        {0.0, 0.5, 0.0},
//        {0.0, 0.5, 0.0},
//        {0.0, 0.5, 0.0}
//    }
//};
//GLfloat knotvec_v[8]={0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0};
//GLfloat knotvec_u[8]={0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0};
//
//GLUnurbsObj *nrb_obj;
//
//void create_nurbs()
//{
//
//    nrb_obj = gluNewNurbsRenderer();
//    gluNurbsProperty(nrb_obj, GLU_SAMPLING_TOLERANCE, 100.0);
//    gluNurbsProperty(nrb_obj, GLU_DISPLAY_MODE, GLU_FILL);
//}
//
//void display(void)
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ウィンドウの背景を塗りつぶす & 隠面処理を可能にする
//    glEnable(GL_DEPTH_TEST);    // 隠面処理開始
//    glEnable(GL_NORMALIZE);
//    glEnable(GL_LIGHTING);
//    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//
//    glPushMatrix();
//    glTranslatef(0.0, 1.0, 0.0);
//    polarview();    // ポーラービューへのビューイング変換
//    glPushMatrix();
//    glLightfv(GL_LIGHT0, GL_POSITION, light0);
//    glPopMatrix();
//
//    shading(5);
//    drawGround2D();
//
//    shading(6);
//    glDisable(GL_LIGHTING);
//    drawGrass();
//    glEnable(GL_LIGHTING);
//
//    shading(5);
//    drawPlant();
//
//    if (rainFlag == GL_TRUE)
//        drawRain();
//
//    glDisable( GL_LIGHTING );
//
//    glDisable(GL_DEPTH_TEST);
//
//    drawCloud();
//
//    if (thunderFlag == GL_TRUE && rainFlag == GL_TRUE) {
//        if (lightningnum == 0 || lightningnum == 4)
//            lightning();
//        lightningnum++;
//        if (lightningnum == 5) {
//            lightningnum = 0;
//            thunderFlag = GL_FALSE;
//        }
//        if (rand()%10 == 4)
//            fallFlag = GL_TRUE;
//    }
//    else thunderFlag = GL_FALSE;
//
//    glPopMatrix();
//    glutSwapBuffers();
//}
//void lightning()
//{
//    glEnable (GL_BLEND);
//    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glColor4f(1.0, 1.0, 1.0, 0.5);
//    glutSolidCube(23);
//    glDisable (GL_BLEND);
//}
//void drawCloud()
//{
//    int i, j;
//    glEnable (GL_BLEND);
//    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glTranslatef(0.0, 6.0, 0.0);
//    glColor4f(0.5, 0.5, 0.5, alpha);
//    if (alpha > 0) {
//        for (i=-5; i<5; ++i) {
//            for (j=-5; j<5; ++j) {
//                glPushMatrix();
//                glTranslatef((float)i, 0.0, (float)j);
//                glutSolidSphere(1.5*(1.0+randcloud[i+5][j+5]), 10, 10);
//                glPopMatrix();
//            }
//        }
//    }
//    glDisable (GL_BLEND);
//}
//void drawRain()
//{
//    int k;
//    glDisable(GL_LIGHTING);
//
//    if (maxrain < RAIN) {
//        raini[rainnum] = raincounter1;
//        rainj[rainnum] = raincounter2;
//        if (rainnum == RAIN)
//            rainnum = 0;
//        else
//            ++rainnum;
//        maxrain++;
//    }
//
//    for (k=0; k<RAIN; k++){
//        if (rainspeedy[k] >= -10.0) {
//
//            glPushMatrix();
//            glTranslatef(0.0,5.0,0.0);
//            glTranslatef(rainspeedx[k]+randrainx[raini[k]][rainj[k]]*GRASSAREA/4.,rainspeedy[k],randrainz[raini[k]][rainj[k]]*GRASSAREA/4.);
//            glColor3f(0.5, 0.5, 1.0);
//            glBegin(GL_LINES);
//            glVertex3f(-0.4*wind2, 0.5, 0.0);
//            glVertex3f(0.0, 0.0, 0.0);
//            glEnd();
//            glPopMatrix();
//        }
//    }
//
//    glEnable(GL_LIGHTING);
//}
//void drawGrass()
//{
//    int i, j, k;
//    int num = 0;
//    glPushMatrix();
//    glTranslatef(0.0, -5.0, 0.0);
//    glColor3f(0.0, grasscolor, 0.0);
//    for(i=-GRASSAREA; i<GRASSAREA; ++i){
//        for(j=-GRASSAREA; j<GRASSAREA; ++j){
//            glPushMatrix();
//            glTranslatef((0.5+i+((randx[num]%(100+1))/100.0 - 0.5))/4.0, 0.0, (0.5+j+((randz[num]%(100+1))/100.0 - 0.5))/4.0);
//            glRotatef(90*(randrotation[num]%101)/100, 0.0, 1.0, 0.0);
//            for (k=0; k<4; ++k) {
//                cpoint[3][k][2] = 0.2*sin(2*PI*(randrotation[num]%100 + 1)/100 + thetagrass*PI/180.0)*(1-wind) + (0.5*sin(90*(randrotation[num]%101)/100*PI/180.0))*wind;
//                cpoint[3][k][0] = 0.5*cos(90*(randrotation[num]%101)/100*PI/180.0)*wind;
//                cpoint[3][k][1] = 0.5 + 0.05*sin(2*PI*(randrotation[num]%100 + 1)/100 + thetawind*PI/180.0)*wind;
//            }
//            gluBeginSurface(nrb_obj);
//            gluNurbsSurface(nrb_obj,
//                            8,knotvec_u,
//                            8,knotvec_v,
//                            3,
//                            3*4,
//                            &cpoint[0][0][0],
//                            4,4,
//                            GL_MAP2_VERTEX_3
//                            );
//            gluEndSurface(nrb_obj);
//            glPopMatrix();
//            ++num;
//        }
//    }
//    glPopMatrix();
//}
//void drawGround2D()
//{
//    float d = GRASSAREA/4.0;
//    float h = 5.0;
//    /* 地面 */
//    glColor3f(0.4, 0.2, 0.1);
//    glBegin(GL_POLYGON);
//    glVertex3f(d, -h, d);
//    glVertex3f(-d, -h, d);
//    glVertex3f(-d, -h, -d);
//    glVertex3f(d, -h, -d);
//    glEnd();
//}
//void drawPlant()
//{
//    int i;
//    glEnable(GL_LIGHTING);
//    shading(4);
//    glPushMatrix();
//    glTranslatef(0.0, -5.0+plantHeight/2, 0.0);
//    glRotatef(2*wind, 0.0, 0.0, -1.0);
//    glTranslatef(0.0, -plantHeight/2., 0.0);
//    glRotatef(falltheta-1.0, 0.0, 0.0, -1.0);
//    glTranslatef(0.0, plantHeight/2., 0.0);
//    mySolidCylinder(plantWidth, plantHeight, 10);
//    if (grow>1){
//        shading(3);
//        glPushMatrix();
//        glRotatef(100.0, 0.0, 1.0, 0.0);
//        glTranslatef(plantWidth, plantHeight/10., 0.0);
//        glRotatef(10*sin(2*PI*(randrotation[GRASSAREA-6]%100 + 1)/100 + thetawind*PI/180.0)*wind, 0.0, 0.0, 1.0);
//        myDisc(diskR, 10);
//
//        glPopMatrix();
//        glPushMatrix();
//        glRotatef(-45.0, 0.0, 1.0, 0.0);
//        glTranslatef(plantWidth, -plantHeight/4., 0.0);
//        glRotatef(10*sin(2*PI*(randrotation[GRASSAREA-7]%100 + 1)/100 + thetawind*PI/180.0)*wind, 0.0, 0.0, 1.0);
//        myDisc(diskR, 10);
//        glPopMatrix();
//        glPushMatrix();
//        glRotatef(170.0, 0.0, 1.0, 0.0);
//        glTranslatef(plantWidth, -plantHeight/10., 0.0);
//        glRotatef(10*sin(2*PI*(randrotation[GRASSAREA-8]%100 + 1)/100 + thetawind*PI/180.0)*wind, 0.0, 0.0, 1.0);
//        myDisc(diskR, 10);
//        glPopMatrix();
//    }
//
//    glTranslatef(0.0, plantHeight/2.-0.2, 0.0);
//
//    if (falltheta <= 70)
//        glTranslatef(0.3*wind, 0.0, 0.0);
//    else
//        glTranslatef(-0.1*pow(2.71, -t4/10)*sin(t4*PI/4), 0.0, 0.0);
//
//    if(grow>2) {
//        for (i=0; i<5; i++){
//            glPushMatrix();
//            glRotatef((float)i*360./5., 0.0, 1.0, 0.0);
//            if (falltheta <= 70)
//                glTranslatef(0.0, 0.05*sin(2*PI*(randrotation[GRASSAREA-i]%100 + 1)/100 + thetawind*PI/180.0)*wind, 0.0);
//            glTranslatef(growleaves*0.1 + plantWidth, 0.2*movey[i], 0.0);
//            glScalef(1.+.5*scalex[i], 0.9, 1.+.2*scalez[i]);
//            glutSolidSphere(leafR, 10, 10);
//            glPopMatrix();
//        }
//    }
//
//    glPopMatrix();
//    glDisable(GL_LIGHTING);
//}
//void idle(void)
//{
//    int i;
//
//    if (windFlag == GL_FALSE) {
//        if (wind > 0)
//            wind -= 0.01;
//        if (wind2 > 0)
//            wind2 -= 0.05;
//        thetagrass = fmod(thetagrass - 5, 360.0);
//    }
//
//    if (fallFlag == GL_TRUE && falltheta <= 70)
//        falltheta = 1.1*falltheta;
//
//    if (falltheta > 70)
//        t4 += 1.0;
//
//    if ( windFlag == GL_TRUE) {
//        if ( wind < 1)
//            wind += 0.1;
//        else
//            wind = 1.0;
//        if ( wind2 < 1)
//            wind2 += 0.1;
//        else
//            wind2 = 1.0;
//        thetawind = fmod(thetawind - 90, 360.0);
//    }
//
//    if (cloudFlag == GL_TRUE && alpha < 0.4) {
//        alpha += 0.005;
//        diffuse3[1] = 0.95*diffuse3[1] + 0.05*0.1;
//        grasscolor = 0.95*grasscolor + 0.05*0.4;
//    }
//    else if (cloudFlag == GL_FALSE && alpha > 0) {
//        alpha -= 0.005;
//    }
//    if (cloudFlag == GL_FALSE && alpha <= 0.1) {
//        diffuse3[1] = 0.95*diffuse3[1] + 0.05*0.9;
//    }
//    if (cloudFlag == GL_FALSE && alpha <= 0.2) {
//        grasscolor = 0.95*grasscolor + 0.05*0.8;
//    }
//    if (alpha <= 0) {
//        diffuse3[1] = 0.9;
//        grasscolor = 0.8;
//    }
//
//    theta = fmod(theta - 5, 360.0);
//
//    for (i=0; i<RAIN; ++i) {
//        if ( raini[i] != -1 && rainj[i] != -1) {
//            rainspeedy[i] -= 0.5;
//            if (rainspeedx[i]+randrainx[raini[i]][rainj[i]]*GRASSAREA/4. >= GRASSAREA/4.)
//                rainspeedx[i] = -GRASSAREA/4.-randrainx[raini[i]][rainj[i]]*GRASSAREA/4.;
//            else
//                rainspeedx[i] = wind*rainspeedx[i]+0.4;
//        }
//        if (rainspeedy[i] <= -10.0) {
//            if (rainFlag2 == GL_TRUE) {
//                rainspeedy[i] = 0;
//                rainspeedx[i] = 0;
//                if (maxrain == RAIN)
//                    maxrain--;
//            }
//            ++rainfinishcounter;
//        }
//
//    }
//    if (rainfinishcounter == RAIN) {
//        rainFlag = GL_FALSE;
//        cloudFlag = GL_FALSE;
//        for (i=0; i<RAIN; ++i) {
//            rainspeedy[i] = 0;
//            maxrain = 0;
//            raini[i] = -1;
//            rainj[i] = -1;
//        }
//    }
//    rainfinishcounter = 0;
//
//    if (raincounter1 <= 50)
//        raincounter1++;
//    else if (raincounter1 >= 50) {
//        raincounter1 = 0;
//        raincounter2++;
//        if (raincounter2 >= 50)
//            raincounter2 = 0;
//    }
//
//    t += 0.05;
//    plantHeight = 1.0 + 0.6*(grow - powf(2.71, -t));
//    plantWidth = 0.2 + 0.06*(grow - powf(2.71, -t));
//
//    if (grow>2 && growleaves<4)
//        t2 += 0.05;
//    leafR = 0.4*(growleaves - powf(2.71, -t2));
//    if (grow>1 && growdisks<4)
//        t3 += 0.05;
//    diskR = 0.1*(growdisks - powf(2.71, -t3));
//
//    if (alpha >= 0.4) {
//        rainFlag = GL_TRUE;
//        rainFlag2 = GL_TRUE;
//    }
//
//    glutPostRedisplay();
//}
//void resetview(void)
//{
//    distance = 30.0;
//    twist = 0.0;
//    elevation = -20.0;
//    azimuth = 20.0;
//}
//void polarview(void)
//{
//    glTranslatef(0.0, 0.0, -distance);
//    glRotatef(-twist, 0.0, 0.0, 1.0);
//    glRotatef(-elevation, 1.0, 0.0, 0.0);
//    glRotatef(-azimuth, 0.0, 1.0, 0.0);
//}
///* ウィンドウの拡大縮小に対してロケットの高さと幅の比を持続させる */
//void myReshape(int width, int height)
//{
//    float aspect = (float)width / (float)height;
//    glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(30.0, aspect, 1.0, 50.0);
//    glMatrixMode(GL_MODELVIEW);
//}
///* シェーディングの種類を定義；描写する際にこの関数を呼び、表面属性を設定する */
//void shading(int num)
//{
//    switch(num) {
//        case 1:
//            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse1);
//            glMaterialfv(GL_FRONT, GL_SPECULAR, specular1);
//            glMaterialfv(GL_FRONT, GL_AMBIENT, ambient1);
//            glMaterialf(GL_FRONT, GL_SHININESS, shininess1);
//            break;
//        case 2:
//            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse2);
//            glMaterialfv(GL_FRONT, GL_SPECULAR, specular2);
//            glMaterialfv(GL_FRONT, GL_AMBIENT, ambient2);
//            glMaterialf(GL_FRONT, GL_SHININESS, shininess2);
//            break;
//        case 3:
//            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse3);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular3);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient3);
//            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess3);
//            break;
//        case 4:
//            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse4);
//            glMaterialfv(GL_FRONT, GL_SPECULAR, specular4);
//            glMaterialfv(GL_FRONT, GL_AMBIENT, ambient4);
//            glMaterialf(GL_FRONT, GL_SHININESS, shininess4);
//            break;
//        case 5:
//            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse5);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular5);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient5);
//            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess5);
//            break;
//        case 6:
//            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse6);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular6);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient6);
//            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess6);
//            break;
//    }
//}
//void setRand()
//{
//    int i, j;
//    for(i=0; i<2*GRASSAREA*2*GRASSAREA; ++i){
//        randrotation[i] = rand();
//        randx[i] = rand();
//        randz[i] = rand();
//    }
//    for (i=0; i<50; ++i)
//        for (j=0; j<50; ++j) {
//            randrainx[i][j] = (rand()%201)/100.0-1.0;
//            randrainz[i][j] = (rand()%201)/100.0-1.0;
//        }
//    for (i=0; i<RAIN; ++i) {
//        rainspeedy[i] = 0;
//        rainspeedx[i] = 0;
//        raini[i] = -1;
//        rainj[i] = -1;
//    }
//    for (i=0; i<10; ++i)
//        for (j=0; j<10; ++j) {
//            randcloud[i][j] = (rand()%201)/100.0-1.0;
//        }
//    for (i=0; i<5; ++i) {
//        scalex[i] = (rand()%201)/100.0-1.0;
//        scalez[i] = (rand()%201)/100.0-1.0;
//        movey[i] = (rand()%201)/100.0-1.0;
//    }
//}
//void camera(int camnum)
//{
//    switch (camnum) {
//        case 1:
//            distance = 30.0;
//            twist = 0.0;
//            elevation = -20.0;
//            azimuth = 20.0;
//            break;
//        case 2:
//            distance = 30.0;
//            twist = 0.0;
//            elevation = 0.0;
//            azimuth = 0.0;
//            break;
//        case 3:
//            distance = 30.0;
//            twist = 0.0;
//            elevation = -20.0;
//            azimuth = -20.0;
//            break;
//        case 4:
//            distance = 20.0;
//            twist = 0.0;
//            elevation = -90.0;
//            azimuth = 0.0;
//            break;
//        case 5:
//            distance = 10.0;
//            twist = 0.0;
//            elevation = 0.0;
//            azimuth = 0.0;
//            break;
//        case 6:
//            distance = 14.0;
//            twist = 0.0;
//            elevation = 18.0;
//            azimuth = 45.0;
//            break;
//    }
//}
//void myKbd(unsigned char key, int x, int y)
//{
//    int i;
//    switch (key) {
//        case '1':
//            camera(1);
//            break;
//        case '2':
//            camera(2);
//            break;
//        case '3':
//            camera(3);
//            break;
//        case '4':
//            camera(4);
//            break;
//        case '5':
//            camera(5);
//            break;
//        case '6':
//            camera(6);
//            break;
//            /* wキーで風のON/OFF切り替え */
//        case 'w':
//            windFlag = !windFlag;
//            break;
//            /* Rキーでビューのリセット */
//        case 'R':
//            resetview();
//            break;
//            /* rキーで雨のON/OFF切り替え */
//        case 'r':
//            cloudFlag = GL_TRUE;
//            if (rainFlag2 == GL_TRUE && maxrain == RAIN){
//                cloudFlag = GL_FALSE;
//                rainFlag2 = GL_FALSE;
//            }
//            break;
//        case 't':
//            thunderFlag = GL_TRUE;
//            break;
//        case 'f':
//            fallFlag = GL_TRUE;
//            break;
//        case 'g':
//            if (grow<5) {
//                ++grow;
//                t = 0.0;
//            }
//            if (grow>2 && growleaves<4){
//                ++growleaves;
//                t2 = 0.0;
//            }
//            if (grow>1 && growdisks<4){
//                ++growdisks;
//                t3 = 0.0;
//            }
//            for (i=0; i<3; ++i) {
//                diffuse4[i] = 0.8*diffuse4[i] + 0.2*diffuse5[i];
//                specular4[i] = 0.8*specular4[i] + 0.2*specular5[i];
//                ambient4[i] = 0.8*ambient4[i] + 0.2*ambient5[i];
//            }
//            break;
//            /* ESCキーでプログラムの終了 */
//        case KEY_ESC:
//            exit(0);
//    }
//}
//void mySkey(int key, int x, int y)
//{
//    switch (key) {
//
//    }
//    glutPostRedisplay();
//}
///* マウスクリック時の位置と、押されたボタンの種類の記録 */
//void myMouse(int button, int state, int x, int y)
//{
//    if (state == GLUT_DOWN) {
//        switch (button) {
//            case GLUT_LEFT_BUTTON:
//                mButton = button;
//                break;
//            case GLUT_MIDDLE_BUTTON:
//                mButton = button;
//                break;
//            case GLUT_RIGHT_BUTTON:
//                mButton = button;
//                break;
//        }
//        xBegin = x;
//        yBegin = y;
//    }
//}
///* マウスドラッグ時にポーラービューでの回転や拡大、縮小を行う */
//void myMotion(int x, int y)
//{
//    int xDisp, yDisp;
//
//    xDisp = x - xBegin;
//    yDisp = y - yBegin;
//
//    switch (mButton) {
//        case GLUT_LEFT_BUTTON:
//            azimuth += (float)xDisp / 5.0;
//            elevation -= (float)yDisp / 5.0;
//            break;
//        case GLUT_RIGHT_BUTTON:
//            distance += (float)yDisp / 20.0;
//            break;
//        case GLUT_MIDDLE_BUTTON:
//            twist += (float)yDisp / 10.0;
//            break;
//    }
//    xBegin = x;
//    yBegin = y;
//    glutPostRedisplay();
//}
//void myInit(char *progname)
//{
//    int width = 600, height = 600;
//    float aspect = (float)width / (float)height;
//
//    glutInitWindowPosition(0, 0);
//    glutInitWindowSize(width, height);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);  // アニメーションのバッファの追加、使用する色の種類の設定、隠面処理の有効化
//    glutCreateWindow(progname);
//    glClearColor(0.8, 1.0, 1.0, 1.0);
//
//    /* それぞれの割り込み処理を有効化 */
//    glutKeyboardFunc(myKbd);
//    glutSpecialFunc(mySkey);
//    glutMouseFunc(myMouse);
//    glutMotionFunc(myMotion);
//
//    resetview();    //ビューの初期化
//
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(30.0, aspect, 1.0, 30.0);
//    glMatrixMode(GL_MODELVIEW);
//
//    initLighting();
//}
//
///* 光源の設定 */
//void initLighting( void )
//{
//    float  diffuse[] = { 1.0, 1.0, 1.0, 1.0 };      //拡散反射成分の強度
//    float  specular[] = { 1.0, 1.0, 1.0, 1.0 };     //鏡面反射成分の強度
//    float  ambient[] = { 1.0, 1.0, 1.0, 1.0 };      //環境光成分の強度
//
//    /* 光源１にそれぞれの成分を入れる */
//    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
//    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
//    glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
//
//    glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse );
//    glLightfv( GL_LIGHT1, GL_SPECULAR, specular );
//    glLightfv( GL_LIGHT1, GL_AMBIENT, ambient );
//
//    glEnable( GL_LIGHT0 );      //光源１を作動させる
//    //glEnable( GL_LIGHT1 );      //光源１を作動させる
//}
//
//
//int main(int argc, char** argv)
//{
//    glutInit(&argc, argv);
//    myInit(argv[0]);
//    create_nurbs();
//    setRand();
//    glutReshapeFunc(myReshape);
//    glutDisplayFunc(display);
//    glutIdleFunc(idle);
//    glutMainLoop();
//    return(0);
//}
//

//macÇ©ÇÁwindowsÇ…à⁄Ç∑Ç∆Ç´Ç…ï∂éöâªÇØÇµÇΩÇΩÇﬂÉRÉÅÉìÉgÇÕÇ›ÇÁÇÍÇ»Ç



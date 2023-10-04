#include <GL/gl.h>
#include <GL/glut.h>

#if defined(_WIN32) || defined(WIN32)
     #include <windows.h>
#endif // defined

#define MAX_POINTS_ON_ON_FRAME 1000
#define MAX_POLY_ON_ON_FRAME 1000

int height = 500;
int width = 500;
int mode = GL_POINTS;

typedef struct {
    GLfloat tabPos[MAX_POINTS_ON_ON_FRAME][2];
    int tab_size_point;
}PointCloud;

typedef struct {
    PointCloud tabPoly[MAX_POLY_ON_ON_FRAME];
    int tab_size;
}TabPoly;

PointCloud pc;
TabPoly tp;

void window_reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width/2, width/2, -height/2, height/2, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void window_display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    //render_scene();
    //drawLineNaif(50, 150, 300, 350);
    //incrementale(0, 150, 0, 350);
    //bresenhamNE(50, 150, 300, 350);
    //bresenhamGeneral(50, 150, 300, 50);
    pointMilieuCircle(200);
    glFlush();
}

void render_scene()  {
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(3.0);
    for(int i = 0; i < tp.tab_size; i++) {
        glBegin(mode);
            for(int j = 0; j < tp.tabPoly[i].tab_size_point; j++) {
                glVertex2f(tp.tabPoly[i].tabPos[j][0], tp.tabPoly[i].tabPos[j][1]);
            }
        glEnd();
    }
}

void draw_pixel(float x, float y) {
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

void drawLineNaif(int xa, int xb, int ya, int yb) {
    float dx, dy, m, y, b;
    dx = (xb - xa);
    dy = (yb - ya);
    m = (dy/dx);
    b = ya-(m*xa);
    for(int x = xa; x <= xb; x++) {
        y = (m*x) + b;
        draw_pixel(x, y);
    }
}

void incrementaleBase(int xa, int xb, int ya, int yb) {
    float m, dx, dy;
    float y;
    dx = (xb - xa);
    dy = (yb - ya);
    m = dy/dx;
    y = ya;
    for(int x = xa; x <= xb; x++) {
        draw_pixel(x, y);
        y =y + m;
    }
}

void bresenhamNE(int xa, int xb, int ya, int yb) {
    float dx, dy, increE, increNE, dp;
    int x, y;
    dy = (yb - ya);
    dx = (xb - xa);
    increE = 2*dy;
    increNE = 2*(dy - dx);
    dp = 2*dy - dx;
    y = ya;
    for(int x = xa; x <= xb; x++){
        draw_pixel(x, y);
        if(dp <= 0){
            dp += increE;
        } else {
            y -= 1;
            dp += increNE;
        }
    }
}

void bresenhamGeneral(int xa, int xb, int ya, int yb) {
    float dx, dy, incrX, incrY, dp;
    dx = (xb - xa);
    dy = (ya - yb);
    if( dx > 0) {
        incrX = 1;
    } else {
        incrX = -1;
        dx = -dx;
    }
    if(dy > 0) {
        incrY = 1;
    } else {
        incrY = -1;
        dy = -dy;
    }
    if(dx >= dy) {
        dp = 2*dy - dx;
        int incrE = 2 * dy;
        int incrNE = 2 * (dy - dx);
        int y = ya;
        for(int x = xa; x <= xb; x++){
            draw_pixel(x, y);
            if(dp <= 0){
                dp += incrE;
            } else {
                y = y - incrY;
                dp += incrNE;
            }
        }
    } else {
        dp = 2*dx-dy;
        int x = xa;
        int incrN = 2*dx;
        int incrNE = 2*(dx - dy);
        for(int y = ya; y <= yb; y++) {
            if(dp <= 0) {
                dp = dp + incrN;
            } else {
                x = x - incrX;
                dp = dp + incrNE;
            }
        }
    }
}

void affichePixelCircle(int x, int y) {
    draw_pixel(x, y);
    draw_pixel(x, -y);
    draw_pixel(-x, y);
    draw_pixel(-x, -y);
    draw_pixel(y, x);
    draw_pixel(y, -x);
    draw_pixel(-y, x);
    draw_pixel(-y, -x);
}

void pointMilieuCircle(int r) {
    int x = 0;
    int y = r;
    float d = 1 - r;
    draw_pixel(x, y);
    while (y > x) {
        if(d < 0) {
            d = d + 2*x + 3;
        } else {
            d = d + 2*x - 2*y +5;
            y = y - 1;
        }
        x = x + 1;
        affichePixelCircle(x, y);
    }

}

void mouse_click(int button, int state, int x, int y) {
    GLfloat a = x;
    GLfloat b = y;

    if((button == 0) && (state == 0)) {
        tp.tabPoly[tp.tab_size-1].tabPos[tp.tabPoly[tp.tab_size-1].tab_size_point][0] = a;
        tp.tabPoly[tp.tab_size-1].tabPos[tp.tabPoly[tp.tab_size-1].tab_size_point][1] = b;
        tp.tabPoly[tp.tab_size-1].tab_size_point += 1;
    } else if((button == 2)&&(state == 0)) {
        tp.tab_size += 1;
        PointCloud pc1 = pc;
        tp.tabPoly[tp.tab_size-1] = pc1;
        pc.tab_size_point = 0;
    }
    glutPostRedisplay();
}

void keyboard_press(unsigned char key, int x, int y) {
    switch(key) {
        case 'd' :
            mode = GL_POINTS;
            break;
        case 'l' :
            mode = GL_LINES;
            break;
        case 'p' :
            mode = GL_POLYGON;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {

    tp.tabPoly[0] = pc;
    tp.tab_size = 1;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(height, width);
    glutCreateWindow("Fixed Rectangle");
    glutDisplayFunc(window_display);
    glutReshapeFunc(window_reshape);
    glutKeyboardFunc(keyboard_press);
    glutMouseFunc(mouse_click);

    glutMainLoop();

    return 0;
}

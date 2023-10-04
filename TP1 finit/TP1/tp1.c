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
    glOrtho(-width/2, width/2, -height/2, height/2, -250.0, 250.0);
    glMatrixMode(GL_MODELVIEW);
}

void window_display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    render_scene();
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

void mouse_click(int button, int state, int x, int y) {
    GLfloat a = x - (width / 2);
    GLfloat b = -(y-(height/2));

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

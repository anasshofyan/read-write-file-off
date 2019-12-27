#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>

using namespace std;


typedef struct
{
    float x, y, z;
} point3D_t;

typedef struct
{
    float r, g, b;
} color_t;

typedef struct
{
    float x,y,z,r,g,b,s;
} point3D_color_t;

typedef struct
{
    float v[3];
} vector3D_t;

typedef struct
{
    float m[3][3];
} matrix3D_t;

typedef struct
{
    int numofVertices;
    short int pnt[68];
} face_t;

typedef struct
{
    int numofVertices;
    point3D_color_t pnt[100];
    int numofFaces;
    face_t fc[68];
} object3D_t;

typedef struct
{
    int numofVertices;
    point3D_color_t pnt[100];
    int numofFaces;
    face_t fc[66];
} object3D_color_t;

void setColor(color_t col)
{
    glColor3f(col.r, col.g, col.b);
}

void timer (int value)
{
    glutPostRedisplay();
    glutTimerFunc(10,timer,0);
}

void create3DObject(object3D_color_t object)
{
    for(int i=0; i<object.numofFaces; i++)
    {
        color_t col = {object.pnt[i].r, object.pnt[i].g, object.pnt[i].b};
        setColor(col);
        glBegin(GL_POLYGON);
        for(int j=0; j<object.fc[i].numofVertices; j++)
        {
            int p=object.fc[i].pnt[j];
            float x=object.pnt[p].x;
            float y=object.pnt[p].y;
            glVertex3f(x, y, 0.0);
        }
        glEnd();
    }
}

void createInvisible_color(point3D_color_t p[3])
{
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<3; i++)
    {
        p[i].z=0.0;
        glVertex3f(p[i].x, p[i].y,p[i].z);
    }
    glEnd();
}

void createVisible_color(point3D_color_t p[3])
{
    glBegin(GL_POLYGON);
    //interpolasi warna dari tiga titik
    float r= (p[0].r+p[1].r+p[2].r)/3;
    float g= (p[0].g+p[1].g+p[2].g)/3;
    float b= (p[0].b+p[1].b+p[2].b)/3;
    glColor3f(r, g, b);
    for(int i=0; i<3; i++)
    {
        p[i].z=0.0;
        glVertex3f(p[i].x, p[i].y,p[i].z);
    }
    glEnd();
}

matrix3D_t createIdentity()
{
    matrix3D_t rotate;
    rotate.m[0][0]=0.0;
    rotate.m[0][1]=0.0;
    rotate.m[0][2]=0.0;
    rotate.m[1][0]=0.0;
    rotate.m[1][1]=0.0;
    rotate.m[1][2]=0.0;
    rotate.m[2][0]=0.0;
    rotate.m[2][1]=0.0;
    rotate.m[2][2]=0.0;
    return rotate;
}

matrix3D_t rotationX(float teta)
{
    matrix3D_t rotate=createIdentity();
    rotate.m[0][0]=1.0;
    rotate.m[0][1]=0.0;
    rotate.m[0][2]=0.0;
    rotate.m[1][0]=0.0;
    rotate.m[1][1]=cos(teta/57.3);
    rotate.m[1][2]=-sin(teta/57.3);
    rotate.m[2][0]=0.0;
    rotate.m[2][1]=sin(teta/57.3);
    rotate.m[2][2]=cos(teta/57.3);
    return rotate;
}

matrix3D_t rotationY(float teta)
{
    matrix3D_t rotate=createIdentity();
    rotate.m[0][0]=cos(teta/57.3);
    rotate.m[0][1]=0.0;
    rotate.m[0][2]=sin(teta/57.3);
    rotate.m[1][0]=0.0;
    rotate.m[1][1]=1.0;
    rotate.m[1][2]=0.0;
    rotate.m[2][0]=-sin(teta/57.3);
    rotate.m[2][1]=0.0;
    rotate.m[2][2]=cos(teta/57.3);
    return rotate;
}

matrix3D_t rotationZ(float teta)
{
    matrix3D_t rotate=createIdentity();
    rotate.m[0][0]=cos(teta/57.3);
    rotate.m[0][1]=-sin(teta/57.3);
    rotate.m[0][2]=0.0;
    rotate.m[1][0]=sin(teta/57.3);
    rotate.m[1][1]=cos(teta/57.3);
    rotate.m[1][2]=0.0;
    rotate.m[2][0]=0.0;
    rotate.m[2][1]=0.0;
    rotate.m[2][2]=1.0;
    return rotate;
}

vector3D_t operator +(vector3D_t a, vector3D_t b)
{
    vector3D_t c;
    for(int i=0; i<3; i++)
    {
        c.v[i]=a.v[i]+b.v[i];
    }
    return c;
}

vector3D_t operator -(vector3D_t a, vector3D_t b)
{
    vector3D_t c;
    for(int i=0; i<3; i++)
    {
        c.v[i]=a.v[i]-b.v[i];
    }
    return c;
}

vector3D_t operator *(matrix3D_t a, vector3D_t b)
{
    vector3D_t c;
    for(int i=0; i<3; i++)
    {
        c.v[i]=0;
        for(int j=0; j<3; j++)
        {
            c.v[i]+=a.m[i][j]*b.v[j];
        }
    }
    return c;
}

vector3D_t operator ^(vector3D_t a,vector3D_t b)
{
    vector3D_t c;
    c.v[0]=a.v[1]*b.v[2]-a.v[2]*b.v[1];
    c.v[1]=a.v[2]*b.v[0]-a.v[0]*b.v[2];
    c.v[2]=a.v[0]*b.v[1]-a.v[1]*b.v[0];
    return c;
}

void create3DObject(object3D_t object, color_t col)
{
    for(int i=0; i<object.numofFaces; i++)
    {
        setColor(col);
        glBegin(GL_LINE_LOOP);
        for(int j=0; j<object.fc[i].numofVertices; j++)
        {
            int p=object.fc[i].pnt[j];
            float x=object.pnt[p].x;
            float y=object.pnt[p].y;
            glVertex3f(x, y, 0.0);
        }
        glEnd();
    }

}

void writeOff(object3D_t angka_tiga,float scale)
{
    ofstream myfile;
    myfile.open("D:/My Progress/File Off/39.off");
    myfile << "COFF\n";
    myfile << angka_tiga.numofVertices << " " << angka_tiga.numofFaces << " " << "0.1" << "\n";;

    for(int i = 0; i < angka_tiga.numofVertices; i++)
    {
        myfile << angka_tiga.pnt[i].x*scale << " " << angka_tiga.pnt[i].y*scale << " " << angka_tiga.pnt[i].z*scale << " " << angka_tiga.pnt[i].r << " " << angka_tiga.pnt[i].g << " " << angka_tiga.pnt[i].b << " " << angka_tiga.pnt[i].s <<"\n"; ;
    }
    for(int j = 0; j < angka_tiga.numofFaces; j++)
    {
        myfile << angka_tiga.fc[j].numofVertices << " " ; ;
        for(int k = 0; k < angka_tiga.fc[j].numofVertices; k++)
        {
            myfile << angka_tiga.fc[j].pnt[k] << " "; ;
        }
        myfile << " " <<"\n"; ;
    }
    myfile.close();
}

vector3D_t point_color2vector(point3D_color_t pnt)
{
    vector3D_t vec;
    vec.v[0]=pnt.x;
    vec.v[1]=pnt.y;
    vec.v[2]=pnt.z;
    return vec;
}
float sudut=360;

void drawThree()
{
    int n=8;
    object3D_t angkaTiga = {42,
        {
            //sisi 1

            {-92,60,0, 0,0,255,255},
            {-92,80,0, 0,255,255,255},
            {-92,120,0, 0,0,255,255},
            {90,120,0, 0,255,255,255},
            {90,80,0, 0,255,255,255},
            {90,-80,0, 0,0,255,255},
            {90,-120,0, 0,255,255,255},
            {-92,-120,0, 0,0,255,255},
            {-92,-80,0, 0,255,255,255},
            {-92,-60,0, 0,0,255,255},
            {-52,-60,0, 0,255,255,255},
            {-52,-80,0, 0,0,255,255},
            {52,-80,0, 0,255,255,255},
            {52,-20,0, 0,0,255,255},
            {-55,-20,0, 0,255,255,255},
            {-55,20,0, 0,0,255,255},
            {52,20,0, 0,0,255,255},
            {52,80,0, 255,0,255,255},
            {-52,80,0, 0,0,255,255},
            {-52,60,0, 255,0,255,255},
            {-92,60,0, 0,0,255,255},
            //sisi 2
            {-92,60,60,0,0,255,255},
            {-92,80,60,0,0,255,255},
            {-92,120,60,0,0,255,255},
            {90,120,60,255,0,255,255},
            {90,80,60,0,255,255,255},
            {90,-80,60,255,0,255,255},
            {90,-120,60,0,0,255,255},
            {-92,-120,60,255,0,255,255},
            {-92,-80,60,255,0,255,255},
            {-92,-60,60,0,0,0,255},
            {-52,-60,60,0,255,255,255},
            {-52,-80,60,0,0,255,255},
            {52,-80,60,255,0,255,255},
            {52,-20,60,0,0,255,255},
            {-55,-20,60,255,0,255,255},
            {-55,20,60,255,0,255,255},
            {52,20,60,0,0,255,255},
            {52,80,60,0,255,255,255},
            {-52,80,60,255,0,255,255},
            {-52,60,60,0,255,255,255},
            {-92,60,60,0,255,255,255}

        },
        //face

        68,
        {
            {3,{0,19,18}},
            {3,{1,0,18}},
            {3,{1,3,4}},
            {3,{1,2, 3}},
            {3,{4,5,17}},
            {3,{5,12,17}},
            {3,{4,1,3}},
            {3,{14,15,16}},
            {3,{13,14,16}},
            {3,{8,7,5}},
            {3,{7,6,5}},
            {3,{8,9,10}},
            {3,{11,10,8}},
            {3,{8,7,5}},
            {3,{21,40,39}},
            {3,{22,39,21}},
            {3,{22,24,23}},
            {3,{22,24,25}},
            {3,{25,26,38}},
            {3,{38,33,26}},
            {3,{21,40,39}},
            {3,{26,29,28}},
            {3,{27,28,26}},
            {3,{29,32,31}},
            {3,{29,30,31}},
            {3,{35,37,34}},
            {3,{37,35,36}},
            {3,{0,21,1}},
            {3,{21,22,1}},
            {3,{8,9,29}},
            {3,{29,9,30}},
            {3,{30,9,31}},
            {3,{31,9,10}},
            {3,{7,8,29}},
            {3,{7,28,29}},
            {3,{11,10,32}},
            {3,{32,31,10}},
            {3,{32,11,26}},
            {3,{5,11,26}},
            {3,{28,7,27}},
            {3,{7,6,27}},
            {3,{4,5,26}},
            {3,{25,4,26}},
            {3,{4,3,25}},
            {3,{25,3,24}},
            {3,{2,1,22}},
            {3,{23,2,22}},
            {3,{2,3,24}},
            {3,{23,2,24}},
            {3,{0,19,40}},
            {3,{21,40,0}},
            {3,{18,40,39}},
            {3,{19,40,18}},
            {3,{18,39,38}},
            {3,{38,17,18}},
            {3,{15,37,16}},
            {3,{36,15,37}},
            {3,{37,15,16}},
            {3,{15,14,35}},
            {3,{35,36,15}},
            {3,{14,15,34}},
            {3,{34,35,14 }},
            {3,{13,33,12}},
            {3,{13,34,33}},
            {3,{17,37,16}},
            {3,{17,38,37}},
            {3,{6,5,27}},
            {3,{5,27,26}}
        }
    };
    matrix3D_t matrix_X=rotationX(sudut);
    matrix3D_t matrix_Y=rotationY(sudut);
    matrix3D_t matrix_Z=rotationZ(sudut);
    for(int i=0; i < angkaTiga.numofVertices; i++)
    {
//teta=15.0;
        vector3D_t p;
        p.v[0]=angkaTiga.pnt[i].x;
        p.v[1]=angkaTiga.pnt[i].y;
        p.v[2]=angkaTiga.pnt[i].z;
        p=(matrix_Y)*(p);
        p=(matrix_X)*(p);
        p=(matrix_Z)*(p);
        angkaTiga.pnt[i].x=p.v[0];
        angkaTiga.pnt[i].y=p.v[1];
        angkaTiga.pnt[i].z=p.v[2];
    }
    color_t color1;
    color1.r=0.4;
    color1.g=0.6;
    color1.b=0.2;
    create3DObject(angkaTiga,color1);
    //scale
    writeOff(angkaTiga,1);
    sudut++;
    if(sudut>=360.0)
        sudut=0.8;
    glFlush();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawThree();
    glutSwapBuffers();
}

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Angka 3 | Anas Shofyan Martunis Fateh | 2103171004");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(-320., 320., -240.0, 240.0);
    //glutIdleFunc(display);
    glutDisplayFunc(display);
    glutTimerFunc(1, timer, 0);
    glutMainLoop();
    return EXIT_SUCCESS;
}


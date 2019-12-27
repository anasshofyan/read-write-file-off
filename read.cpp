#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <cerrno>
#include <string>
#include <fstream>
#include <math.h>
#include <GL/glut.h>


typedef struct
{
    float x;
    float y;
} Point2D_t;
typedef struct
{
    float r;
    float g;
    float b;
} color_rgb;

typedef struct
{
    float m[2][2];
} Matrix2D_t;

typedef struct
{
    float m[3][3];
} Matrix3D_t;

typedef struct
{
    float x,y,z;
} Point3D_t;

typedef struct
{
    float v[2];
} Vector2D_t;
typedef struct
{
    float v[3];
} Vector3D_t;

typedef struct
{
    int NumberofVertices;
    short int pnt[100];
} face_t;

typedef struct
{
    int NumberofVertices;
    Point3D_t pnt[100];
    int NumberofFaces;
    face_t fc[100];
} object3D_t;

Vector3D_t vec[128];

object3D_t object;
color_rgb colors[100];
char str[255];
char *tok;
char delims[]=" ";
int numVertices;
int numFaces;
int x1=0,x2=0,x3=0,y=0, num[50],num2[50];
float rotation = 0;

void setColor(color_rgb color)
{
    glColor3f(color.r/255,color.g/255,color.b/255);
}
void drawDot(float x, float y)
{
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();
    glColor3f(0,0,0);
    glPointSize(10);
}
void fillPolygon(Point2D_t point[], int n, color_rgb color[])
{
    glBegin(GL_POLYGON);
    for (int i=0; i<n; i++)
    {
        setColor(color[i]);
        glVertex2f(point[i].x, point[i].y);
    }
    glEnd();
}

void polyLine_loop(Point2D_t point[], int n,color_rgb color[])
{
    glBegin(GL_LINE_LOOP);
    for (int i=0; i<n; i++)
    {
        setColor(color[i]);
        glVertex2f(point[i].x, point[i].y);
    }
    glEnd();
    setColor({0,0,0});
}

Matrix3D_t createIdentity()
{
    Matrix3D_t mat;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            mat.m[i][j]=0;
        }
        mat.m[i][i]=1;
    }
    return mat;
}

Matrix3D_t rotationXMTX(float theta)
{
    Matrix3D_t rotate=createIdentity();
    float cs=cos(theta);
    float sn=sin(theta);
    rotate.m[1][1]=cs;
    rotate.m[1][2]=-sn;
    rotate.m[2][1]=sn;
    rotate.m[2][2]=cs;
    return rotate;
}

Matrix3D_t rotationYMTX(float theta)
{
    Matrix3D_t rotate=createIdentity();
    float cs=cos(theta);
    float sn=sin(theta);
    rotate.m[0][0]=cs;
    rotate.m[0][2]=sn;
    rotate.m[2][0]=-sn;
    rotate.m[2][2]=cs;
    return rotate;
}

Point2D_t Vector2Point2D(Vector3D_t vec)
{
    Point2D_t pnt;
    pnt.x=vec.v[0];
    pnt.y=vec.v[1];
    return pnt;
}

Vector3D_t Point2Vector(Point3D_t pnt)
{
    Vector3D_t vec;
    vec.v[0]=pnt.x;
    vec.v[1]=pnt.y;
    vec.v[2]=pnt.z;
    return vec;
}

Matrix3D_t operator * (Matrix3D_t a, Matrix3D_t b)
{
    Matrix3D_t c;
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
        {
            c.m[i][j]=0;
            for(int k=0; k<4; k++)
                c.m[i][j]+=a.m[i][k]*b.m[k][j];
        }
    return c;
}

Vector3D_t operator*(Matrix3D_t a, Vector3D_t b)
{
    Vector3D_t c;

    for(int i=0; i<4; i++)
    {
        c.v[i]=0;
        for(int j=0; j<4; j++)
            c.v[i]+=a.m[i][j]*b.v[j];
    }
    return c;
}

void draw_object3D(object3D_t object)
{
    rotation -= 0.1;
    Matrix3D_t tilting = operator*(rotationXMTX(0.25),rotationYMTX(rotation));
    for(int i=0; i<object.NumberofVertices; i++)
    {
        vec[i]=Point2Vector(object.pnt[i]);
        vec[i]=operator*(tilting,vec[i]);
    }
    printf("\n\nJumlah Sudut : %d\n", object.NumberofVertices);
    printf("Jumlah Face : %d\n", object.NumberofFaces);
    for(int i=0; i<object.NumberofFaces; i++)
    {
        Point2D_t titik2d[object.fc[i].NumberofVertices];
        printf("\nFace ke-%d, jumlah sudut : %d\n",i+1,object.fc[i].NumberofVertices);
        for(int j=0; j<object.fc[i].NumberofVertices; j++)
        {
            titik2d[j] = Vector2Point2D(vec[object.fc[i].pnt[j]]);
            printf("%d : %.2f, %.2f, %.2f \n", object.fc[i].pnt[j],
                   object.pnt[object.fc[i].pnt[j]], object.pnt[object.fc[i].pnt[j]],
                   object.pnt[object.fc[i].pnt[j]]);
        }
        //fillPolygon(titik2d,object.fc[i].NumberofVertices,colors);
        polyLine_loop(titik2d,object.fc[i].NumberofVertices,colors);
    }
}

void convertFile()
{
    // Lokasi Convert file
    std::ifstream  ifs1("D:/My Progress/File Off/39.off");

    if(!ifs1)
    {
        std::cout << "cannot open file_readoff";
    }
    object.pnt[numVertices];
    object.fc[numFaces];

    while(!ifs1.eof() && y<2+numFaces+numVertices)
    {
        ifs1.getline(str,255);

        if(y==1)
        {

            tok=strtok(str,delims);

            x1=0;
            while(tok && x1<2)
            {
                num[x1]=atof(tok);
                tok=strtok(NULL, delims);
                x1++;
            }
            numVertices=(int) num[0];
            numFaces=(int) num[1];

            object.NumberofVertices=numVertices;
            object.NumberofFaces=numFaces;
            printf("\n  %d",object.NumberofVertices);
            printf("  %d  0",object.NumberofFaces);
        }
        if(y>1 && y<=1+numVertices)
        {
            tok=strtok(str,delims);
            x2=0;
            while(tok && x2<numVertices)
            {
                num[x2]=atof(tok);
                tok=strtok(NULL, delims);
                x2++;
            }
            int scale = 200;
            object.pnt[y-2].x=num[0];
            object.pnt[y-2].y=num[1];
            object.pnt[y-2].z=num[2];
            colors[y-2].r = num[3];
            colors[y-2].g = num[4];
            colors[y-2].b = num[5];

            printf("\n");
            for(int i=0; i<x2; i++)
            {
                printf("  %d",num[i]);
            }
        }
        if(y>1+numVertices && y<=1+numVertices+numFaces)
        {
            tok=strtok(str,delims);
            x3=0;
            while(tok && x3<=numVertices)
            {
                num[x3]=atof(tok);
                tok=strtok(NULL, delims);
                x3++;
            }

            object.fc[y-2-numVertices].NumberofVertices = num[0];
            for (int i=0; i<num[0]; i++)
            {
                object.fc[y-2-numVertices].pnt[i]=num[i+1];
            }

            printf("\n");
            printf("  %d", object.fc[y-2-numVertices].NumberofVertices);
            for(int i=0; i<x3-1; i++)
            {

                printf("  %d", object.fc[y-2-numVertices].pnt[i]);
            }
        }
        y++;
    }
    draw_object3D(object);
}


void userdraw(void)
{
    convertFile();
}

void display(void)
{
    glClear( GL_COLOR_BUFFER_BIT);
    userdraw();
    glutSwapBuffers();
}
void Initialize()
{
//    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(-320., 320., -240.0, 240.0);
    //gluOrtho2D(0., 640., 0., 480.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}
void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(100,timer,0);
}
int main(int argc, char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowPosition(150,150);
    glutInitWindowSize(640,480);
    glutCreateWindow(" Anas Shofyan Martunis Fateh - 2103171004 | Read File OFF");
    Initialize();
//    glutIdleFunc(display);
    glutDisplayFunc(display);
    glutTimerFunc(1,timer,0);
    glutMainLoop();
    return 0;
}

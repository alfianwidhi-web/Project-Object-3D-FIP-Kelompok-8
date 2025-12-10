#define _USE_MATH_DEFINES
#include <GL/freeglut.h>
#include <GL/glu.h> // Tambahkan untuk gluNewQuadric, gluCylinder
#include <math.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>

// =========================================================================
//  VARIABEL & KONSTANTA ASLI (JANGAN DIUBAH) - DARI KODE 1
// =========================================================================

// --- VARIABEL KAMERA KODE 1 ---
float x = 35.0f, y = 40.0f, z = 240.0f;
float lx = 0.0f, ly = -0.2f, lz = -1.0f;
float angle = 0.0f;
float pitch = -10.0f;
float deltaAngle = 0.0f;
int xOrigin = -1;
int yOrigin = -1;

// --- VARIABEL KONTROL KODE 2 ---
float buildingRotateY = 0.0f;

// --- KONSTANTA WARNA KODE 1 ---
const float COL_WALL[3] = { 0.95f, 0.90f, 0.75f };
const float COL_ROOF[3] = { 0.18f, 0.08f, 0.05f };
const float COL_GLASS[3] = { 0.1f, 0.15f, 0.3f };
const float COL_FRAME[3] = { 0.05f, 0.05f, 0.05f };
const float COL_MAGENTA[3] = { 0.65f, 0.0f, 0.65f };
const float COL_GREY[3] = { 0.4f, 0.4f, 0.4f };
const float COL_BROWN_ROOF[3] = { 0.4f, 0.2f, 0.1f };
const float COL_PILLAR_RED[3] = { 0.5f, 0.2f, 0.1f };

// --- HELPER MATERIAL KODE 1 ---
void setMaterial(float r, float g, float b, bool selfIlluminated = false) {
    GLfloat mat_ambient[4];
    GLfloat mat_diffuse[] = { r, g, b, 1.0f };
    GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_shininess[] = { 10.0f };
    GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    if (selfIlluminated) {
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b; mat_ambient[3] = 1.0f;
        mat_emission[0] = r * 0.2f; mat_emission[1] = g * 0.2f; mat_emission[2] = b * 0.2f;
    }
    else {
        mat_ambient[0] = r * 0.5f; mat_ambient[1] = g * 0.5f; mat_ambient[2] = b * 0.5f; mat_ambient[3] = 1.0f;
    }

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glColor3f(r, g, b); // Set color untuk fallback (GL_COLOR_MATERIAL)
}

// Wrapper Box KODE 1 (Menggunakan setMaterial)
void drawBalok(float x, float y, float z, float sx, float sy, float sz, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    setMaterial(r, g, b, false);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// =========================================================================
//  FUNGSI DETAIL KODE 1
// =========================================================================

void drawRowOfSquareWindows(int count, float size, float spacing) {
    float totalWidth = (count * size) + ((count - 1) * spacing);
    float startX = -totalWidth / 2.0f + size / 2.0f;
    for (int i = 0; i < count; i++) {
        glPushMatrix();
        float currentX = startX + (i * (size + spacing));
        glTranslatef(currentX, 0.0f, 0.0f);
        drawBalok(0.0f, 0.0f, 0.0f, size, size, 0.2f, COL_FRAME[0], COL_FRAME[1], COL_FRAME[2]);
        drawBalok(0.0f, 0.0f, 0.05f, size - 0.3f, size - 0.3f, 0.22f, COL_GLASS[0], COL_GLASS[1], COL_GLASS[2]);
        drawBalok(0.0f, 0.0f, 0.08f, 0.1f, size - 0.3f, 0.25f, COL_FRAME[0], COL_FRAME[1], COL_FRAME[2]);
        drawBalok(0.0f, 0.0f, 0.08f, size - 0.3f, 0.1f, 0.25f, COL_FRAME[0], COL_FRAME[1], COL_FRAME[2]);
        glPopMatrix();
    }
}

// --- FUNGSI ATAP JOGLO (FIXED: TUTUP ATAS & BAWAH) ---
void drawJogloRoof(float x, float y, float z, float width, float height, float depth) {
    glPushMatrix(); glTranslatef(x, y, z);
    float w = width / 2.0f; float d = depth / 2.0f;
    float topW = w * 0.6f; float topD = d * 0.6f;

    setMaterial(COL_ROOF[0], COL_ROOF[1], COL_ROOF[2]);
    glBegin(GL_QUADS);
    // Sisi Miring
    glNormal3f(0.0f, 0.5f, 1.0f); glVertex3f(-w, 0.0f, d); glVertex3f(w, 0.0f, d); glVertex3f(topW, height, topD); glVertex3f(-topW, height, topD);
    glNormal3f(0.0f, 0.5f, -1.0f); glVertex3f(w, 0.0f, -d); glVertex3f(-w, 0.0f, -d); glVertex3f(-topW, height, -topD); glVertex3f(topW, height, -topD);
    glNormal3f(-1.0f, 0.5f, 0.0f); glVertex3f(-w, 0.0f, -d); glVertex3f(-w, 0.0f, d); glVertex3f(-topW, height, topD); glVertex3f(-topW, height, -topD);
    glNormal3f(1.0f, 0.5f, 0.0f); glVertex3f(w, 0.0f, d); glVertex3f(w, 0.0f, -d); glVertex3f(topW, height, -topD); glVertex3f(topW, height, topD);

    // TUTUP ATAS (TOP CAP)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-topW, height, topD); glVertex3f(topW, height, topD);
    glVertex3f(topW, height, -topD); glVertex3f(-topW, height, -topD);

    // TUTUP BAWAH (PLAFON)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-w, 0.0f, d); glVertex3f(-w, 0.0f, -d); glVertex3f(w, 0.0f, -d); glVertex3f(w, 0.0f, d);
    glEnd(); glPopMatrix();
}

void drawWindowsSurrounding(float width, float depth) {
    glPushMatrix();
    glPushMatrix(); glTranslatef(width / 2.0f + 0.1f, 0.0f, 0.0f); glRotatef(90.0f, 0.0f, 1.0f, 0.0f); drawRowOfSquareWindows(6, 2.0f, 1.2f); glPopMatrix();
    glPushMatrix(); glTranslatef(-width / 2.0f - 0.1f, 0.0f, 0.0f); glRotatef(90.0f, 0.0f, 1.0f, 0.0f); drawRowOfSquareWindows(6, 2.0f, 1.2f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 0.0f, depth / 2.0f + 0.1f); drawRowOfSquareWindows(3, 2.0f, 1.5f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 0.0f, -depth / 2.0f - 0.1f); drawRowOfSquareWindows(3, 2.0f, 1.5f); glPopMatrix();
    glPopMatrix();
}

void drawNumber06(float posX, float posY, float posZ, float scale) {
    float t = 0.2f * scale; float h = 1.0f * scale; float w = 0.6f * scale;
    float r = 0.1f, g = 0.1f, b = 0.1f;
    float x0 = posX - (w / 1.5f);
    drawBalok(x0 - w / 2.0f, posY, posZ, t, h, t, r, g, b); drawBalok(x0 + w / 2.0f, posY, posZ, t, h, t, r, g, b);
    drawBalok(x0, posY + h / 2.0f, posZ, w, t, t, r, g, b); drawBalok(x0, posY - h / 2.0f, posZ, w, t, t, r, g, b);
    float x6 = posX + (w / 1.5f);
    drawBalok(x6 - w / 2.0f, posY, posZ, t, h, t, r, g, b); drawBalok(x6, posY - h / 2.0f, posZ, w, t, t, r, g, b);
    drawBalok(x6, posY, posZ, w, t, t, r, g, b); drawBalok(x6, posY + h / 2.0f, posZ, w, t, t, r, g, b);
    drawBalok(x6 + w / 2.0f, posY - h / 4.0f, posZ, t, h / 2.0f, t, r, g, b);
}

void drawTextPlaque(float posX, float posY, float posZ) {
    float boardW = 8.0f; float boardH = 4.0f;
    drawBalok(posX, posY, posZ, boardW, boardH, 0.5f, 0.6f, 0.2f, 0.2f);
    drawBalok(posX, posY + 1.2f, posZ + 0.3f, 7.0f, 0.4f, 0.1f, 1.0f, 1.0f, 1.0f);
    drawBalok(posX, posY, posZ + 0.3f, 5.0f, 0.4f, 0.1f, 1.0f, 1.0f, 1.0f);
    drawBalok(posX, posY - 1.2f, posZ + 0.3f, 4.0f, 0.4f, 0.1f, 1.0f, 1.0f, 1.0f);
}

// --- FUNGSI ATAP PERSEGI (FIXED: TUTUP ATAS & BAWAH) ---
void drawRectRoofHelper(float w, float d, float ov, float h) {
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.5f, 1.0f); glVertex3f(-w / 2.0f, h, d / 2.0f); glVertex3f(w / 2.0f, h, d / 2.0f); glVertex3f(w / 2.0f + ov, 0.0f, d / 2.0f + ov); glVertex3f(-w / 2.0f - ov, 0.0f, d / 2.0f + ov);
    glNormal3f(0.0f, 0.5f, -1.0f); glVertex3f(w / 2.0f, h, -d / 2.0f); glVertex3f(-w / 2.0f, h, -d / 2.0f); glVertex3f(-w / 2.0f - ov, 0.0f, -d / 2.0f - ov); glVertex3f(w / 2.0f + ov, 0.0f, -d / 2.0f - ov);
    glNormal3f(-1.0f, 0.5f, 0.0f); glVertex3f(-w / 2.0f, h, -d / 2.0f); glVertex3f(-w / 2.0f, h, d / 2.0f); glVertex3f(-w / 2.0f - ov, 0.0f, d / 2.0f + ov); glVertex3f(-w / 2.0f - ov, 0.0f, -d / 2.0f - ov);
    glNormal3f(1.0f, 0.5f, 0.0f); glVertex3f(w / 2.0f, h, d / 2.0f); glVertex3f(w / 2.0f, h, -d / 2.0f); glVertex3f(w / 2.0f + ov, 0.0f, -d / 2.0f - ov); glVertex3f(w / 2.0f + ov, 0.0f, d / 2.0f + ov);

    // TUTUP BAWAH
    glNormal3f(0.0f, -1.0f, 0.0f); glVertex3f(-w / 2.0f - ov, 0.0f, d / 2.0f + ov); glVertex3f(w / 2.0f + ov, 0.0f, d / 2.0f + ov); glVertex3f(w / 2.0f + ov, 0.0f, -d / 2.0f - ov); glVertex3f(-w / 2.0f - ov, 0.0f, -d / 2.0f - ov);

    // TUTUP ATAS
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-w / 2.0f, h, d / 2.0f); glVertex3f(w / 2.0f, h, d / 2.0f); glVertex3f(w / 2.0f, h, -d / 2.0f); glVertex3f(-w / 2.0f, h, -d / 2.0f);
    glEnd();
}

// =========================================================================
//  FUNGSI GEDUNG 03 & 04 (PERSEGI) - KODE 1
// =========================================================================
const float RECT_FLOOR_H = 5.0f; const float RECT_WIDTH = 40.0f; const float RECT_DEPTH = 15.0f;

void drawRectangularBuilding(float posX, bool isBld04) {
    glPushMatrix(); glTranslatef(posX, 0.0f, 0.0f);
    for (int i = 0; i < 3; i++) {
        float y = i * RECT_FLOOR_H;
        drawBalok(0.0f, y + 2.5f, 0.0f, RECT_WIDTH - 2.0f, 4.0f, RECT_DEPTH - 4.0f, COL_WALL[0], COL_WALL[1], COL_WALL[2]);
        for (float j = -15.0f; j <= 15.0f; j += 5.0f) {
            drawBalok(j, y + 2.5f, (RECT_DEPTH / 2.0f) - 1.9f, 2.5f, 2.0f, 0.2f, COL_GLASS[0], COL_GLASS[1], COL_GLASS[2]);
            drawBalok(j, y + 2.5f, -(RECT_DEPTH / 2.0f) + 1.9f, 2.5f, 2.0f, 0.2f, COL_GLASS[0], COL_GLASS[1], COL_GLASS[2]);
        }
        if (i > 0) {
            float rH = 1.3f; float rT = 0.3f; float cY = y + (rH / 2.0f); float zOff = (RECT_DEPTH / 2.0f) - 0.4f; float xOff = (RECT_WIDTH / 2.0f) - 0.4f;
            drawBalok(0.0f, cY, zOff, RECT_WIDTH, rH, rT, COL_WALL[0], COL_WALL[1], COL_WALL[2]);
            drawBalok(0.0f, cY, -zOff, RECT_WIDTH, rH, rT, COL_WALL[0], COL_WALL[1], COL_WALL[2]);
            drawBalok(xOff, cY, 0.0f, rT, rH, RECT_DEPTH - (rT * 2.0f), COL_WALL[0], COL_WALL[1], COL_WALL[2]);
            drawBalok(-xOff, cY, 0.0f, rT, rH, RECT_DEPTH - (rT * 2.0f), COL_WALL[0], COL_WALL[1], COL_WALL[2]);
        }
    }
    float h = RECT_FLOOR_H * 3.0f; int n = 9; float gap = RECT_WIDTH / (n - 1);
    for (int i = 0; i < n; i++) {
        float cx = -(RECT_WIDTH / 2.0f) + (i * gap);
        drawBalok(cx, h / 2.0f, RECT_DEPTH / 2.0f, 1.0f, h, 1.0f, COL_WALL[0], COL_WALL[1], COL_WALL[2]);
        drawBalok(cx, h / 2.0f, -RECT_DEPTH / 2.0f, 1.0f, h, 1.0f, COL_WALL[0], COL_WALL[1], COL_WALL[2]);
    }
    if (!isBld04) {
        setMaterial(COL_ROOF[0], COL_ROOF[1], COL_ROOF[2]);
        glPushMatrix(); glTranslatef(0.0f, RECT_FLOOR_H - 1.0f, 0.0f); drawRectRoofHelper(RECT_WIDTH + 1.0f, RECT_DEPTH + 1.0f, 2.5f, 1.5f); glPopMatrix();
        glPushMatrix(); glTranslatef(0.0f, (RECT_FLOOR_H * 2.0f) - 1.0f, 0.0f); drawRectRoofHelper(RECT_WIDTH + 1.0f, RECT_DEPTH + 1.0f, 2.5f, 1.5f); glPopMatrix();
    }
    else {
        drawBalok(0.0f, RECT_FLOOR_H - 0.5f, 0.0f, RECT_WIDTH + 1.5f, 1.0f, RECT_DEPTH + 1.5f, COL_WALL[0], COL_WALL[1], COL_WALL[2]);
        drawBalok(0.0f, (RECT_FLOOR_H * 2.0f) - 0.5f, 0.0f, RECT_WIDTH + 1.5f, 1.0f, RECT_DEPTH + 1.5f, COL_WALL[0], COL_WALL[1], COL_WALL[2]);
    }
    setMaterial(COL_ROOF[0], COL_ROOF[1], COL_ROOF[2]);
    glPushMatrix(); glTranslatef(0.0f, (RECT_FLOOR_H * 3.0f) - 0.6f, 0.0f); drawRectRoofHelper(RECT_WIDTH + 2.0f, RECT_DEPTH + 2.0f, 3.0f, 3.0f); glPopMatrix();

    glPushMatrix(); glTranslatef(0.0f, (RECT_FLOOR_H * 3.0f) + 2.4f, 0.0f);
    float wT = RECT_WIDTH / 2.5f; float dT = RECT_DEPTH / 2.5f; float hT = 6.0f;
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.5f, 1.0f); glVertex3f(0.0f, hT, 0.0f); glVertex3f(-wT / 2, 0.0f, dT / 2); glVertex3f(wT / 2, 0.0f, dT / 2);
    glNormal3f(1.0f, 0.5f, 0.0f); glVertex3f(0.0f, hT, 0.0f); glVertex3f(wT / 2, 0.0f, dT / 2); glVertex3f(wT / 2, 0.0f, -dT / 2);
    glNormal3f(0.0f, 0.5f, -1.0f); glVertex3f(0.0f, hT, 0.0f); glVertex3f(wT / 2, 0.0f, -dT / 2); glVertex3f(-wT / 2, 0.0f, -dT / 2);
    glNormal3f(-1.0f, 0.5f, 0.0f); glVertex3f(0.0f, hT, 0.0f); glVertex3f(-wT / 2, 0.0f, -dT / 2); glVertex3f(-wT / 2, 0.0f, dT / 2);
    glEnd(); glPopMatrix();

    float accX[] = { -7.0f, -5.0f, 5.0f, 7.0f }; for (int i = 0; i < 4; i++) drawBalok(accX[i], RECT_FLOOR_H * 2.0f, RECT_DEPTH / 2.0f + 0.5f, 0.8f, RECT_FLOOR_H * 1.8f, 0.5f, COL_MAGENTA[0], COL_MAGENTA[1], COL_MAGENTA[2]);
    drawBalok(0.0f, RECT_FLOOR_H * 0.7f, RECT_DEPTH / 2.0f + 3.0f, 10.0f, 1.0f, 5.0f, COL_WALL[0], COL_WALL[1], COL_WALL[2]);
    drawBalok(-4.0f, RECT_FLOOR_H * 0.35f, RECT_DEPTH / 2.0f + 5.0f, 0.5f, RECT_FLOOR_H * 0.7f, 0.5f, COL_GREY[0], COL_GREY[1], COL_GREY[2]);
    drawBalok(4.0f, RECT_FLOOR_H * 0.35f, RECT_DEPTH / 2.0f + 5.0f, 0.5f, RECT_FLOOR_H * 0.7f, 0.5f, COL_GREY[0], COL_GREY[1], COL_GREY[2]);
    float tz = RECT_DEPTH / 2.0f + 5.6f; float ty = RECT_FLOOR_H * 0.7f; float s = 0.3f; float nr = 0.1f;
    drawBalok(-1.5f, ty, tz, s, s * 5.0f, s, nr, nr, nr); drawBalok(-0.5f, ty, tz, s, s * 5.0f, s, nr, nr, nr); drawBalok(-1.0f, ty + s * 2.0f, tz, s * 3.0f, s, s, nr, nr, nr); drawBalok(-1.0f, ty - s * 2.0f, tz, s * 3.0f, s, s, nr, nr, nr);
    if (!isBld04) { drawBalok(1.0f, ty + s * 2.0f, tz, s * 3.0f, s, s, nr, nr, nr); drawBalok(1.0f, ty, tz, s * 3.0f, s, s, nr, nr, nr); drawBalok(1.0f, ty - s * 2.0f, tz, s * 3.0f, s, s, nr, nr, nr); drawBalok(1.5f, ty, tz, s, s * 5.0f, s, nr, nr, nr); }
    else { drawBalok(1.5f, ty, tz, s, s * 5.0f, s, nr, nr, nr); drawBalok(0.5f, ty, tz, s * 4.0f, s, s, nr, nr, nr); drawBalok(-0.1f, ty + s * 1.5f, tz, s, s * 2.5f, s, nr, nr, nr); }
    glPopMatrix();
}

// =========================================================================
//  FUNGSI GEDUNG L-SHAPE - KODE 1
// =========================================================================

void drawLRightWing() {
    float floorH = 4.0f; float wingDepth = 28.0f; float wingWidth = 14.0f; float overhang = 5.0f;
    glPushMatrix(); glTranslatef(12.0f, 0.0f, 0.0f);
    drawBalok(0.0f, 6.5f, 0.0f, wingWidth - 0.2f, 13.0f, wingDepth - 0.2f, COL_WALL[0], COL_WALL[1], COL_WALL[2]);
    glPushMatrix(); glTranslatef(0.0f, 2.2f, 0.0f); drawWindowsSurrounding(wingWidth, wingDepth); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 6.2f, 0.0f); drawWindowsSurrounding(wingWidth, wingDepth); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 10.2f, 0.0f); drawWindowsSurrounding(wingWidth, wingDepth); glPopMatrix();

    // REVISI ATAP MENYATU
    drawJogloRoof(0.0f, floorH + 0.1f, 0.0f, wingWidth + overhang, 1.5f, wingDepth + overhang);
    drawJogloRoof(0.0f, (floorH * 2.0f) + 0.1f, 0.0f, wingWidth + overhang - 1.0f, 1.5f, wingDepth + overhang - 1.0f);
    drawJogloRoof(0.0f, (floorH * 3.0f) + 1.6f, 0.0f, wingWidth + overhang, 5.0f, wingDepth + overhang);
    glPopMatrix();
}

void drawLLeftWing() {
    float floorH = 4.0f; float wingLength = 22.0f; float overhang = 5.0f;
    glPushMatrix(); glTranslatef(-6.0f, 0.0f, -8.0f);
    drawBalok(0.0f, 4.25f, 0.0f, wingLength, 8.5f, 8.0f, COL_WALL[0], COL_WALL[1], COL_WALL[2]);
    drawBalok(0.0f, floorH, 4.0f, wingLength, 0.5f, 4.0f, COL_WALL[0], COL_WALL[1], COL_WALL[2]);

    // REVISI PILAR UNGU: Tinggi Pas & Warna Ungu (COL_MAGENTA)
    for (float i = -9.0f; i <= 9.0f; i += 3.0f) {
        glPushMatrix();
        glTranslatef(i, floorH + 2.0f, 5.8f); // Center di 6.0
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        GLUquadric* q = gluNewQuadric();
        // Warna Ungu manual karena cylinder pakai GL_COLOR_MATERIAL state
        glColor3f(COL_MAGENTA[0], COL_MAGENTA[1], COL_MAGENTA[2]);
        gluCylinder(q, 0.5f, 0.5f, floorH, 16, 2);
        gluDeleteQuadric(q);
        glPopMatrix();
    }

    glPushMatrix(); glTranslatef(0.0f, 2.2f, 4.1f); drawRowOfSquareWindows(6, 2.0f, 1.2f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 2.2f, -4.1f); drawRowOfSquareWindows(6, 2.0f, 1.2f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 6.0f, 4.1f); drawRowOfSquareWindows(6, 2.0f, 1.2f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 6.0f, -4.1f); drawRowOfSquareWindows(6, 2.0f, 1.2f); glPopMatrix();

    // REVISI ATAP MENYATU (Turunkan Y)
    drawJogloRoof(0.0f, floorH + 0.1f, 0.0f, wingLength + overhang, 1.8f, 12.0f + overhang);
    drawJogloRoof(0.0f, (floorH * 2.0f) + 1.1f, 0.0f, wingLength + overhang, 4.5f, 12.0f + overhang);
    glPopMatrix();
}

void drawLSignage() {
    glPushMatrix(); glTranslatef(-10.0f, 0.0f, 12.0f);
    drawBalok(0.0f, 0.2f, 0.0f, 8.5f, 0.4f, 1.5f, COL_GREY[0], COL_GREY[1], COL_GREY[2]);
    glPushMatrix(); glTranslatef(-2.5f, 3.0f, 0.0f); drawBalok(0.0f, 0.0f, 0.0f, 2.5f, 6.0f, 0.6f, COL_MAGENTA[0], COL_MAGENTA[1], COL_MAGENTA[2]); glTranslatef(0.0f, 3.1f, 0.0f); drawBalok(0.0f, 0.0f, 0.0f, 2.7f, 0.2f, 0.7f, COL_MAGENTA[0], COL_MAGENTA[1], COL_MAGENTA[2]); glPopMatrix();
    glPushMatrix(); glTranslatef(1.5f, 1.5f, 0.0f); drawBalok(0.0f, 0.0f, 0.0f, 5.5f, 3.0f, 0.4f, 1.0f, 1.0f, 1.0f); glPopMatrix();
    drawBalok(1.5f, 2.0f, 0.25f, 4.5f, 0.3f, 0.02f, COL_GREY[0], COL_GREY[1], COL_GREY[2]); drawBalok(1.5f, 1.3f, 0.25f, 3.5f, 0.2f, 0.02f, COL_GREY[0], COL_GREY[1], COL_GREY[2]);
    drawBalok(-2.5f, 4.5f, 0.35f, 1.0f, 1.2f, 0.02f, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

void drawLShapedBuilding(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    drawLRightWing();
    drawLLeftWing();
    drawLSignage();
    glPopMatrix();
}

// =========================================================================
//  FUNGSI GEDUNG 06 - KODE 1
// =========================================================================

void drawBuilding06(float posX, float posZ) {
    glPushMatrix();
    glTranslatef(posX, 0.0f, posZ);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

    // REVISI: Tinggi lantai dikurangi jadi 6.0f
    float bWidth = 60.0f; float bFloorH = 6.0f; float bHeight = bFloorH * 4.0f; float bDepth = 20.0f;
    float frontZ = bDepth / 2.0f;

    // Dinding
    drawBalok(0.0f, bHeight / 2.0f, 0.0f, bWidth - 0.4f, bHeight, bDepth - 0.4f, COL_WALL[0], COL_WALL[1], COL_WALL[2]);

    // List Horizontal (Warna Coklat Kemerahan)
    for (int i = 1; i <= 4; i++) {
        float lY = (i * bFloorH) - (i == 4 ? 0.5f : 0.0f);
        drawBalok(0.0f, lY, frontZ, bWidth, 1.0f, 0.8f, COL_PILLAR_RED[0], COL_PILLAR_RED[1], COL_PILLAR_RED[2]);
    }

    // REVISI: Jumlah pilar dikurangi jadi 8 (lebih renggang)
    int nP = 8; float pW = 0.8f; float sp = bWidth / (nP - 1); float sX = -bWidth / 2.0f;
    for (int i = 0; i < nP; i++) {
        float pX = sX + (i * sp);
        drawBalok(pX, bHeight / 2.0f, frontZ, pW, bHeight, 0.8f, COL_PILLAR_RED[0], COL_PILLAR_RED[1], COL_PILLAR_RED[2]); // Depan
        drawBalok(pX, bHeight / 2.0f, -frontZ, pW, bHeight, 0.8f, COL_PILLAR_RED[0], COL_PILLAR_RED[1], COL_PILLAR_RED[2]); // Belakang
        if (i < nP - 1) {
            float cX = pX + sp / 2.0f; float wW = sp - pW - 0.6f;
            if (i == 2) { // Pintu di tengah (karena nP=8, i=2 bukan tengah persis, tapi cukup)
                float dH = 6.0f;
                drawBalok(cX, dH / 2.0f + 0.5f, frontZ, wW + 0.4f, dH + 0.5f, 0.3f, 0.3f, 0.3f, 0.15f);
                drawBalok(cX, dH / 2.0f + 0.5f, frontZ + 0.1f, wW, dH, 0.1f, 0.55f, 0.27f, 0.07f);
                drawNumber06(cX, dH + 2.5f, frontZ + 0.5f, 1.2f);
                for (int f = 1; f < 4; f++) drawBalok(cX, (f * bFloorH) + bFloorH / 2.0f, frontZ, wW, 4.0f, 0.2f, COL_GLASS[0], COL_GLASS[1], COL_GLASS[2]);
            }
            else {
                for (int f = 0; f < 4; f++) {
                    float wY = (f * bFloorH) + bFloorH / 2.0f;
                    drawBalok(cX, wY, frontZ, wW, 4.0f, 0.2f, COL_GLASS[0], COL_GLASS[1], COL_GLASS[2]);
                    drawBalok(cX, wY, frontZ + 0.1f, wW, 0.2f, 0.2f, COL_FRAME[0], COL_FRAME[1], COL_FRAME[2]);
                }
            }
        }
    }
    // REVISI: Pilar samping dikurangi jadi 3
    int nSide = 3; float spSide = bDepth / (nSide - 1); float sZ = -bDepth / 2.0f;
    for (int i = 0; i < nSide; i++) {
        float pZ = sZ + (i * spSide);
        drawBalok(bWidth / 2.0f, bHeight / 2.0f, pZ, 0.8f, bHeight, pW, COL_PILLAR_RED[0], COL_PILLAR_RED[1], COL_PILLAR_RED[2]);
        drawBalok(-bWidth / 2.0f, bHeight / 2.0f, pZ, 0.8f, bHeight, pW, COL_PILLAR_RED[0], COL_PILLAR_RED[1], COL_PILLAR_RED[2]);
    }

    drawBalok(0.0f, bHeight + 1.0f, 0.0f, bWidth + 2.0f, 2.0f, bDepth + 2.0f, COL_BROWN_ROOF[0], COL_BROWN_ROOF[1], COL_BROWN_ROOF[2]);
    drawJogloRoof(0.0f, bHeight + 2.0f, 0.0f, bWidth + 6.0f, 9.0f, bDepth + 6.0f);
    drawTextPlaque(15.0f, (bFloorH * 3.0f) + 4.0f, frontZ + 0.2f);
    glPopMatrix();
}

// =========================================================================
//  FUNGSI GEDUNG ULABK - KODE 1
// =========================================================================

void drawBuildingULABK(float posX, float posZ) {
    glPushMatrix();
    glTranslatef(posX, 0.0f, posZ);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

    glScalef(1.3f, 1.3f, 1.3f);

    float bWidth = 24.0f; float bHeight = 10.0f; float bDepth = 12.0f;
    float frontZ = bDepth / 2.0f;

    // Dinding
    drawBalok(0.0f, bHeight / 2.0f, 0.0f, bWidth - 0.4f, bHeight, bDepth - 0.4f, COL_WALL[0], COL_WALL[1], COL_WALL[2]);

    // REVISI: List Horizontal warna Coklat Kemerahan (COL_PILLAR_RED)
    drawBalok(0.0f, bHeight / 2.0f, frontZ, bWidth, 0.6f, 0.6f, COL_PILLAR_RED[0], COL_PILLAR_RED[1], COL_PILLAR_RED[2]);
    drawBalok(0.0f, bHeight - 0.5f, frontZ, bWidth, 0.6f, 0.6f, COL_PILLAR_RED[0], COL_PILLAR_RED[1], COL_PILLAR_RED[2]);

    int numPillars = 7; float pillarW = 0.5f; float pillarD = 0.6f; float spacing = bWidth / (numPillars - 1); float startX = -bWidth / 2.0f;
    for (int i = 0; i < numPillars; i++) {
        float pX = startX + (i * spacing);
        // REVISI: Pilar Vertikal warna Coklat Kemerahan (COL_PILLAR_RED)
        drawBalok(pX, bHeight / 2.0f, frontZ, pillarW, bHeight, pillarD, COL_PILLAR_RED[0], COL_PILLAR_RED[1], COL_PILLAR_RED[2]);

        if (i < numPillars - 1) {
            float cX = pX + (spacing / 2.0f); float wW = spacing - pillarW - 0.8f;
            if (i == 0) {
                drawBalok(cX, 2.25f, frontZ, wW + 0.2f, 4.5f, 0.2f, 0.3f, 0.15f, 0.05f);
                drawBalok(cX, 2.25f, frontZ + 0.05f, wW, 4.3f, 0.1f, 0.55f, 0.27f, 0.07f);
                drawBalok(cX, 7.5f, frontZ, wW + 0.2f, 2.8f, 0.2f, COL_FRAME[0], COL_FRAME[1], COL_FRAME[2]);
                drawBalok(cX, 7.5f, frontZ + 0.05f, wW, 2.4f, 0.1f, COL_GLASS[0], COL_GLASS[1], COL_GLASS[2]);
            }
            else {
                drawBalok(cX, 2.5f, frontZ, wW + 0.2f, 2.8f, 0.2f, COL_FRAME[0], COL_FRAME[1], COL_FRAME[2]);
                drawBalok(cX, 2.5f, frontZ + 0.05f, wW, 2.4f, 0.1f, COL_GLASS[0], COL_GLASS[1], COL_GLASS[2]);
                drawBalok(cX, 2.5f, frontZ + 0.06f, wW, 0.1f, 0.1f, COL_FRAME[0], COL_FRAME[1], COL_FRAME[2]);
                drawBalok(cX, 7.5f, frontZ, wW + 0.2f, 2.8f, 0.2f, COL_FRAME[0], COL_FRAME[1], COL_FRAME[2]);
                drawBalok(cX, 7.5f, frontZ + 0.05f, wW, 2.4f, 0.1f, COL_GLASS[0], COL_GLASS[1], COL_GLASS[2]);
                drawBalok(cX, 7.5f, frontZ + 0.06f, wW, 0.1f, 0.1f, COL_FRAME[0], COL_FRAME[1], COL_FRAME[2]);
            }
        }
    }
    // Atap Flat
    drawBalok(0.0f, bHeight + 0.4f, 0.0f, bWidth + 2.5f, 0.8f, bDepth + 2.5f, COL_PILLAR_RED[0], COL_PILLAR_RED[1], COL_PILLAR_RED[2]); // List atap merah
    drawBalok(0.0f, bHeight + 0.9f, 0.0f, bWidth + 2.3f, 0.2f, bDepth + 2.3f, COL_WALL[0], COL_WALL[1], COL_WALL[2]);
    glPopMatrix();
}

// =========================================================================
//  FUNGSI GEDUNG 05 (MODIFIKASI: LEBIH KECIL, ATAP TRAPESIUM COKLAT MUDA) - KODE 1
// =========================================================================

// Fungsi untuk set color dan material khusus Gedung 05 - Disesuaikan ke setMaterial utama
void setColor05(float r, float g, float b) {
    setMaterial(r, g, b, false);
}

// Fungsi box khusus Gedung 05 dengan normal yang benar - Mengganti drawBox05
void drawBox05(float w, float h, float d) {
    float x = w / 2.0f;
    float y = h / 2.0f;
    float z = d / 2.0f;

    glBegin(GL_QUADS);
    // Front
    glNormal3f(0, 0, 1); glVertex3f(-x, -y, z); glVertex3f(x, -y, z); glVertex3f(x, y, z); glVertex3f(-x, y, z);
    // Back
    glNormal3f(0, 0, -1); glVertex3f(-x, -y, -z); glVertex3f(-x, y, -z); glVertex3f(x, y, -z); glVertex3f(x, -y, -z);
    // Top
    glNormal3f(0, 1, 0); glVertex3f(-x, y, -z); glVertex3f(-x, y, z); glVertex3f(x, y, z); glVertex3f(x, y, -z);
    // Bottom
    glNormal3f(0, -1, 0); glVertex3f(-x, -y, -z); glVertex3f(x, -y, -z); glVertex3f(x, -y, z); glVertex3f(-x, -y, z);
    // Right
    glNormal3f(1, 0, 0); glVertex3f(x, -y, -z); glVertex3f(x, y, -z); glVertex3f(x, y, z); glVertex3f(x, -y, z);
    // Left
    glNormal3f(-1, 0, 0); glVertex3f(-x, -y, -z); glVertex3f(-x, -y, z); glVertex3f(-x, y, z); glVertex3f(-x, y, -z);
    glEnd();
}

// Fungsi Baru: Atap Trapesium Coklat Muda dengan Normal yang Benar
void drawTrapezoidRoof05(float width, float height, float depth) {
    float w = width / 2.0f; float d = depth / 2.0f;
    float topScale = 0.7f; // Bagian atas lebih kecil (70% dari alas)
    float topW = w * topScale; float topD = d * topScale;

    // Warna Coklat Muda (Light Brown)
    setColor05(0.7f, 0.5f, 0.3f);

    glBegin(GL_QUADS);
    // Sisi Depan (Miring)
    // Normal aproksimasi untuk kemiringan agar pencahayaan konsisten
    glNormal3f(0.0f, height, d - topD);
    glVertex3f(-w, 0.0f, d); glVertex3f(w, 0.0f, d); glVertex3f(topW, height, topD); glVertex3f(-topW, height, topD);
    // Sisi Belakang (Miring)
    glNormal3f(0.0f, height, -(d - topD));
    glVertex3f(w, 0.0f, -d); glVertex3f(-w, 0.0f, -d); glVertex3f(-topW, height, -topD); glVertex3f(topW, height, -topD);
    // Sisi Kiri (Miring)
    glNormal3f(-(w - topW), height, 0.0f);
    glVertex3f(-w, 0.0f, -d); glVertex3f(-w, 0.0f, d); glVertex3f(-topW, height, topD); glVertex3f(-topW, height, -topD);
    // Sisi Kanan (Miring)
    glNormal3f(w - topW, height, 0.0f);
    glVertex3f(w, 0.0f, d); glVertex3f(w, 0.0f, -d); glVertex3f(topW, height, -topD); glVertex3f(topW, height, topD);

    // Tutup Atas (Datar)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-topW, height, topD); glVertex3f(topW, height, topD);
    glVertex3f(topW, height, -topD); glVertex3f(-topW, height, -topD);

    // Tutup Bawah (Plafon - untuk menutup celah jika dilihat dari bawah)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-w, 0.0f, d); glVertex3f(-w, 0.0f, -d); glVertex3f(w, 0.0f, -d); glVertex3f(w, 0.0f, d);
    glEnd();
}

// Fungsi untuk menggambar jendela dengan jalusi
void drawWindow05(float x, float y, float z, float width, float height) {
    glPushMatrix();
    glTranslatef(x, y, z);
    setColor05(1.0f, 1.0f, 1.0f);
    drawBox05(width, height, 0.01f);
    glPopMatrix();

    int numStripes = 10;
    float stripeHeight = height / (numStripes + 2);

    for (int i = 0; i < numStripes; i++) {
        glPushMatrix();
        float yPos = y - height / 2 + stripeHeight * (i + 1.5f);
        glTranslatef(x, yPos, z + 0.015f);
        setColor05(0.70f, 0.20f, 0.40f);
        drawBox05(width * 0.85f, stripeHeight * 0.6f, 0.01f);
        glPopMatrix();
    }
}

// Fungsi untuk menggambar balkon
void drawBalcony05(float x, float y, float z, float width) {
    glPushMatrix();
    glTranslatef(x, y, z);
    setColor05(0.85f, 0.85f, 0.85f);
    drawBox05(width, 0.1f, 0.5f);
    glPopMatrix();

    float railHeight = 0.5f;
    int numRails = (int)(width / 0.23f);

    for (int i = 0; i < numRails; i++) {
        float xPos = x - width / 2 + (width / (numRails - 1)) * i;
        glPushMatrix();
        glTranslatef(xPos, y + railHeight / 2, z + 0.25f);
        setColor05(0.90f, 0.90f, 0.90f);
        drawBox05(0.03f, railHeight, 0.03f);
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(x, y + railHeight, z + 0.25f);
    setColor05(0.90f, 0.90f, 0.90f);
    drawBox05(width, 0.04f, 0.04f);
    glPopMatrix();
}


// Fungsi untuk menggambar gedung 05 (Dimodifikasi: Tanpa panel pink atas, Atap Trapesium)
void drawPerfectUnifiedBuilding05() {
    float buildingWidth = 10.6f;
    float buildingDepth = 15.0f;
    float zBase = 0.0f;

    // GROUND BASE - MENEMPEL KE LAPANGAN HIJAU (Y = -1.5)
    glPushMatrix();
    glTranslatef(0.0f, -1.4f, 1.8f);
    setColor05(0.90f, 0.90f, 0.90f);
    drawBox05(11.5f, 0.2f, 2.0f);
    glPopMatrix();

    // FONDASI GEDUNG
    glPushMatrix();
    glTranslatef(0.0f, -1.15f, zBase);
    setColor05(0.92f, 0.92f, 0.92f);
    drawBox05(buildingWidth, 0.5f, buildingDepth);
    glPopMatrix();

    // STRUKTUR UTAMA GEDUNG
    float totalHeight = 8.0f;
    float buildingCenterY = totalHeight / 2 - 1.0f; // Sekitar Y=3.0

    glPushMatrix();
    glTranslatef(0.0f, buildingCenterY, zBase);
    setColor05(0.95f, 0.95f, 0.95f);
    drawBox05(buildingWidth, totalHeight, buildingDepth);
    glPopMatrix();

    // --- ATAP TRAPESIUM BARU ---
    // Diletakkan di atas struktur utama. Tinggi struktur utama sampai Y = 3.0 + (8.0/2) = 7.0f
    glPushMatrix();
    glTranslatef(0.0f, 7.0f, zBase);
    // Lebar dan kedalaman atap dibuat sedikit lebih besar dari gedung (overhang)
    drawTrapezoidRoof05(buildingWidth + 1.5f, 2.5f, buildingDepth + 1.5f);
    glPopMatrix();
    // ---------------------------

    float zFront = zBase + buildingDepth / 2 + 0.01f;

    // ... (Detail Lantai 1, 2, 3, Pintu/Jendela Hitam tetap sama)
    // ==================== LANTAI 1 ====================
    float floor1Y = 0.8f;

    // TEMBOK BAWAH PENUH
    glPushMatrix();
    glTranslatef(0.0f, floor1Y - 1.2f, zFront);
    setColor05(0.94f, 0.94f, 0.94f);
    drawBox05(buildingWidth + 0.5f, 1.8f, 0.12f);
    glPopMatrix();

    // Jendela jalusi pink - wing kiri
    drawWindow05(-4.8f, floor1Y, zFront, 0.45f, 1.2f);
    drawWindow05(-4.0f, floor1Y, zFront, 0.45f, 1.2f);
    drawWindow05(-3.2f, floor1Y, zFront, 0.45f, 1.2f);
    drawWindow05(-2.4f, floor1Y, zFront, 0.45f, 1.2f);

    // Jendela jalusi pink - wing kanan
    drawWindow05(2.4f, floor1Y, zFront, 0.45f, 1.2f);
    drawWindow05(3.2f, floor1Y, zFront, 0.45f, 1.2f);
    drawWindow05(4.0f, floor1Y, zFront, 0.45f, 1.2f);
    drawWindow05(4.8f, floor1Y, zFront, 0.45f, 1.2f);

    // TEMBOK BAWAH LENGKAP - SATU KESATUAN
    glPushMatrix();
    glTranslatef(0.0f, floor1Y - 1.3f, zFront + 0.05f);
    setColor05(0.92f, 0.92f, 0.92f);
    drawBox05(buildingWidth, 0.8f, 0.1f);
    glPopMatrix();

    // Balkon dengan pagar lantai 1
    drawBalcony05(-3.5f, floor1Y - 0.8f, zFront + 0.05f, 3.5f);
    drawBalcony05(3.5f, floor1Y - 0.8f, zFront + 0.05f, 3.5f);

    // LANTAI DASAR TAMBAHAN
    glPushMatrix();
    glTranslatef(0.0f, floor1Y - 2.0f, zFront);
    setColor05(0.93f, 0.93f, 0.93f);
    drawBox05(buildingWidth + 0.3f, 0.6f, 0.15f);
    glPopMatrix();

    // PENUTUP CELAH BAWAH TOTAL
    glPushMatrix();
    glTranslatef(0.0f, -0.8f, zFront - 0.05f);
    setColor05(0.94f, 0.94f, 0.94f);
    drawBox05(buildingWidth + 1.5f, 2.2f, 0.25f);
    glPopMatrix();

    // EXTENSION SAMPING KIRI
    glPushMatrix();
    glTranslatef(-5.5f, -0.5f, 0.0f);
    setColor05(0.93f, 0.93f, 0.93f);
    drawBox05(1.2f, 2.5f, buildingDepth + 0.5f);
    glPopMatrix();

    // EXTENSION SAMPING KANAN
    glPushMatrix();
    glTranslatef(5.5f, -0.5f, 0.0f);
    setColor05(0.93f, 0.93f, 0.93f);
    drawBox05(1.2f, 2.5f, buildingDepth + 0.5f);
    glPopMatrix();

    // ==================== LANTAI 2 ====================
    float floor2Y = floor1Y + 2.7f;

    drawWindow05(-4.8f, floor2Y, zFront, 0.45f, 1.2f);
    drawWindow05(-4.0f, floor2Y, zFront, 0.45f, 1.2f);
    drawWindow05(-3.2f, floor2Y, zFront, 0.45f, 1.2f);
    drawWindow05(-2.4f, floor2Y, zFront, 0.45f, 1.2f);

    drawWindow05(2.4f, floor2Y, zFront, 0.45f, 1.2f);
    drawWindow05(3.2f, floor2Y, zFront, 0.45f, 1.2f);
    drawWindow05(4.0f, floor2Y, zFront, 0.45f, 1.2f);
    drawWindow05(4.8f, floor2Y, zFront, 0.45f, 1.2f);

    drawBalcony05(-3.5f, floor2Y - 1.3f, zFront + 0.05f, 3.5f);
    drawBalcony05(3.5f, floor2Y - 1.3f, zFront + 0.05f, 3.5f);

    // ==================== LANTAI 3 ====================
    float floor3Y = floor2Y + 2.7f;

    drawWindow05(-4.8f, floor3Y, zFront, 0.45f, 1.2f);
    drawWindow05(-4.0f, floor3Y, zFront, 0.45f, 1.2f);
    drawWindow05(-3.2f, floor3Y, zFront, 0.45f, 1.2f);
    drawWindow05(-2.4f, floor3Y, zFront, 0.45f, 1.2f);

    drawWindow05(2.4f, floor3Y, zFront, 0.45f, 1.2f);
    drawWindow05(3.2f, floor3Y, zFront, 0.45f, 1.2f);
    drawWindow05(4.0f, floor3Y, zFront, 0.45f, 1.2f);
    drawWindow05(4.8f, floor3Y, zFront, 0.45f, 1.2f);

    drawBalcony05(-3.5f, floor3Y - 1.3f, zFront + 0.05f, 3.5f);
    drawBalcony05(3.5f, floor3Y - 1.3f, zFront + 0.05f, 3.5f);

    // ==================== PINTU DAN JENDELA HITAM ====================
    float centerZ = zBase + buildingDepth / 2 + 0.02f;

    // PINTU HITAM
    glPushMatrix();
    glTranslatef(0.0f, floor1Y - 0.2f, centerZ);
    setColor05(0.05f, 0.05f, 0.05f);
    drawBox05(1.0f, 1.9f, 0.02f);
    glPopMatrix();

    // JENDELA HITAM KIRI
    glPushMatrix();
    glTranslatef(-1.0f, floor1Y - 0.2f, centerZ);
    setColor05(0.05f, 0.05f, 0.05f);
    drawBox05(0.6f, 1.9f, 0.02f);
    glPopMatrix();

    // JENDELA HITAM KANAN
    glPushMatrix();
    glTranslatef(1.0f, floor1Y - 0.2f, centerZ);
    setColor05(0.05f, 0.05f, 0.05f);
    drawBox05(0.6f, 1.9f, 0.02f);
    glPopMatrix();
}

// Fungsi utama wrapper untuk menggambar Gedung 05 lengkap
void drawBuilding05_Aggregate() {
    drawPerfectUnifiedBuilding05();
}

// =========================================================================
//  FUNGSI KOMPONEN KODE 2 - DISESUAIKAN
// =========================================================================

// Fungsi helper untuk menggambar satu unit jendela rapi - KODE 2
void drawWindowUnit(float w, float h) {
    // Kaca - biru gelap
    setMaterial(0.15f, 0.25f, 0.4f, false);
    glutSolidCube(1.0f); // Cube 1x1x1

    // Kusen - abu gelap
    setMaterial(0.2f, 0.2f, 0.2f, false);
    // Bingkai
    float frameThick = 0.2f;
    // Atas
    drawBalok(0.0f, h / 2, 0.05f, w + frameThick, frameThick, 0.2f, 0.2f, 0.2f, 0.2f);
    // Bawah
    drawBalok(0.0f, -h / 2, 0.05f, w + frameThick, frameThick, 0.2f, 0.2f, 0.2f, 0.2f);
    // Kanan
    drawBalok(w / 2, 0.0f, 0.05f, frameThick, h + frameThick, 0.2f, 0.2f, 0.2f, 0.2f);
    // Kiri
    drawBalok(-w / 2, 0.0f, 0.05f, frameThick, h + frameThick, 0.2f, 0.2f, 0.2f, 0.2f);

    // Garis tengah (kisi-kisi)
    drawBalok(0.0f, 0.0f, 0.05f, w, 0.1f, 0.15f, 0.2f, 0.2f, 0.2f); // Horizontal
    drawBalok(0.0f, 0.0f, 0.05f, 0.1f, h, 0.15f, 0.2f, 0.2f, 0.2f); // Vertikal
}

// Fungsi Atap Tradisional - KODE 2
void drawRoofTraditional(float baseWidth, float baseDepth) {
    // Atap pelana utama - warna merah terakota/coklat
    setMaterial(0.72f, 0.33f, 0.29f, false);
    float roofHeight = 7.0f;
    float overhang = 1.5f;
    float w = baseWidth / 2 + overhang;
    float d = baseDepth / 2 + overhang;

    glBegin(GL_TRIANGLES);
    // Front gable
    glNormal3f(0.0f, 0.5f, 1.0f);
    glVertex3f(-w, 0.0f, d); glVertex3f(w, 0.0f, d); glVertex3f(0.0f, roofHeight, d);
    // Back gable
    glNormal3f(0.0f, 0.5f, -1.0f);
    glVertex3f(-w, 0.0f, -d); glVertex3f(0.0f, roofHeight, -d); glVertex3f(w, 0.0f, -d);
    glEnd();

    glBegin(GL_QUADS);
    // Left slope
    glNormal3f(-roofHeight, w, 0.0f);
    glVertex3f(-w, 0.0f, -d); glVertex3f(-w, 0.0f, d); glVertex3f(0.0f, roofHeight, d); glVertex3f(0.0f, roofHeight, -d);
    // Right slope
    glNormal3f(roofHeight, w, 0.0f);
    glVertex3f(w, 0.0f, -d); glVertex3f(0.0f, roofHeight, -d); glVertex3f(0.0f, roofHeight, d); glVertex3f(w, 0.0f, d);
    glEnd();

    // Bubungan atap (Ridge)
    drawBalok(0.0f, roofHeight + 0.1f, 0.0f, 1.2f, 0.3f, baseDepth + overhang * 2 + 0.2f, 0.55f, 0.20f, 0.18f);
}

// Fungsi Teks FIP - KODE 2
void drawTextFIP() {
    setMaterial(0.9f, 0.1f, 0.1f, true); // Merah Terang, menyala sedikit

    glDisable(GL_LIGHTING); // Matikan lighting agar teks terlihat jelas
    glPushMatrix();

    // Teks diposisikan di dinding bagian depan L (yang menghadap ke Gedung Kanan)
    // Koordinat lokal (11.1 adalah pinggir blok + 0.1 offset)
    glTranslatef(11.1f, 10.0f, 0.0f);

    // Rotasi -90 derajat Y agar menghadap ke kanan (positif X)
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);

    glScalef(0.02f, 0.02f, 0.1f); // Skala teks

    const char* text = "FAKULTAS ILMU PENDIDIKAN";
    // Estimasi lebar
    float width_estimate = strlen(text) * 80.0f;
    glTranslatef(-width_estimate / 2, 0.0f, 0.0f); // Center text

    for (const char* c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
    glEnable(GL_LIGHTING); // Nyalakan kembali lighting
}

// Fungsi Tiang Bendera - KODE 2
void drawFlagPole(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z); // Pindahkan ke koordinat yang ditentukan

    // Base tiang (pondasi kecil)
    drawBalok(0.0f, 0.5f, 0.0f, 3.0f, 1.0f, 3.0f, 0.4f, 0.4f, 0.4f);

    // Tiang bendera - logam abu-abu
    setMaterial(0.7f, 0.7f, 0.75f, false);
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f); // Mulai dari atas base
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Tegak lurus ke atas
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.4f, 0.4f, 35.0f, 16, 1); // Tinggi tiang 35
    gluDeleteQuadric(quad);
    glPopMatrix();

    // Bendera - Merah Putih
    float flagBaseY = 1.0f + 35.0f; // Tinggi tiang + Base Y
    // Merah
    drawBalok(3.0f, flagBaseY - 2.0f, 0.0f, 6.0f, 3.5f, 0.1f, 0.9f, 0.1f, 0.1f);
    // Putih
    drawBalok(3.0f, flagBaseY - 5.5f, 0.0f, 6.0f, 3.5f, 0.1f, 1.0f, 1.0f, 1.0f);

    glPopMatrix();
}

// Fungsi Gedung FIP (L-Shape) - KODE 2
void drawBuilding() {
    glPushMatrix();
    glRotatef(buildingRotateY, 0.0f, 1.0f, 0.0f);

    float floorHeight = 20.0f; // Tinggi gedung
    float baseY = floorHeight / 2.0f;
    float winW = 3.5f; // Lebar jendela
    float winH = 5.0f; // Tinggi jendela
    int floors = 3;
    float floorSpacing = floorHeight / floors; // Spasi antar lantai (sekitar 6.67)

    // ================= GEDUNG KIRI (L-SHAPE) =================
    // 1. Bagian Belakang L (Horizontal)
    float backL_Width = 50.0f;
    float backL_Depth = 22.0f;
    float backL_CenterX = -15.0f;
    float backL_CenterZ = -45.0f; // Geser jauh ke belakang

    glPushMatrix();
    glTranslatef(backL_CenterX, baseY, backL_CenterZ);
    drawBalok(0.0f, 0.0f, 0.0f, backL_Width, floorHeight, backL_Depth, 0.92f, 0.92f, 0.90f);
    // Atap bagian belakang
    glPushMatrix(); glTranslatef(0.0f, floorHeight / 2 + 0.1f, 0.0f); drawRoofTraditional(backL_Width, backL_Depth); glPopMatrix();

    // Jendela belakang (3 baris x 5 kolom)
    for (int j = 0; j < floors; j++) {
        for (int i = 0; i < 5; i++) {
            glPushMatrix();
            glTranslatef(-backL_Width / 2 + 5.0f + i * 10.0f, -floorHeight / 2 + 5.0f + j * floorSpacing, backL_Depth / 2 + 0.1f);
            glScalef(winW, winH, 1.0f); // Scaling untuk drawWindowUnit
            drawWindowUnit(1.0f, 1.0f);
            glPopMatrix();
        }
    }
    glPopMatrix();

    // 2. Bagian Depan L (Vertikal memanjang ke depan)
    float frontL_Width = 22.0f;
    float frontL_Depth = 70.0f;
    float frontL_CenterX = -40.0f; // Ujung kiri Back L
    float frontL_CenterZ = -5.0f; // Agar sejajar dengan Gedung Kanan

    glPushMatrix();
    glTranslatef(frontL_CenterX, baseY, frontL_CenterZ);
    drawBalok(0.0f, 0.0f, 0.0f, frontL_Width, floorHeight, frontL_Depth, 0.92f, 0.92f, 0.90f);

    // Atap bagian depan (Diputar 90 derajat)
    glPushMatrix();
    glTranslatef(0.0f, floorHeight / 2 + 0.1f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawRoofTraditional(frontL_Depth, frontL_Width);
    glPopMatrix();

    // --- JENDELA FRONT L ---
    // Sisi KIRI LUAR (menghadap kiri) - 3 baris x 6 kolom
    for (int j = 0; j < floors; j++) {
        for (int i = 0; i < 6; i++) {
            glPushMatrix();
            glTranslatef(-frontL_Width / 2 - 0.1f, -floorHeight / 2 + 5.0f + j * floorSpacing, -frontL_Depth / 2 + 6.0f + i * 11.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(winW, winH, 1.0f);
            drawWindowUnit(1.0f, 1.0f);
            glPopMatrix();
        }
    }

    // Sisi DEPAN (menghadap Z+) - 3 baris x 3 kolom
    for (int j = 0; j < floors; j++) {
        for (int i = 0; i < 3; i++) {
            glPushMatrix();
            glTranslatef(-frontL_Width / 2 + 5.0f + i * 5.5f, -floorHeight / 2 + 5.0f + j * floorSpacing, frontL_Depth / 2 + 0.1f);
            glScalef(winW, winH, 1.0f);
            drawWindowUnit(1.0f, 1.0f);
            glPopMatrix();
        }
    }

    // Sisi KANAN DALAM (Menghadap kanan, tempat tulisan) - 3 baris x 6 kolom
    for (int j = 0; j < floors; j++) {
        for (int i = 0; i < 6; i++) {
            glPushMatrix();
            glTranslatef(frontL_Width / 2 + 0.1f, -floorHeight / 2 + 5.0f + j * floorSpacing, -frontL_Depth / 2 + 6.0f + i * 11.0f);
            glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(winW, winH, 1.0f);
            drawWindowUnit(1.0f, 1.0f);
            glPopMatrix();
        }
    }

    // Tulisan FIP (Diposisikan dalam transformasi blok ini)
    drawTextFIP(); // Sudah dikoreksi posisinya di dalam fungsi drawTextFIP

    glPopMatrix(); // End Front L block

    // ================= GEDUNG KANAN (UTAMA) =================
    float rightWidth = 40.0f;
    float rightDepth = 22.0f;
    float rightCenterX = 15.0f; // Geser ke kanan
    float rightCenterZ = -5.0f; // Sejajar front L

    glPushMatrix();
    glTranslatef(rightCenterX, baseY, rightCenterZ);
    drawBalok(0.0f, 0.0f, 0.0f, rightWidth, floorHeight, rightDepth, 0.92f, 0.92f, 0.90f);

    // ATAP TRADISIONAL (Sama seperti kiri)
    glPushMatrix(); glTranslatef(0.0f, floorHeight / 2 + 0.1f, 0.0f); drawRoofTraditional(rightWidth, rightDepth); glPopMatrix();

    // --- JENDELA GEDUNG KANAN ---
    // Jendela depan (3 baris x 4 kolom)
    for (int j = 0; j < floors; j++) {
        for (int i = 0; i < 4; i++) {
            glPushMatrix();
            float startX = -rightWidth / 2 + 8.0f;
            float spacing = 8.0f;
            glTranslatef(startX + i * spacing, -floorHeight / 2 + 5.0f + j * floorSpacing, rightDepth / 2 + 0.1f);
            glScalef(winW, winH, 1.0f);
            drawWindowUnit(1.0f, 1.0f);
            glPopMatrix();
        }
    }

    // Jendela samping kanan (3 baris x 2 kolom)
    for (int j = 0; j < floors; j++) {
        for (int i = 0; i < 2; i++) {
            glPushMatrix();
            glTranslatef(rightWidth / 2 + 0.1f, -floorHeight / 2 + 5.0f + j * floorSpacing, -5.0f + i * 10.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(winW, winH, 1.0f);
            drawWindowUnit(1.0f, 1.0f);
            glPopMatrix();
        }
    }

    glPopMatrix(); // End Gedung Kanan

    // ===== PENGHUBUNG TENGAH (Mencoba mengisi ruang 'L' belakang) =====
    glPushMatrix();
    glTranslatef(-2.0f, baseY - 2.0f, -25.0f);
    drawBalok(0.0f, 0.0f, 0.0f, 30.0f, floorHeight - 4.0f, 5.0f, 0.85f, 0.85f, 0.85f);
    glPopMatrix();

    // ===== TIANG BENDERA (Di depan GEDUNG KANAN) =====
    drawFlagPole(15.0f, 0.0f, rightDepth / 2 + 10.0f);

    // ===== TANGGA MASUK (Untuk gedung kanan) =====
    glPushMatrix();
    glTranslatef(15.0f, 0.0f, rightDepth / 2 + 5.0f);
    for (int i = 0; i < 3; i++) {
        drawBalok(0.0f, i * 0.5f, i * 1.5f, 15.0f, 0.5f, 1.5f, 0.6f, 0.6f, 0.6f);
    }
    glPopMatrix();

    glPopMatrix(); // End buildingRotateY
}

// =========================================================================
//  MAIN SCENE - DARI KODE 1
// =========================================================================

void drawEnvironment() {
    setMaterial(0.6f, 0.6f, 0.6f);
    glBegin(GL_QUADS); glNormal3f(0, 1, 0); glVertex3f(-300.0f, -0.1f, -300.0f); glVertex3f(-300.0f, -0.1f, 300.0f); glVertex3f(300.0f, -0.1f, 300.0f); glVertex3f(300.0f, -0.1f, -300.0f); glEnd();
}

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    lx = sinf(angle) * cosf(pitch);
    lz = -cosf(angle) * cosf(pitch);
    ly = sinf(pitch);

    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
    GLfloat light_pos[] = { 100.0f, 200.0f, 150.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    drawEnvironment();

    // --- KUMPULAN GEDUNG KAMPUS DARI KODE 1 ---
    // Gedung Persegi (Building 03)
    drawRectangularBuilding(0.0f, false);
    // Gedung Persegi (Building 04)
    drawRectangularBuilding(70.0f, true);
    // Gedung L-Shape (Kampus 1)
    drawLShapedBuilding(70.0f, 60.0f);
    // Gedung 06
    drawBuilding06(0.0f, -80.0f);
    // Gedung ULABK
    drawBuildingULABK(70.0f, -80.0f);

    // --- TAMBAHAN GEDUNG 05 (REVISI UKURAN & ATAP) - KODE 1 ---
    glPushMatrix();
    glTranslatef(110.0f, 5.0f, 60.0f);
    glScalef(2.5f, 2.5f, 2.5f);
    drawBuilding05_Aggregate();
    glPopMatrix();
    // --------------------------

    // --- GEDUNG FIP DARI KODE 2 (DIPINDAHKAN KE LOKASI PUSAT DEPAN) ---
    glPushMatrix();
    // Koordinat: Di tengah (X=0) dan di depan (Z=110.0f)
    // Z dimajukan ke 110.0f
    glTranslatef(0.0f, 0.0f, 110.0f);
    // Skala disesuaikan agar ukurannya lebih sejajar/proporsional (1.0x)
    glScalef(1.0f, 1.0f, 1.0f);
    drawBuilding(); // Gedung FIP
    glPopMatrix();
    // --------------------------

    glutSwapBuffers();
}

void changeSize(int w, int h) {
    if (h == 0) h = 1;
    float ratio = 1.0f * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, ratio, 1.0f, 2000.0f);
    glMatrixMode(GL_MODELVIEW);
}

void processKeys(unsigned char key, int xx, int yy) {
    float fraction = 2.0f;
    switch (key) {
    case 'w': x += lx * fraction; z += lz * fraction; break;
    case 's': x -= lx * fraction; z -= lz * fraction; break;
    case 'a': x += lz * fraction; z -= lx * fraction; break;
    case 'd': x -= lz * fraction; z += lx * fraction; break;
    case 32:  y += fraction; break; // Spasi
    case 'c': y -= fraction; break;
    case 'r': buildingRotateY -= 5.0f; glutPostRedisplay(); break; // Rotasi Gedung FIP
    case 't': buildingRotateY += 5.0f; glutPostRedisplay(); break; // Rotasi Gedung FIP
    case 27: exit(0); break; // ESC
    }
}

void processMouseMove(int x, int y) {
    if (xOrigin >= 0) {
        deltaAngle = (x - xOrigin) * 0.005f;
        angle += deltaAngle;
        int dy = y - yOrigin;
        pitch -= dy * 0.005f;

        // Batasi Pitch agar tidak flip
        if (pitch > M_PI_2 - 0.01f) pitch = M_PI_2 - 0.01f;
        if (pitch < -M_PI_2 + 0.01f) pitch = -M_PI_2 + 0.01f;

        xOrigin = x;
        yOrigin = y;
        glutPostRedisplay();
    }
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) { xOrigin = x; yOrigin = y; }
        else { xOrigin = -1; yOrigin = -1; }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("KOMPLEK FIP UNESA & Gedung Kampus Lainnya (Posisi Disesuaikan)");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    // Setup material lighting default
    GLfloat matSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat matShininess[] = { 20.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Warna langit

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(processKeys);
    glutMouseFunc(mouseButton);
    glutMotionFunc(processMouseMove);

    printf("=== KONTROL KAMERA KODE 1 ===\n");
    printf("W/S/A/D     : Gerak Maju/Mundur/Samping\n");
    printf("Spasi/C     : Gerak Naik/Turun\n");
    printf("Mouse Kiri  : Putar Sudut Pandang\n");
    printf("R/T         : Putar Gedung FIP (Rotasi)\n");

    glutMainLoop();
    return 0;
}
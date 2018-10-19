/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <QImage>
#include <QtMath>
struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
    QVector3D color;
};

//! [0]
GeometryEngine::GeometryEngine(int pSeason)
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    this->season = pSeason;

    heightmap = QImage(":/heightmap-3.png"); //load map

    // Initializes cube geometry and transfers it to VBOs
    initPlaneGeometry();
    //initMapGeometry();

}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void GeometryEngine::initMapGeometry(){

}

void GeometryEngine::drawMapGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
}

void GeometryEngine::initPlaneGeometry()
{
    int vertexCount = 16 * 16;
    int sqrtVertexCount = (int)sqrt(vertexCount);

     VertexData* vertices = new VertexData[vertexCount];

    float size = 2.0f;

    float step = size / (sqrtVertexCount - 1) ;

    int x = 0;
    int y = 0;
    int index = 0;
    float maxY = 1.0f;
    float stepTexX = (1.0f / 3.0f) /(sqrtVertexCount - 1);
    float stepTexY = (1.0f / 2.0f) /(sqrtVertexCount - 1);
    for (float j = 0.0 - (size / 2.0); j <= 0.0 + (size/ 2.0); j += step)
    {
        x = 0;
        for (float i = 0.0 - (size / 2.0); i <= 0.0 + (size/ 2.0); i += step)
        {
            VertexData vertex;
            vertex.texCoord = QVector2D(0.6666+x * stepTexX, y * stepTexY);

            QRgb color = heightmap.pixel(qFloor(x * 1.0f / (sqrtVertexCount - 1) * (heightmap.width() - 1)), qFloor(y * 1.0f / (sqrtVertexCount - 1) * (heightmap.height() - 1)));

            vertex.position = QVector3D(i, maxY * qGray(color) / 255, j);
            switch (this->season)
            {
                case 0:
                    vertex.color = QVector3D(1.0f, 1.0f , 0.0f);
                    break;
                case 1:
                    vertex.color = QVector3D(1.0f, 0.5f, 0.0f);
                    break;
                case 2:
                    vertex.color = QVector3D(1.0f, 1.0f, 1.0f);
                    break;
                case 3:
                    vertex.color = QVector3D(0.0f, 1.0f, 0.0f);
                    break;
                default:
                    vertex.color = QVector3D(1.0f, 1.0f, 1.0f);
                    break;
            }

            vertices[index++] = vertex;
            x++;
        }
        y++;
    }

    int indicesCount = pow(sqrtVertexCount-1, 2) * 2 * 3;
    index = 0;
    GLushort* indices = new GLushort[indicesCount];

    for (int y = 0; y < sqrtVertexCount - 1; y++)
    {
        for (int x = 0; x < sqrtVertexCount - 1; x++)
        {
            // top left
            indices[index++] = x + y * sqrtVertexCount;
            // bottom left
            indices[index++] = x + (y+1) * sqrtVertexCount;
            // bottom right
            indices[index++] = (x+1) + (y+1) * sqrtVertexCount;

           // top left
           indices[index++] = x + y * sqrtVertexCount;
           // bottom right
           indices[index++] = (x+1) + (y+1) * sqrtVertexCount;
           // top right
           indices[index++] = (x+1) + y * sqrtVertexCount;

        }
    }


//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, vertexCount * sizeof(VertexData));
    delete[] vertices;

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, indicesCount * sizeof(GLushort));
    delete[] indices;
//! [1]
}

void GeometryEngine::drawPlaneGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    if(!arrayBuf.bind())
    {
        std::cout << "error bind arrayBuf" << std::endl;
    }
    if(!indexBuf.bind())
    {
        std::cout << "error bind indexBuf" << std::endl;
    }

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int colorLocation = program->attributeLocation("a_color");
    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer(colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, 1350, GL_UNSIGNED_SHORT, 0);
}


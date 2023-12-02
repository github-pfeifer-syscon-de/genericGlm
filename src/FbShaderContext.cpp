/*
 * Copyright (C) 2018 rpf
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <fstream>
#include <glibmm.h>

#include "FbShaderContext.hpp"

#undef FB_DEBUG

Framebuffer::Framebuffer()
: m_framebufferId{0}
, m_textureId{0}
, m_depthbufId{0}
, atype{GL_RGBA8}
, type{GL_RGBA}
, format{GL_UNSIGNED_BYTE}
, fbWidth{0}
, fbHeight{0}
, fbSampling{1}
{
    glGenFramebuffers(1, &m_framebufferId);
    checkError("TexShaderCtx glGenFramebuffers");
}

Framebuffer::~Framebuffer()
{
    freeBuffer();
    unbind();
    if (m_framebufferId > 0) {
        glDeleteFramebuffers(1, &m_framebufferId);
        m_framebufferId = 0;
    }
}

void
Framebuffer::freeBuffer()
{
    bind();
    if (m_textureId > 0) {
        glDeleteTextures(1, &m_textureId);
        m_textureId = 0;
    }
    if (m_depthbufId > 0) {
        glDeleteRenderbuffers(1, &m_depthbufId);
        m_depthbufId = 0;
    }
}

void
Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
    checkError("TexShaderCtx glBindFramebuffer");
}

void
Framebuffer::unbind()
{
    //Bind 0, which means render to back buffer, as a result, fb is unbound
	#ifndef USE_GLES
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	#else
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	#endif
    checkError("TexShaderCtx glBindFramebuffer 0");
}

void
Framebuffer::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    checkError("TexShaderCtx glBindTexture");
}

GLuint
Framebuffer::getWidth()
{
    return fbWidth;
}

GLuint
Framebuffer::getHeight()
{
    return fbHeight;
}

GLint
Framebuffer::getSampling()
{
    return fbSampling;
}

GLuint
Framebuffer::getName()
{
    return m_framebufferId;
}

void
Framebuffer::scale(GLuint width, GLuint height, GLint _sampling, bool depth)
{
    fbWidth = width;
    fbHeight = height;
    fbSampling = _sampling;
    freeBuffer();
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
    checkError("TexShaderCtx glBindFramebuffer");
    //RGBA8 2D texture, 24 bit depth texture
    glGenTextures(1, &m_textureId);
    checkError("TexShaderCtx glGenTextures"); // gets an error but everything seems ok
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    checkError("TexShaderCtx glBindTexture");
    //nullptr means reserve texture memory, but texels are undefined
    // this leads to a warning as we pass nullptr, the doc says this should be allowed ...
    glTexImage2D(GL_TEXTURE_2D, 0, atype, fbWidth, fbHeight, 0, type, format, nullptr);
    checkError("TexShaderCtx glTexImage2D");
    #ifndef USE_GLES
    GLint maxSamples{-1};
    // we need to find out what the maximum supported samples is
    glGetIntegerv(GL_MAX_SAMPLES_EXT, &maxSamples);
    GLint useSampling = std::min(maxSamples, fbSampling*fbSampling);
    if (useSampling > 1) {
        #ifdef FB_DEBUG
        std::cout << __FILE__ << "::createFramebuffer useSampling " << useSampling << std::endl;
        #endif
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, useSampling, GL_RGBA, fbWidth, fbHeight, GL_TRUE);
    }
    #endif
    checkError("TexShaderCtx glTexImage2DMultisample");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // linear shoud give optimal result, weightened to effort
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    checkError("TexShaderCtx glTexParameteri");
    if (depth) {
        // The depth buffer
        glGenRenderbuffers(1, &m_depthbufId);
        checkError("TexShaderCtx glGenRenderbuffers");
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthbufId);
        checkError("TexShaderCtx glBindRenderbuffer");
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbWidth, fbHeight);
        checkError("TexShaderCtx glRenderbufferStorage");
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthbufId);
        checkError("TexShaderCtx glFramebufferRenderbuffer");
    }
    // Set "renderedTexture" as our colour attachement #0
	#ifndef USE_GLES
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureId, 0);
	#else
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0);
	#endif
    checkError("TexShaderCtx glFramebufferTexture");
    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    checkError("TexShaderCtx glDrawBuffers");

    //-------------------------
    //Does the GPU support current FBO configuration?
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
		#ifndef USE_GLES
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            //std::cout << "good" << std::endl;
            break;
		#else
        case GL_FRAMEBUFFER_COMPLETE:
            //std::cout << "good" << std::endl;
            break;
		#endif
        default:
            std::cout << "creating framebuffer err " << status << std::endl;
            break;
    }
}

FbShaderContext::FbShaderContext()
: ShaderContext()
, winWidth{0}
, winHeight{0}
, m_box{nullptr}
, m_texLocation{0}
, m_clear_color{0.1, 0.1, 0.15}
{
}

FbShaderContext::~FbShaderContext()
{
    if (m_box != nullptr) {
        delete m_box;
        m_box = nullptr;
    }
}

void
FbShaderContext::updateLocation() {
    m_texLocation = glGetUniformLocation(m_program, "renderedTexture");
    checkError("TexShaderCtx unif");
}

bool
FbShaderContext::useColor()
{
    return false;
}

bool
FbShaderContext::useNormal()
{
    return false;
}

bool
FbShaderContext::useUV()
{
    return true;
}

void
FbShaderContext::setup(unsigned int _width, unsigned int _height, unsigned int sampling) {
    // use twice the display size, for optimal anti aliasing, or at least matching what we are going to use on drawing
    if (!m_renderBuffer) {
        m_renderBuffer = std::make_shared<Framebuffer>();
    }
    //if (!m_resolveBuffer) {
    //    m_resolveBuffer = std::make_shared<Framebuffer>();
    //}
    if (winWidth != _width
     || winHeight != _height
     /*|| static_cast<GLuint>(m_resolveBuffer->getSampling()) != sampling*/) {
        winWidth = _width;
        winHeight = _height;
        m_renderBuffer->scale(_width * sampling, _height * sampling, 1, true);
        //m_resolveBuffer->scale(_width, _height, sampling, false);
    }
    if (m_box == nullptr) {
        m_box = new Geometry(GL_TRIANGLES, this);

        //             x      y     z                u     v
        Position p0(-1.0f, -1.0f, 0.0f);    UV uv0(0.0f, 0.0f);
        Position p1( 1.0f, -1.0f, 0.0f);    UV uv1(1.0f, 0.0f);
        Position p2(-1.0f,  1.0f, 0.0f);    UV uv2(0.0f, 1.0f);
        Position p3(-1.0f,  1.0f, 0.0f);    UV uv3(0.0f, 1.0f);
        Position p4( 1.0f, -1.0f, 0.0f);    UV uv4(1.0f, 0.0f);
        Position p5( 1.0f,  1.0f, 0.0f);    UV uv5(1.0f, 1.0f);
        m_box->addPoint(&p0, nullptr, nullptr, &uv0);
        m_box->addPoint(&p1, nullptr, nullptr, &uv1);
        m_box->addPoint(&p2, nullptr, nullptr, &uv2);
        m_box->addPoint(&p3, nullptr, nullptr, &uv3);
        m_box->addPoint(&p4, nullptr, nullptr, &uv4);
        m_box->addPoint(&p5, nullptr, nullptr, &uv5);
        m_box->create_vao();
    }
}


void
FbShaderContext::set_clear_color(const Color& clear_color)
{
    m_clear_color = clear_color;
}

// use buffer for rendering, so the following drawing will be in fb
void
FbShaderContext::prepare() {
    //and now you can render to GL_TEXTURE_2D
    if (m_renderBuffer ) {
        m_renderBuffer->bind();
        glViewport(0, 0, m_renderBuffer->getWidth(), m_renderBuffer->getHeight());
        checkError("TexShaderCtx glViewport");
    }
    glClearDepthf(1.0f);
    checkError("clear depth");
    glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, 1.0);
    checkError("TexShaderCtx clearColor");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    checkError("TexShaderCtx clear");
}

// back to normal rendering
void
FbShaderContext::done() {
    if (m_renderBuffer) {
        m_renderBuffer->unbind();
    }
//    if (m_resolveBuffer) {
//        #ifdef FB_DEBUG
//        std::cout << "::done blit "
//                  << " width " << m_renderBuffer->getWidth()
//                  << " to " << m_resolveBuffer->getWidth()
//                  << std::endl;
//        #endif
//        glEnable(GL_MULTISAMPLE);
//        // Resolved multisampling
//        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_renderBuffer->getName());
//        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolveBuffer->getName());
//        glBlitFramebuffer(
//            0, 0, m_renderBuffer->getWidth(), m_renderBuffer->getHeight(),
//            0, 0, m_resolveBuffer->getWidth(), m_resolveBuffer->getHeight(),
//            GL_COLOR_BUFFER_BIT, GL_LINEAR);
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        glDisable(GL_MULTISAMPLE);
//    }
}

// draw texture to back-buffer
void
FbShaderContext::draw() {
    use();
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    checkError("TexShaderCtx glActiveTexture");
    //if (m_resolveBuffer) {
    //    m_resolveBuffer->bindTexture();
    //}
    if (m_renderBuffer) {
        m_renderBuffer->bindTexture();
    }
    // Set our "renderedTexture" sampler to use Texture Unit 0
    glUniform1i(m_texLocation, 0);      // texture unit 0 see above
    checkError("TexShaderCtx glUniform1i texID");
    Matrix mvp(1.0f);       // use default matrix, as this will not get used
    m_box->display(mvp);

    checkError("TexShaderCtx fbCtx display");
    unuse();
}

Matrix
FbShaderContext::setScalePos(const Matrix &m, Position &p, GLfloat scale)
{
    return m;
}

bool
FbShaderContext::createProgram(Glib::Error &error)
{
	#ifndef USE_GLES
	const char* version = "130";
	const char* precision = "";
	#else
    const char* version = "300 es";
	const char* precision = "precision mediump float;";
	#endif
	auto vertex = Glib::ustring::sprintf(
		"#version %s\n"
		"in vec3 position;\n"
		"in vec2 uv;\n"
		"out vec2 UV;\n"
		"void main() {\n"
		"	gl_Position = vec4(position,1.0);\n"
		"	UV = uv;\n"
		"}\n", version);
    auto fragment = Glib::ustring::sprintf(
		"#version %s\n"
		"%s\n"
		"in vec2 UV;\n"
		"out vec3 color;\n"
		"uniform sampler2D renderedTexture;\n"
		"void main() {\n"
		"	color = texture(renderedTexture,UV).rgb;\n"
		"}\n", version, precision);
    bool ret = ShaderContext::createProgram((gconstpointer)vertex.c_str(), (gconstpointer)fragment.c_str(), error);
    return ret;
}
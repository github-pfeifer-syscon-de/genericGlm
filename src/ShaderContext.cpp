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
#include <iostream>
#include <sstream>
#include <glibmm.h>
#include <cmath>

#include "GenericGlmCompat.hpp"
#include "glarea-error.h"
#include "ShaderContext.hpp"

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
	#ifndef USE_GLES
    const char *typ;
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            typ = "error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typ = "deprecated behavior";
            break;
        case  GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typ = "undef behavior";
            break;
        case  GL_DEBUG_TYPE_PORTABILITY:
            typ = "portability";
            break;
        case  GL_DEBUG_TYPE_PERFORMANCE:
            typ = "performance";
            break;
        case  GL_DEBUG_TYPE_OTHER:
            typ = "other";
            break;
        case  GL_DEBUG_TYPE_MARKER:
            typ = "mark";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            typ = "push";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            typ = "pop";
            break;
        default:
            typ = "?";
            break;
    }
    const char *sever;
    switch(severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            sever = "notify";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            sever = "high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            sever = "medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            sever = "low";
            break;
        default:
            sever = "?";
            break;
    }
   // https://www.khronos.org/opengl/wiki/Debug_Output
  fprintf( stderr, "GL CALLBACK: type = %s, severity = %s, message = %s\n",
            typ, sever, message );
//void glObjectLabel(GLenum identifier​, GLuint name​, GLsizei length​, const char * label​);
//void glObjectPtrLabel(void * ptr​, GLsizei length​, const char * label​);
	#else
	fprintf( stderr, "GL CALLBACK: type = %d, severity = %d, message = %s\n",
            type, severity, message );
	#endif
}

static gboolean
checkProgram(guint program, guint glstatus, Glib::Error &error)
{
    int status = 0;
    glGetProgramiv(program, glstatus, &status);
    if (status == GL_FALSE) {
        int log_len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = (char *) g_malloc(log_len + 1);
        glGetProgramInfoLog(program, log_len, nullptr, buffer);

        gchar *msg = g_strdup_printf("%s failure in program: %s",
			glstatus == GL_LINK_STATUS ? "Linking" : "Validating",  buffer);
        error = Glib::Error(GLAREA_ERROR, GLAREA_ERROR_SHADER_LINK, msg);
        g_free(buffer);
        g_free(msg);
    }
    return status;
}

ShaderContext::ShaderContext()
: GeometryContext()
, m_program{0}
{
}


ShaderContext::~ShaderContext()
{
    if (m_program != 0) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

guint
ShaderContext::createShader(int shader_type,
                            gconstpointer source,
                            Glib::Error &error)
{
    guint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const GLchar * const*) &source, nullptr);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        int log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = (char *) g_malloc(log_len + 1);
        const char *type;
        glGetShaderInfoLog(shader, log_len, nullptr, buffer);
        if (shader_type == GL_VERTEX_SHADER)
            type = "vertex";
        else if (shader_type == GL_FRAGMENT_SHADER)
            type = "fragment";
		#ifndef USE_GLES
        else if (shader_type == GL_GEOMETRY_SHADER)
            type = "geometry";
		#endif
        else
            type = "(this came as a total surprise)";
        gchar *msg = g_strdup_printf("Compilation failure in %s shader: %s",
                                     type,
                                     buffer);
        error = Glib::Error(GLAREA_ERROR, GLAREA_ERROR_SHADER_COMPILATION, msg);
        g_free(msg);
        g_free(buffer);

        glDeleteShader(shader);
        shader = 0;
		// it can be helpful to see what shader did fail, as we have some around ....
		fprintf(stderr, "type %s status %s\n----\n%s\n-------\n", type, msg, (char*)source);
    }

    return shader;
}

bool
ShaderContext::createProgram(guint vertex, guint fragment, guint geometry, Glib::Error &error)
{
    /* link the vertex and fragment shaders together */
    guint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    if (geometry != 0)
        glAttachShader(program, geometry);
    glLinkProgram(program);
    if (checkProgram(program, GL_LINK_STATUS, error)) {

        glValidateProgram(program); // Check shader is supported
        if (checkProgram(program, GL_VALIDATE_STATUS, error)) {

            /* the individual shaders can be detached and destroyed */
            glDetachShader(program, vertex);
            glDetachShader(program, fragment);
            if (geometry != 0)
                glDetachShader(program, geometry);
            m_program = program;
            updateLocation();
            updateIndexes(m_program);
            return TRUE;
        }
        else {
            glDeleteProgram(program);
        }
    }
    else {
        glDeleteProgram(program);
    }
    return FALSE;
}

static
void deleteShader(guint vertex, guint fragment, guint geometry)
{
    // can delete in a last step as they are reference a second time
    if (vertex != 0)
        glDeleteShader(vertex);
    if (fragment != 0)
        glDeleteShader(fragment);
    if (geometry != 0)
        glDeleteShader(geometry);
}

bool
ShaderContext::createProgram(gconstpointer pVertex, gconstpointer pFragment, Glib::Error &error)
{
#ifdef DEBUG
    // During init, enable debug output, dont care about notifications as these are quite specific not to say irrelevant
    //                    source        type          severity
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0 );
#endif
    /* load the vertex shader */
    guint vertex = createShader(GL_VERTEX_SHADER, pVertex, error);
    guint fragment = 0;
    bool ret = FALSE;
    if (vertex != 0) {
        /* load the fragment shader */
        fragment = createShader(GL_FRAGMENT_SHADER, pFragment, error);
        if (fragment != 0) {
            ret = createProgram(vertex, fragment, 0, error);
			if (!ret) {
				printf("---- vertex ----\n%s\n-------------", (char*)pVertex);
				printf("---- fragment ----\n%s\n-------------", (char*)pFragment);
			}
        }
    }
    deleteShader(vertex, fragment, 0);

    return ret;
}

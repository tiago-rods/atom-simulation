#include "Shader.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>

Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
    unsigned int vert = compile(GL_VERTEX_SHADER,   readFile(vertPath));
    unsigned int frag = compile(GL_FRAGMENT_SHADER, readFile(fragPath));

    m_id = glCreateProgram();
    glAttachShader(m_id, vert);
    glAttachShader(m_id, frag);
    glLinkProgram(m_id);

    int ok;
    glGetProgramiv(m_id, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(m_id, 512, nullptr, log);
        throw std::runtime_error(std::string("Shader link error: ") + log);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

Shader::~Shader() {
    glDeleteProgram(m_id);
}

void Shader::use() const { glUseProgram(m_id); }

void Shader::setMat4(const std::string& name, const glm::mat4& v) const {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(v));
}

void Shader::setVec3(const std::string& name, const glm::vec3& v) const {
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(v));
}

void Shader::setFloat(const std::string& name, float v) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), v);
}

unsigned int Shader::compile(unsigned int type, const std::string& src) {
    unsigned int id = glCreateShader(type);
    const char* c = src.c_str();
    glShaderSource(id, 1, &c, nullptr);
    glCompileShader(id);

    int ok;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(id, 512, nullptr, log);
        throw std::runtime_error(std::string("Shader compile error: ") + log);
    }
    return id;
}

std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open shader: " + path);
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

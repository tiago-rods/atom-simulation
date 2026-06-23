#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& vertPath, const std::string& fragPath);
    ~Shader();

    void use() const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setFloat(const std::string& name, float value) const;

    unsigned int id() const { return m_id; }

private:
    unsigned int m_id = 0;

    static unsigned int compile(unsigned int type, const std::string& source);
    static std::string readFile(const std::string& path);
};

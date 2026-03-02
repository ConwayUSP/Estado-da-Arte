#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(){
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)1000/(float)1000, 0.1f, 100.0f);
    glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
}

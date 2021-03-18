#pragma once
#include "Light.h"
class PointLight :
    public Light
{
    public:
        PointLight();
        PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
                    GLfloat xPos, GLfloat yPos, GLfloat zPos,
                    GLfloat con, GLfloat lin, GLfloat exp);

        // NOTE: For location variables, it should be GLuint. Float works but is not... appropriate.
        void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
            GLfloat diffuseIntensityLocation, GLfloat positionLocation,
            GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);
    private:
        glm::vec3 position; // Where the point light is.

        GLfloat constant, linear, exponent; // Values controlling the attenuation of our light.
};


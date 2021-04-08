#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
    public:
        DirectionalLight();

        DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
            GLfloat xDirection, GLfloat yDirection, GLfloat zDirection);

        // NOTE: For location variables, it should be GLuint. Float works but is not... appropriate.
        void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
            GLfloat diffuseIntensityLocation, GLfloat directionLocation);

    private:
        glm::vec3 direction; // The direction of the light. Where it goes, essentially.
};


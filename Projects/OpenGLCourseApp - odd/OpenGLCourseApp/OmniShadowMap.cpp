#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap() : ShadowMap()
{
}

bool OmniShadowMap::Init(unsigned int width, unsigned int height)
{
    shadowWidth = width;
    shadowHeight = height;

    glGenFramebuffers(1, &FBO);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);

    for (size_t i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, // Which side to target. Adding i will get the right axis, starting from Positive X. They are all adjacent in value.
            0, // Mipmap level
            GL_DEPTH_COMPONENT, // Internal component
            shadowWidth, // width
            shadowHeight, // height
            0, // Legacy item
            GL_DEPTH_COMPONENT, // Format. Same as internal component
            GL_FLOAT, // Type of data we are passing
            nullptr // Not assigning data yet, assigned on Shadow Pass.
        );
    }

    // Setting texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, // Type of Texture
        GL_TEXTURE_WRAP_S, // Which parameter to change. Here, we change the way it wraps around the x-axis of the image.
        GL_CLAMP_TO_EDGE // 
    );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, // Type of Texture
        GL_TEXTURE_WRAP_T, // Which parameter to change. Here, we change the way it wraps around the x-axis of the image.
        GL_CLAMP_TO_EDGE // The value. 
    );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, // Type of Texture
        GL_TEXTURE_WRAP_R, // Which parameter to change. Here, we change the way it wraps around the x-axis of the image. R represents the Z.
        GL_CLAMP_TO_EDGE // The value. 
    );

    glTexParameteri(GL_TEXTURE_CUBE_MAP, // Type of Texture
        GL_TEXTURE_MIN_FILTER, // Which parameter to change. Here, we change the way the texture interacts when we zoom out on it.
        GL_LINEAR // The value.  Here, we could also use GL_LINEAR. It's personnal preference.
    );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, // Type of Texture
        GL_TEXTURE_MAG_FILTER, // Which parameter to change. Here, we change the way the texture interacts when we zoom in on it.
        GL_LINEAR // The value. Here, we could also use GL_LINEAR. It's personnal preference.
    );

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);

    // Disabling reading colors.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    // Checking all went well
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Shadowmap Framebuffer Error: %i\n", status);
    }

    return true;
}

void OmniShadowMap::Write()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); // Binding buffer
}

void OmniShadowMap::Read(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
}

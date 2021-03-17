#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
    ClearShader();
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);

    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    CreateFromString(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open())
    {
        printf("Failed to open file at %s", fileLocation);
        return "";
    }

    std::string line = "";

    while (!fileStream.eof())
    {
        std::getline(fileStream, line);

        content.append(line + "\n");
    }

    fileStream.close();

    return content;
}

GLuint Shader::GetProjectionLocation()
{
    return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
    return uniformModel;
}

void Shader::UseShader()
{
    glUseProgram(shaderID);
}

// Deletes the program from the GPU.
void Shader::ClearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    // Creating the shader program. This will handle our shaders, created earlier as strings.
    // So now this creates an empty shader program. It has nothing in yet, but we will put in our custom shaders in a moment.
    shaderID = glCreateProgram();

    // Here we check if we got our program successfully created. If there is nothing in shader, then we have an error.
    if (!shaderID) {
        printf("Error creating shader program.\n");
        return;
    }

    // Adding our shaders to the program. This method will also compile the shaders.
    // shader is our program, vShader is the code for the vertex shader we created, and the last value is the type of shader it is.
    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    // Same as above, but for the fragment shader.
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    // These are debug variables. We will use these to detect errors that come from the GPU when compiling shaders and stuff.
    GLint result = 0; // This will store the result of our operations
    GLchar eLog[1024] = { 0 }; // This will store the error message. We first initialize it to be empty.

    // Create the executable for the shader program and link it correctly to make sure it all works.
    // Essentially, this takes our shader program, links it together like a C++ linker would do, and gives it to the graphics card.
    glLinkProgram(shaderID);
    // Get info on the program to make sure it's linked properly
    glGetProgramiv(shaderID, // the program to check 
        GL_LINK_STATUS, // Which info we want to gather. Here, we want the Link status, to see if it is linked or not.
        &result // Where to store the result of our query.
    );

    // We now check if result is false. If it is, it means our program is not linked correctly.
    if (!result)
    {
        // We fetch the error.
        glGetProgramInfoLog(
            shaderID, // Program to get the error from
            sizeof(eLog), // Size of the variable to store the error information in
            NULL,
            eLog // Our error log that will contain the error.
        );

        // Print out the error to the console. So we print the content of eLog.
        printf("Error linking shader program: '%s'\n", eLog);
        return;
    }

    // Check if the shader program we created is valid in the current OpenGL context.
    glValidateProgram(shaderID);

    // Get info on the program to make sure it's validated properly
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

    // Check if it validated properly. Just like above.
    if (!result)
    {
        // We fetch the error.
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);

        // Print out the error to the console. So we print the content of eLog.
        printf("Error validating shader program: '%s'\n", eLog);
        return;
    }

    // Get the ID of the uniform variable.
    uniformModel = glGetUniformLocation(shaderID, // Program to get the variable from
        "model"); // Name of the variable in the program.
    // Get ID of our uniform projection variable.
    uniformProjection = glGetUniformLocation(shaderID, // Program to get the variable from
        "projection"); // Name of the variable in the program.
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    // Create the individual shader.
    // The shader will be empty, and of the type we passed.
    // We saved the returned ID.
    GLuint theShader = glCreateShader(shaderType);

    // Creating an array pointer, pointing to the first element.
    // We are going to store the code here.
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    // Defining length of the code.
    GLint codeLength[1];
    // We use strlen to get the exact length of our code.
    codeLength[0] = strlen(shaderCode);

    // Setup the shader source code
    glShaderSource(theShader, // Shader we will be modifying the code for
        1, // This is for multiple strings. We could have a bunch of chunks of code, and combine them. But we only have one file.
        theCode, // Code to be put in our shader
        codeLength // Length of the code to be put in our shader
    );

    // Compile the shader!
    glCompileShader(theShader);

    // Now we need to verify it compiled properly.

    // These are debug variables. We will use these to detect errors that come from the GPU when compiling shaders and stuff.
    GLint result = 0; // This will store the result of our operations
    GLchar eLog[1024] = { 0 }; // This will store the error message. We first initialize it to be empty.

    // Get info on the program to make sure it's linked properly
    glGetShaderiv(theShader, // the program to check 
        GL_COMPILE_STATUS, // Which info we want to gather. Here, we want the compile status, to see if it is compiled successfully or not.
        &result // Where to store the result of our query.
    );

    // We now check if result is false. If it is, it means our program is not compiled correctly.
    if (!result)
    {
        // We fetch the error.
        glGetShaderInfoLog(
            theShader, // Program to get the error from
            sizeof(eLog), // Size of the variable to store the error information in
            NULL,
            eLog // Our error log that will contain the error.
        );

        // Print out the error to the console. So we print the content of eLog, and which type of shader failed.
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    // This will attach the shader, which is now compiled, to the shader program.
    glAttachShader(theProgram, theShader);

    return;
}

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#if 0
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& file_path)
{
    std::ifstream stream(file_path);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;
    std::string line;
    std::stringstream ss[2];
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                //set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                //set mode to fragment
                type = ShaderType::FRAGMENT;
            }
            else
            {

            }
        }
        else
        {
            //ss[(int)type] << line << "\n";
            if (type == ShaderType::VERTEX)
            {
                ss[0] << line << "\n";
            }
            else if (type == ShaderType::FRAGMENT)
            {
                ss[1] << line << "\n";
            }
            else
            {

            }
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader! " << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);


    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

#endif

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* V-Sync 垂直同步 */
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;

    }

    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    {
        float pos[] = {
            -50.0f, -50.0f, 0.0f, 0.0f,  //0
             50.0f, -50.0f, 1.0f, 0.0f,  //1
             50.0f,  50.0f, 1.0f, 1.0f,  //2
            -50.0f,  50.0f, 0.0f, 1.0f   //3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        

        //unsigned int vao;
        //glGenVertexArrays(1, &vao);
        //glBindVertexArray(vao);

        /*
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), pos, GL_STATIC_DRAW);
        */
        VertexArray va;
        VertexBuffer vb(pos, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
        //glEnableVertexAttribArray(0);

        /*
        unsigned int ibo;   //index buffer object
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6  * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        */
        IndexBuffer ib(indices, 6);

        glm::vec3 tranlationA(200, 200, 0);
        glm::vec3 tranlationB(400, 200, 0);

        //根据窗口分辨率把它改成了每个像素 x在0-960 y在0-540
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), tranlationA);

		glm::mat4 mvp = proj * view * model;

        /**********3***********/
        //std::string vertexshader =
        //    "#version 330 core\n"
        //    "\n"
        //    "layout(location = 0) in vec4 position;\n"
        //    "\n"
        //    "void main()\n"
        //    "{\n"
        //    "   gl_Position = position;\n"
        //    "}\n";

        //std::string fragmentshader =
        //    "#version 330 core\n"
        //    "\n"
        //    "layout(location = 0) out vec4 color;\n"
        //    "\n"
        //    "void main()\n"
        //    "{\n"
        //    "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        //    "}\n";

        //unsigned int shader = CreateShader(vertexshader, fragmentshader);
        //glUseProgram(shader);

        /*
        ShaderProgramSource source = ParseShader("res\\shaders\\Basic.shader");

        std::cout << "VertexSource: " << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "FragmentSource: " << std::endl;
        std::cout << source.FragmentSource << std::endl;

        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        glUseProgram(shader);
        */
		Shader shader("res\\shaders\\Basic.shader");
	    shader.Bind();

        /*
        int location = glGetUniformLocation(shader, "u_Color");
        if (location == -1)
        {
            std::cout << "glGetUniformLocation error " << std::endl;
            return -1;
        }
        glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f);
        */
        shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);

        shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/textures/ChernoLogo.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        //unbind
        //glBindVertexArray(0);
        va.UnBind();
        //glUseProgram(0);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        shader.UnBind();
        vb.UnBind();
        ib.UnBind();


        float r = 0.0f;
        float increment = 0.05f;

        Renderer renderer;

        // Setup Dear ImGui context
        ImGui::CreateContext();
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

		// 需要指定glsl版本, 也就是shader中的version
		const char* glsl_version = "#version 330";
		ImGui_ImplOpenGL3_Init(glsl_version);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            //glClear(GL_COLOR_BUFFER_BIT);
            renderer.Clear();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

            
            {
                model = glm::translate(glm::mat4(1.0f), tranlationA);
                mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }

			{
				model = glm::translate(glm::mat4(1.0f), tranlationB);
				mvp = proj * view * model;
                shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);

				renderer.Draw(va, ib, shader);
			}


            if (r > 1.0f)
            {
                increment = -0.05f;
            }
            else if (r < 0.0f)
            {
                increment = 0.05f;
            }
            r += increment;

            //std::cout << "r " << r << std::endl;

			{
				ImGui::Begin("ImGui");
                ImGui::SliderFloat3("Tranlation A", &tranlationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("Tranlation B", &tranlationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}
			// Rendering
			ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        //glDeleteProgram(shader);
    }

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);

    //如果没有上面这个作用域，glfwTerminate之后才会调用析构函数，但这时opengl上下文已经没有了
    glfwTerminate();
    
    return 0;
}
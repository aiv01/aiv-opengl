#version 330 core

void main()
{
    vec3 v = vec3(0, 1, 0);
    if (gl_VertexID == 1)
    {
        v = vec3(-1, 0, 0);
    }
    else if (gl_VertexID == 2)
    {
        v = vec3(1, 0, 0);
    }
    gl_Position = vec4(v, 1);
}
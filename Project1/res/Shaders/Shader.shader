#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 atexture;

out vec2 tex;

uniform mat4 transform;

void main()
{
	gl_Position = transform*vec4(position.x , position.y , position.z , 1.0);
	tex = atexture;
};

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 tex;

uniform sampler2D ourTex;
uniform vec4 ourColor;

void main()
{
	FragColor = texture(ourTex , tex) * ourColor;
};
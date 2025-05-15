#version 460 core
// out vec4 FragColor;
// // uniform vec4 myCol;
// in vec3 myCol;
// void main()
// {
//    FragColor = vec4(myCol, 1.0f);
// }


// 07 texture
// #version 460 core
out vec4 FragColor;

in vec3 myCol;
in vec2 myTex;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

void main()
{
   // FragColor = texture(myTexture, myTex);
   // FragColor = texture(myTexture, myTex) * vec4(myCol, 1.0);
   FragColor = mix(texture(myTexture1, myTex), texture(myTexture2, myTex), 0.2);
}


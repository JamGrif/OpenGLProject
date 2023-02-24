#version 430 
out vec4 color;

in vec2 texCoords;

uniform sampler2D screenTexture;

uniform int screenFilter;

void main()
{
    if (screenFilter == 1) //Default
    {
        color = texture(screenTexture, texCoords);
    }
    if (screenFilter == 2) //Inverse
    {
        color = vec4(vec3(1.0 - texture(screenTexture, texCoords)), 1.0); //Inverts colours
    }
    if (screenFilter == 3) //Greyscale
    {
        color = texture(screenTexture, texCoords);
        float average = (color.r + color.g + color.b) / 3.0;
        color = vec4(average, average, average, 1.0);
    }
    if (screenFilter == 4) //Edge detection
    {
        const float offset = 1.0 / 300.0;

        //Surrounding texture coordinates of current pixel
        vec2 offsets[9] = vec2[]
        (
            vec2(-offset,  offset), // top-left
            vec2( 0.0f,    offset), // top-center
            vec2( offset,  offset), // top-right
            vec2(-offset,  0.0f),   // center-left
            vec2( 0.0f,    0.0f),   // center-center
            vec2( offset,  0.0f),   // center-right
            vec2(-offset, -offset), // bottom-left
            vec2( 0.0f,   -offset), // bottom-center
            vec2( offset, -offset)  // bottom-right 
        );

        //Customize this kernal to create different post-processing effects
        float kernel[9] = float[]
        (
            1, 1, 1,
            1, -8, 1,
            1, 1, 1
        );

        //Add the offset to the current texture coordinates
        vec3 sampleTex[9];
        for (int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
        }
        //Then multiple value with the weighted kernel values
        vec3 col = vec3(0.0);
        for (int i = 0; i < 9; i++)
            col += sampleTex[i] * kernel[i];

        color = vec4(col, 1.0);

    }
    if (screenFilter == 5) //Edge detection
    {
        const float offset = 1.0 / 50.0;

        //Surrounding texture coordinates of current pixel
        vec2 offsets[9] = vec2[]
        (
            vec2(-offset,  offset), // top-left
            vec2( 0.0f,    offset), // top-center
            vec2( offset,  offset), // top-right
            vec2(-offset,  0.0f),   // center-left
            vec2( 0.0f,    0.0f),   // center-center
            vec2( offset,  0.0f),   // center-right
            vec2(-offset, -offset), // bottom-left
            vec2( 0.0f,   -offset), // bottom-center
            vec2( offset, -offset)  // bottom-right 
        );

        //Customize this kernal to create different post-processing effects
        float kernel[9] = float[]
        (
            -1,-1,-1,-1,9,-1,-1,-1,-1
        );

        //Add the offset to the current texture coordinates
        vec3 sampleTex[9];
        for (int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
        }
        //Then multiple value with the weighted kernel values
        vec3 col = vec3(0.0);
        for (int i = 0; i < 9; i++)
            col += sampleTex[i] * kernel[i];

        color = vec4(col, 1.0);

    }
}
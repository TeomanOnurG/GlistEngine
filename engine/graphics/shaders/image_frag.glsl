#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform sampler2D maskimage;

uniform vec4 spriteColor;
uniform int maskMode;
uniform float maskRotation;
uniform float imageRotation;

const int MASKMODE_BOTH = 0;
const int MASKMODE_PICTURE = 1;
const int MASKMODE_MASK = 2;
const int MASKMODE_BOTH_FREE_ROTATE = 3;

void main() {
    vec2 pivot = vec2(0.5, 0.5);
    vec2 uv = TexCoords - pivot;

    float maskAngle = radians(maskRotation);
    float maskC = cos(maskAngle);
    float maskS = sin(maskAngle);
    float imageAngle = radians(imageRotation);
    float imageC = cos(imageAngle);
    float imageS = sin(imageAngle);

    vec2 maskRotated = vec2(
    uv.x * maskC + uv.y * maskS,
   -uv.x * maskS + uv.y * maskC
    ) + pivot;
    
    vec2 imageRotated = vec2(
    uv.x * imageC + uv.y * imageS,
   -uv.x * imageS + uv.y * imageC
    ) + pivot;

    vec4 tex;
    vec4 msk;

    if(maskMode == MASKMODE_BOTH) {
        tex = texture(image, TexCoords);
        msk = texture(maskimage, TexCoords);
    }
    else if(maskMode == MASKMODE_PICTURE) {
        tex = texture(image, imageRotated);
        msk = texture(maskimage, TexCoords);
    }
    else if(maskMode == MASKMODE_MASK) {
        tex = texture(image, TexCoords);
        msk = texture(maskimage, maskRotated);
    }
    else if(maskMode == MASKMODE_BOTH_FREE_ROTATE) {
        tex = texture(image, TexCoords);
        msk = texture(maskimage, maskRotated);
    }
    else {
        tex = texture(image, TexCoords);
        msk = texture(maskimage, TexCoords);
    }

    float maskAlpha = 1.0 - msk.r;
    color = spriteColor * vec4(tex.rgb, tex.a * maskAlpha);
}
#version 410 core

in vec3 WorldPos;

out vec4 FragColor;

uniform vec3 cameraPosition;

// Grid shader heavily influenced by Ben Golus implementation
// https://bgolus.medium.com/the-best-darn-grid-shader-yet-727f9278b9d8

void main()
{
    const float thicknessNorm = 0.50f; // grid line size (in pixels)
    const float thicknessBold = 2.0 * thicknessNorm;
    const float frequencyBold = 10.0;
    const float maxCameraDist = 100.0;
    const vec3 colorDefault   = vec3(0.70, 0.70, 0.70);
    const vec3 colorBold      = vec3(0.85, 0.85, 0.85);
    const vec3 colorAxisX     = vec3(0.85, 0.40, 0.30);
    const vec3 colorAxisZ     = vec3(0.40, 0.50, 0.85);
    
    // Grid Test Normal
    vec2 derivative = fwidth(WorldPos.xz);
    vec2 gridAA     = derivative * 1.5;
    vec2 gridUV     = 1.0 - abs(fract(WorldPos.xz) * 2.0 - 1.0);
    vec2 lineWidth  = thicknessNorm * derivative;
    vec2 drawWidth  = clamp(lineWidth, derivative, vec2(0.5));
    vec2 gridTest   = 1.0 - smoothstep(drawWidth - gridAA, drawWidth + gridAA, gridUV);
    gridTest       *= clamp(lineWidth / drawWidth, 0.0, 1.0);
    
    float gridNorm = mix(gridTest.x, 1.0, gridTest.y);
    float alphaGridNorm = clamp(gridNorm, 0.0, 0.8);
    
    // Grid Test Bold
    derivative = fwidth(WorldPos.xz / frequencyBold);
    gridAA     = derivative * 1.5;
    gridUV     = 1.0 - abs(fract(WorldPos.xz / frequencyBold) * 2.0 - 1.0);
    lineWidth  = thicknessBold * derivative;
    drawWidth  = clamp(lineWidth, derivative, vec2(0.5));
    gridTest   = 1.0 - smoothstep(drawWidth - gridAA, drawWidth + gridAA, gridUV);
    gridTest  *= clamp(lineWidth / drawWidth, 0.0, 1.0);
    
    float gridBold = mix(gridTest.x, 1.0, gridTest.y);
    float alphaGridBold = clamp(gridBold, 0.0, 0.9);
    
    // Final grid alpha
    float alphaGrid = max(alphaGridNorm, alphaGridBold);
    
    // Color Test
    vec3 colorOutput = mix(colorDefault, colorBold, gridBold);
    
    float alignAxisX = step(abs(WorldPos.z), gridTest.y);
    float alignAxisZ = step(abs(WorldPos.x), gridTest.x);
    float sum = clamp(alignAxisX + alignAxisZ, 0.0, 1.0);
    
    colorOutput = mix(colorOutput, colorAxisZ, alignAxisZ);
    colorOutput = mix(colorOutput, colorAxisX, alignAxisX);
    
    // Camera distance test
    float cameraDist = length(cameraPosition.xz - WorldPos.xz);
    float alphaDist = 1.0 - smoothstep(0.0, maxCameraDist, cameraDist);
    
    float alpha = min(alphaDist, alphaGrid);
    
    FragColor = vec4(colorOutput, alpha);
}

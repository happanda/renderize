const int NumStars = 100;
const float NumStarsF = float(NumStars);
float Radius = 3.0;
float Intensity = 0.8;

void main()
{
    init();
    
    timeFactor = 0.5;
    time = iGlobalTime * timeFactor;
    
    mat4 pMat = mat4(0.0);
    float fov = M_PI2;
    float tanFov = tan(fov / 2.0);
    const float near = -1.0;
    const float far = 1.0;

    pMat[0][0] = 1.0 / (Aspect * tanFov);
    pMat[1][1] = 1.0 / tanFov;
    pMat[2][2] = -(near + far) / (far - near);
    pMat[2][3] = -1.0;
    pMat[3][2] = 2.0 * near * far / (far - near);
    
    float R = 0.0;
    for (int i = 0; i < NumStars; ++i)
    {
        float iFl = float(i);
        float locTime = time + (iFl * 2.0 / NumStarsF) * (NumStarsF * 1.1137);
        float locCTime = floor(locTime);
        float locFTime = locTime - locCTime;

        vec2 pPolar;
        pPolar.x = SimpleNoise(vec2(locCTime, 1.0));
        pPolar.y = SimpleNoise(vec2(locCTime, 2.0)) * M_2PI;
        // move a little bit from zero
        pPolar.x = TRANSIT(0.0, 1.0, 0.02, 0.99, abs(pPolar.x));

        vec4 dPos = vec4(DECART(pPolar), 1.0 - locFTime, 1.0);

        vec4 persPos = pMat * dPos;
        persPos.xy = persPos.xy * Center.y / persPos.w;
        float transVal = TRANSIT(locFTime, 0.0, 1.0, 0.3, 1.0);
        float intens = Intensity * transVal;
        R += cPRESSENCE(persPos, Radius * transVal) * intens;
    }
    fragColor = vec4(R, R, R, 1.);
}

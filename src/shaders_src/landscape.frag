const int NumPnts = 16;
const float NumPntsF = float(NumPnts);
float Heights[1];
const int NumPntsSq = (NumPnts + 1) * (NumPnts + 1);
uniform float Land[NumPntsSq];
float Radius = 2.0;
float Intensity = 0.8;

void mapHeights(float line)
{
    //float H = 0.7;
    //Heights[0] = 0;//snoise(vec2(line, 0.0));
    //Heights[NumPnts] = 0;//snoise(vec2(line, NumPnts));
    //for (int step = NumPnts; step > 1; step /= 2)
    //{
    //    float hardness = 1 / pow(2.0, H / float(step) * NumPntsF);
    //    int halfStep = step / 2;
    //    for (int k = halfStep; k < NumPnts; k += step)
    //    {
    //        Heights[k] = (Heights[k - halfStep] + Heights[k + halfStep]) / 2.0
    //            + TRANSIT(snoise(vec2(line, k)), -1.0, 1.0, -hardness, hardness);
    //    }
    //}
}

#define idx(x, y) (y * (NumPnts + 1) + x)
#define lval(x, y) Land[idx(x, y)]
#define lset(x, y, v) Land[idx(x, y)] = v
/*
void land()
{
    float H = 0.3;
    lset(0, 0, 0.0);
    lset(0, NumPnts, 0.0);
    lset(NumPnts, 0, 0.0);
    lset(NumPnts, NumPnts, 0.0);
    for (int y = 0; y <= NumPnts; ++y)
    {
        for (int x = 0; x <= NumPnts; ++x)
        {
            lset(x, y, 0);
        }
    }

    for (int step = NumPnts; step > 1; step /= 2)
    {
        float hardness = 1 / pow(2.0, H / float(step) * NumPntsF);
        int halfStep = step / 2;
        for (int y = halfStep; y < NumPnts; y += step)
        {
            for (int x = halfStep; x < NumPnts; x += step)
            {
                float value = (lval(x - halfStep, y - halfStep) + lval(x - halfStep, y + halfStep)
                    + lval(x + halfStep, y - halfStep) + lval(x + halfStep, y + halfStep)) / 4.0
                    + TRANSIT(snoise(vec2(x, y)), 0.0, 1.0, -hardness, hardness);
                lset(x, y, value);
            }
        }
        for (int y = 0; y < NumPnts; y += step)
        {
            for (int x = halfStep; x < NumPnts; x += step)
            {
                int ym = y - halfStep;
                if (ym < 0)
                    ym += NumPnts;
                float value = (lval(x - halfStep, y) + lval(x, ym)
                    + lval(x + halfStep, y) + lval(x, y + halfStep)) / 4.0
                    + TRANSIT(snoise(vec2(x, y)), 0.0, 1.0, -hardness, hardness);
                lset(x, y, value);
            }
        }
        for (int y = halfStep; y < NumPnts; y += step)
        {
            for (int x = 0; x < NumPnts; x += step)
            {
                int xm = x - halfStep;
                if (xm < 0)
                    xm += NumPnts;
                float value = (lval(xm, y) + lval(x, y - halfStep)
                    + lval(x + halfStep, y) + lval(x, y + halfStep)) / 4.0
                    + TRANSIT(snoise(vec2(x, y)), 0.0, 1.0, -hardness, hardness);
                lset(x, y, value);
            }
        }
    }
}
*/

void main()
{
    init();
    
    mat4 pMat = mat4(0.0);
    float fov = M_PI2;
    float tanFov = tan(fov / 2.0);
    const float near = 0.0;
    const float far = 10.0;

    pMat[0][0] = 1.0 / (Aspect * tanFov);
    pMat[1][1] = 1.0 / tanFov;
    pMat[2][2] = -(near + far) / (far - near);
    pMat[2][3] = -1.0;
    pMat[3][2] = 2.0 * near * far / (far - near);
    
    float centLen = length(Center);
    float R = 0.0;
    float xWidth = 4.0;
    float xWidth2 = xWidth / 2.0;
    float zWidth = 16.0;
    
    /*
    mapHeights(floor(iGlobalTime));
    for (int i = 0; i < NumPnts; ++i)
    {
        float iFl = float(i);
        
        float locTime = time + iFl / NumPntsF;
        float locCTime = floor(locTime);
        float locFTime = locTime - locCTime;

        float xc = iFl / NumPntsF;
        float yc = Heights[i];
        vec3 screenPos;
        screenPos.x = TRANSIT(xc, 0.0, 1.0, -Center.x, Center.x);
        screenPos.y = TRANSIT(yc, -1.0, 1.0, -Center.y, Center.y);
        R += cPRESSENCE(screenPos, Radius);
    }
    */
    /*
    for (int i = 0; i < NumPnts; ++i)
    {
        float iFl = float(i);
        
        float locTime = time + iFl / NumPntsF;
        float locCTime = floor(locTime);
        float locFTime = locTime - locCTime;

        mapHeights(iFl);
        for (int j = 0; j < NumPnts; ++j)
        {
            float jFl = float(j);
            
            float xc = jFl / NumPntsF * xWidth - xWidth2;
            float zc = (1.0 - locFTime) * zWidth;//(1.0 - iFl / NumPntsF) * zWidth;//
            float yc = 2.0 - Heights[j];

            vec4 dPos = vec4(xc, yc, zc, 1.0);
            vec4 persPos = pMat * dPos;
            persPos.xy = persPos.xy * centLen / persPos.w;
            float transVal = TRANSIT(zWidth - zc, 0.0, 1.0, 0.2, 0.3);
            float intens = Intensity * transVal;
            R += cPRESSENCE(persPos.xyz, Radius * transVal) * intens;
        }
    }
    */
    //land();
    for (int i = 0; i < NumPnts; ++i)
    {
        float iFl = float(i);
        float locTime = time + iFl / NumPntsF;
        float locCTime = floor(locTime);
        float locFTime = locTime - locCTime;

        for (int j = 0; j < NumPnts; ++j)
        {
            float jFl = float(j);
            
            float xc = jFl / NumPntsF * xWidth - xWidth2 + 0.51;
            float zc = (1.0 - locFTime) * zWidth;//(1.0 - iFl / NumPntsF) * zWidth;//
            float yc = 1.5 - lval(i, j);

            vec4 dPos = vec4(xc, yc, zc, 1.0);
            vec4 persPos = pMat * dPos;
            persPos.xy = persPos.xy * centLen / persPos.w;
            float transVal = TRANSIT(zWidth - zc, 0.0, 1.0, 0.2, 0.3);
            float intens = Intensity * transVal;
            R += cPRESSENCE(persPos.xy, Radius * transVal) * intens;
        }
    }
    fragColor = vec4(R, R, R, 1.);
}

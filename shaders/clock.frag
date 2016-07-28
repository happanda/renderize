float Radius = 5.0;
float Intensity = 0.3;

void main()
{
    init();
    float date = iDate.w;
    float R = 0.0,
          G = 0.0,
          B = 0.0;
    
    const float Seconds = 60.0;
    const float Minutes = 60.0;
    const float Hours = 12.0;
    float timeS = mod(date, Seconds);
    float timeM = mod(floor(date / Seconds), Minutes);
    float timeH = mod(floor(floor(date / Seconds) / Minutes), Hours);

    float radius = Center.y / 2.0;
    vec2 polPnt, decPnt;
    polPnt.x = radius;
    for (float i = 0.0; i < Seconds; ++i)
    {
        polPnt.y = M_2PI * (1.25 - i / Seconds); //M_2PI / Seconds * (Seconds - i + Seconds / 4.0);
        
        decPnt = DECART(polPnt);
        R += cPRESSENCE(decPnt, Radius);
    }

    polPnt.y = M_2PI * (1.25 - timeS / Seconds); //M_2PI / Seconds * (Seconds - timeS + Seconds / 4.0);
    polPnt.x = radius + sin(timeS * M_PI) * 25.0;
    decPnt = DECART(polPnt);
    R += cPRESSENCE(decPnt, Radius);
    
    polPnt.y = M_2PI * (1.25 - timeM / Minutes); //M_2PI / Minutes * (Minutes - timeM + Minutes / 4.0);
    polPnt.x = radius;
    if (timeS < 1.0 && date >= 1.0)
    {
        polPnt.y += (1.0 - timeS) * M_2PI / Minutes;
        polPnt.x += sin(timeS * M_PI) * 25.0;
    }
    decPnt = DECART(polPnt);
    G += cPRESSENCE(decPnt, Radius * 4.0);
    
    polPnt.y = M_2PI * (1.25 - timeH / Hours); //M_2PI / Hours * (Hours - timeH + Hours / 4.0);
    polPnt.x = radius;
    if (timeS < 1.0 && timeM < 1.0 && date >= Seconds)
    {
        polPnt.y += (1.0 - timeS) * M_2PI / Hours;
        polPnt.x += sin(timeS * M_PI) * 25.0;
    }
    decPnt = DECART(polPnt);
    B += cPRESSENCE(decPnt, Radius * 8.0);
    
    if (G > 0.0)
        B = 0.0;
    if (B > 0.0 || G > 0.0)
        R = 0.0;
    fragColor = vec4(R, G, B, 1.);// * Intensity;
}

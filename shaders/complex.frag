
#define PLXMULT(u, v) vec2(u.x * v.x - u.y * v.y, u.x * v.y + u.y * v.x)
#define PLXSQR(u) vec2(u.x * u.x - u.y * u.y, 2 * u.x * u.y)
#define PLXTRI(u) vec2(u.x * (u.x * u.x - 3 * u.y * u.y), u.y * (3 * u.x * u.x - u.y * u.y))

/*
* General formula: z_{n+1} = A * z_{n}^D + B
*/
const vec2 A = vec2(1.0, 0.0);
const vec2 B = vec2(-0.9, 0.18);
float D = 7.0;//SINN(time) * 15.0 + 1.0;

vec2 plxPow(vec2 plx, float deg)
{
    float len = length(plx);
    if (len == 0.0)
        return plx;
    float phi = deg * acos(plx.x / len);
    return pow(len, deg) * CS(phi);
}

vec2 nextIter(vec2 plx)
{
    vec2 res = plxPow(plx, D);
    return PLXMULT(A, res) + B;
}

void main()
{
    init();
    float R = 0.3,
          G = 0.3,
          B = 0.3;

    int iter = 0;
    float Coeff = 1.5;
    vec2 coord = cUV * Coeff;
    while (iter < 10)
    {
        coord = nextIter(coord);
        if (abs(coord.x) > 5.0 || abs(coord.y) > 6.0)
            break;
        ++iter;
    }
    if (abs(coord.x) < 7.0 || abs(coord.y) > 3.0)
        R = iter * 0.1;

    fragColor = vec4(R, R, R, 1.);
}